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
 *   This file implements TCP/IPv6 sockets.
 */

#include "tcp6.hpp"

#include "common/code_utils.hpp"
#include "common/error.hpp"

namespace ot {
namespace Ip6 {

Tcp::Tcp(Instance &aInstance)
    : InstanceLocator(aInstance)
{
}

Error Tcp::EndpointInitialize(Endpoint &aEndpoint, otTcpEndpointInitializeArgs &aArgs)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aArgs);

    return kErrorNotImplemented;
}

Error Tcp::SetEstablishedCallback(Endpoint &aEndpoint, otTcpEstablished aEstablishedCallback)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aEstablishedCallback);

    return kErrorNotImplemented;
}

Error Tcp::SetSendDoneCallback(Endpoint &aEndpoint, otTcpSendDone aSendDoneCallback)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aSendDoneCallback);

    return kErrorNotImplemented;
}

Error Tcp::SetSendReadyCallback(Endpoint &aEndpoint, otTcpSendReady aSendReadyCallback)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aSendReadyCallback);

    return kErrorNotImplemented;
}

Error Tcp::SetReceiveAvailableCallback(Endpoint &aEndpoint, otTcpReceiveAvailable aReceiveAvailableCallback)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aReceiveAvailableCallback);

    return kErrorNotImplemented;
}

Error Tcp::SetDisconnectedCallback(Endpoint &aEndpoint, otTcpDisconnected aDisconnectedCallback)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aDisconnectedCallback);

    return kErrorNotImplemented;
}

Error Tcp::Bind(Endpoint &aEndpoint, const SockAddr &aSockName)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aSockName);

    return kErrorNotImplemented;
}

Error Tcp::Connect(Endpoint &aEndpoint, const SockAddr &aSockName, uint32_t aFlags)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aSockName);
    OT_UNUSED_VARIABLE(aFlags);

    return kErrorNotImplemented;
}

Error Tcp::SendByReference(Endpoint &aEndpoint, otLinkedBuffer &aBuffer, uint32_t aFlags)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aBuffer);
    OT_UNUSED_VARIABLE(aFlags);

    return kErrorNotImplemented;
}

Error Tcp::SendByExtension(Endpoint &aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aNumBytes);
    OT_UNUSED_VARIABLE(aFlags);

    return kErrorNotImplemented;
}

Error Tcp::ReceiveByReference(Endpoint &aEndpoint, const otLinkedBuffer *&aBuffer)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aBuffer);

    return kErrorNotImplemented;
}

Error Tcp::ReceiveContiguify(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return kErrorNotImplemented;
}

Error Tcp::CommitReceive(Endpoint &aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_UNUSED_VARIABLE(aNumBytes);
    OT_UNUSED_VARIABLE(aFlags);

    return kErrorNotImplemented;
}

Error Tcp::SendEndOfStream(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return kErrorNotImplemented;
}

Error Tcp::Abort(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return kErrorNotImplemented;
}

Error Tcp::EndpointDeinitialize(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return kErrorNotImplemented;
}

Error Tcp::ListenerInitialize(Listener &aListener, otTcpListenerInitializeArgs &aArgs)
{
    OT_UNUSED_VARIABLE(aListener);
    OT_UNUSED_VARIABLE(aArgs);

    return kErrorNotImplemented;
}

Error Tcp::SetAcceptReadyCallback(Listener &aListener, otTcpAcceptReady aAcceptReadyCallback)
{
    OT_UNUSED_VARIABLE(aListener);
    OT_UNUSED_VARIABLE(aAcceptReadyCallback);

    return kErrorNotImplemented;
}

Error Tcp::SetAcceptDoneCallback(Listener &aListener, otTcpAcceptDone aAcceptDoneCallback)
{
    OT_UNUSED_VARIABLE(aListener);
    OT_UNUSED_VARIABLE(aAcceptDoneCallback);

    return kErrorNotImplemented;
}

Error Tcp::Listen(Listener &aListener, const SockAddr &aSockName)
{
    OT_UNUSED_VARIABLE(aListener);
    OT_UNUSED_VARIABLE(aSockName);

    return kErrorNotImplemented;
}

Error Tcp::StopListening(Listener &aListener)
{
    OT_UNUSED_VARIABLE(aListener);

    return kErrorNotImplemented;
}

Error Tcp::ListenerDeinitialize(Listener &aListener)
{
    OT_UNUSED_VARIABLE(aListener);

    return kErrorNotImplemented;
}

Error Tcp::ProcessReceivedSegment(Message &aMessage, MessageInfo &aMessageInfo)
{
    OT_UNUSED_VARIABLE(aMessage);
    OT_UNUSED_VARIABLE(aMessageInfo);

    return kErrorNotImplemented;
}

} // namespace Ip6
} // namespace ot
