// -*- C++ -*-

//=============================================================================
/**
 *  @file    ErrorHandler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_ERRORHANDLER_H_
#define _ACEXML_ERRORHANDLER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/SAXExceptions.h"

/**
 * @class ACEXML_ErrorHandler
 *
 * @brief ACEXML_ErrorHandler
 *
 * If a SAX application needs to implement customized error handling,
 * it must implement this interface and then register an instance with
 * the XML reader using the setErrorHandler method. The parser will
 * then report all errors and warnings through this interface.
 *
 * @b WARNING: If an application does not register an ErrorHandler,
 * XML parsing errors will go unreported and bizarre behaviour may
 * result.
 *
 * For XML processing errors, a SAX driver must use this interface
 * instead of throwing an exception: it is up to the application to
 * decide whether to throw an exception for different types of errors
 * and warnings. Note, however, that there is no requirement that the
 * parser continue to provide useful information after a call to
 * fatalError (in other words, a SAX driver class could catch an
 * exception and report a fatalError).
 */
class ACEXML_Export ACEXML_ErrorHandler
{
public:

  virtual ~ACEXML_ErrorHandler (void);

  /**
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception) = 0;

  /**
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception) = 0;

  /**
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception) = 0;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_ERRORHANDLER_H_ */
