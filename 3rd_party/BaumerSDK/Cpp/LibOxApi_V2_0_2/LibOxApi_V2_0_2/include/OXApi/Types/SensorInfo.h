/*
 * OXApi/Types/SensorInfo.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_SENSORINFO_H_
#define OXAPI_TYPES_SENSORINFO_H_

#include <string>

namespace Baumer {
namespace OXApi {
namespace Types {

struct SensorInfo {
    const std::string Type;
    const std::string SerialNumber;
    const std::string VendorName;
    const std::string AggregateVersion;
    const std::string SoftwareVersion;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_SENSORINFO_H_ */
