/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   CaptivePortal.c
 *
 * @remark
 *
 */

#include <bootstrap/Bootstrap.h>
#include <tiny_malloc.h>
#include <tiny_log.h>
#include <channel/SocketChannel.h>
#include "CaptivePortal.h"
#include "DNSServer.h"
#include "WebServer.h"

#define TAG     "CaptivePortal"

TINY_LOR
CaptivePortal * CaptivePortal_New(BootstrapLoopHook hook, void *ctx)
{
    CaptivePortal *thiz = NULL;

    do
    {
        thiz = (CaptivePortal *) tiny_malloc(sizeof(CaptivePortal));
        if (thiz == NULL)
        {
            LOG_D(TAG, "tiny_malloc FAILED.");
            break;
        }

        if (RET_FAILED(CaptivePortal_Construct(thiz, hook, ctx)))
        {
            CaptivePortal_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TINY_LOR
void CaptivePortal_Delete(CaptivePortal *thiz)
{
    RETURN_IF_FAIL(thiz);

    CaptivePortal_Dispose(thiz);
    tiny_free(thiz);
}

TINY_LOR
TinyRet CaptivePortal_Construct(CaptivePortal *thiz, BootstrapLoopHook hook, void *ctx)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        memset(thiz, 0, sizeof(CaptivePortal));

        ret = Bootstrap_Construct(&thiz->bootstrap, hook, ctx);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

TINY_LOR
void CaptivePortal_Dispose(CaptivePortal *thiz)
{
    RETURN_IF_FAIL(thiz);

    Bootstrap_Dispose(&thiz->bootstrap);
    memset(thiz, 0, sizeof(CaptivePortal));
}

TINY_LOR
TinyRet CaptivePortal_Run(CaptivePortal *thiz, uint32_t ip)
{
    TinyRet ret = TINY_RET_OK;
    Channel *dns = NULL;
    Channel *server = NULL;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        if (thiz->started)
        {
            ret = TINY_RET_E_STARTED;
            break;
        }

        thiz->started = true;

        dns = DNSServer_New(ip);
        if (dns == NULL)
        {
            ret = TINY_RET_E_STARTED;
            break;
        }

        server = WebServer_New(ip, 80);
        if (server == NULL)
        {
            ret = TINY_RET_E_STARTED;
            SocketChannel_Delete(dns);
            break;
        }

        ret = Bootstrap_AddChannel(&thiz->bootstrap, dns);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "Bootstrap_AddChannel failed");
            SocketChannel_Delete(dns);
            SocketChannel_Delete(server);
            break;
        }

        ret = Bootstrap_AddChannel(&thiz->bootstrap, server);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "Bootstrap_AddChannel failed");
            SocketChannel_Delete(server);
            break;
        }

        ret = Bootstrap_Sync(&thiz->bootstrap);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "Bootstrap_Sync failed");
            SocketChannel_Delete(dns);
            SocketChannel_Delete(server);
            break;
        }
    } while (false);

    return ret;
}

TINY_LOR
TinyRet CaptivePortal_Stop(CaptivePortal *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    if (! thiz->started)
    {
        return TINY_RET_E_STOPPED;
    }

    Bootstrap_Shutdown(&thiz->bootstrap);

    return TINY_RET_OK;
}