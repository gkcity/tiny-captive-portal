/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   CaptivePortalHandle.c
 *
 * @remark
 *
 */

#include "CaptivePortalHandle.h"

const char * txt = "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body><h1>Hello Ouyang!</h1><p>This is a captive portal example. All requests will be redirected here.</p></body></html>";

HttpMessage* handleRoot (HttpMessage *request, void *ctx)
{
    return HttpMessage_NewHttpResponse(200, "OK", "text/html", (const uint8_t *) txt, strlen(txt));
}

HttpMessage* handleNotFound (HttpMessage *request, void *ctx)
{
    return HttpMessage_NewHttpResponse(200, "OK", "text/html", (const uint8_t *) txt, strlen(txt));
}