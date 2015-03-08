set( CMAKE_INCLUDE_CURRENT_DIR ON )

# Globbing is not recommended -- CMake cannot tell when new files are added
set( LIBGEXF_INCLUDE_FILES 
    abstractiter.h
    abstractparser.h
    attributeiter.h
    attvalueiter.h
    conv.h
    data.h
    directedgraph.h
    dynamicgraph.h
    edgeiter.h
    exceptions.h
    filereader.h
    filewriter.h
    gexf.h
    gexfparser.h
    graph.h
    legacyparser.h
    legacywriter.h
    libgexf.h
    memoryvalidator.h
    metadata.h
    nodeiter.h
    rngvalidator.h
    schemavalidator.h
    typedefs.h
    undirectedgraph.h
    )

set(LIBGEXF_SOURCE_FILES
    abstractparser.cpp
    attributeiter.cpp
    attvalueiter.cpp
    conv.cpp
    data.cpp
    directedgraph.cpp
    dynamicgraph.cpp
    edgeiter.cpp
    filereader.cpp
    filewriter.cpp
    gexf.cpp
    gexfparser.cpp
    graph.cpp
    legacyparser.cpp
    legacywriter.cpp
    memoryvalidator.cpp
    metadata.cpp
    nodeiter.cpp
    rngvalidator.cpp
    schemavalidator.cpp
    undirectedgraph.cpp
    )


set( ALL_SOURCES ${LIBGEXF_INCLUDE_FILES} ${LIBGEXF_SOURCE_FILES} )

find_package(LibXml2)

include_directories(${LIBXML2_INCLUDE_DIR})

# TARGET
add_library( LibGEXF STATIC ${ALL_SOURCES} )

set_target_properties(LibGEXF PROPERTIES OUTPUT_NAME gexf)
target_link_libraries(LibGEXF ${LibXml2_LIBRARIES})

# INSTALL
# -------

install (
    TARGETS LibGEXF
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/libgexf
    )
install(
    FILES ${LIBGEXF_INCLUDE_FILES} DESTINATION include/libgexf
    )
