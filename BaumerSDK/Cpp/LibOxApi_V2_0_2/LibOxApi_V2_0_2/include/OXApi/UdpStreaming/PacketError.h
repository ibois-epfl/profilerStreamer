/*
 * OXApi/UdpStreaming/PacketError.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_PACKETERROR_H_
#define OXAPI_UDPSTREAMING_PACKETERROR_H_

#include <OXApi/UdpStreaming/PacketErrorType.h>

#include <string>
#include <cstdint>

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

struct PacketError
{
    const std::uint32_t BlockId;
    const PacketErrorType ErrorType;
    const std::string Message;
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_PACKETERROR_H_ */
