// -*- C++ -*-

//=============================================================================
/**
 *  @file    XMLFilter.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_XMLFILTER_H_
#define _ACEXML_XMLFILTER_H_

#include "common/XMLReader.h"

/**
 * @class ACEXML_XMLFilter XMLFilter.h "common/XMLFilter.h"
 *
 * @brief ACEXML_XMLFilter
 *
 * An XML filter is like an XML reader, except
 * that it obtains its events from another XML reader rather than a
 * primary source like an XML document or database. Filters can modify
 * a stream of events as they pass on to the final application.
 *
 * The XMLFilterImpl helper class provides a convenient base for
 * creating SAX2 filters, by passing on all ACEXML_EntityResolver,
 * ACEXML_DTDHandler, ACEXML_ContentHandler and ACEXML_ErrorHandler
 * events automatically.
 */
class ACEXML_Export ACEXML_XMLFilter : public ACEXML_XMLReader
{
public:
  /**
   * Get the parent reader.
   */
  virtual ACEXML_XMLReader *getParent (void) const = 0;

  /**
   * Set the parent reader.
   */
  virtual void setParent (ACEXML_XMLReader *parent) = 0;
};

#endif /* _ACEXML_XMLFILTER_H_ */
