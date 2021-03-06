/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.gephi.libgexf;

public class AbstractParser {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected AbstractParser(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AbstractParser obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libgexfJNI.delete_AbstractParser(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void bind(GEXF gexf) {
    libgexfJNI.AbstractParser_bind(swigCPtr, this, GEXF.getCPtr(gexf), gexf);
  }

  public void processNode(SWIGTYPE_p_xmlTextReaderPtr reader, SWIGTYPE_p_xmlChar name) {
    libgexfJNI.AbstractParser_processNode(swigCPtr, this, SWIGTYPE_p_xmlTextReaderPtr.getCPtr(reader), SWIGTYPE_p_xmlChar.getCPtr(name));
  }

}
