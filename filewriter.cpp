/*! \file FileWriter.cpp
   \author sebastien heymann
   \date 8 juillet 2009, 17:58
    \version 0.1
 */

#include "metadata.h"


/*
# Copyright (c) <2009> <Sebastien Heymann>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
*/

#include "filewriter.h"
#include "exceptions.h"
#include "typedefs.h"
#include "conv.h"

#include <stdexcept>
#include <libxml/xmlwriter.h>


using namespace std;

namespace libgexf {

FileWriter::FileWriter(): _gexf(0), _filepath("") {
}

FileWriter::FileWriter(const string filepath, GEXF* gexf): _gexf(gexf),_filepath(filepath) {

}

FileWriter::FileWriter(const FileWriter& orig): _gexf(orig._gexf), _filepath(orig._filepath) {
}

FileWriter::~FileWriter() {
    // do not delete _gexf
}

//-----------------------------------------
GEXF FileWriter::getGEXFCopy() {
//-----------------------------------------
    GEXF gexf_copy(*_gexf);
    return gexf_copy;
}

//-----------------------------------------
void FileWriter::init(const string filepath, GEXF* gexf) {
//-----------------------------------------
    _filepath = filepath;
    _gexf = gexf;
}


//-----------------------------------------
void FileWriter::write() {
//-----------------------------------------
    #ifndef LIBXML_READER_ENABLED
    throw FileWriterException( "LIBXML NOT FOUND" );
    #endif

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    int rc;
    xmlTextWriterPtr writer;

    /* Create a new XmlWriter for _filepath, with no compression. */
    writer = xmlNewTextWriterFilename(_filepath.c_str(), 0);
    if (writer == NULL) {
        throw FileWriterException( "Error creating the xml FileWriter" );
    }

    /* Start the document with the xml default for the version,
     * encoding _ENCODING and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, _ENCODING, NULL);
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterStartDocument" );
    }

    this->writeGexfNode(writer);
    
    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterEndDocument" );
    }

    /* Close file and free memory buffers */
    xmlFreeTextWriter(writer);
    xmlCleanupParser();
}

//-----------------------------------------
void FileWriter::writeGexfNode(xmlTextWriterPtr writer) {
//-----------------------------------------
int rc;

    /* Start an element named "gexf". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "gexf");
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterStartElement" );
    }

    /* Start an element named "xmlns" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST _gexf->getMetaData().getXmlns().c_str());
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteAttribute");
    }

    /* Start an element named "xsi" */
    rc = xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "xsi", NULL, BAD_CAST _gexf->getMetaData().getXsi().c_str());
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteAttributeNS");
    }

    /* Start an element named "schemaLocation" */
    rc = xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xsi", BAD_CAST "schemaLocation", NULL, BAD_CAST _gexf->getMetaData().getSchema().c_str());
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteAttributeNS");
    }

    /* Add an attribute with name "version" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST _gexf->getMetaData().getVersion().c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    this->writeMetaNode(writer);

    this->writeGraphNode(writer);

    /* Close the element named gexf. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterEndElement" );
    }
}

//-----------------------------------------
void FileWriter::writeMetaNode(xmlTextWriterPtr writer) {
//-----------------------------------------
int rc;

    /* Do we have to write all these nodes? */
    bool doCreator = _gexf->getMetaData().getCreator().compare("") != 0;
    bool doDesc = _gexf->getMetaData().getDescription().compare("") != 0;
    bool doKw = _gexf->getMetaData().getKeywords().compare("") != 0;
    bool doMeta = doCreator || doDesc || doKw;

    if( !doMeta ) return;

    /* Start an element named "meta" as child of gexf. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "meta");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }
    // TODO add lastmodifieddate attribute

    /* Write a text element named "creator" */
    if( doCreator ) {
        rc = xmlTextWriterWriteElement(writer, BAD_CAST "creator", BAD_CAST _gexf->getMetaData().getCreator().c_str());
        if (rc < 0) {
            throw FileWriterException("Error at xmlTextWriterWriteElement");
        }
    }

    /* Write a text element named "description" */
    if( doDesc ) {
        rc = xmlTextWriterWriteElement(writer, BAD_CAST "description", BAD_CAST _gexf->getMetaData().getDescription().c_str());
        if (rc < 0) {
            throw FileWriterException("Error at xmlTextWriterWriteElement");
        }
    }

    /* Write a text element named "keywords" */
    if( doKw ) {
        rc = xmlTextWriterWriteElement(writer, BAD_CAST "keywords", BAD_CAST _gexf->getMetaData().getKeywords().c_str());
        if (rc < 0) {
            throw FileWriterException("Error at xmlTextWriterWriteElement");
        }
    }

    /* Close the element named meta. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeGraphNode(xmlTextWriterPtr writer) {
//-----------------------------------------
int rc;

    /* Start an element named "graph" as child of gexf. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "graph");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Add an attribute with name "mode" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "mode", BAD_CAST "static");
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "defaultedgetype" */
    t_graph t = _gexf->getGraphType();
    if (t == GRAPH_DIRECTED) {
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "defaultedgetype", BAD_CAST "directed");
    }
    else if (t == GRAPH_MIXED) {
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "defaultedgetype", BAD_CAST "mixed");
    }
    else {
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "defaultedgetype", BAD_CAST "undirected");
    }
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    //_gexf->getData().
    this->writeAttributesNode(writer,"node");
    this->writeAttributesNode(writer,"edge");
    this->writeNodesNode(writer);
    this->writeEdgesNode(writer);

    /* Close the element named graph. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeNodesNode(xmlTextWriterPtr writer) {
//-----------------------------------------
int rc;

    /* Start an element named "nodes" as child of graph. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "nodes");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Iterate on each node */
    NodeIter* it = _gexf->getUndirectedGraph().getNodes();
    while(it->hasNext()) {
        t_id node_id = it->next();
        string label = _gexf->getData().getLabel(node_id);
        this->writeNodeNode(writer, Conv::unsignedIntToStr(node_id), label);
    }
    
    /* Close the element named nodes. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeNodeNode(xmlTextWriterPtr writer, const string node_id, const string label) {
//-----------------------------------------
int rc;

    /* Write an element named "node" as child of nodes. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "node");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteElement");
    }

    /* Add an attribute with name "id" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST node_id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "label" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "label", BAD_CAST label.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    if( !_gexf->getData().getNodeAttributeRow((t_id)Conv::strToUnsignedInt(node_id)).empty() )
        this->writeAttvaluesNode(writer, NODE, node_id);

    /* Close the element named node. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeEdgesNode(xmlTextWriterPtr writer) {
//-----------------------------------------
int rc;

    /* Start an element named "edges" as child of graph. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "edges");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Iterate on each edge */
    EdgeIter* it = _gexf->getUndirectedGraph().getEdges();
    while(it->hasNext()) {
        t_id edge_id = it->next();
        t_id source_id = it->currentSource();
        t_id target_id = it->currentTarget();
        unsigned int card = (unsigned int)it->currentProperty(EDGE_COUNT);
        t_edge_type type = (t_edge_type)it->currentProperty(EDGE_TYPE);
        this->writeEdgeNode(writer, Conv::unsignedIntToStr(edge_id), Conv::unsignedIntToStr(source_id), Conv::unsignedIntToStr(target_id), Conv::unsignedIntToStr(card), Conv::edgeTypeToStr(type));
    }

    /* Close the element named edges. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeEdgeNode(xmlTextWriterPtr writer, const string edge_id, const string source_id, const string target_id, const string cardinal, const string type) {
//-----------------------------------------
int rc;

    /* Write an element named "edge" as child of edges. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "edge");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteElement");
    }

    /* Add an attribute with name "id" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST edge_id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "source" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "source", BAD_CAST source_id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "target" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "target", BAD_CAST target_id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "cardinal" */
    if(cardinal.compare("1") > 0) {
        /* 1 is a defaultValue and can be omitted */
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "cardinal", BAD_CAST cardinal.c_str());
        if (rc < 0) {
            throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
        }
    }

    /* Add an attribute with name "type" */
    t_graph t = _gexf->getGraphType();
    if( (t != GRAPH_DIRECTED && type.compare("undirected") != 0) || /* undirected is the default value and can be omitted */
        (t == GRAPH_DIRECTED && type.compare("directed") != 0) ) { /* directed can be omitted if it is the default value */
        
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST type.c_str());
        if (rc < 0) {
            throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
        }
    }
    
    if( !_gexf->getData().getEdgeAttributeRow((t_id)Conv::strToUnsignedInt(edge_id)).empty() )
        this->writeAttvaluesNode(writer, EDGE, edge_id);

    /* Close the element named edge. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeAttributesNode(xmlTextWriterPtr writer, const string element_class) {
//-----------------------------------------
int rc;
AttributeIter* it;

    if( element_class.compare("node") != 0 && element_class.compare("edge") != 0 ) {
        throw invalid_argument("writeAttributesNode: invalid element_class");
    }

    if(element_class.compare("node") == 0) {
        it = _gexf->getData().getNodeAttributeColumn();
    }
    else if(element_class.compare("edge") == 0) {
        it = _gexf->getData().getEdgeAttributeColumn();
    }
    if( !it->hasNext() ) return;

    /* Start an element named "attributes" as child of graph. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "attributes");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Add an attribute with name "class" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "class", BAD_CAST element_class.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    while(it->hasNext()) {
        t_id attr_id = it->next();
        string title = it->currentTitle();
        t_attr_type type = it->currentType();
        this->writeAttributeNode(writer, Conv::unsignedIntToStr(attr_id), title, Conv::attrTypeToStr(type));
    }

    /* Close the element named attributes. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeAttributeNode(xmlTextWriterPtr writer, const string id, const string title, const string type) {
//-----------------------------------------
int rc;

    /* Start an element named "attribute" as child of attributes. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Add an attribute with name "id" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "title" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "title", BAD_CAST title.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "type" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST type.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Close the element named attribute. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeAttributeDefaultNode(xmlTextWriterPtr writer, const string default_value) {
//-----------------------------------------
int rc;

    /* Start an element named "default" as child of attribute. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "default");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Write a text element named "default" */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "default", BAD_CAST default_value.c_str());
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterWriteElement");
    }

    /* Close the element named default. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeAttvaluesNode(xmlTextWriterPtr writer, const ElemType type, const std::string id) {
//-----------------------------------------
int rc;

    /* Start an element named "attvalues" as child of node or edge. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "attvalues");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Write each attribute row */
    map<t_id,string > row = map<t_id,string >();
    if( type == NODE ) {
        row = _gexf->getData().getNodeAttributeRow((t_id)Conv::strToUnsignedInt(id));
    }
    else if( type == EDGE ) {
        row = _gexf->getData().getEdgeAttributeRow((t_id)Conv::strToUnsignedInt(id));
    }
    if( !row.empty() ) {
        for(map<t_id,string >::const_iterator it = row.begin(); it != row.end(); it++) {
            this->writeAttvalueNode(writer, Conv::unsignedIntToStr(it->first), it->second);
        }
    }
    

    /* Close the element named default. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

//-----------------------------------------
void FileWriter::writeAttvalueNode(xmlTextWriterPtr writer, const string attribute_id, const string value) {
//-----------------------------------------
int rc;

    /* Start an element named "attvalue" as child of attvalues. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "attvalue");
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterStartElement");
    }

    /* Add an attribute with name "id" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST attribute_id.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Add an attribute with name "value" */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST value.c_str());
    if (rc < 0) {
        throw FileWriterException( "Error at xmlTextWriterWriteAttribute");
    }

    /* Close the element named attvalue. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        throw FileWriterException("Error at xmlTextWriterEndElement");
    }
}

} /* namespace libgexf */