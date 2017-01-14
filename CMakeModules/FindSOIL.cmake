# - Try to find SOIL
# Once done this will define
#
#  SOIL_FOUND - system has SOIL
#  SOIL_INCLUDE_DIR - the SOIL include directory
#  SOIL_LIBRARIES - Link these to use SOIL
#

SET(SOIL "SOIL")

FIND_PATH(SOIL_INCLUDE_DIR NAMES SOIL/SOIL.h
  PATHS
  /usr/include
  /usr/local/include
  /opt/local/include
  NO_DEFAULT_PATH
)
 

FIND_LIBRARY(LIBSOIL
  NAMES 
  ${SOIL}
  PATHS
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)

SET (SOIL_LIBRARIES
  ${LIBSOIL} 
)

IF(SOIL_INCLUDE_DIR AND SOIL_LIBRARIES)
   SET(SOIL_FOUND TRUE)
ENDIF(SOIL_INCLUDE_DIR AND SOIL_LIBRARIES)

IF(SOIL_FOUND)
  MARK_AS_ADVANCED(SOIL_INCLUDE_DIR SOIL_LIBRARIES )
ENDIF(SOIL_FOUND)