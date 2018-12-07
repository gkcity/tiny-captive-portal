/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   DNSServer.h
 *
 * @remark
 *
 */

#ifndef __DNS_SERVER_H__
#define __DNS_SERVER_H__

#include <channel/Channel.h>

TINY_BEGIN_DECLS


TINY_LOR
Channel *DNSServer_New(uint32_t ip);


TINY_END_DECLS

#endif /* __DNS_SERVER_H__ */