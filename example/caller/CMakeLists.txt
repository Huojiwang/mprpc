set(SRC_LIST callfriendservice.cc ../friend.pb.cc)

add_executable(friendconsumer ${SRC_LIST})

target_link_libraries(friendconsumer mprpc protobuf)