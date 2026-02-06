/*
 * OXApi/Types/FieldOfView.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_FIELDOFVIEW_H_
#define OXAPI_TYPES_FIELDOFVIEW_H_

#include <string>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct FieldOfView {
    double LimitLeft;
    double LimitRight;
    double Offset;
    double Height;
};

struct FieldOfViewDistance {
    double LimitLeft;
    double LimitRight;
    double Near;
    double Far;
};

struct FieldOfViewLimits {
    const double MaxXMinus;
    const double MaxXPlus;
    const double MinWidth;
    const double MinHeight;
    const double MaxHeight;
    const double MinDistance;
    const double MaxDistance;
};

struct FieldOfViewInfo {
    const std::string XUnit;
    const std::string ZUnit;
    const std::uint32_t XPrecision;
    const std::uint32_t ZPrecision;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_FIELDOFVIEW_H_ */
