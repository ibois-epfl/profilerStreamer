/*
 * OXApi/Types/MeasureValues.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_MEASUREVALUES_H_
#define OXAPI_TYPES_MEASUREVALUES_H_

#include <vector>
#include <string>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct MeasurementValue {
    const std::uint32_t Quality;
    const bool ConfigModeActive;
    const bool ValuesValid;
    const bool Alarm;
    const std::vector<bool> DigitalOuts;
    const std::uint32_t EncoderValue;
    const double MeasurementRate;
    const std::uint64_t TimeStamp;
    const std::vector<double> Values;
};

struct QualityValue {
    const std::uint32_t Id;
    const std::string Name;
};

struct MeasurementInfo {
    const std::vector<std::string> TimeStampUnits;
    const std::vector<QualityValue> QualityValues;
    const std::string MeasurementRateUnit;
    const std::uint32_t MeasurementRatePrecision;
};

struct MeasurementType {
    const std::uint32_t ToolId;
    const std::string Mode;
    const std::string Tool;
    const std::string Name;
    const std::string Unit;
    const double Precision;
    const double Minimum;
    const double Maximum;
};

struct MeasurementValuesInfo {
    const std::vector<MeasurementType> MeasurementTypes;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_MEASUREVALUES_H_ */
