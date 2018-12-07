/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   DNSServer.c
 *
 * @remark
 *
 */

#include <tiny_log.h>
#include <channel/SocketChannel.h>
#include <codec/DnsMessageCodec.h>
#include <DnsServerHandler.h>
#include <channel/unicast/UnicastChannel.h>
#include <DnsConstant.h>
#include "DNSServer.h"

#define TAG     "DNSServer"

TINY_LOR
static void _Initializer(Channel *channel, void *ctx)
{
    ChannelHandler *handler = DnsServerHandler(*(uint32_t *)ctx);
    SocketChannel_AddLast(channel, DnsMessageCodec());
    SocketChannel_AddLast(channel, handler);
}

TINY_LOR
Channel *DNSServer_New(uint32_t ip)
{
    Channel *thiz = NULL;

    do
    {
        thiz = UnicastChannel_New();
        if (thiz == NULL)
        {
            LOG_D(TAG, "UnicastChannel_New failed");
            break;
        }

        UnicastChannel_Initialize(thiz, _Initializer, &ip);

        if (RET_FAILED(UnicastChannel_Open(thiz, DNS_PORT, false)))
        {
            LOG_D(TAG, "UnicastChannel_Open failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}