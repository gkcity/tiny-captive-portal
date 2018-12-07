CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(WebCaptivePortal C)

SET(PROJ_NAME "WebCaptivePortal")

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
SET(SOURCE_ROOT ${PROJECT_SOURCE_DIR}/source/src)
SET(TINY_ROOT ${PROJECT_SOURCE_DIR}/../tiny)
SET(TINYMDNS_ROOT ${PROJECT_SOURCE_DIR}/../tinymdns)

INCLUDE_DIRECTORIES(${SOURCE_ROOT}/captive-portal)

INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyBase)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyPortable)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyContainer)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyLog)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyStringUtil)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyRandom)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/Netty)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/HttpClient)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TcpClient)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/Json)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/StringArray)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/ByteBuffer)
INCLUDE_DIRECTORIES(${TINY_ROOT}/source/src/TinyThread)

INCLUDE_DIRECTORIES(${TINYMDNS_ROOT}/source/src/dns-common)
INCLUDE_DIRECTORIES(${TINYMDNS_ROOT}/source/src/dns-server)

#---------------------------------------------------------------------------------------
#
# link dir
#
#---------------------------------------------------------------------------------------
LINK_DIRECTORIES(${TINY_ROOT}/target)
LINK_DIRECTORIES(${TINYMDNS_ROOT}/target)

#-------------------------------------------------------------------------------------
# output dir
#-------------------------------------------------------------------------------------
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/target)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/target)
MESSAGE(STATUS "executable output:  ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "library output: ${LIBRARY_OUTPUT_PATH}")

#-------------------------------------------------------------------------------------
#
# Source
#
#-------------------------------------------------------------------------------------
SET(CaptivePortal_Header
		${SOURCE_ROOT}/captive-portal/CaptivePortal.h
		${SOURCE_ROOT}/captive-portal/DNSServer.h
		${SOURCE_ROOT}/captive-portal/WebServer.h
		${SOURCE_ROOT}/captive-portal/WebServerHandler.h
        )

SET(CaptivePortal_Source
		${SOURCE_ROOT}/captive-portal/CaptivePortal.c
		${SOURCE_ROOT}/captive-portal/DNSServer.c
		${SOURCE_ROOT}/captive-portal/WebServer.c
		${SOURCE_ROOT}/captive-portal/WebServerHandler.c
        )

SET(SRC
        ${CaptivePortal_Header}
        ${CaptivePortal_Source}
        )


#----------------------------------------------------------------------------
# static library
#----------------------------------------------------------------------------
IF (EXPORT_STATIC_LIBRARY EQUAL 1)
ADD_LIBRARY(webcp_static STATIC ${SRC})
SET_TARGET_PROPERTIES(webcp_static PROPERTIES OUTPUT_NAME "webcp_static")
SET_TARGET_PROPERTIES(webcp_static PROPERTIES LINKER_LANGUAGE C)
ENDIF ()

#----------------------------------------------------------------------------
# shared library
#----------------------------------------------------------------------------
IF (EXPORT_SHARED_LIBRARY EQUAL 1)
MESSAGE(STATUS "link library: ${DEPENDENCE_LIB}")
ADD_LIBRARY(webcp SHARED ${SRC})
SET_TARGET_PROPERTIES(webcp PROPERTIES OUTPUT_NAME "webcp")
SET_TARGET_PROPERTIES(webcp PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(webcp ${DEPENDENCE_LIB})
ENDIF ()
