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

    return instance.Get<Ip6::Tcp>().Initialize(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aArgs);
}

otInstance *otTcpEndpointGetInstance(otTcpEndpoint *aEndpoint)
{
    return Ip6::Tcp::GetInstance(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

void *otTcpEndpointGetContext(otTcpEndpoint *aEndpoint)
{
    return Ip6::Tcp::GetContext(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

const otSockAddr *otTcpGetLocalAddress(otTcpEndpoint *aEndpoint)
{
    return &Ip6::Tcp::GetLocalAddress(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

const otSockAddr *otTcpGetPeerAddress(otTcpEndpoint *aEndpoint)
{
    return &Ip6::Tcp::GetPeerAddress(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpBind(otTcpEndpoint *aEndpoint, const otSockAddr *aSockName)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().Bind(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint),
                                         *static_cast<const Ip6::SockAddr *>(aSockName));
}

otError otTcpConnect(otTcpEndpoint *aEndpoint, const otSockAddr *aSockName, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().Connect(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint),
                                            *static_cast<const Ip6::SockAddr *>(aSockName), aFlags);
}

otError otTcpSendByReference(otTcpEndpoint *aEndpoint, otLinkedBuffer *aBuffer, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().SendByReference(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aBuffer, aFlags);
}

otError otTcpSendByExtension(otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().SendByExtension(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aNumBytes, aFlags);
}

otError otTcpReceiveByReference(otTcpEndpoint *aEndpoint, const otLinkedBuffer **aBuffer)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().ReceiveByReference(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), *aBuffer);
}

otError otTcpReceiveContiguify(otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().ReceiveContiguify(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpCommitReceive(otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().CommitReceive(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint), aNumBytes, aFlags);
}

otError otTcpSendEndOfStream(otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().SendEndOfStream(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpAbort(otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().Abort(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpEndpointDeinitialize(otTcpEndpoint *aEndpoint)
{
    Instance &instance = *static_cast<Instance *>(otTcpEndpointGetInstance(aEndpoint));

    return instance.Get<Ip6::Tcp>().Deinitialize(*static_cast<Ip6::Tcp::Endpoint *>(aEndpoint));
}

otError otTcpListenerInitialize(otInstance *aInstance, otTcpListener *aListener, otTcpListenerInitializeArgs *aArgs)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.Get<Ip6::Tcp>().Initialize(*static_cast<Ip6::Tcp::Listener *>(aListener), *aArgs);
}

otInstance *otTcpListenerGetInstance(otTcpListener *aListener)
{
    return Ip6::Tcp::GetInstance(*static_cast<Ip6::Tcp::Listener *>(aListener));
}

void *otTcpListenerGetContext(otTcpListener *aListener)
{
    return Ip6::Tcp::GetContext(*static_cast<Ip6::Tcp::Listener *>(aListener));
}

otError otTcpListen(otTcpListener *aListener, const otSockAddr *aSockName)
{
    Instance &instance = *static_cast<Instance *>(otTcpListenerGetInstance(aListener));

    return instance.Get<Ip6::Tcp>().Listen(*static_cast<Ip6::Tcp::Listener *>(aListener), *static_cast<const Ip6::SockAddr *>(aSockName));
}

otError otTcpStopListening(otTcpListener *aListener)
{
    Instance &instance = *static_cast<Instance *>(otTcpListenerGetInstance(aListener));

    return instance.Get<Ip6::Tcp>().StopListening(*static_cast<Ip6::Tcp::Listener *>(aListener));
}

otError otTcpListenerDeinitialize(otTcpListener *aListener)
{
    Instance &instance = *static_cast<Instance *>(otTcpListenerGetInstance(aListener));

    return instance.Get<Ip6::Tcp>().Deinitialize(*static_cast<Ip6::Tcp::Listener *>(aListener));
}
