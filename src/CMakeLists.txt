file(GLOB_RECURSE SOURCES "*.cpp")

file (GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/qservice/*hpp)

set(SOURCE_FILES
    ${SOURCES}
    ${INCLUDE_FILES}
)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set(QSERVICE_INCLUDE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(${PROJECT_NAME} PUBLIC ${QSERVICE_INCLUDE})

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

target_link_libraries(${PROJECT_NAME}_static PRIVATE ${CMAKE_REQUIRED_LIBRARIES})
target_include_directories(${PROJECT_NAME}_static INTERFACE ${QSERVICE_INCLUDE})

if(UNIX)
    string(REGEX MATCH ".*/" CXX_COMPILER_DIR ${CMAKE_CXX_COMPILER})
    string(REGEX MATCH ".*[^bin/]" CXX_COMPILER_DIR ${CXX_COMPILER_DIR})
    message(STATUS CXX_COMPILER_DIR=${CXX_COMPILER_DIR})
    target_link_options(${PROJECT_NAME}_static PUBLIC "-Wl,-rpath=${CXX_COMPILER_DIR}/lib64")
    target_include_directories(${PROJECT_NAME}_static PUBLIC ${CXX_COMPILER_DIR}/include)
endif(UNIX)



set_target_properties(${PROJECT_NAME}_static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)