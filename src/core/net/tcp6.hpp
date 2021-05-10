/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file includes definitions for TCP/IPv6 sockets.
 */

#ifndef TCP6_HPP_
#define TCP6_HPP_

#include "openthread-core-config.h"

#include <openthread/tcp.h>

#include "net/ip6_headers.hpp"
#include "net/socket.hpp"

#include "common/linked_list.hpp"
#include "common/locator.hpp"
#include "common/non_copyable.hpp"
#include "common/timer.hpp"

namespace ot {
namespace Ip6 {

/**
 * @addtogroup core-tcp
 *
 * @brief
 *   This module includes definitions for TCP/IPv6 sockets.
 *
 * @{
 *
 */
class Tcp : public InstanceLocator, private NonCopyable
{
public:
    /**
     * This class represents an endpoint of a TCP/IPv6 connection.
     *
     */
    class Endpoint : public otTcpEndpoint, public LinkedListEntry<Endpoint>
    {
        friend class Tcp;
        friend class LinkedList<Endpoint>;

        enum : uint8_t
        {
            kNumTimers = 4,
        };

        static_assert(sizeof(mTimers[0]) >= sizeof(TimerMilli), "mTimers entries are too small");
        static_assert(OT_ARRAY_LENGTH(mTimers) == kNumTimers, "mTimers has an incorrect array length");

        void Initialize(Instance &aInstance);
        void Deinitialize();

        TimerMilli &GetTimer(std::size_t aIndex)
        {
            return *reinterpret_cast<TimerMilli *>(&mTimers[aIndex].mSize[0]);
        }

        static Endpoint &FromTimer(TimerMilli &aTimer, std::size_t aIndex)
        {
            uint8_t *ptr = reinterpret_cast<uint8_t *>(&aTimer);
            ptr -= offsetof(Endpoint, mTimers[aIndex].mSize[0]);
            return *reinterpret_cast<Endpoint *>(ptr);
        }

        TimerMilli &GetDelackTimer() { return GetTimer(0); }
        static Endpoint &FromDelackTimer(TimerMilli &aTimer) { return FromTimer(aTimer, 0); }

        TimerMilli &GetRexmtPersistTimer() { return GetTimer(1); }
        static Endpoint &FromRexmtPersistTimer(TimerMilli &aTimer) { return FromTimer(aTimer, 1); }

        TimerMilli &GetKeepTimer() { return GetTimer(2); }
        static Endpoint &FromKeepTimer(TimerMilli &aTimer) { return FromTimer(aTimer, 2); }

        TimerMilli &Get2MslTimer() { return GetTimer(3); }
        static Endpoint &From2MslTimer(TimerMilli &aTimer) { return FromTimer(aTimer, 3); }
    };

    /**
     * This class represents a TCP/IPv6 listener.
     */
    class Listener : public otTcpListener, public LinkedListEntry<Listener>
    {
        friend class Tcp;
        friend class LinkedList<Listener>;
    };

    /**
     * This class implements TCP header parsing.
     *
     */
    OT_TOOL_PACKED_BEGIN
    class Header
    {
    public:
        enum : uint8_t
        {
            kChecksumFieldOffset = 16, ///< The byte offset of the Checksum field in the TCP header.
        };

        /**
         * This method returns the TCP Source Port.
         *
         * @returns The TCP Source Port.
         *
         */
        uint16_t GetSourcePort(void) const { return HostSwap16(mSource); }

        /**
         * This method returns the TCP Destination Port.
         *
         * @returns The TCP Destination Port.
         *
         */
        uint16_t GetDestinationPort(void) const { return HostSwap16(mDestination); }

        /**
         * This method returns the TCP Sequence Number.
         *
         * @returns The TCP Sequence Number.
         *
         */
        uint32_t GetSequenceNumber(void) const { return HostSwap32(mSequenceNumber); }

        /**
         * This method returns the TCP Acknowledgment Sequence Number.
         *
         * @returns The TCP Acknowledgment Sequence Number.
         *
         */
        uint32_t GetAcknowledgmentNumber(void) const { return HostSwap32(mAckNumber); }

        /**
         * This method returns the TCP Flags.
         *
         * @returns The TCP Flags.
         *
         */
        uint16_t GetFlags(void) const { return HostSwap16(mFlags); }

        /**
         * This method returns the TCP Window.
         *
         * @returns The TCP Window.
         *
         */
        uint16_t GetWindow(void) const { return HostSwap16(mWindow); }

        /**
         * This method returns the TCP Checksum.
         *
         * @returns The TCP Checksum.
         *
         */
        uint16_t GetChecksum(void) const { return HostSwap16(mChecksum); }

        /**
         * This method returns the TCP Urgent Pointer.
         *
         * @returns The TCP Urgent Pointer.
         *
         */
        uint16_t GetUrgentPointer(void) const { return HostSwap16(mUrgentPointer); }

    private:
        uint16_t mSource;
        uint16_t mDestination;
        uint32_t mSequenceNumber;
        uint32_t mAckNumber;
        uint16_t mFlags;
        uint16_t mWindow;
        uint16_t mChecksum;
        uint16_t mUrgentPointer;

    } OT_TOOL_PACKED_END;

    /**
     * This constructor initializes the object.
     *
     * @param[in] aInstance  A reference to the OpenThread instance.
     *
     */
    explicit Tcp(Instance &aInstance);

    /**
     * See documentation for otTcpEndpointInitialize in include/openthread/tcp.h.
     */
    Error Initialize(Endpoint &aEndpoint, otTcpEndpointInitializeArgs &aArgs);

    /**
     * See documentation for otTcpEndpointGetInstance in include/openthread/tcp.h.
     */
    static Instance *GetInstance(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpEndpointGetContext in include/openthread/tcp.h.
     */
    static void *GetContext(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpGetLocalAddress in include/openthread/tcp.h.
     */
    static const SockAddr &GetLocalAddress(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpGetPeerAddress in include/openthread/tcp.h.
     */
    static const SockAddr &GetPeerAddress(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpBind in include/openthread/tcp.h.
     */
    Error Bind(Endpoint &aEndpoint, const SockAddr &aSockName);

    /**
     * See documentation for otTcpConnect in include/openthread/tcp.h.
     */
    Error Connect(Endpoint &aEndpoint, const SockAddr &aSockName, uint32_t aFlags);

    /**
     * See documentation for otTcpSendByReference in include/openthread/tcp.h.
     */
    Error SendByReference(Endpoint &aEndpoint, otLinkedBuffer &aBuffer, uint32_t aFlags);

    /**
     * See documentation for otTcpSendByExtension in include/openthread/tcp.h.
     */
    Error SendByExtension(Endpoint &aEndpoint, size_t aNumBytes, uint32_t aFlags);

    /**
     * See documentation for otTcpReceiveByReference in include/openthread/tcp.h.
     */
    Error ReceiveByReference(Endpoint &aEndpoint, const otLinkedBuffer *&aBuffer);

    /**
     * See documentation for otTcpReceiveContiguify in include/openthread/tcp.h.
     */
    Error ReceiveContiguify(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpCommitReceive in include/openthread/tcp.h.
     */
    Error CommitReceive(Endpoint &aEndpoint, size_t aNumBytes, uint32_t aFlags);

    /**
     * See documentation for otTcpSendEndOfStream in include/openthread/tcp.h.
     */
    Error SendEndOfStream(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpAbort in include/openthread/tcp.h.
     */
    Error Abort(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpEndpointDeinitialize in include/openthread/tcp.h.
     */
    Error Deinitialize(Endpoint &aEndpoint);

    /**
     * See documentation for otTcpListenerInitialize in include/openthread/tcp.h.
     */
    Error Initialize(Listener &aListener, otTcpListenerInitializeArgs &aArgs);

    /**
     * See documentation for otTcpListenerGetInstance in include/openthread/tcp.h.
     */
    static Instance *GetInstance(Listener &aListener);

    /**
     * See documentation for otTcpListenerGetContext in include/openthread/tcp.h.
     */
    static void *GetContext(Listener &aListener);

    /**
     * See documentation for otTcpListen in include/openthread/tcp.h.
     */
    Error Listen(Listener &aListener, const SockAddr &aSockName);

    /**
     * See documentation for otTcpStopListening in include/openthread/tcp.h.
     */
    Error StopListening(Listener &aListener);

    /**
     * See documentation for otTcpListenerDeinitialize in include/openthread/tcp.h.
     */
    Error Deinitialize(Listener &aListener);

    /**
     * Processes a received TCP segment.
     *
     * @param[in]  aMessage      A reference to the message containing the TCP segment.
     * @param[in]  aMessageInfo  A refernce to the message info associated with @p aMessage.
     *
     * @retval kErrorNone  Successfully processed the TCP segment.
     * @retval kErrorDrop  Dropped the TCP segment due to an invalid checksum.
     *
     */
    Error ProcessReceivedSegment(Message &aMessage, MessageInfo &aMessageInfo);

private:
    enum
    {
        kDynamicPortMin = 49152, ///< Service Name and Transport Protocol Port Number Registry
        kDynamicPortMax = 65535, ///< Service Name and Transport Protocol Port Number Registry
    };

    static void HandleDelackTimer(Timer &aTimer);
    static void HandleRexmtPersistTimer(Timer &aTimer);
    static void HandleKeepTimer(Timer &aTimer);
    static void Handle2MslTimer(Timer &aTimer);

    LinkedList<Endpoint> mEndpoints;
    LinkedList<Listener> mListeners;
    uint16_t             mEphemeralPort;
};

} // namespace Ip6
} // namespace ot

#endif // TCP6_HPP_
