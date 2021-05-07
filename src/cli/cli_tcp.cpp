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
 *   This file implements a TCP CLI tool.
 */

#include "cli_tcp.hpp"

#include <openthread/tcp.h>

#include "cli/cli.hpp"
#include "common/encoding.hpp"

namespace ot {
namespace Cli {

constexpr TcpExample::Command TcpExample::sCommands[];

TcpExample::TcpExample(Interpreter &aInterpreter)
    : mInterpreter(aInterpreter)
    , mEndpointInitialized(false)
    , mListenerInitialized(false)
    , mEndpointConnected(false)
    , mSendBusy(false)
{
    memset(&mEndpoint, 0x00, sizeof(mEndpoint));
    memset(&mListener, 0x00, sizeof(mListener));
}

otError TcpExample::ProcessHelp(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    for (const Command &command : sCommands)
    {
        mInterpreter.OutputLine(command.mName);
    }

    return OT_ERROR_NONE;
}

otError TcpExample::ProcessInit(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgs);

    otError error = OT_ERROR_NONE;

    VerifyOrExit(aArgsLength == 0, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(!mEndpointInitialized || !mListenerInitialized, error = OT_ERROR_INVALID_STATE);

    if (!mEndpointInitialized)
    {
        otTcpEndpointInitializeArgs endpointArgs;
        memset(&endpointArgs, 0x00, sizeof(endpointArgs));
        endpointArgs.mEstablishedCallback = HandleTcpEstablished;
        endpointArgs.mSendDoneCallback = HandleTcpSendDone;
        endpointArgs.mReceiveAvailableCallback = HandleTcpReceiveAvailable;
        endpointArgs.mDisconnectedCallback = HandleTcpDisconnected;
        endpointArgs.mContext = this;
        endpointArgs.mReceiveBuffer = mReceiveBuffer;
        endpointArgs.mReceiveBufferSize = sizeof(mReceiveBuffer);

        SuccessOrExit(error = otTcpEndpointInitialize(mInterpreter.mInstance, &mEndpoint, &endpointArgs));
        mEndpointInitialized = true;
    }

    if (!mListenerInitialized)
    {
        otTcpListenerInitializeArgs listenerArgs;
        memset(&listenerArgs, 0x00, sizeof(listenerArgs));
        listenerArgs.mAcceptReadyCallback = HandleTcpAcceptReady;
        listenerArgs.mAcceptDoneCallback = HandleTcpAcceptDone;
        listenerArgs.mContext = this;

        SuccessOrExit(error = otTcpListenerInitialize(mInterpreter.mInstance, &mListener, &listenerArgs));
        mListenerInitialized = true;
    }

exit:
    return error;
}

otError TcpExample::ProcessDeinit(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgs);

    otError error = OT_ERROR_NONE;

    VerifyOrExit(aArgsLength == 0, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized || mListenerInitialized, error = OT_ERROR_INVALID_STATE);

    if (mEndpointInitialized)
    {
        SuccessOrExit(error = otTcpEndpointDeinitialize(mInterpreter.mInstance, &mEndpoint));
        mSendBusy = false;
        mEndpointInitialized = false;
    }

    if (mListenerInitialized)
    {
        SuccessOrExit(error = otTcpListenerDeinitialize(mInterpreter.mInstance, &mListener));
        mListenerInitialized = false;
    }

exit:
    return error;
}

otError TcpExample::ProcessBind(uint8_t aArgsLength, Arg aArgs[])
{
    otError    error;
    otSockAddr sockaddr;

    VerifyOrExit(aArgsLength == 2, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized, error = OT_ERROR_INVALID_STATE);

    SuccessOrExit(error = aArgs[0].ParseAsIp6Address(sockaddr.mAddress));
    SuccessOrExit(error = aArgs[1].ParseAsUint16(sockaddr.mPort));

    error = otTcpBind(mInterpreter.mInstance, &mEndpoint, &sockaddr);

exit:
    return error;
}

otError TcpExample::ProcessConnect(uint8_t aArgsLength, Arg aArgs[])
{
    otError    error;
    otSockAddr sockaddr;

    VerifyOrExit(aArgsLength == 2, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized, error = OT_ERROR_INVALID_STATE);

    SuccessOrExit(error = aArgs[0].ParseAsIp6Address(sockaddr.mAddress));
    SuccessOrExit(error = aArgs[1].ParseAsUint16(sockaddr.mPort));

    SuccessOrExit(error = otTcpConnect(mInterpreter.mInstance, &mEndpoint, &sockaddr, OT_TCP_CONNECT_NO_FAST_OPEN));
    mEndpointConnected = false;

exit:
    return error;
}

otError TcpExample::ProcessSend(uint8_t aArgsLength, Arg aArgs[])
{
    otError        error;

    VerifyOrExit(aArgsLength == 1, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized, error = OT_ERROR_INVALID_STATE);
    VerifyOrExit(!mSendBusy, error = OT_ERROR_INVALID_STATE);

    mSendLink.mNext = nullptr;
    mSendLink.mData = mSendBuffer;
    mSendLink.mLength = OT_MIN(aArgs[0].GetLength(), sizeof(mSendBuffer));
    memcpy(mSendBuffer, aArgs[0].GetCString(), mSendLink.mLength);

    SuccessOrExit(error = otTcpSendByReference(mInterpreter.mInstance, &mEndpoint, &mSendLink, 0));
    mSendBusy = true;

exit:
    return error;
}

otError TcpExample::ProcessSendEnd(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgs);

    otError error;

    VerifyOrExit(aArgsLength == 0, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized, error = OT_ERROR_INVALID_STATE);

    error = otTcpSendEndOfStream(mInterpreter.mInstance, &mEndpoint);

exit:
    return error;
}

otError TcpExample::ProcessAbort(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgs);

    otError error;

    VerifyOrExit(aArgsLength == 0, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mEndpointInitialized, error = OT_ERROR_INVALID_STATE);

    SuccessOrExit(error = otTcpAbort(mInterpreter.mInstance, &mEndpoint));
    mEndpointConnected = false;

exit:
    return error;
}

otError TcpExample::ProcessListen(uint8_t aArgsLength, Arg aArgs[])
{
    otError    error;
    otSockAddr sockaddr;

    VerifyOrExit(aArgsLength == 2, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mListenerInitialized, error = OT_ERROR_INVALID_STATE);

    SuccessOrExit(error = aArgs[0].ParseAsIp6Address(sockaddr.mAddress));
    SuccessOrExit(error = aArgs[1].ParseAsUint16(sockaddr.mPort));

    SuccessOrExit(error = otTcpStopListening(mInterpreter.mInstance, &mListener));
    error = otTcpListen(mInterpreter.mInstance, &mListener, &sockaddr);

exit:
    return error;
}

otError TcpExample::ProcessStopListening(uint8_t aArgsLength, Arg aArgs[])
{
    OT_UNUSED_VARIABLE(aArgs);

    otError error;

    VerifyOrExit(aArgsLength == 0, error = OT_ERROR_INVALID_ARGS);
    VerifyOrExit(mListenerInitialized, error = OT_ERROR_INVALID_STATE);

    error = otTcpStopListening(mInterpreter.mInstance, &mListener);

exit:
    return error;
}

otError TcpExample::Process(uint8_t aArgsLength, Arg aArgs[])
{
    otError       error = OT_ERROR_INVALID_ARGS;
    const Command *command;

    VerifyOrExit(aArgsLength != 0, IgnoreError(ProcessHelp(0, nullptr)));

    command = Utils::LookupTable::Find(aArgs[0].GetCString(), sCommands);
    VerifyOrExit(command != nullptr, error = OT_ERROR_INVALID_COMMAND);

    error = (this->*command->mHandler)(aArgsLength - 1, aArgs + 1);

exit:
    return error;
}

void TcpExample::HandleTcpEstablished(void *aContext, otTcpEndpoint *aEndpoint)
{
    static_cast<TcpExample *>(aContext)->HandleTcpEstablished(aEndpoint);
}

void TcpExample::HandleTcpSendDone(void *aContext, otTcpEndpoint *aEndpoint, otLinkedBuffer *aData)
{
    static_cast<TcpExample *>(aContext)->HandleTcpSendDone(aEndpoint, aData);
}

void TcpExample::HandleTcpReceiveAvailable(void *aContext, otTcpEndpoint *aEndpoint, size_t aBytesAvailable, bool aEndOfStream, size_t aBytesRemaining)
{
    static_cast<TcpExample *>(aContext)->HandleTcpReceiveAvailable(aEndpoint, aBytesAvailable, aEndOfStream, aBytesRemaining);
}

void TcpExample::HandleTcpDisconnected(void *aContext, otTcpEndpoint *aEndpoint, otTcpDisconnectedReason aReason)
{
    static_cast<TcpExample *>(aContext)->HandleTcpDisconnected(aEndpoint, aReason);
}

otTcpEndpoint *TcpExample::HandleTcpAcceptReady(void *aContext, otTcpListener *aListener, const otSockAddr *aPeer)
{
    return static_cast<TcpExample *>(aContext)->HandleTcpAcceptReady(aListener, aPeer);
}

void TcpExample::HandleTcpAcceptDone(void *aContext, otTcpListener *aListener, otTcpEndpoint *aEndpoint, const otSockAddr *aPeer)
{
    static_cast<TcpExample *>(aContext)->HandleTcpAcceptDone(aListener, aEndpoint, aPeer);
}

void TcpExample::HandleTcpEstablished(otTcpEndpoint *aEndpoint)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    mInterpreter.OutputLine("TCP: Connection established");
}

void TcpExample::HandleTcpSendDone(otTcpEndpoint *aEndpoint, otLinkedBuffer *aData)
{
    OT_UNUSED_VARIABLE(aEndpoint);
    OT_ASSERT(aData == &mSendLink);
    OT_ASSERT(mSendBusy);

    mSendBusy = false;
}

void TcpExample::HandleTcpReceiveAvailable(otTcpEndpoint *aEndpoint, size_t aBytesAvailable, bool aEndOfStream, size_t aBytesRemaining)
{
    OT_UNUSED_VARIABLE(aBytesRemaining);
    OT_ASSERT(aEndpoint == &mEndpoint);

    if (aBytesAvailable > 0)
    {
        const otLinkedBuffer *data;
        IgnoreError(otTcpReceiveByReference(mInterpreter.mInstance, aEndpoint, &data));
        size_t totalReceived = 0;
        for (; data != nullptr; data = data->mNext)
        {
            mInterpreter.OutputFormat("TCP: Received %u bytes: ", static_cast<unsigned int>(data->mLength));
            mInterpreter.Output(reinterpret_cast<const char*>(data->mData), data->mLength);
            mInterpreter.OutputLine("");
            totalReceived += data->mLength;
        }
        OT_ASSERT(aBytesAvailable == totalReceived);
        otTcpCommitReceive(mInterpreter.mInstance, aEndpoint, totalReceived, 0);
    }

    if (aEndOfStream)
    {
        mInterpreter.OutputLine("TCP: Reached end of stream");
    }
}

void TcpExample::HandleTcpDisconnected(otTcpEndpoint *aEndpoint, otTcpDisconnectedReason aReason)
{
    OT_UNUSED_VARIABLE(aEndpoint);

    switch (aReason)
    {
    case OT_TCP_DISCONNECTED_REASON_NORMAL:
        mInterpreter.OutputLine("TCP: Disconnected");
        break;
    case OT_TCP_DISCONNECTED_REASON_TIME_WAIT:
        mInterpreter.OutputLine("TCP: Entered TIME-WAIT state");
        break;
    case OT_TCP_DISCONNECTED_REASON_TIMED_OUT:
        mInterpreter.OutputLine("TCP: Connection timed out");
        break;
    case OT_TCP_DISCONNECTED_REASON_REFUSED:
        mInterpreter.OutputLine("TCP: Connection refused");
        break;
    case OT_TCP_DISCONNECTED_REASON_RESET:
        mInterpreter.OutputLine("TCP: Connection reset");
        break;
    }

    /*
    * We set this to false even for the TIME-WAIT state, so that we can reuse
     * the active socket if an incoming connection comes in instead of waiting
     * for the 2MSL timeout.
     */
    mEndpointConnected = false;
}

otTcpEndpoint *TcpExample::HandleTcpAcceptReady(otTcpListener *aListener, const otSockAddr *aPeer)
{
    OT_UNUSED_VARIABLE(aListener);

    if (mEndpointConnected)
    {
        mInterpreter.OutputFormat("TCP: Ignoring incoming connection request from [");
        mInterpreter.OutputIp6Address(aPeer->mAddress);
        mInterpreter.OutputLine("]:%u (active socket is busy)", static_cast<unsigned int>(aPeer->mPort));

        return OT_TCP_DEFER;
    }

    return &mEndpoint;
}

void TcpExample::HandleTcpAcceptDone(otTcpListener *aListener, otTcpEndpoint *aEndpoint, const otSockAddr *aPeer)
{
    OT_UNUSED_VARIABLE(aListener);
    OT_UNUSED_VARIABLE(aEndpoint);

    mInterpreter.OutputFormat("Accepted connection from [");
    mInterpreter.OutputIp6Address(aPeer->mAddress);
    mInterpreter.OutputLine("]:%u", static_cast<unsigned int>(aPeer->mPort));
}

} // namespace Cli
} // namespace ot
