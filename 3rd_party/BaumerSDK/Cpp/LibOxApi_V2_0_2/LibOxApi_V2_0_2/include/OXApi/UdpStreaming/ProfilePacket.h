/*
 * OXApi/UdpStreaming/ProfilePacket.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_PROFILEPACKET_H_
#define OXAPI_UDPSTREAMING_PROFILEPACKET_H_

#include <vector>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

struct ProfilePacket
{
    std::uint32_t BlockId;
    bool ConfigModeActive;
    bool TimeSyncedByNtp;
    bool ValuesValid;
    bool Alarm;
    std::uint8_t Quality;
    float MeasurementRateHz;
    std::uint64_t TimeStamp;
    std::uint16_t Encoder;
    std::uint32_t Length;
    std::vector<std::int32_t> X;
    std::vector<std::uint32_t> Z;
    std::vector<std::uint16_t> I;
    bool ZValid;
    bool IntensityValid;
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_PROFILEPACKET_H_ */
