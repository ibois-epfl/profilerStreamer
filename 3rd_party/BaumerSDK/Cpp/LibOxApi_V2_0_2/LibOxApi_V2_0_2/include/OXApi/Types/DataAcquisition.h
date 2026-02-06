/*
 * OXApi/Types/DataAcquisition.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_DATAACQUISITION_H_
#define OXAPI_TYPES_DATAACQUISITION_H_

#include <string>
#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct LaserPowerInfo {
    const std::string FactorUnit;
    const double FactorPrecision;
};

struct LaserPowerLimits {
    const double MinFactor;
    const double MaxFactor;
    const std::vector<double> PredefinedFactors;
};

struct ExposureTimeInfo {
    const std::string Unit;
};

struct ExposureTimeLimits {
    const std::uint32_t Minimum;
    const std::uint32_t Maximum;
};

struct Trigger {
    std::uint32_t Mode;
    std::uint32_t Option;
    std::uint32_t Time;
    std::uint32_t EncoderSteps;
};

struct TriggerLimits {
    const std::uint32_t MinTime;
    const std::uint32_t MaxTime;
    const std::uint32_t MinSteps;
    const std::uint32_t MaxSteps;
};

struct TriggerMode
{
    const std::uint32_t Id;
    const std::string Name;
    const std::vector<std::uint32_t> Options;
};

struct TriggerOption
{
    const std::uint32_t Id;
    const std::string Name;
};

struct TriggerInfo {
    const std::string TimeUnit;
    const std::vector<TriggerMode> TriggerModes;
    const std::vector<TriggerOption> TriggerOptions;
};

struct Resolution {
    std::uint32_t XResolution;
    std::uint32_t ZResolution;
};

struct ResolutionInfo {
    const std::vector<std::uint32_t> XResolutions;
    const std::vector<std::uint32_t> ZResolutions;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_DATAACQUISITION_H_ */
