cmake_minimum_required(VERSION 3.5)

project(rank C CXX)

set(RANK_VERSION 1.0.0)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(LIBRARYS_STATIC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

message(STATUS CMAKE_FIND_LIBRARY_SUFFIXES: ${CMAKE_FIND_LIBRARY_SUFFIXES})

# pthread
find_package(Threads REQUIRED)

# gflags
find_package(GFLAGS REQUIRED)

if(GFLAGS_FOUND)
  message(STATUS "find Gflags:")
  message(STATUS GFLAGS_LIBRARY: ${GFLAGS_LIBRARY})
  message(STATUS GFLAGS_INCLUDE_PATH: ${GFLAGS_INCLUDE_PATH})
elseif()
  message(FATAL_ERROR "can not find Gflags")
endif()

# protobuf
find_package(Protobuf REQUIRED)

if(Protobuf_FOUND)
  message(STATUS "find Protobuf:")
  message(STATUS Protobuf_LIBRARIES: ${Protobuf_LIBRARIES})
  message(STATUS Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS})
  message(STATUS Protobuf_PROTOC_LIBRARIES: ${Protobuf_PROTOC_LIBRARIES})
  message(STATUS Protobuf_PROTOC_EXECUTABLE: ${Protobuf_PROTOC_EXECUTABLE})
  message(STATUS PROTOC_FLAGS: ${PROTOC_FLAGS})

  include(CompileProto)
  set(PROTO_FILES adinfo.proto consistent_feature.proto orderfeature.proto userbehavior.proto echo.proto)
  compile_proto(PROTO_HDRS PROTO_SRCS ${PROJECT_SOURCE_DIR}/pb ${PROJECT_SOURCE_DIR}/pb "${PROTO_FILES}")
elseif()
  message(FATAL_ERROR "can not find Protobuf")
endif()

# leveldb
find_package(LEVELDB REQUIRED)

if(LEVELDB_FOUND)
  message(STATUS "find Leveldb:")
  message(STATUS LEVELDB_LIB: ${LEVELDB_LIB})
  message(STATUS LEVELDB_INCLUDE_PATH: ${LEVELDB_INCLUDE_PATH})
else()
  message(FATAL_ERROR "can not find Leveldb")
endif()

# openssl
find_package(OpenSSL)

if(OpenSSL_FOUND)
  message(STATUS "find OpenSSL:")
  message(STATUS OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY})
  message(STATUS OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY})
  message(STATUS OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR})
else()
  message(FATAL_ERROR "can not find Leveldb")
endif()

# boost
find_path(BOOST_INCLUDE_PATH NAMES boost/align/align.hpp PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/boost/include)
find_library(BOOST_LIB_FILE NAMES libboost_filesystem.a PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/boost/lib)
find_library(BOOST_LIB_REGEX NAMES libboost_regex.a PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/boost/lib)
find_library(BOOST_LIB_SYSTEM NAMES libboost_system.a PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/boost/lib)

message(STATUS "BOOST_INCLUDE_PATH: ${BOOST_INCLUDE_PATH}")
message(STATUS "BOOST_LIB_FILE: ${BOOST_LIB_FILE}")
message(STATUS "BOOST_LIB_REGEX: ${BOOST_LIB_REGEX}")
message(STATUS "BOOST_LIB_SYSTEM: ${BOOST_LIB_SYSTEM}")

# json
find_path(JSON_INCLUDE_PATH NAMES json/json.h PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/json/include)
find_library(JSON_LIB NAMES libjsoncpp.a PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/json/lib)

message(STATUS "JSON_INCLUDE_PATH: ${JSON_INCLUDE_PATH}")
message(STATUS "JSON_LIB: ${JSON_LIB}")

# brpc
find_path(BRPC_INCLUDE_PATH NAMES brpc/amf.h PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/brpc/build/output/include)
find_library(BRPC_LIB NAMES brpc libbrpc.a PATHS ${PROJECT_SOURCE_DIR}/../com/thirdparty/brpc/build/output/lib)

message(STATUS "BRPC_INCLUDE_PATH: ${BRPC_INCLUDE_PATH}")
message(STATUS "BRPC_LIB: ${BRPC_LIB}")

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -fPIC")

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(CLIENT_SOURCES
  main/multi_client.cpp 
  ${PROTO_SRCS}
)

set(SERVER_SOURCES
  main/server.cpp
  concurrent/rank_thread.cpp
  concurrent/rank_pool.cpp
  conf/rs_conf.cpp
  base/wait_group.cpp
  ${PROTO_SRCS}
  ranklib/ranker/ranker.cpp
  ranklib/ranker/rank_adapter.cpp
  ranklib/search/search_ranker.cpp
  ranklib/mobsearch/mobsearch_ranker.cpp
  ranklib/config/ConfigReader.cpp
  ranklib/config/JsonConfigLoader.cpp
  ranklib/store/info_center.cpp
  ranklib/feature/feature_adapter.cpp
  ranklib/feature/feature_base.cpp
  ranklib/feature/feature_center.cpp
  ranklib/feature/feature_funcs.cpp
  ranklib/feature/twoval_combiner_v2.cpp
  ranklib/util/StrNormalize.cpp
)

set(DYNAMIC_LIB
  ${BRPC_LIB}
  ${GFLAGS_LIBRARY}
  ${PROTOBUF_LIBRARIES}
  ${LEVELDB_LIB}
  ${OPENSSL_CRYPTO_LIBRARY}
  ${OPENSSL_SSL_LIBRARY}
  ${BOOST_LIB_FILE}
  ${BOOST_LIB_REGEX}
  ${BOOST_LIB_SYSTEM}
  ${JSON_LIB}
  dl
  z
)

set(DYNAMIC_INCLUDE_PATH
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/ranklib
  ${BRPC_INCLUDE_PATH}
  ${GFLAGS_INCLUDE_PATH}
  ${PROTOBUF_INCLUDE_DIRS}
  ${LEVELDB_INCLUDE_PATH}
  ${OPENSSL_INCLUDE_DIR}
  ${BOOST_INCLUDE_PATH}
  ${JSON_INCLUDE_PATH}
)

add_executable(rank_client ${CLIENT_SOURCES})
add_executable(rank_server ${SERVER_SOURCES})

target_link_libraries(rank_client
  PUBLIC ${BRPC_LIB}
  PUBLIC ${DYNAMIC_LIB}
)

target_link_libraries(rank_server
  PUBLIC ${BRPC_LIB}
  PUBLIC ${DYNAMIC_LIB}
)

target_include_directories(rank_client
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${BRPC_INCLUDE_PATH}
  PRIVATE ${DYNAMIC_INCLUDE_PATH}
)

target_include_directories(rank_server
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${BRPC_INCLUDE_PATH}
  PRIVATE ${DYNAMIC_INCLUDE_PATH}
)

