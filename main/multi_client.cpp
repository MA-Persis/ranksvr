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

// Benchmark http-server by multiple threads.

#include <gflags/gflags.h>
#include <bthread/bthread.h>
#include <butil/logging.h>
#include <brpc/channel.h>
#include <brpc/server.h>
#include <bvar/bvar.h>
#include "pb/echo.pb.h"

DEFINE_int32(thread_num, 10, "Number of threads to send requests");
DEFINE_bool(use_bthread, false, "Use bthread to send requests");
DEFINE_int32(dummy_port, -1, "Launch dummy server at this port");
DEFINE_string(attachment, "", "Carry this along with requests");
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "0.0.0.0:8000", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)"); 
DEFINE_int32(interval_ms, 1000, "Milliseconds between consecutive requests");

bvar::LatencyRecorder g_latency_recorder("client");

static void random_set_values(example::RankRequest& request) {
    example::RankStruct* rank_struct = request.add_rank_struct();
    example::VisitInfo* visit_info = rank_struct->mutable_visitinfo();
    example::QueryInfo* query_info = rank_struct->add_querylist();

    // VisitInfo
    visit_info->set_province(1);
    visit_info->set_citystr("其他");
    visit_info->set_city(10001);
    visit_info->set_pvid("1a3dd024441d24b1");
    visit_info->set_oaid("");

    // QueryInfo
    query_info->set_query("上海专科学校");

    // AdInfo4Rank
    int ad_nums = 3;
    for (int i = 0; i < ad_nums; i++) {
        example::AdInfo4Rank* rank_list = query_info->add_adlist();
        adinfo::AdInfo* ad_info = rank_list->mutable_adinfo();
        adinfo::AdAccount* ad_account = ad_info->mutable_account();
        ad_account->set_ad_id(10521852996);
        rank_list->set_adid(10521852996);
    }
}

static void* sender(void* arg) {
    brpc::Channel* channel = static_cast<brpc::Channel*>(arg);

    // Normally, you should not call a Channel directly, but instead construct
    // a stub Service wrapping it. stub can be shared by all threads as well.
    example::RankService_Stub stub(channel);

    int log_id = 0;
    while (!brpc::IsAskedToQuit()) {
      // We will receive response synchronously, safe to put variables
      // on stack.
      example::RankRequest request;
      example::RankResponse response;
      brpc::Controller cntl;

      random_set_values(request);

      cntl.set_log_id(log_id ++);  // set by user
      // Set attachment which is wired to network directly instead of 
      // being serialized into protobuf messages.
      cntl.request_attachment().append(FLAGS_attachment);

      // Because `done'(last parameter) is NULL, this function waits until
      // the response comes back or error occurs(including timedout).
      stub.Rank(&cntl, &request, &response, NULL);
      if (!cntl.Failed()) {
          LOG(INFO) << "Received response from " << cntl.remote_side()
              << " to " << cntl.local_side()
              << ": " << response.rank_response() << " (attached="
              << cntl.response_attachment() << ")"
              << " latency=" << cntl.latency_us() << "us";
      } else {
          LOG(WARNING) << cntl.ErrorText();
      }
      usleep(FLAGS_interval_ms * 1000L);
    }



    return NULL;
}

int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);

    // A Channel represents a communication line to a Server. Notice that 
    // Channel is thread-safe and can be shared by all threads in your program.
    brpc::Channel channel;
    
    // Initialize the channel, NULL means using default options.
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms/*milliseconds*/;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    std::vector<bthread_t> bids;
    std::vector<pthread_t> pids;
    if (!FLAGS_use_bthread) {
        pids.resize(FLAGS_thread_num);
        for (int i = 0; i < FLAGS_thread_num; ++i) {
            if (pthread_create(&pids[i], NULL, sender, &channel) != 0) {
                LOG(ERROR) << "Fail to create pthread";
                return -1;
            }
        }
    } else {
        bids.resize(FLAGS_thread_num);
        for (int i = 0; i < FLAGS_thread_num; ++i) {
            if (bthread_start_background(
                    &bids[i], NULL, sender, &channel) != 0) {
                LOG(ERROR) << "Fail to create bthread";
                return -1;
            }
        }
    }

    if (FLAGS_dummy_port >= 0) {
        brpc::StartDummyServerAt(FLAGS_dummy_port);
    }

    while (!brpc::IsAskedToQuit()) {
        sleep(1);
        LOG(INFO) << "Sending " << FLAGS_protocol << " requests at qps=" 
                  << g_latency_recorder.qps(1)
                  << " latency=" << g_latency_recorder.latency(1);
    }

    LOG(INFO) << "benchmark_http is going to quit";
    for (int i = 0; i < FLAGS_thread_num; ++i) {
        if (!FLAGS_use_bthread) {
            pthread_join(pids[i], NULL);
        } else {
            bthread_join(bids[i], NULL);
        }
    }

    return 0;
}