//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_PULL_POLICY_H
#define AWSFORWARDER_DTO_KUBERNETES_PULL_POLICY_H

// C++ includes
#include <map>
#include <string>

enum PullPolicy {
    PullAlways,
    PullNever,
    PullIfNotPresent,
    Unknown
};

static std::map<PullPolicy, QString> PullPolicyNames{
    {PullAlways, "Always"},
    {PullNever, "Never"},
    {PullIfNotPresent, "IfNotPresent"},
    {Unknown, "Unknown"},
};

[[maybe_unused]] static QString PullPolicyToString(const PullPolicy pullPolicy) {
    return PullPolicyNames[pullPolicy];
}

[[maybe_unused]] static PullPolicy PullPolicyFromString(const QString &pullPolicy) {
    for (auto &[fst, snd]: PullPolicyNames) {
        if (snd == pullPolicy) {
            return fst;
        }
    }
    return Unknown;
}

#endif// AWSFORWARDER_DTO_KUBERNETES_PULL_POLICY_H
