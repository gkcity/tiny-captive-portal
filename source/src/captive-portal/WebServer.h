/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   WebServer.h
 *
 * @remark
 *
 */

#ifndef __HAP_SERVER_H__
#define __HAP_SERVER_H__

#include <channel/Channel.h>

TINY_BEGIN_DECLS


#define WEB_MAX_CONNECTIONS         10
#define WEB_CONNECTION_TIMEOUT      60

TINY_LOR
Channel * WebServer_New(uint16_t port);



TINY_END_DECLS

#endif /* __HAP_SERVER_H__ */