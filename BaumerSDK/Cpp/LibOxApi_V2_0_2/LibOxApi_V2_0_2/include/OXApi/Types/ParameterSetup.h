/*
 * OXApi/Types/ParameterSetup.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_PARAMETERSETUP_H_
#define OXAPI_TYPES_PARAMETERSETUP_H_

#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct ActiveSetup {
    const std::uint32_t Number;
    const bool Saved;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_PARAMETERSETUP_H_ */
