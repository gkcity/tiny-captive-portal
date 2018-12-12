/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   CaptivePortalHandle.h
 *
 * @remark
 *
 */

#ifndef __CAPTIVE_PORTAL_HANDLER_H__
#define __CAPTIVE_PORTAL_HANDLER_H__

#include <tiny_base.h>
#include <codec-http/HttpMessage.h>

TINY_BEGIN_DECLS


HttpMessage* handleRoot (HttpMessage *request, void *ctx);

HttpMessage* handleNotFound (HttpMessage *request, void *ctx);


TINY_END_DECLS

#endif /* __CAPTIVE_PORTAL_HANDLER_H__ */
