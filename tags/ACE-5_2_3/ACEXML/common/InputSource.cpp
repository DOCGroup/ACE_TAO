// -*- C++ -*- $Id$

#include "ACEXML/common/InputSource.h"
#include "ace/ACE.h"

ACEXML_InputSource::ACEXML_InputSource (void)
  : publicId_ (0),
    systemId_ (0),
    charStream_ (0),
    encoding_ (0)
{
}

ACEXML_InputSource::ACEXML_InputSource (ACEXML_CharStream *stm)
  : publicId_ (0),
    systemId_ (0),
    charStream_ (stm),
    encoding_ (0)
{
}

  /*
   * Create a new input source with a character stream.
   * /
    InputSource (Reader);
  */

ACEXML_InputSource::ACEXML_InputSource (const ACEXML_Char *systemId)
  : systemId_ (ACE::strnew (systemId))
{
}

ACEXML_InputSource::~ACEXML_InputSource (void)
{
  delete this->publicId_;
  delete this->systemId_;
  delete this->charStream_;
  delete this->encoding_;
}

ACEXML_CharStream *
ACEXML_InputSource::getCharStream (void)
{
  return this->charStream_;
}

  /*
   * Get the character stream for this input source.
   * /
   virtual Reader *getCharacterStream (void);
  */

const ACEXML_Char *
ACEXML_InputSource::getEncoding (void)
{
  return this->encoding_;
}

const ACEXML_Char *
ACEXML_InputSource::getPublicId (void)
{
  return this->publicId_;
}

const ACEXML_Char *
ACEXML_InputSource::getSystemId (void)
{
  return this->systemId_;
}

void
ACEXML_InputSource::setCharStream (ACEXML_CharStream *stm)
{
  delete this->charStream_;
  this->charStream_ = stm;
}

  /*
   * Set the character stream for this input source.
   * /
   virtual void setCharacterStream (Reader *characterStream);
  */

void
ACEXML_InputSource::setEncoding (const ACEXML_Char *encoding)
{
  this->encoding_ = ACE::strnew (encoding);
}

void
ACEXML_InputSource::setPublicId (const ACEXML_Char *publicId)
{
  this->publicId_ = ACE::strnew (publicId);
}

void
ACEXML_InputSource::setSystemId (const ACEXML_Char *systemId)
{
  this->systemId_ = ACE::strnew (systemId);
}
