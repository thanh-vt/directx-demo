cmake_minimum_required(VERSION 3.16)
project(directx_demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(directx_demo WIN32 main.cpp)

set(DirectX_LIBRARY_PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x86")
find_library(DirectX_D3D9_LIBRARY d3d9 ${DirectX_LIBRARY_PATHS} NO_DEFAULT_PATH)

message(D3D9_LOCATION="${DirectX_D3D9_LIBRARY}")
target_link_libraries(directx_demo "${DirectX_D3D9_LIBRARY}")
