cmake_minimum_required(VERSION 3.5)

project(readingsconsumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

if (BUILD_WITH_VCPKG)
    find_package(jsoncpp CONFIG REQUIRED)
    find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
endif()

set(HEADERS
    queuereader.h

    consumers/accumulatedpower.h
    consumers/accumulatedprice.h
    consumers/consumer.h
    consumers/livepower.h
    consumers/power.h

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
    consumers/power.cpp

    parsers/jsonmessage.cpp

    providers/dayaheadprice.cpp

    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${HIREDIS_HEADER} ${REDIS_PLUS_PLUS_HEADER}
)

if (NOT BUILD_WITH_VCPKG)
    target_include_directories(${PROJECT_NAME}
        PRIVATE /usr/include/jsoncpp
    )
endif()


target_link_libraries(${PROJECT_NAME}
    PRIVATE ${HIREDIS_LIB}
    PRIVATE ${REDIS_PLUS_PLUS_LIB}
)

if (BUILD_WITH_VCPKG)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE JsonCpp::JsonCpp
        PRIVATE mysqlcppconn-static
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE jsoncpp
        PRIVATE mysqlcppconn
    )
endif()

include(GNUInstallDirs)
install(TARGETS readingsconsumer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
