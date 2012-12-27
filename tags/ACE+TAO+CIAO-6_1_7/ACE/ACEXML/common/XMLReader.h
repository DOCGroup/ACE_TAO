// -*- C++ -*-

//=============================================================================
/**
 *  @file    XMLReader.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_XMLREADER_H_
#define _ACEXML_XMLREADER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/ContentHandler.h"
#include "ACEXML/common/DTDHandler.h"
#include "ACEXML/common/EntityResolver.h"
#include "ACEXML/common/ErrorHandler.h"

/**
 * @class ACEXML_XMLReader
 *
 * @brief ACEXML_XMLReader
 *
 * XMLReader is the interface that an XML parser's SAX2 driver must
 * implement. This interface allows an application to set and query
 * features and properties in the parser, to register event handlers
 * for document processing, and to initiate a document parse.
 *
 * All SAX interfaces are assumed to be synchronous: the parse methods
 * must not return until parsing is complete, and readers must wait
 * for an event-handler callback to return before reporting the next
 * event.
 */
class ACEXML_Export ACEXML_XMLReader
{
public:

  virtual ~ACEXML_XMLReader (void);

  /**
   * Return the current content handler.
   */
  virtual ACEXML_ContentHandler *getContentHandler (void) const = 0;

  /**
   * Return the current DTD handler.
   */
  virtual ACEXML_DTDHandler *getDTDHandler (void) const = 0;

  /**
   * Return the current entity resolver.
   */
  virtual ACEXML_EntityResolver *getEntityResolver (void) const = 0;

  /**
   * Return the current error handler.
   */
  virtual ACEXML_ErrorHandler *getErrorHandler (void) const = 0;

  /**
   * Look up the value of a feature.  This method allows
   * programmers to check whether a specific feature has been
   * activated in the parser.
   */
  virtual int getFeature (const ACEXML_Char *name) = 0;

  /**
   * Look up the value of a property.
   */
  virtual void * getProperty (const ACEXML_Char *name) = 0;

  /**
   * Parse an XML document.
   */
  virtual void parse (ACEXML_InputSource *input) = 0;

  /**
   * Parse an XML document from a system identifier (URI).
   */
  virtual void parse (const ACEXML_Char *systemId) = 0;

  /**
   * Allow an application to register a content event handler.
   */
  virtual void setContentHandler (ACEXML_ContentHandler *handler) = 0;

  /**
   * Allow an application to register a DTD event handler.
   */
  virtual void setDTDHandler (ACEXML_DTDHandler *handler) = 0;

  /**
   * Allow an application to register an entity resolver.
   */
  virtual void setEntityResolver (ACEXML_EntityResolver *resolver) = 0;

  /**
   * Allow an application to register an error event handler.
   */
  virtual void setErrorHandler (ACEXML_ErrorHandler *handler) = 0;

  /**
   * Activating or deactivating a feature.
   */
  virtual void setFeature (const ACEXML_Char *name, int boolean_value) = 0;

  /**
   * Set the value of a property.
   */
  virtual void setProperty (const ACEXML_Char *name, void *value) = 0;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_XMLREADER_H_ */
