// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

// A server to receive EchoRequest and send back EchoResponse.

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/nshead_service.h>
#include <brpc/policy/snappy_compress.h>

#include "concurrent/rank_thread.h"
#include "conf/rs_conf.h"
#include "main/rs_service.h"
// #include "pb/echo.pb.h"


DEFINE_bool(echo_attachment, true, "Echo attachment as well");
DEFINE_int32(port, 9999, "TCP Port of this server");
DEFINE_string(listen_addr, "", "Server listen address, may be IPV4/IPV6/UDS."
            " If this is set, the flag port will be ignored");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
             "read/write operations during the last `idle_timeout_s'");
DEFINE_int32(session_local_num, 20, "session local num");

namespace example {
class RankService0 : public brpc::NsheadService {
public:
    void ProcessNsheadRequest(const brpc::Server&,
                              brpc::Controller* cntl,
                              const brpc::NsheadMessage& request,
                              brpc::NsheadMessage* response,
                              brpc::NsheadClosure* done) {
        // This object helps you to call done->Run() in RAII style. If you need
        // to process the request asynchronously, pass done_guard.release().
        brpc::ClosureGuard done_guard(done);

        RankRequest rank_request;
        if (!brpc::policy::SnappyDecompress(request.body, &rank_request)) {
            printf("Request snappy decompress failed\n");
            return;
        }

        rs::RankThreadManager* sd = static_cast<rs::RankThreadManager*>(cntl->session_local_data());

        sd->process_request(const_cast<example::RankRequest*>(&rank_request));

        if (!brpc::policy::SnappyCompress(rank_request, &response->body)) {
            printf("Requresponseest snappy compress failed\n");
            return;
        }

        // The purpose of following logs is to help you to understand
        // how clients interact with servers more intuitively. You should 
        // remove these logs in performance-sensitive servers.
        LOG(INFO) << "Received request[log_id=" << cntl->log_id() 
                  << "] from " << cntl->remote_side() 
                  << " to " << cntl->local_side()
                  << " thread_point:" << sd;

        // You can compress the response by setting Controller, but be aware
        // that compression may be costly, evaluate before turning on.
        // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

        if (FLAGS_echo_attachment) {
            // Set attachment which is wired to network directly instead of
            // being serialized into protobuf messages.
            cntl->response_attachment().append(cntl->request_attachment());
        }
    }                                                        
};
}  // namespace example

int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);

    rs::RsConfig &rsconfig = rs::RsConfig::get_cur_config();
    std::string rs_conf_path = "../conf/rs.conf";
    int n = rsconfig.init(rs_conf_path);
    if (0 != n) {
        LOG(FATAL) << "init rs config failed!";
        return -1;
    }

    // Generally you only need one Server.
    brpc::Server server;

    butil::EndPoint point;
    if (!FLAGS_listen_addr.empty()) {
        if (butil::str2endpoint(FLAGS_listen_addr.c_str(), &point) < 0) {
            LOG(ERROR) << "Invalid listen address:" << FLAGS_listen_addr;
            return -1;
        }
    } else {
        point = butil::EndPoint(butil::IP_ANY, FLAGS_port);
    }
    // Start the server.
    brpc::ServerOptions options;

    rs::SessionLocalDataFactory session_local_data_factory;

    options.nshead_service = new example::RankService0();
    options.session_local_data_factory = &session_local_data_factory;
    options.reserved_session_local_data = FLAGS_session_local_num;
    if (server.Start(point, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    // Wait until Ctrl-C is pressed, then Stop() and Join() the server.
    server.RunUntilAskedToQuit();
    return 0;
}