cmake_minimum_required(VERSION 3.28)

# Read version.txt
include(cmake/ReadVersion.cmake)
include(cmake/UpdateXmlVersion.cmake)
include(cmake/UpdateReleaseDate.cmake)

project(awsforwarder VERSION ${VERSION_STRING} LANGUAGES CXX)

message(STATUS "Configured version: ${PROJECT_VERSION}")

# Export version to source code
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.in ${CMAKE_SOURCE_DIR}/include/Version.h)
update_xml_version("${CMAKE_SOURCE_DIR}/installer/config/config.xml" "${VERSION_STRING}" "Version")
update_xml_version("${CMAKE_SOURCE_DIR}/installer/packages/de.jensvogt.awsforwarder/meta/package.xml" "${VERSION_STRING}" "Version")
update_release_date("${CMAKE_SOURCE_DIR}/installer/packages/de.jensvogt.awsforwarder/meta/package.xml")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    add_compile_options(/Zc:__cplusplus)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_PREFIX_PATH "c:\\work\\private\\Qt\\6.10.1\\msvc2022_64")
    set(CMAKE_PREFIX_PATH "C:\\work\\private\\extern\\vcpkg\\installed\\x64-windows")
    set(INSTALL_DIR "C:/Program Files (x86)/awsforwarder")
    set(AWSSDK_INCLUDE_DIR "C:/Program Files (x86)/aws-cpp-sdk-all/include")
    set(AWSSDK_LIB_DIR "C:/Program Files (x86)/aws-cpp-sdk-all/bin")
    set(LIBSSL_INCLUDE_DIR "C:/Program Files (x86)/OpenSSL-Win64/include")
    set(LIBSSL_LIB_DIR "C:/Program Files (x86)/OpenSSL-Win64/lib/VC/x64/MT")
    set(YAML_CPP_INCLUDE_DIR "C:/Program Files (x86)/YAML_CPP/include")
    set(YAML_CPP_LIB_DIR "C:/Program Files (x86)/YAML_CPP/lib")
    set(WINDOWS_ICON ${CMAKE_SOURCE_DIR}/appicon.rc)
    set(APP_ICON_RESOURCE_WINDOWS "${WINDOWS_ICON}")
else ()
    set(EXTERNAL_LIB_DIR /usr/local/lib)
    set(EXTERNAL_INC_DIR /usr/local/include)
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg Network Xml)
find_package(AWSSDK CONFIG COMPONENTS core secretsmanager sts eks REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

qt_add_resources(APP_RESOURCES resources/awsforwarder.qrc)

set(PROJECT_SOURCES
        src/main.cpp
        src/utils/IconUtils.cpp
        include/utils/IconUtils.h
        include/utils/JsonUtils.h
        include/utils/Configuration.h
        src/utils/Configuration.cpp
        src/MainWidget.cpp
        include/MainWidget.h
        src/MainWidget.ui
        include/utils/ForwarderConfig.h
        include/utils/AwsUtils.h
        src/utils/AwsUtils.cpp
        include/utils/OneLoginUtils.h
        include/utils/Otp.h
        include/utils/Bytes.h
        src/utils/Bytes.cpp
        src/utils/Otp.cpp
        include/utils/OtpUtils.h
        src/utils/OtpUtils.cpp
        include/dto/onelogin/OneLoginApiTokenRequest.h
        include/dto/onelogin/OneLoginApiTokenResponse.h
        include/dto/onelogin/SamlAssertionRequest.h
        include/dto/onelogin/SamlAssertionResponse.h
        include/utils/EventBus.h
        src/utils/OneLoginUtils.cpp
        src/utils/Sha1.cpp
        include/utils/BaseService.h
        include/utils/RestManager.h
        src/utils/RestManager.cpp
        include/service/OneLoginService.h
        src/service/OneLoginService.cpp
        include/utils/LoggingUtils.h
        include/secrets/SecretsWorker.h
        src/secrets/SecretsWorker.cpp
        src/ServiceDetailsDialog.cpp
        include/ServiceDetailsDialog.h
        src/ServiceDetailsDialog.ui
        include/MainWindow.h
        src/MainWindow.cpp
        include/utils/KubernetesUtils.h
        include/service/KubernetesService.h
        src/service/KubernetesService.cpp
        src/utils/KubernetesUtils.cpp
        include/kubernetes/KubernetesWorker.h
        src/kubernetes/KubernetesWorker.cpp
        include/dto/kubernetes/Pod.h
        include/dto/kubernetes/ObjectMeta.h
        include/dto/kubernetes/PodSpec.h
        include/dto/kubernetes/Container.h
        include/dto/kubernetes/Environment.h
        include/dto/kubernetes/Forwarder.h
        include/dto/kubernetes/Item.h
        include/dto/kubernetes/PullPolicy.h
        src/session/SessionWorker.cpp
        include/session/SessionWorker.h
        include/utils/SystemUtils.h
)

qt_add_executable(awsforwarder MANUAL_FINALIZATION ${PROJECT_SOURCES} ${APP_ICON_RESOURCE_WINDOWS} ${APP_RESOURCES})

target_link_libraries(awsforwarder PRIVATE Qt::Core Qt::Gui Qt::Widgets Qt6::Network Qt6::Xml ${AWSSDK_LIBRARIES} yaml-cpp::yaml-cpp)
target_include_directories(awsforwarder PRIVATE ./include ${AWSSDK_INCLUDE_DIRS})

set_target_properties(awsforwarder PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}" MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE)

if (WIN32)
    install(TARGETS awsforwarder BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_DIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else ()
    include(GNUInstallDirs)
    install(TARGETS awsforwarder BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    # Generate the deployment script for the target MyApp.
    qt_generate_deploy_app_script(TARGET awsforwarder OUTPUT_SCRIPT deploy_script NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${deploy_script})
endif ()

qt_finalize_executable(awsforwarder)