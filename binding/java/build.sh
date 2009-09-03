#!/bin/bash
echo " ---------------------------------"
echo " |    Java (jdk6) building   |"
echo " ---------------------------------"
echo ""
echo "Generating interface files.."
swig -c++ -java -package org.gephi.libgexf -outdir org/gephi/libgexf -o libgexf_wrap.cpp libgexf.i

echo "Compiling.."
gcc -fPIC -c libgexf_wrap.cpp \
    ../../libgexf/gexf.cpp \
    ../../libgexf/memoryvalidator.cpp \
    ../../libgexf/filereader.cpp \
    ../../libgexf/gexfparser.cpp \
    ../../libgexf/legacyparser.cpp \
    ../../libgexf/rngvalidator.cpp \
    ../../libgexf/schemavalidator.cpp \
    ../../libgexf/filewriter.cpp \
    ../../libgexf/legacywriter.cpp \
    ../../libgexf/conv.cpp \
    ../../libgexf/graph.cpp \
    ../../libgexf/directedgraph.cpp \
    ../../libgexf/undirectedgraph.cpp \
    ../../libgexf/nodeiter.cpp \
    ../../libgexf/edgeiter.cpp \
    ../../libgexf/data.cpp \
    ../../libgexf/metadata.cpp \
    ../../libgexf/attributeiter.cpp \
    ../../libgexf/attvalueiter.cpp \
    -I/usr/lib/jvm/java-6-sun-1.6.0.10/include -I/usr/lib/jvm/java-6-sun-1.6.0.10/include/linux -I/usr/include/libxml2
g++ -shared -lxml2 libgexf_wrap.o \
    gexf.o \
    memoryvalidator.o \
    filereader.o \
    gexfparser.o \
    legacyparser.o \
    rngvalidator.o \
    schemavalidator.o \
    filewriter.o \
    legacywriter.o \
    conv.o \
    graph.o \
    directedgraph.o \
    undirectedgraph.o \
    nodeiter.o \
    edgeiter.o \
    data.o \
    metadata.o \
    attributeiter.o \
    attvalueiter.o \
    -o libgexf.so

#echo "Creating JAR.."
#javac org/gephi/libgexf/*.java
#jar cfm libgexf.jar Manifest.txt org/gephi/libgexf/*.class
# TODO edit Manifest.txt Main-Class

echo "Cleaning.."
rm *.o
#rm libgexf_wrap.cpp
echo "Testing.."
javac runme.java
java -Xmx256m runme #-verbose:jni

echo "Done!"

