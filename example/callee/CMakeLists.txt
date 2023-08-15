set(SRC_LIST friendservice.cc  ../friend.pb.cc)

add_executable(friendprovider ${SRC_LIST})

target_link_libraries(friendprovider mprpc protobuf)