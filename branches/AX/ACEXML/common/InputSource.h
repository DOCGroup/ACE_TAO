// -*- C++ -*- $Id$

#ifndef _ACEXML_INPUTSOURCE_H_
#define _ACEXML_INPUTSOURCE_H_

#include "common/CharStream.h"

class ACEXML_Export ACEXML_InputSource
{
public:
  /*
   * Default constructor.
   */
  ACEXML_InputSource (void);

  /*
   * Create a new input source with a ACEXML_Char stream.
   * Notice that ACEXML_InputSource assumes the ownership
   * of <stream>
   */
  ACEXML_InputSource (ACEXML_CharStream *stream);

  /*
   * Create a new input source with a system identifier.
   */
  ACEXML_InputSource (const ACEXML_Char *systemId);

  /*
   * Default destructor.
   */
  virtual ~ACEXML_InputSource (void);

  /*
   * Get the ACEXML_Char stream for this input source.
   */
  virtual ACEXML_CharStream *getCharStream (void);

  /*
   * Get the character encoding for a byte stream or URI.
   */
  virtual const ACEXML_Char *getEncoding (void);

  /*
   * Get the public identifier for this input source.
   */
  virtual const ACEXML_Char *getPublicId (void);

  /*
   * Get the system identifier for this input source.
   */
  virtual const ACEXML_Char *getSystemId (void);

  /*
   * Set the ACEXML_Char stream for this input source.
   * Notice that ACEXML_InputSource assumes the ownership
   * of <stream>
   */
  virtual void setCharStream (ACEXML_CharStream *charStream);

  /*
   * Set the character encoding, if known.
   */
  virtual void setEncoding (const ACEXML_Char *encoding);

  /*
   * Set the public identifier for this input source.
   */
  virtual void setPublicId (const ACEXML_Char *publicId);

  /*
   * Set the public identifier for this input source.
   */
  virtual void setSystemId (const ACEXML_Char *systemId);

private:
  ACEXML_Char *publicId_;
  ACEXML_Char *systemId_;
  ACEXML_CharStream *charStream_;
  ACEXML_Char *encoding_;
};

#endif /* _ACEXML_INPUTSOURCE_H_ */
