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

#include <new>

#include "common/code_utils.hpp"
#include "common/error.hpp"
#include "net/ip6.hpp"

namespace ot {
namespace Ip6 {

void Tcp::Endpoint::Initialize(Instance &aInstance)
{
    IgnoreReturnValue(new (&GetDelackTimer()) TimerMilli(aInstance, HandleDelackTimer));
    IgnoreReturnValue(new (&GetRexmtPersistTimer()) TimerMilli(aInstance, HandleRexmtPersistTimer));
    IgnoreReturnValue(new (&GetKeepTimer()) TimerMilli(aInstance, HandleKeepTimer));
    IgnoreReturnValue(new (&Get2MslTimer()) TimerMilli(aInstance, Handle2MslTimer));
}

void Tcp::Endpoint::Deinitialize()
{
    GetDelackTimer().~TimerMilli();
    GetRexmtPersistTimer().~TimerMilli();
    GetKeepTimer().~TimerMilli();
    Get2MslTimer().~TimerMilli();
}

Tcp::Tcp(Instance &aInstance)
    : InstanceLocator(aInstance)
    , mEphemeralPort(kDynamicPortMin)
{
}

Error Tcp::Initialize(Endpoint &aEndpoint, otTcpEndpointInitializeArgs &aArgs)
{
    Error error = kErrorNone;

    aEndpoint.Initialize(InstanceLocator::GetInstance());

    aEndpoint.mContext = aArgs.mContext;
    aEndpoint.mEstablishedCallback = aArgs.mEstablishedCallback;
    aEndpoint.mSendDoneCallback = aArgs.mSendDoneCallback;
    aEndpoint.mSendReadyCallback = aArgs.mSendReadyCallback;
    aEndpoint.mReceiveAvailableCallback = aArgs.mReceiveAvailableCallback;
    aEndpoint.mDisconnectedCallback = aArgs.mDisconnectedCallback;

    error = mEndpoints.Add(aEndpoint);

    return error;
}

Instance *Tcp::GetInstance(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return nullptr;
}

void *Tcp::GetContext(Endpoint &aEndpoint)
{
    return aEndpoint.mContext;
}

const SockAddr &Tcp::GetLocalAddress(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return *static_cast<SockAddr *>(nullptr);
}

const SockAddr &Tcp::GetPeerAddress(Endpoint &aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    return *static_cast<SockAddr *>(nullptr);
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

Error Tcp::Deinitialize(Endpoint &aEndpoint)
{
    Error    error;
    Endpoint *prev;

    SuccessOrExit(error = mEndpoints.Find(aEndpoint, prev));
    mEndpoints.PopAfter(prev);
    aEndpoint.SetNext(nullptr);

    aEndpoint.Deinitialize();

exit:
    return error;
}

Error Tcp::Initialize(Listener &aListener, otTcpListenerInitializeArgs &aArgs)
{
    Error error = kErrorNone;

    aListener.mContext = aArgs.mContext;
    aListener.mAcceptReadyCallback = aArgs.mAcceptReadyCallback;
    aListener.mAcceptDoneCallback = aArgs.mAcceptDoneCallback;

    error = mListeners.Add(aListener);

    return error;
}

Instance *Tcp::GetInstance(Listener &aListener)
{
    OT_UNUSED_VARIABLE(aListener);

    return nullptr;
}

void *Tcp::GetContext(Listener &aListener)
{
    return aListener.mContext;
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

Error Tcp::Deinitialize(Listener &aListener)
{
    Error    error;
    Listener *prev;

    SuccessOrExit(error = mListeners.Find(aListener, prev));
    mListeners.PopAfter(prev);
    aListener.SetNext(nullptr);

exit:
    return error;
}

Error Tcp::ProcessReceivedSegment(Message &aMessage, MessageInfo &aMessageInfo)
{
    OT_UNUSED_VARIABLE(aMessage);
    OT_UNUSED_VARIABLE(aMessageInfo);

    Error error = kErrorNotImplemented;

    for (Endpoint *active = mEndpoints.GetHead(); active != nullptr; active = active->GetNext())
    {
    }

    for (Listener *passive = mListeners.GetHead(); passive != nullptr; passive = passive->GetNext())
    {
    }

    return error;
}

void Tcp::HandleDelackTimer(Timer &aTimer)
{
    TimerMilli &timer = *static_cast<TimerMilli *>(&aTimer);
    OT_UNUSED_VARIABLE(timer);
}

void Tcp::HandleRexmtPersistTimer(Timer &aTimer)
{
    TimerMilli &timer = *static_cast<TimerMilli *>(&aTimer);
    OT_UNUSED_VARIABLE(timer);
}

void Tcp::HandleKeepTimer(Timer &aTimer)
{
    TimerMilli &timer = *static_cast<TimerMilli *>(&aTimer);
    OT_UNUSED_VARIABLE(timer);
}

void Tcp::Handle2MslTimer(Timer &aTimer)
{
    TimerMilli &timer = *static_cast<TimerMilli *>(&aTimer);
    OT_UNUSED_VARIABLE(timer);
}

} // namespace Ip6
} // namespace ot