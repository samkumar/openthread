/*
 *  Copyright (c) 2016, The OpenThread Authors.
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
 *   This file implements the OpenThread CoAP API.
 */
#if OPENTHREAD_ENABLE_APPLICATION_COAP
#include "openthread/coap.h"

#include "openthread-instance.h"
#include "coap/coap_header.hpp"

using namespace Thread;

extern "C" {

void otCoapHeaderInit(otCoapHeader *aHeader, otCoapType aType, otCoapCode aCode)
{
    Coap::Header *header = static_cast<Coap::Header *>(aHeader);
    header->Init(aType, aCode);
}

void otCoapHeaderSetToken(otCoapHeader *aHeader, const uint8_t *aToken, uint8_t aTokenLength)
{
    static_cast<Coap::Header *>(aHeader)->SetToken(aToken, aTokenLength);
}

void otCoapHeaderGenerateToken(otCoapHeader *aHeader, uint8_t aTokenLength)
{
    static_cast<Coap::Header *>(aHeader)->SetToken(aTokenLength);
}

ThreadError otCoapHeaderAppendOption(otCoapHeader *aHeader, const otCoapOption *aOption)
{
    return static_cast<Coap::Header *>(aHeader)->AppendOption(*static_cast<const Coap::Header::Option *>(aOption));
}

ThreadError otCoapHeaderAppendObserveOption(otCoapHeader *aHeader, uint32_t aObserve)
{
    return static_cast<Coap::Header *>(aHeader)->AppendObserveOption(aObserve);
}

ThreadError otCoapHeaderAppendUriPathOptions(otCoapHeader *aHeader, const char *aUriPath)
{
    return static_cast<Coap::Header *>(aHeader)->AppendUriPathOptions(aUriPath);
}

ThreadError otCoapHeaderAppendMaxAgeOption(otCoapHeader *aHeader, uint32_t aMaxAge)
{
    return static_cast<Coap::Header *>(aHeader)->AppendMaxAgeOption(aMaxAge);
}

ThreadError otCoapHeaderAppendUriQueryOption(otCoapHeader *aHeader, const char *aUriQuery)
{
    return static_cast<Coap::Header *>(aHeader)->AppendUriQueryOption(aUriQuery);
}

void otCoapHeaderSetPayloadMarker(otCoapHeader *aHeader)
{
    static_cast<Coap::Header *>(aHeader)->SetPayloadMarker();
}

void otCoapHeaderSetMessageId(otCoapHeader *aHeader, uint16_t aMessageId)
{
    return static_cast<Coap::Header *>(aHeader)->SetMessageId(aMessageId);
}

otCoapType otCoapHeaderGetType(const otCoapHeader *aHeader)
{
    return static_cast<const Coap::Header *>(aHeader)->GetType();
}

otCoapCode otCoapHeaderGetCode(const otCoapHeader *aHeader)
{
    return static_cast<const Coap::Header *>(aHeader)->GetCode();
}

uint16_t otCoapHeaderGetMessageId(const otCoapHeader *aHeader)
{
    return static_cast<const Coap::Header *>(aHeader)->GetMessageId();
}

uint8_t otCoapHeaderGetTokenLength(const otCoapHeader *aHeader)
{
    return static_cast<const Coap::Header *>(aHeader)->GetTokenLength();
}

const uint8_t *otCoapHeaderGetToken(const otCoapHeader *aHeader)
{
    return static_cast<const Coap::Header *>(aHeader)->GetToken();
}

const otCoapOption *otCoapHeaderGetCurrentOption(const otCoapHeader *aHeader)
{
    return static_cast<const otCoapOption *>(static_cast<const Coap::Header *>(aHeader)->GetCurrentOption());
}

const otCoapOption *otCoapHeaderGetNextOption(otCoapHeader *aHeader)
{
    return static_cast<const otCoapOption *>(static_cast<Coap::Header *>(aHeader)->GetNextOption());
}

otMessage *otCoapNewMessage(otInstance *aInstance, const otCoapHeader *aHeader)
{
    return aInstance->mThreadNetif.GetCoapClient().NewMessage(*(static_cast<const Coap::Header *>(aHeader)));
}

ThreadError otCoapSendRequest(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo,
                              otCoapResponseHandler aHandler, void *aContext)
{
    return aInstance->mThreadNetif.GetCoapClient().SendMessage(
               *static_cast<Message *>(aMessage),
               *static_cast<const Ip6::MessageInfo *>(aMessageInfo),
               aHandler, aContext);
}

ThreadError otCoapServerStart(otInstance *aInstance)
{
    return aInstance->mApplicationCoapServer.Start();
}

ThreadError otCoapServerStop(otInstance *aInstance)
{
    return aInstance->mApplicationCoapServer.Stop();
}

ThreadError otCoapServerSetPort(otInstance *aInstance, uint16_t aPort)
{
    return aInstance->mApplicationCoapServer.SetPort(aPort);
}

ThreadError otCoapServerAddResource(otInstance *aInstance, otCoapResource *aResource)
{
    return aInstance->mApplicationCoapServer.AddResource(*static_cast<Coap::Resource *>(aResource));
}

void otCoapServerRemoveResource(otInstance *aInstance, otCoapResource *aResource)
{
    aInstance->mApplicationCoapServer.RemoveResource(*static_cast<Coap::Resource *>(aResource));
}

ThreadError otCoapSendResponse(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    return aInstance->mApplicationCoapServer.SendMessage(
               *static_cast<Message *>(aMessage), *static_cast<const Ip6::MessageInfo *>(aMessageInfo));
}

}  // extern "C"
#endif
