ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pyeastwood.so ${CMAKE_CURRENT_BINARY_DIR}/eastwood.py
    POST_BUILD
    COMMAND CPPFLAGS="-I${LIBEASTWOOD_SOURCE_DIR}/include" LDFLAGS="-L${CMAKE_CURRENT_BINARY_DIR}/../src" ${PYTHON_EXECUTABLE} setup.py build -t "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/pyeastwood.dir" --build-lib "${CMAKE_CURRENT_BINARY_DIR}/lib"
    COMMAND touch ${LIBEASTWOOD_SOURCE_DIR}/python/CMakeLists.txt
    WORKING_DIRECTORY ${LIBEASTWOOD_SOURCE_DIR}/python
    DEPENDS eastwood
    VERBATIM
    )
ADD_CUSTOM_TARGET(pyeastwood ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pyeastwood.so)

INSTALL(CODE "
    set( ENV{CPPFLAGS} -I${LIBEASTWOOD_SOURCE_DIR}/include )
    set( ENV{LDFLAGS} -L${CMAKE_CURRENT_BINARY_DIR}/../src )
    set( ENV{PWD} ${CMAKE_CACHEFILE_DIR} )
    execute_process(
	COMMAND ${PYTHON_EXECUTABLE} setup.py build -t \"${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/pyeastwood.dir\" --build-lib \"${CMAKE_CURRENT_BINARY_DIR}/lib\" install --prefix \"${CMAKE_INSTALL_PREFIX}\" --root \"$ENV{DESTDIR}\"
        WORKING_DIRECTORY ${LIBEASTWOOD_SOURCE_DIR}/python
        )
    "
)
