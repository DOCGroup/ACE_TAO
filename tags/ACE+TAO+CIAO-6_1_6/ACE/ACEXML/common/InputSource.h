// -*- C++ -*-

//=============================================================================
/**
 *  @file    InputSource.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_INPUTSOURCE_H_
#define _ACEXML_INPUTSOURCE_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/CharStream.h"
#include "ace/Copy_Disabled.h"

/**
 * @class ACEXML_InputSource
 *
 * @brief ACEXML_InputSource encapsulates the actual input stream with some
 * added information.
 *
 * This class allows a SAX application to encapsulate information
 * about an input source in a single object, which may include a
 * public identifier, a system identifier, a byte stream (possibly
 * with a specified encoding), and/or a character stream.
 *
 * There are two places that the application will deliver this input
 * source to the parser: as the argument to the Parser.parse method,
 * or as the return value of the EntityResolver.resolveEntity method.
 *
 * The SAX parser will use the InputSource object to determine how to
 * read XML input. If there is a character stream available, the
 * parser will read that stream directly; if not, the parser will use
 * a byte stream, if available; if neither a character stream nor a
 * byte stream is available, the parser will attempt to open a URI
 * connection to the resource identified by the system identifier.
 *
 * An InputSource object belongs to the application: the SAX parser
 * shall never modify it in any way (it may modify a copy if
 * necessary).
 *
 * @sa ACEXML_CharStream
 */
class ACEXML_Export ACEXML_InputSource : private ACE_Copy_Disabled
{
public:
  /**
   * Default constructor.
   */
  ACEXML_InputSource (void);


  /**
   * Create a new input source with a ACEXML_Char stream.
   * Notice that ACEXML_InputSource assumes the ownership
   * of <stream>
   */
  explicit ACEXML_InputSource (ACEXML_CharStream *stream);

  /**
   * Create a new input source with a system identifier.
   */
  ACEXML_InputSource (const ACEXML_Char *systemId);

  /**
   * Default destructor.
   */
  virtual ~ACEXML_InputSource (void);

  /**
   * Get the ACEXML_Char stream for this input source.
   */
  virtual ACEXML_CharStream *getCharStream (void) const;

  /**
   * Get the character encoding for a byte stream or URI.
   */
  virtual const ACEXML_Char *getEncoding (void) const;

  /**
   * Get the public identifier for this input source.
   */
  virtual const ACEXML_Char *getPublicId (void) const;

  /**
   * Get the system identifier for this input source.
   */
  virtual const ACEXML_Char *getSystemId (void) const;

  /**
   * Set the ACEXML_Char stream for this input source.
   * Notice that ACEXML_InputSource assumes the ownership
   * of <stream>
   */
  virtual void setCharStream (ACEXML_CharStream *charStream);

  /**
   * Set the character encoding, if known.
   */
  virtual void setEncoding (const ACEXML_Char *encoding);

  /**
   * Set the public identifier for this input source.
   */
  virtual void setPublicId (const ACEXML_Char *publicId);

  /**
   * Set the public identifier for this input source.
   */
  virtual void setSystemId (const ACEXML_Char *systemId);

private:
  ACEXML_CharStream *charStream_;
  ACEXML_Char *encoding_;
  ACEXML_Char *publicId_;
  ACEXML_Char *systemId_;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_INPUTSOURCE_H_ */
