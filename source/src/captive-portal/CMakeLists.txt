#-----------------------------------------------------------------------------
#
# captive-portal
#
#-----------------------------------------------------------------------------

SET(CaptivePortal_Header
		CaptivePortal.h
		CaptivePortalHandle.h
		DNSServer.h
		WebServer.h
		WebServerHandler.h
        )

SET(CaptivePortal_Source
		CaptivePortal.c
		CaptivePortalHandle.c
		DNSServer.c
		WebServer.c
		WebServerHandler.c
        )

SET(SRC
		${CaptivePortal_Header}
		${CaptivePortal_Source}
        )

#----------------------------------------------------------------------------
# static library
#----------------------------------------------------------------------------
ADD_LIBRARY(captive_portal_static STATIC ${SRC})
SET_TARGET_PROPERTIES(captive_portal_static PROPERTIES OUTPUT_NAME "captive_portal_static")
SET_TARGET_PROPERTIES(captive_portal_static PROPERTIES LINKER_LANGUAGE C)
