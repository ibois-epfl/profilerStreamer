/*
 * OXApi/UdpStreaming/MeasurementPacket.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_MEASUREMENTPACKET_H_
#define OXAPI_UDPSTREAMING_MEASUREMENTPACKET_H_

#include <array>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

struct MeasurementPacket
{
    std::uint32_t BlockId;
    std::uint32_t Quality;
    bool ConfigModeActive;
    bool TimeSyncedByNtp;
    bool ValuesValid;
    bool Alarm;
    std::array<bool, 2> DigitalOuts;
    std::uint32_t EncoderValue;
    double MeasurementRate;
    std::uint64_t TimeStamp;
    std::array<double, 7> Values;
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_MEASUREMENTPACKET_H_ */
