/*
 * OXApi/Types/Profile.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_PROFILE_H_
#define OXAPI_TYPES_PROFILE_H_

#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct Profile {
    const std::uint32_t QualityId;
    const std::uint64_t TimeStamp;
    const std::uint32_t Precision;
    const std::int32_t XStart;
    const std::uint32_t Length;
    const std::vector<std::int32_t> X;
    const std::vector<std::int32_t> Z;
};

struct ProfileInfo {
    const std::uint32_t MaxLength;
    const std::string XUnit;
    const std::string ZUnit;
};

struct IntensityProfile {
    const std::uint32_t QualityId;
    const std::uint64_t TimeStamp;
    const std::uint32_t Precision;
    const std::int32_t XStart;
    const std::uint32_t Length;
    const std::vector<std::int32_t> X;
    const std::vector<std::int32_t> Z;
    const std::vector<std::int32_t> I;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_PROFILE_H_ */
