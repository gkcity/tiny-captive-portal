/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   WebServerHandler.h
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __WEB_SERVER_HANDLER_H__
#define __WEB_SERVER_HANDLER_H__

#include <channel/ChannelHandler.h>
#include <handler/HttpRequestHandlers.h>

TINY_BEGIN_DECLS


#define WebServerHandler_Name  "WebServerHandler"

TINY_LOR
ChannelHandler * WebServerHandler(void);

TINY_LOR
HttpRequestHandlers * WebServerHandler_GetHandlers(ChannelHandler *thiz);


TINY_END_DECLS

#endif /* __WEB_SERVER_HANDLER_H__ */
