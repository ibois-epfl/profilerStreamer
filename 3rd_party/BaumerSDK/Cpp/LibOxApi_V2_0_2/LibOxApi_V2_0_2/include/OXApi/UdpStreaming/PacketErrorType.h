/*
 * OXApi/UdpStreaming/PacketErrorType.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_PACKETERRORTYPE_H_
#define OXAPI_UDPSTREAMING_PACKETERRORTYPE_H_

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

enum class PacketErrorType
{
    PacketLost,
    PacketOutOfSequence,
    PacketDuplicateReceived,
    MultiframePacketTimeOut,
    InvalidPacketReceived,
    PacketContentEmpty,
    InvalidContentSize,
    UnknownError
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_PACKETERRORTYPE_H_ */
