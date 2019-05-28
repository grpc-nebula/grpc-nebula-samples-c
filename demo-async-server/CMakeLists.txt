# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (demo-async-server CXX)

# 版本信息
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)


# 编译参数
set (CMAKE_BUILE_TYPE RELEASE) 
add_definitions(-DRELEASE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -pthread")


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
#aux_source_directory(model DIR_LIB_SRCS)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
#设置proto工具目录
get_filename_component(proto_tool "${PROJECT_SOURCE_DIR}/../../libs/protobuf" ABSOLUTE)
#生成PB
execute_process(COMMAND ${proto_tool}/protoc -I ${PROJECT_SOURCE_DIR} --cpp_out=${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/helloworld.proto)
execute_process(COMMAND ${proto_tool}/protoc -I ${PROJECT_SOURCE_DIR} --grpc_out=${PROJECT_SOURCE_DIR}
--plugin=protoc-gen-grpc=${proto_tool}/grpc_cpp_plugin ${PROJECT_SOURCE_DIR}/helloworld.proto)

# 链接库目录
link_directories ("${PROJECT_SOURCE_DIR}/../../libs/opt"
                  "${PROJECT_SOURCE_DIR}/../../libs/lib"
                  "${PROJECT_SOURCE_DIR}/../../libs/protobuf"
                  "${PROJECT_SOURCE_DIR}/../../libs/zookeeper")

# 添加包含目录
include_directories("${PROJECT_SOURCE_DIR}"
	            "${PROJECT_SOURCE_DIR}/../../include/orientsec_grpc"
                    "${PROJECT_SOURCE_DIR}/../../include/include"
                    "${PROJECT_SOURCE_DIR}/../../include/third_party/protobuf/src/"
	            "${PROJECT_SOURCE_DIR}/../../include/third_party/protobuf"
	            "${PROJECT_SOURCE_DIR}/model")


# 生成链接库
add_executable (demo-async-server ${DIR_LIB_SRCS})

# 添加链接库
target_link_libraries(demo-async-server libgpr.so)
target_link_libraries(demo-async-server libgrpc.so)
target_link_libraries(demo-async-server libgrpc++.so)
target_link_libraries(demo-async-server libprotobuf.a)
target_link_libraries(demo-async-server libssl.so)
target_link_libraries(demo-async-server liborientsec_common.a)
target_link_libraries(demo-async-server liborientsec_provider.a)
target_link_libraries(demo-async-server liborientsec_consumer.a)
target_link_libraries(demo-async-server liborientsec_registry.a)
target_link_libraries(demo-async-server libzookeeper_mt.so)

