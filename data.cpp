/*! \file data.cpp
    \author sebastien heymann
    \date 30 juin 2009, 13:35
    \version 0.1
 */

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

#include <map>

#include "data.h"
#include "attributeiter.h"
#include <sstream>
#include <map>

using namespace std;

namespace libgexf {

Data::Data() {
    this->init();
}

Data::Data(const Data& orig): _node_labels(orig._node_labels), _node_attributes(orig._node_attributes), _edge_attributes(orig._edge_attributes), 
        _node_attributes_types(orig._node_attributes_types), _edge_attributes_types(orig._edge_attributes_types),
        _node_values(orig._node_values), _edge_values(orig._edge_values) {
}

Data::~Data() {
}

void Data::init() {
    _node_labels = map<t_id,string >();
    _node_attributes = _edge_attributes = map<t_id,string >();
    _node_attributes_types = _edge_attributes_types = map<t_id,t_attr_type >();
    _node_values = _edge_values = map<t_id,map<t_id,string > >();
}

//-----------------------------------------
string Data::getLabel(const t_id node_id) const {
//-----------------------------------------
string label = "";
    map<t_id,string >::const_iterator it = _node_labels.find(node_id);
    if(it != _node_labels.end()) {
        label = it->second;
    }

    return label;
}

//-----------------------------------------
bool Data::hasLabel(const t_id node_id) const {
//-----------------------------------------
    return _node_labels.find(node_id) != _node_labels.end();
}

//-----------------------------------------
void Data::setLabel(const t_id node_id, const std::string label) {
//-----------------------------------------
    _node_labels[node_id] = label;
}

//-----------------------------------------
void Data::addNodeAttributeColumn(const t_id id, const string title, const t_attr_type type) {
//-----------------------------------------
    _node_attributes.insert(pair<t_id,string>(id,title));
    _node_attributes_types.insert(pair<t_id,t_attr_type>(id,type));
}

//-----------------------------------------
void Data::addEdgeAttributeColumn(const t_id id, const string title, const t_attr_type type) {
//-----------------------------------------
    _edge_attributes.insert(pair<t_id,string>(id,title));
    _edge_attributes_types.insert(pair<t_id,t_attr_type>(id,type));
}

//-----------------------------------------
AttributeIter* Data::getNodeAttributeColumn() const {
//-----------------------------------------
    return new AttributeIter(this, AttributeIter::NODE);
}

//-----------------------------------------
AttributeIter* Data::getEdgeAttributeColumn() const {
//-----------------------------------------
    return new AttributeIter(this, AttributeIter::EDGE);
}

//-----------------------------------------
string Data::getNodeAttribute(const t_id node_id, const t_id attr_id) const {
//-----------------------------------------
    map<t_id,map<t_id,string > >::const_iterator it = _node_values.find(node_id);
    if( it != _node_values.end() ) {
        map<t_id,string >::const_iterator it2 = (it->second).find(attr_id);
        if( it2 != (it->second).end() ) {
            return it2->second;
        }
    }
    
    return NULL;
}

//-----------------------------------------
string Data::getEdgeAttribute(const t_id edge_id, const t_id attr_id) const {
//-----------------------------------------
    map<t_id,map<t_id,string > >::const_iterator it = _edge_values.find(edge_id);
    if( it != _edge_values.end() ) {
        map<t_id,string >::const_iterator it2 = (it->second).find(attr_id);
        if( it2 != (it->second).end() ) {
            return it2->second;
        }
    }

    return NULL;
}

//-----------------------------------------
map<t_id,string > Data::getNodeAttributeRow(const t_id node_id) const {
//-----------------------------------------
    map<t_id,map<t_id,string > >::const_iterator it = _node_values.find(node_id);
    if( it != _node_values.end() ) {
        return it->second;
    }

    return map<t_id,string >();
}

//-----------------------------------------
map<t_id,string > Data::getEdgeAttributeRow(const t_id edge_id) const {
//-----------------------------------------
    map<t_id,map<t_id,string > >::const_iterator it = _edge_values.find(edge_id);
    if( it != _edge_values.end() ) {
        return it->second;
    }

    return map<t_id,string >();
}


//-----------------------------------------
void Data::setNodeValue(const t_id node_id, const t_id attr_id, const string value) {
//-----------------------------------------
    _node_values[node_id][attr_id] = value;
}

//-----------------------------------------
void Data::setEdgeValue(const t_id edge_id, const t_id attr_id, const string value) {
//-----------------------------------------
    _node_values[edge_id][attr_id] = value;
}

//-----------------------------------------
unsigned int Data::countNodeAttributeColumn() const {
//-----------------------------------------
    return _node_attributes.size();
}

//-----------------------------------------
unsigned int Data::countEdgeAttributeColumn() const {
//-----------------------------------------
    return _edge_attributes.size();
}


//-----------------------------------------
std::ostream& operator<<(std::ostream& os, const Data& o) {
//-----------------------------------------
    os << "Data [" << std::endl;

    std::map<t_id,std::string>::const_iterator it;

    for ( it=o._node_labels.begin() ; it != o._node_labels.end(); it++ ) {
        os << it->first << " => " << it->second << std::endl;
    }
    os << "]" << std::endl;
    return os;
}

} /* namespace libgexf */
