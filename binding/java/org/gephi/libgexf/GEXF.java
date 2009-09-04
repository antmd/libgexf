/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.gephi.libgexf;

public class GEXF {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected GEXF(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(GEXF obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libgexfJNI.delete_GEXF(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public GEXF() {
    this(libgexfJNI.new_GEXF__SWIG_0(), true);
  }

  public GEXF(GEXF orig) {
    this(libgexfJNI.new_GEXF__SWIG_1(GEXF.getCPtr(orig), orig), true);
  }

  public UndirectedGraph getUndirectedGraph() {
    return new UndirectedGraph(libgexfJNI.GEXF_getUndirectedGraph(swigCPtr, this), false);
  }

  public DirectedGraph getDirectedGraph() {
    return new DirectedGraph(libgexfJNI.GEXF_getDirectedGraph(swigCPtr, this), false);
  }

  public Data getData() {
    return new Data(libgexfJNI.GEXF_getData(swigCPtr, this), false);
  }

  public MetaData getMetaData() {
    return new MetaData(libgexfJNI.GEXF_getMetaData(swigCPtr, this), false);
  }

  public void setGraphType(t_graph t) {
    libgexfJNI.GEXF_setGraphType(swigCPtr, this, t.swigValue());
  }

  public t_graph getGraphType() {
    return t_graph.swigToEnum(libgexfJNI.GEXF_getGraphType(swigCPtr, this));
  }

  public boolean checkIntegrity() {
    return libgexfJNI.GEXF_checkIntegrity(swigCPtr, this);
  }

  public void set_graph(Graph value) {
    libgexfJNI.GEXF__graph_set(swigCPtr, this, Graph.getCPtr(value), value);
  }

  public Graph get_graph() {
    long cPtr = libgexfJNI.GEXF__graph_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Graph(cPtr, false);
  }

  public void set_type(t_graph value) {
    libgexfJNI.GEXF__type_set(swigCPtr, this, value.swigValue());
  }

  public t_graph get_type() {
    return t_graph.swigToEnum(libgexfJNI.GEXF__type_get(swigCPtr, this));
  }

  public void set_data(Data value) {
    libgexfJNI.GEXF__data_set(swigCPtr, this, Data.getCPtr(value), value);
  }

  public Data get_data() {
    long cPtr = libgexfJNI.GEXF__data_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Data(cPtr, false);
  }

  public void set_meta(MetaData value) {
    libgexfJNI.GEXF__meta_set(swigCPtr, this, MetaData.getCPtr(value), value);
  }

  public MetaData get_meta() {
    long cPtr = libgexfJNI.GEXF__meta_get(swigCPtr, this);
    return (cPtr == 0) ? null : new MetaData(cPtr, false);
  }

}
