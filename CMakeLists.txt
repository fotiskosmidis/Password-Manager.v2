cmake_minimum_required(VERSION 3.20)

project(password_manager_backend VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(password_manager_backend SHARED
    src-cpp-lib/account.cpp
    src-cpp-lib/app_paths.cpp
    src-cpp-lib/c_api.cpp
    src-cpp-lib/email.cpp
    src-cpp-lib/func.cpp
    src-cpp-lib/security.cpp
)

target_include_directories(password_manager_backend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src-cpp-lib
)

target_link_libraries(password_manager_backend
    PRIVATE
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

# Export C++ symbols on Windows so the DLL can be used by a future bridge.
set_target_properties(password_manager_backend PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    WINDOWS_EXPORT_ALL_SYMBOLS YES
)

include(GNUInstallDirs)
install(TARGETS password_manager_backend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
