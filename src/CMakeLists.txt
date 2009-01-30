INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET( LIBEASTWOOD_SRCS
  AdlFile.cpp
  CpsFile.cpp
  Decode.cpp
  EastwoodException.cpp
  Font.cpp
  IcnFile.cpp
  IniFile.cpp
  PakFile.cpp
  PalFile.cpp
  ShpFile.cpp
  StringFile.cpp
  VocFile.cpp
  WsaFile.cpp
  adl/emuopl.cpp
  adl/fmopl.c
)

SET( LIBEASTWOOD_HEADERS
  AdlFile.h
  CpsFile.h
  Decode.h
  EastwoodException.h
  Font.h
  IcnFile.h
  IniFile.h
  Log.h
  PakFile.h
  PalFile.h
  ShpFile.h
  StringFile.h
  VocFile.h
  WsaFile.h
  adl/emuopl.h
  adl/fmopl.h
  adl/opl.h
)

INSTALL( FILES ${LIBEASTWOOD_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eastwood" )

ADD_LIBRARY( eastwood SHARED ${LIBEASTWOOD_SRCS} )

TARGET_LINK_LIBRARIES(eastwood ${SDL_LIBRARY} )
TARGET_LINK_LIBRARIES(eastwood ${SDLMIXER_LIBRARY} )
TARGET_LINK_LIBRARIES(eastwood ${LIBSAMPLERATE_LIBRARIES} )


SET_TARGET_PROPERTIES( eastwood PROPERTIES VERSION "${LIBEASTWOOD_VERSION_INFO}" )
SET_TARGET_PROPERTIES( eastwood PROPERTIES SOVERSION "${LIBEASTWOOD_SOVERSION_INFO}" )

INSTALL(TARGETS eastwood LIBRARY DESTINATION ${LIB_INSTALL_DIR} )

IF(LIBEASTWOOD_BUILD_PYTHON)
  GET_FILENAME_COMPONENT(PYTHON_LIB_INSTALL "${PYTHON_LIBRARY}" PATH)
  INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_PATH} ${BOOST_INCLUDE_DIR} )
  PYTHON_ADD_MODULE( pyeastwood PyEastwood.cpp)
  TARGET_LINK_LIBRARIES( pyeastwood boost_python )
  TARGET_LINK_LIBRARIES( pyeastwood boost_filesystem )
  TARGET_LINK_LIBRARIES( pyeastwood eastwood )
  SET_TARGET_PROPERTIES(pyeastwood PROPERTIES PREFIX "")
ENDIF(LIBEASTWOOD_BUILD_PYTHON)
