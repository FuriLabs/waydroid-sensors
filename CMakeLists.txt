cmake_minimum_required(VERSION 3.16)
project(andromeda-sensors)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)
pkg_check_modules(GBINDER REQUIRED libgbinder>=1.1.20)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")

add_subdirectory(sensorfw-core)

add_executable(
    andromeda-sensors

    SensorFW.cpp
    Sensors.cpp
    service.cpp
)

target_link_libraries(andromeda-sensors PUBLIC
    ${GIO_UNIX_LDFLAGS} ${GIO_UNIX_LIBRARIES}
    ${GBINDER_LDFLAGS} ${GBINDER_LIBRARIES}

    sensorfw-core
)

target_include_directories(andromeda-sensors PUBLIC
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GBINDER_INCLUDE_DIRS}

    sensorfw-core/include
)

install(TARGETS andromeda-sensors RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR})
