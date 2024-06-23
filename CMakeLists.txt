cmake_minimum_required(VERSION 3.22.1)

project(Pelegs)

set(Sources
    main.cpp
)

set(Headers
    Connection.h
    Node.h
)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
