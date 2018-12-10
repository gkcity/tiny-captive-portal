#-----------------------------------------------------------------------------
#
# sample
#
#-----------------------------------------------------------------------------

SET(SRC main.c)

ADD_EXECUTABLE(sample ${SRC})
TARGET_LINK_LIBRARIES(sample captive_portal_static ${DEPENDENCE_LIB})
