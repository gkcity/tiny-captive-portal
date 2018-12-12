/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   WebServer.c
 *
 * @remark
 *
 */

#include <tiny_log.h>
#include <channel/SocketChannel.h>
#include <channel/stream/StreamServerChannel.h>
#include <channel/stream/StreamServerChannelContext.h>
#include "WebServer.h"
#include "WebServerHandler.h"

#define TAG     "WebServer"

TINY_LOR
Channel * WebServer_New(uint32_t ip, uint16_t port, ChannelInitializer initializer, void *ctx)
{
    Channel *thiz = NULL;

    do
    {
        thiz = StreamServerChannel_New(WEB_MAX_CONNECTIONS);
        if (thiz == NULL)
        {
            LOG_D(TAG, "StreamServerChannel_New failed");
            break;
        }

        StreamServerChannel_Initialize(thiz, initializer, ctx);

        if (RET_FAILED(SocketChannel_Open(thiz, TYPE_TCP_SERVER)))
        {
            LOG_D(TAG, "SocketChannel_Open failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(SocketChannel_Bind(thiz, port, false)))
        {
            LOG_D(TAG, "SocketChannel_Bind failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(SocketChannel_SetBlock(thiz, false)))
        {
            LOG_D(TAG, "SocketChannel_SetBlock failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(SocketChannel_Listen(thiz, ((StreamServerChannelContext *) thiz->context)->maxConnections)))
        {
            LOG_D(TAG, "SocketChannel_Listen failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}