/*
 * OXApi/UdpStreaming/FullQueueHandling.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_FULLQUEUEHANDLING_H_
#define OXAPI_UDPSTREAMING_FULLQUEUEHANDLING_H_

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

enum class FullQueueHandling
{
    /**
     * @brief  If the queue is full and new data is received, the oldest data will be dropped.
     */
    DropOldest,
    /**
     * @brief  If the queue is full and new data is received, the new data will be dropped.
     */
    IgnoreNew,
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_FULLQUEUEHANDLING_H_ */
