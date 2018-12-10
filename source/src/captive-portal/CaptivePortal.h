/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   CaptivePortal.h
 *
 * @remark
 *
 */

#ifndef __CAPTIVE_PORTAL_H__
#define __CAPTIVE_PORTAL_H__

#include <tiny_base.h>
#include <bootstrap/Bootstrap.h>

TINY_BEGIN_DECLS


typedef struct _CaptivePortal
{
    Bootstrap               bootstrap;
    bool                    started;
} CaptivePortal;

TINY_LOR
CaptivePortal * CaptivePortal_New(BootstrapLoopHook hook, void *ctx);

TINY_LOR
void CaptivePortal_Delete(CaptivePortal *thiz);

TINY_LOR
TinyRet CaptivePortal_Construct(CaptivePortal *thiz, BootstrapLoopHook hook, void *ctx);

TINY_LOR
void CaptivePortal_Dispose(CaptivePortal *thiz);

TINY_LOR
TinyRet CaptivePortal_Run(CaptivePortal *thiz, uint32_t ip);

TINY_LOR
TinyRet CaptivePortal_Stop(CaptivePortal *thiz);


TINY_END_DECLS

#endif /* __CAPTIVE_PORTAL_H__ */
