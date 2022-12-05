cmake_minimum_required(VERSION 3.20)
project(lambdadb CXX)
set (CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

#Set the location of where Windows can find the installed libraries of the SDK.
if(MSVC)
    string(REPLACE ";" "/aws-cpp-sdk-all;" SYSTEM_MODULE_PATH "${CMAKE_SYSTEM_PREFIX_PATH}/aws-cpp-sdk-all")
    list(APPEND CMAKE_PREFIX_PATH ${SYSTEM_MODULE_PATH})
endif()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
set(BUILD_SHARED_LIBS ON CACHE STRING "Link to shared libraries by default.")

#Load required services/packages: This basic example uses S3.
find_package(AWSSDK REQUIRED COMPONENTS s3)
add_executable(${PROJECT_NAME} main.cpp src/net/Client.cpp src/net/Client.h src/net/Server.cpp src/net/Server.h src/db/Table.cpp src/db/Table.h src/db/OffsetMap.cpp src/db/OffsetMap.h src/db/Database.cpp src/db/Database.h src/db/DataType.h src/storage/StorageService.cpp src/storage/StorageService.h src/sql/lexer/Lexer.cpp src/sql/lexer/Lexer.h src/sql/lexer/Token.cpp src/sql/lexer/Token.h src/sql/code/Repository.cpp src/sql/code/Repository.h src/sql/parser/Node.cpp src/sql/parser/Node.h)

#Windows: This 'if' clause copies the SDK libraries from the installation location to the place where 
#  this project's executable is located.  Without this you'll need to copy the install 
#  /bin folder to the exe location (app.exe in this case) to prevent runtime errors.
if(MSVC AND BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC "USE_IMPORT_EXPORT")
    add_definitions(-DUSE_IMPORT_EXPORT)
    # Copy relevant AWS SDK for C++ libraries into the current binary directory for running and debugging.
    list(APPEND SERVICE_LIST s3)
   
    #For IDE's like Xcode and Visual Studio this line will be ignored because Release/Debug 
    #  is switched internally, but this is necessary for non-IDE builds.
    set(CMAKE_BUILD_TYPE Debug) #TODO: Set to your build type
	
    #TODO:Choose appropriate one of the following two lines, you want to copy to the same folder where your executables are.
    AWSSDK_CPY_DYN_LIBS(SERVICE_LIST "" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})  #Choose this line if your executables are in /build/Debug
    #AWSSDK_CPY_DYN_LIBS(SERVICE_LIST "" ${CMAKE_CURRENT_BINARY_DIR})  #Choose this line for Visual Studio and possibly other IDEs
    
    message(STATUS ">>CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS ">>CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    message(STATUS ">>EXECUTABLE_OUTPUT_PATH : ${EXECUTABLE_OUTPUT_PATH}")
endif()

set_compiler_flags(${PROJECT_NAME})
set_compiler_warnings(${PROJECT_NAME})
target_compile_options (${PROJECT_NAME} PUBLIC -fexceptions)
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES} nlohmann_json::nlohmann_json)