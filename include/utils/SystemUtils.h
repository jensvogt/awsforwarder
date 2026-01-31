//
// Created by vogje01 on 1/25/26.
//

#ifndef AWSFORWARDER_SYSTEM_UTILS_H
#define AWSFORWARDER_SYSTEM_UTILS_H

#ifdef _WIN32
#include <iostream>
#include <comdef.h>
#include <WbemIdl.h>
#pragma comment(lib, "wbemuuid.lib")
#endif

// Qt includes
#include <QProcess>

// Forwarder includes
#include <utils/LoggingUtils.h>

struct RunCommandResult {
    QString stdOut;
    QString stdErr;
    int exitCode{};
    long pid{};
};

class SystemUtils : QObject {
    Q_OBJECT

public:
    SystemUtils() {
        connect(&EventBus::instance(), &EventBus::ShutdownSignal, [this]() {
            StopProcesses();
        });
    };

    void RunCommandAsync(const QString &nameSpace, const QString &command, const QStringList &arguments, QObject *parent = nullptr) {
        auto *_process = new QProcess(this);

        // 1. Capture the output
        connect(_process, &QProcess::readyReadStandardOutput, [_process]() {
            log_info("StdOut:" + _process->readAllStandardOutput().trimmed());
        });

        // 2. Capture errors
        connect(_process, &QProcess::readyReadStandardError, [_process]() {
            log_info("StdErr:" + _process->readAllStandardError().trimmed());
        });

        // 3. Cleanup when finished
        connect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [_process](const int exitCode, QProcess::ExitStatus exitStatus) {
            log_info("Process finished with code:" + QString::number(exitCode));
            _process->deleteLater();
        });

        // Start the executable (e.g., kubectl) with arguments
        _process->start(command, arguments);
        if (_process->waitForStarted()) {
            log_info("Forwarder background process started, nameSpace: "+nameSpace+", PID: " + QString::number(_process->processId()));
            _processes.append(_process);
        } else {
            log_error("Could not start forwarder process, nameSpace: " +nameSpace);
        }
    }

    RunCommandResult RunCommandSync(const QString &command, const QStringList &arguments, QObject *parent = nullptr) {
        RunCommandResult result;

        auto *_process = new QProcess(this);

        _process->start(command, arguments);

        // Wait up to 30 seconds for it to finish
        if (_process->waitForFinished(30000)) {
            result.stdOut = _process->readAllStandardOutput().trimmed();
            result.stdErr = _process->readAllStandardError().trimmed();
            result.pid = _process->processId();
        } else {
            result.stdErr = _process->readAllStandardError().trimmed();
            log_error("Timeout or Error::" + result.stdErr);
        }
        result.exitCode = _process->exitCode();
        _process->deleteLater();
        return result;
    }

    void StopProcesses() {
        for (const auto &process: _processes) {
            if (process->state() == QProcess::Running) {
                process->kill();
            };
        }
    }

    static long FindProcessByName(const QString &fullName) {
#ifdef _WIN32
        QProcess p;
        p.start("tasklist", {"/FI", "IMAGENAME eq kubectl.exe /FO TABLE /NH"});
        p.waitForFinished();

        QString output = p.readAllStandardOutput();
        if (output.isEmpty()) {
            return -1;
        }
        return output.split(' ')[1].toInt();
#else
        QProcess p;
        p.start("pgrep", QStringList() << fullName);
        p.waitForFinished();
        if (!p.readAllStandardOutput().isEmpty()) {
            return -1;
        }
        return p.readAllStandardOutput().toLongLong();
#endif
    }

#ifdef WIN32
    static QString GetProcessCommandLine(const DWORD processId) {
        QString result;
        HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hr)) return {};

        hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);

        IWbemLocator *pLoc = nullptr;
        hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc);

        IWbemServices *pSvc = nullptr;
        hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, 0, 0, 0, 0, &pSvc);

        // Query for the specific process
        std::wstring query = L"SELECT CommandLine FROM Win32_Process WHERE ProcessId = " + std::to_wstring(processId);

        IEnumWbemClassObject *pEnumerator = nullptr;
        hr = pSvc->ExecQuery(bstr_t("WQL"), bstr_t(query.c_str()), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &pEnumerator);

        IWbemClassObject *pclsObj = nullptr;
        ULONG uReturn = 0;

        if (pEnumerator) {
            hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (uReturn != 0) {
                VARIANT vtProp;
                hr = pclsObj->Get(L"CommandLine", 0, &vtProp, 0, 0);
                if (vtProp.vt == VT_BSTR) {
                    result = QString(vtProp.bstrVal);
                }
                VariantClear(&vtProp);
                pclsObj->Release();
            }
        }

        // Cleanup
        if (pSvc) pSvc->Release();
        if (pLoc) pLoc->Release();
        if (pEnumerator) pEnumerator->Release();
        CoUninitialize();
        return result;
    }
#endif

    static long ProcessExistsByName(const QString &fullName) {
        return FindProcessByName(fullName) > 0;
    }

    static QString KillProcessByName(const QString &fullName) {
        const long pid = FindProcessByName(fullName);
        QProcess p;
        p.start("kill", QStringList() << "-9" << QString::number(pid));
        p.waitForFinished();
        return p.readAllStandardOutput();
    }

private:
    QList<QProcess *> _processes;
};
#endif //AWSFORWARDER_SYSTEM_UTILS_H
