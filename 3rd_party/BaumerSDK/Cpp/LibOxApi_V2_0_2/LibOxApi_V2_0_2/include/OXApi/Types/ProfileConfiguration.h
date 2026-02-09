/*
 * OXApi/Types/ProfileConfiguration.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_PROFILECONFIGURATION_H_
#define OXAPI_TYPES_PROFILECONFIGURATION_H_

#include <vector>
#include <string>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct ResamplingGrid {
    bool Enabled;
    double GridValue;
};

struct ResamplingInfo {
    const std::string GridUnit;
    const std::uint32_t GridPrecision;
    const double MinimumGridValue;
    const double MaximumGridValue;
};

struct ProfileFilter {
    bool Enabled;
    std::uint32_t MovingAverageLength;
};

struct ProfileFilterLimits {
    const std::uint32_t MinimumLength;
    const std::uint32_t MaximumLength;
};

struct ProfileAlgorithm {
    const std::uint32_t Id;
    const std::string Name;
};

struct ProfileAlgorithmInfo {
    const std::vector<ProfileAlgorithm> Algorithms;
};

struct ProfileAlgorithmThresholdType {
    const std::uint32_t TypeId;
    const std::string Name;
};

struct Limit {
    const std::uint32_t Minimum;
    const std::uint32_t Maximum;
};

struct ProfileAlgorithmParamsLimit {
    const Limit MinPeakHeight;
    const Limit MinPeakWidth;
    const Limit ThresholdValue;
    const std::vector<ProfileAlgorithmThresholdType> ThresholdTypes;
};

struct ProfileAlgorithmParamsLimits {
    const std::uint32_t AlgorithmId;
    const ProfileAlgorithmParamsLimit Limit;
};

struct ProfileAlgorithmParamsInfo {
    const std::string MinPeakHeightUnit;
    const std::string MinPeakWidthUnit;
    const std::string ThresholdUnit;
};

struct ProfileAlgorithmParameters {
    std::uint32_t MinPeakHeight;
    std::uint32_t MinPeakWidth;
    std::uint32_t ThresholdValue;
    std::uint32_t ThresholdType;
};

struct ZAxisDefinition {
    const std::uint32_t Id;
    const std::string Name;
};

struct AxesInfo {
    const std::vector<ZAxisDefinition> ZAxisDefinitions;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_PROFILECONFIGURATION_H_ */
