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

static void sendResponse(ChannelHandler *thiz, Channel *channel, int code, const char *status, const char *type, const char *body)
{
    HttpMessage response;

    printf("sendResponse: %d %s\n", code, status);

    do
    {
        if (RET_FAILED(HttpMessage_Construct(&response)))
        {
            printf("HttpMessage_Construct FAILED\n");
            break;
        }

        uint32_t length = (uint32_t) ((body == NULL) ? 0 : strlen(body));

        HttpMessage_SetResponse(&response, code, status);
        HttpMessage_SetVersion(&response, 1, 1);

        if (length > 0)
        {
            HttpMessage_SetContent(&response, type, length, (const uint8_t *)body);
        }

        SocketChannel_StartWrite(channel, DATA_HTTP_MESSAGE, &response, 0);
    } while (false);

    HttpMessage_Dispose(&response);
}

static void redirectTo(ChannelHandler *thiz, Channel *channel, uint32_t ip)
{
    HttpMessage response;

    char host[64];
    uint8_t *a = (uint8_t *) &ip;
    snprintf(host, 64, "http://%d.%d.%d.%d", a[0], a[1], a[2], a[3]);

    printf("redirectTo: %s\n", host);

    do
    {
        if (RET_FAILED(HttpMessage_Construct(&response)))
        {
            printf("HttpMessage_Construct FAILED\n");
            break;
        }

        HttpMessage_SetResponse(&response, 302, "Found");
        HttpMessage_SetVersion(&response, 1, 1);
        HttpHeader_Set(&response.header, "Location", host);

        SocketChannel_StartWrite(channel, DATA_HTTP_MESSAGE, &response, 0);
    } while (false);

    HttpMessage_Dispose(&response);
}

static bool isIp(const char *host)
{
    for (size_t i = 0; i < strlen(host); ++i)
    {
        const char c = host[i];
        if (c != '.' && (c < '0' || c > '9'))
        {
            return false;
        }
    }

    return true;
}

static bool redirect(ChannelHandler *thiz, Channel *channel, HttpMessage *request)
{
    bool ret = false;

    do
    {
        const char *host = HttpHeader_GetValue(&request->header, "host");
        if (host == NULL)
        {
            break;
        }

        if (isIp(host))
        {
            break;
        }

        redirectTo(thiz, channel, (uint32_t) thiz->context);
        ret = true;
    } while (false);

    return ret;
}

TINY_LOR
static bool _ChannelRead(ChannelHandler *thiz, Channel *channel, ChannelDataType type, const void *data, uint32_t len)
{
    HttpMessage *request = (HttpMessage *)data;

    if (request->type != HTTP_REQUEST)
    {
        return true;
    }

    LOG_I(TAG, "_ChannelRead: %s %s", request->request_line.method, request->request_line.uri);

    if (redirect(thiz, channel, request))
    {
        return true;
    }

    sendResponse(thiz, channel, 200, "OK", "text/html", "hello");

//    const char *txt = "<meta HTTP-EQUIV='REFRESH' content='0; url=http://192.168.4.1/login'>";
//    sendResponse(thiz, channel, 200, "OK", "text/html", txt);


//    if (STR_EQUAL(URI_IDENTIFY, request->request_line.uri))
//    {
//        if (ctx->identifyHandler == NULL)
//        {
//            ctx->identifyHandler = HapIdentifyRequestHandler(ctx->device, ctx->persistence);
//        }
//
//        handler = ctx->identifyHandler;
//    }
//    else if (STR_EQUAL(URI_PAIR_SETUP, request->request_line.uri))
//    {
//        handler = (HapRequestHandler *)TinyMap_GetValue(&ctx->pairingHandlers, channel->id);
//        if (handler == NULL)
//        {
//            LOG_I(TAG, "PairingHandler not found, create from: %s", channel->id);
//            handler = HapPairingRequestHandler(ctx->device, ctx->persistence);
//            if (handler != NULL)
//            {
//                TinyMap_Insert(&ctx->pairingHandlers, channel->id, handler);
//            }
//        }
//    }
//    else
//    {
//        handler = (HapRequestHandler *)TinyMap_GetValue(&ctx->controllingHandlers, channel->id);
//        if (handler == NULL)
//        {
//            LOG_I(TAG, "ControllingHandler not found, create from: %s", channel->id);
//            handler = HapControllingRequestHandler(ctx->device, ctx->persistence);
//            if (handler != NULL)
//            {
//                TinyMap_Insert(&ctx->controllingHandlers, channel->id, handler);
//            }
//        }
//    }
//
//    if (handler == NULL)
//    {
//        HttpSender_SendResponse(channel, PROTOCOL_HTTP, 1, 1, 500, "internal error");
//    }
//    else
//    {
//        handler->onRequest(handler, channel, request, ctx->device);
//    }

    return true;
}

TINY_LOR
static void WebServerHandler_Dispose(ChannelHandler *thiz)
{
    RETURN_IF_FAIL(thiz);

    memset(thiz, 0, sizeof(ChannelHandler));
}

TINY_LOR
static void WebServerHandler_Delete(ChannelHandler *thiz)
{
    WebServerHandler_Dispose(thiz);
    tiny_free(thiz);
}

TINY_LOR
static TinyRet WebServerHandler_Construct(ChannelHandler *thiz, uint32_t ip)
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
        thiz->context = (void *) ip;
    } while (0);

    return ret;
}

TINY_LOR
ChannelHandler * WebServerHandler(uint32_t ip)
{
    ChannelHandler *thiz = NULL;

    do
    {
        thiz = (ChannelHandler *)tiny_malloc(sizeof(ChannelHandler));
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(WebServerHandler_Construct(thiz, ip)))
        {
            WebServerHandler_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}
