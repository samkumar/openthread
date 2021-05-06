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
 *   This file implements the OpenThread TCP API.
 */

#include "openthread-core-config.h"

#include <openthread/tcp.h>

#include "common/instance.hpp"
#include "net/tcp6.hpp"

using namespace ot;

otError otTcpEndpointInitialize(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpEndpointInitializeArgs *aArgs)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().EndpointInitialize(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aArgs);
}

otError otTcpSetEstablishedCallback(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpEstablished aEstablishedCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetEstablishedCallback(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aEstablishedCallback);
}

otError otTcpSetSendDoneCallback(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpSendDone aSendDoneCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetSendDoneCallback(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aSendDoneCallback);
}

otError otTcpSetSendReadyCallback(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpSendReady aSendReadyCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetSendReadyCallback(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aSendReadyCallback);
}

otError otTcpSetReceiveAvailableCallback(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpReceiveAvailable aReceiveAvailableCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetReceiveAvailableCallback(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aReceiveAvailableCallback);
}

otError otTcpSetDisconnectedCallback(otInstance *aInstance, otTcpEndpoint *aEndpoint, otTcpDisconnected aDisconnectedCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetDisconnectedCallback(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aDisconnectedCallback);
}

otError otTcpBind(otInstance *aInstance, otTcpEndpoint *aEndpoint, const otSockAddr *aSockName)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().Bind(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint),
                                         *static_cast<const Ip6::SockAddr *>(aSockName));
}

otError otTcpConnect(otInstance *aInstance, otTcpEndpoint *aEndpoint, const otSockAddr *aSockName, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().Connect(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint),
                                            *static_cast<const Ip6::SockAddr *>(aSockName), aFlags);
}

otError otTcpSendByReference(otInstance *aInstance, otTcpEndpoint *aEndpoint, otLinkedBuffer *aBuffer, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SendByReference(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aBuffer, aFlags);
}

otError otTcpSendByExtension(otInstance *aInstance, otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SendByExtension(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aNumBytes, aFlags);
}

otError otTcpReceiveByReference(otInstance *aInstance, otTcpEndpoint *aEndpoint, const otLinkedBuffer **aBuffer)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().ReceiveByReference(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aBuffer);
}

otError otTcpReceiveContiguify(otInstance *aInstance, otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().ReceiveContiguify(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpCommitReceive(otInstance *aInstance, otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().CommitReceive(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aNumBytes, aFlags);
}

otError otTcpSendEndOfStream(otInstance *aInstance, otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SendEndOfStream(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpAbort(otInstance *aInstance, otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().Abort(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpEndpointDeinitialize(otInstance *aInstance, otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().EndpointDeinitialize(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpListenerInitialize(otInstance *aInstance, otTcpListener *aListener, otTcpListenerInitializeArgs *aArgs)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().ListenerInitialize(*static_cast<Ip6::Tcp::Listener *>(aListener), *aArgs);
}

otError otTcpSetAcceptReadyCallback(otInstance *aInstance, otTcpListener *aListener, otTcpAcceptReady aAcceptReadyCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetAcceptReadyCallback(*static_cast<Ip6::Tcp::Listener *>(aListener), aAcceptReadyCallback);
}

otError otTcpSetAcceptDoneCallback(otInstance *aInstance, otTcpListener *aListener, otTcpAcceptDone aAcceptDoneCallback)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().SetAcceptDoneCallback(*static_cast<Ip6::Tcp::Listener *>(aListener), aAcceptDoneCallback);
}

otError otTcpListen(otInstance *aInstance, otTcpListener *aListener, const otSockAddr *aSockName)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().Listen(*static_cast<Ip6::Tcp::Listener *>(aListener), *static_cast<const Ip6::SockAddr *>(aSockName));
}

otError otTcpStopListening(otInstance *aInstance, otTcpListener *aListener)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().StopListening(*static_cast<Ip6::Tcp::Listener *>(aListener));
}

otError otTcpListenerDeinitialize(otInstance *aInstance, otTcpListener *aListener)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().ListenerDeinitialize(*static_cast<Ip6::Tcp::Listener *>(aListener));
}
