/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   WebServerHandler.c
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include <tiny_malloc.h>
#include <tiny_log.h>
#include <codec-http/HttpMessage.h>
#include <channel/SocketChannel.h>
#include "WebServerHandler.h"

#define TAG     "WebServerHandler"

TINY_LOR
static void _ChannelEvent(ChannelHandler *thiz, Channel *channel, ChannelTimer *timer)
{
    LOG_D(TAG, "_ChannelEvent: %s", channel->id);
    LOG_D(TAG, "close connection");
    Channel_Close(channel);
}

TINY_LOR
static void _ChannelActive(ChannelHandler *thiz, Channel *channel)
{
    LOG_D(TAG, "_ChannelActive: %s", channel->id);

    thiz->channelEvent = _ChannelEvent;
}

TINY_LOR
static void _ChannelInactive(ChannelHandler *thiz, Channel *channel)
{
    LOG_D(TAG, "_ChannelInactive: %s", channel->id);
}

TINY_LOR
static bool _ChannelRead(ChannelHandler *thiz, Channel *channel, ChannelDataType type, const void *data, uint32_t len)
{
    HttpRequestHandlers *handlers = (HttpRequestHandlers *)thiz->context;
    HttpMessage *request = (HttpMessage *)data;
    HttpMessage *response = NULL;

    if (request->type != HTTP_REQUEST)
    {
        return true;
    }

    LOG_I(TAG, "_ChannelRead: %s %s", request->request_line.method, request->request_line.uri);

    response = HttpRequestHandlers_HandleRequest(handlers, request);
    if (response != NULL)
    {
        SocketChannel_StartWrite(channel, DATA_HTTP_MESSAGE, response, 0);
        HttpMessage_Delete(response);
    }

    return true;
}

TINY_LOR
static void WebServerHandler_Dispose(ChannelHandler *thiz)
{
    RETURN_IF_FAIL(thiz);
    HttpRequestHandlers_Delete((HttpRequestHandlers *)thiz->context);
    memset(thiz, 0, sizeof(ChannelHandler));
}

TINY_LOR
static void WebServerHandler_Delete(ChannelHandler *thiz)
{
    WebServerHandler_Dispose(thiz);
    tiny_free(thiz);
}

TINY_LOR
static TinyRet WebServerHandler_Construct(ChannelHandler *thiz)
{
    TinyRet ret = TINY_RET_OK;

    do
    {
        memset(thiz, 0, sizeof(ChannelHandler));

        strncpy(thiz->name, WebServerHandler_Name, CHANNEL_HANDLER_NAME_LEN);
        thiz->onRemove = WebServerHandler_Delete;

        thiz->inType = DATA_HTTP_MESSAGE;
        thiz->outType = DATA_HTTP_MESSAGE;
        thiz->channelActive = _ChannelActive;
        thiz->channelInactive = _ChannelInactive;
        thiz->channelRead = _ChannelRead;
        thiz->channelWrite = NULL;
        thiz->context = HttpRequestHandlers_New();
        if (thiz->context == NULL)
        {
            ret = TINY_RET_E_NEW;
            break;
        }
    } while (0);

    return ret;
}

TINY_LOR
ChannelHandler * WebServerHandler(void)
{
    ChannelHandler *thiz = NULL;

    do
    {
        thiz = (ChannelHandler *)tiny_malloc(sizeof(ChannelHandler));
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(WebServerHandler_Construct(thiz)))
        {
            WebServerHandler_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}

TINY_LOR
HttpRequestHandlers * WebServerHandler_GetHandlers(ChannelHandler *thiz)
{
    return (HttpRequestHandlers *)thiz->context;
}