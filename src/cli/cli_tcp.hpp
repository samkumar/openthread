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
 *   This file contains definitions for a TCP CLI tool.
 */

#ifndef CLI_TCP_EXAMPLE_HPP_
#define CLI_TCP_EXAMPLE_HPP_

#include "openthread-core-config.h"

#include <openthread/tcp.h>

#include "utils/lookup_table.hpp"
#include "utils/parse_cmdline.hpp"

namespace ot {
namespace Cli {

class Interpreter;

/**
 * This class implements a CLI-based TCP example.
 *
 */
class TcpExample
{
public:
    using Arg = Utils::CmdLineParser::Arg;

    /**
     * Constructor
     *
     * @param[in]  aInterpreter  The CLI interpreter.
     *
     */
    explicit TcpExample(Interpreter &aInterpreter);

    /**
     * This mehtod interprets a list of CLI arguments.
     *
     * @param[in]  aArgsLength  The number of elements in @p aArgs.
     * @param[in]  aArgs        An array of command line arguments.
     *
     */
    otError Process(uint8_t aArgsLength, Arg aArgs[]);

private:
    class Callbacks;

    struct Command
    {
        const char *mName;
        otError (TcpExample::*mHandler)(uint8_t aArgsLength, Arg aArgs[]);
    };

    otError ProcessHelp(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessInit(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessDeinit(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessBind(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessConnect(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessSend(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessSendEnd(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessAbort(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessListen(uint8_t aArgsLength, Arg aArgs[]);
    otError ProcessStopListening(uint8_t aArgslength, Arg aArgs[]);

    static void HandleTcpEstablishedCallback(otTcpEndpoint *aEndpoint);
    static void HandleTcpSendDoneCallback(otTcpEndpoint *aEndpoint, otLinkedBuffer *aData);
    static void HandleTcpReceiveAvailableCallback(otTcpEndpoint *aEndpoint, size_t aBytesAvailable, bool aEndOfStream, size_t aBytesRemaining);
    static void HandleTcpDisconnectedCallback(otTcpEndpoint *aEndpoint, otTcpDisconnectedReason aReason);
    static otTcpIncomingConnectionAction HandleTcpAcceptReadyCallback(otTcpListener *aListener, const otSockAddr *aPeer, otTcpEndpoint **aAcceptInto);
    static void HandleTcpAcceptDoneCallback(otTcpListener *aListener, otTcpEndpoint *aEndpoint, const otSockAddr *aPeer);

    void HandleTcpEstablished(otTcpEndpoint *aEndpoint);
    void HandleTcpSendDone(otTcpEndpoint *aEndpoint, otLinkedBuffer *aData);
    void HandleTcpReceiveAvailable(otTcpEndpoint *aEndpoint, size_t aBytesAvailable, bool aEndOfStream, size_t aBytesRemaining);
    void HandleTcpDisconnected(otTcpEndpoint *aEndpoint, otTcpDisconnectedReason aReason);
    otTcpIncomingConnectionAction HandleTcpAcceptReady(otTcpListener *aListener, const otSockAddr *aPeer, otTcpEndpoint **aAcceptInto);
    void HandleTcpAcceptDone(otTcpListener *aListener, otTcpEndpoint *aEndpoint, const otSockAddr *aPeer);

    static constexpr Command sCommands[] = {
        {"abort", &TcpExample::ProcessAbort},
        {"bind", &TcpExample::ProcessBind},
        {"connect", &TcpExample::ProcessConnect},
        {"deinit", &TcpExample::ProcessDeinit},
        {"help", &TcpExample::ProcessHelp},
        {"init", &TcpExample::ProcessInit},
        {"listen", &TcpExample::ProcessListen},
        {"send", &TcpExample::ProcessSend},
        {"sendend", &TcpExample::ProcessSendEnd},
        {"stoplistening", &TcpExample::ProcessStopListening},
    };

    static_assert(Utils::LookupTable::IsSorted(sCommands), "Command Table is not sorted");

    Interpreter &mInterpreter;

    otTcpEndpoint mEndpoint;
    otTcpListener mListener;

    bool mEndpointInitialized;
    bool mListenerInitialized;
    bool mEndpointConnected;
    bool mSendBusy;

    otLinkedBuffer mSendLink;
    uint8_t mSendBuffer[1024];
    uint8_t mReceiveBuffer[4096];
};

} // namespace Cli
} // namespace ot

#endif // CLI_TCP_EXAMPLE_HPP_
