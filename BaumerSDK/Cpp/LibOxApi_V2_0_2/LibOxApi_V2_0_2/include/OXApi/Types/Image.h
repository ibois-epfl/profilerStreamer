/*
 * OXApi/Types/Image.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_TYPES_IMAGE_H_
#define OXAPI_TYPES_IMAGE_H_

#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace Types {

struct Image {
    const std::uint32_t RoiHeight;
    const std::uint32_t RoiWidth;
    const std::uint32_t RowOffset;
    const std::uint32_t ColumnOffset;
    const std::uint32_t RowBinning;
    const std::uint32_t ColumnBinning;
    const std::vector<std::uint8_t> Pixels;
};

struct ImageInfo {
    const std::uint32_t SensorHeight;
    const std::uint32_t SensorWidth;
    const std::uint32_t MaxROIPixels;
};

} /* namespace Types */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_TYPES_IMAGE_H_ */
