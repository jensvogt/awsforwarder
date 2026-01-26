cmake_minimum_required(VERSION 3.28)
project(awsforwarder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(INSTALL_DIR "C:/Program Files (x86)/awsloginserver")
    set(BOOST_INCLUDE_DIRS "C:/Program Files (x86)/boost/boost_1_89_0")
    set(BOOST_LIB_DIRS "C:/Program Files (x86)/boost/boost_1_89_0/lib64-msvc-14.3")
    set(AWSSDK_INCLUDE_DEBUG_DIRS "c:/Program Files (x86)/aws-sdk-cpp/debug/include")
    set(AWSSDK_LIB_DEBUG_DIRS "c:/Program Files (x86)/aws-sdk-cpp/debug/bin")
    set(AWSSDK_INCLUDE_RELEASE_DIRS "c:/Program Files (x86)/aws-sdk-cpp/release/include")
    set(AWSSDK_LIB_RELEASE_DIRS "c:/Program Files (x86)/aws-sdk-cpp/release/bin")
    set(LIBSSL_INCLUDE_DIRS "C:/Program Files (x86)/OpenSSL-Win64/include")
    set(LIBSSL_LIB_DIRS "C:/Program Files (x86)/OpenSSL-Win64/lib/VC/x64/MT")
else ()
    set(EXTERNAL_LIB_DIR /usr/local/lib)
    set(EXTERNAL_INC_DIR /usr/local/include)
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Xml)

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
        include/utils/OneLoginService.h
        src/utils/OneLoginService.cpp
        include/utils/LoggingUtils.h
        include/secrets/SecretsWorker.h
        src/secrets/SecretsWorker.cpp
        src/ServiceDetailsDialog.cpp
        include/ServiceDetailsDialog.h
        src/ServiceDetailsDialog.ui
        include/MainWindow.h
        src/MainWindow.cpp
        include/utils/KubernetesUtils.h
        include/utils/KubernetesService.h
        src/utils/KubernetesService.cpp
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

target_link_libraries(awsforwarder PRIVATE Qt::Core Qt::Gui Qt::Widgets Qt6::Network Qt6::Xml aws-cpp-sdk-sts aws-cpp-sdk-core aws-cpp-sdk-eks
        aws-cpp-sdk-secretsmanager yaml-cpp)
target_link_directories(awsforwarder PRIVATE ${EXTERNAL_INC_DIR})
target_include_directories(awsforwarder PRIVATE ./include ${EXTERNAL_INC_DIR} Qt6::Network Qt6::Xml)

qt_finalize_executable(awsforwarder)