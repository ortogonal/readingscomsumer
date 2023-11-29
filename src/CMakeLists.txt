cmake_minimum_required(VERSION 3.5)

project(readingsconsumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

set(HEADERS
    queuereader.h

    consumers/accumulatedpower.h
    consumers/accumulatedprice.h
    consumers/consumer.h
    consumers/livepower.h

    parsers/jsonmessage.h

    providers/dayaheadprice.h
    providers/hour.h
    providers/idayaheadprice.h
)

set(SOURCES
    queuereader.cpp

    consumers/accumulatedpower.cpp
    consumers/accumulatedprice.cpp
    consumers/livepower.cpp

    parsers/jsonmessage.cpp

    providers/dayaheadprice.cpp

    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${HIREDIS_HEADER} ${REDIS_PLUS_PLUS_HEADER}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${HIREDIS_LIB}
    PRIVATE ${REDIS_PLUS_PLUS_LIB}
    PRIVATE jsoncpp
    PRIVATE mysqlcppconn
)

include(GNUInstallDirs)
install(TARGETS readingsconsumer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
