// -*- C++ -*- $Id$

#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/StreamFactory.h"
#include "ace/ACE.h"

ACEXML_InputSource::ACEXML_InputSource (void)
  : charStream_ (0),
    encoding_ (0),
    publicId_ (0),
    systemId_ (0)
{
}

ACEXML_InputSource::ACEXML_InputSource (ACEXML_CharStream *stm)
  : charStream_ (stm),
    encoding_ (ACE::strnew (stm->getEncoding())),
    publicId_ (0),
    systemId_ (stm->getSystemId() ? ACE::strnew (stm->getSystemId()): 0)
{
}

  /*
   * Create a new input source with a character stream.
   *
  */

ACEXML_InputSource::ACEXML_InputSource (const ACEXML_Char *systemId)
  : charStream_ (0),
    encoding_ (0),
    publicId_ (0),
    systemId_ (ACE::strnew (systemId))
{
  ACEXML_StreamFactory factory;
  this->setCharStream (factory.create_stream (this->systemId_));
  this->setEncoding (this->charStream_->getEncoding());
}

ACEXML_InputSource::~ACEXML_InputSource (void)
{
  delete[] this->publicId_;
  delete[] this->systemId_;
  delete this->charStream_;
  delete[] this->encoding_;
}

ACEXML_CharStream *
ACEXML_InputSource::getCharStream (void) const
{
  return this->charStream_;
}

const ACEXML_Char *
ACEXML_InputSource::getEncoding (void) const
{
  return this->encoding_;
}

const ACEXML_Char *
ACEXML_InputSource::getPublicId (void) const
{
  return this->publicId_;
}

const ACEXML_Char *
ACEXML_InputSource::getSystemId (void) const
{
  return this->systemId_;
}

void
ACEXML_InputSource::setCharStream (ACEXML_CharStream *stm)
{
  delete this->charStream_;
  this->charStream_ = stm;
}

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
