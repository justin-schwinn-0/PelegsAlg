cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 11)

project(Pelegs)


set(Sources
    main.cpp
)

set(Headers
    Connection.h
    Node.h
)

find_library(SCTP_LIB libsctp.so)


add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SCTP_LIB})
