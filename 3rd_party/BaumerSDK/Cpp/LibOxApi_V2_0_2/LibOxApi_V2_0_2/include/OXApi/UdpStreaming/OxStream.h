/*
 * OXApi/UdpStreaming/OxStream.h
 * Copyright: Baumer Group
 */

#ifndef OXAPI_UDPSTREAMING_OXSTREAM_H_
#define OXAPI_UDPSTREAMING_OXSTREAM_H_

#include <OXApi/UdpStreaming/PacketError.h>
#include <OXApi/UdpStreaming/ProfilePacket.h>
#include <OXApi/UdpStreaming/MeasurementPacket.h>
#include <OXApi/UdpStreaming/FullQueueHandling.h>

namespace Baumer {
namespace OXApi {
namespace UdpStreaming {

class OxStream
{
public:
    virtual ~OxStream( ) = default;

    /**
     * @brief  Starts reading data from the streaming sensor.
     */
    virtual void Start( ) = 0;

    /**
     * @brief  Stops reading data from the streaming sensor.
     */
    virtual void Stop( ) = 0;

    /**
     * @brief  Closes the stream.
     */
    virtual void Close( ) = 0;

    /**
     * @brief  Get the length of all queues. The default value is 10000.
     */
    virtual std::size_t GetQueueSize( ) const = 0;

    /**
     * @brief  Set the length of all queues. The default value is 10000.
     */
    virtual void SetQueueSize( const std::size_t &newSize ) = 0;

    /**
     * @brief  Get the behavior if the queue is full and new data arrives.
     */
    virtual FullQueueHandling GetFullQueueHandling( ) const = 0;

    /**
     * @brief  Defines the behavior if the queue is full and new data arrives.
     */
    virtual void SetFullQueueHandling( const FullQueueHandling handling ) = 0;

    /**
     * @brief Get the UDP socket receive buffer size.
     */
    virtual std::size_t GetReceiveBufferSize( ) const = 0;

    /**
     * @brief Set the UDP socket receive buffer size.
     */
    virtual void SetReceiveBufferSize( const std::size_t &newSize ) = 0;

    /**
     * @brief  Returns true if at least one error occured.
     */
    virtual bool ErrorOccured() const = 0;

    /**
     * @brief  Reads one error from the queue (the error will be removed from the queue).
     * @throw  Throws an exception if the queue is empty.
     * @return The oldest queued error.
     */
    virtual const PacketError ReadError() = 0;

    /**
     * @brief  Reads one measurement from the queue (the measurement will be removed from the queue).
     * @throw  Throws an exception if the queue is empty.
     * @return The oldest queued measurement.
     */
    virtual const MeasurementPacket ReadMeasurement() = 0;

    /**
     * @brief  Returns true if at least one measurement is available.
     */
    virtual bool MeasurementAvailable() const = 0;

    /**
     * @brief  The number of queued measurements.
     */
    virtual std::size_t MeasurementCount() const = 0;

    /**
     * @brief  Clears the measurement queue.
     */
    virtual void ClearMeasurementQueue() = 0;

    /**
     * @brief  Reads one profile from the queue (the profile will be removed from the queue).
     * @throw  Throws an exception if the queue is empty.
     * @return The oldest queued profile.
     */
    virtual const ProfilePacket ReadProfile() = 0;

    /**
     * @brief  Returns true if at least one profile is available.
     */
    virtual bool ProfileAvailable() const = 0;

    /**
     * @brief  The number of queued profiles.
     */
    virtual std::size_t ProfileCount() const = 0;

    /**
     * @brief  Clears the profile queue.
     */
    virtual void ClearProfileQueue() = 0;
};

} /* namespace UdpStreaming */
} /* namespace OXApi */
} /* namespace Baumer */

#endif /* OXAPI_UDPSTREAMING_OXSTREAM_H_ */
