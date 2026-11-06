// -*- C++ -*-

#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/StreamFactory.h"
#include "ace/ACE.h"

ACEXML_InputSource::ACEXML_InputSource ()
{
}

ACEXML_InputSource::ACEXML_InputSource (ACEXML_CharStream *stm)
  : charStream_ (stm),
    encoding_ (ACE::strnew (stm->getEncoding())),
    systemId_ (stm->getSystemId () ? ACE::strnew (stm->getSystemId ()) : nullptr)
{
}

  /*
   * Create a new input source with a character stream.
   *
  */

ACEXML_InputSource::ACEXML_InputSource (ACEXML_Char const *systemId)
  : systemId_ (ACE::strnew (systemId))
{
  ACEXML_StreamFactory factory;
  ACEXML_CharStream* stm = factory.create_stream (this->systemId_.get ());
  if (stm)
    {
      this->setCharStream (stm);
      this->setEncoding (this->charStream_->getEncoding());
    }
}

ACEXML_InputSource::~ACEXML_InputSource () = default;

ACEXML_CharStream *
ACEXML_InputSource::getCharStream () const
{
  return this->charStream_.get ();
}

ACEXML_Char const *
ACEXML_InputSource::getEncoding () const
{
  return this->encoding_.get ();
}

ACEXML_Char const *
ACEXML_InputSource::getPublicId () const
{
  return this->publicId_.get ();
}

ACEXML_Char const *
ACEXML_InputSource::getSystemId () const
{
  return this->systemId_.get ();
}

void
ACEXML_InputSource::setCharStream (ACEXML_CharStream *stm)
{
  this->charStream_.reset (stm);
}

void
ACEXML_InputSource::setEncoding (ACEXML_Char const *encoding)
{
  this->encoding_.reset (ACE::strnew (encoding));
}

void
ACEXML_InputSource::setPublicId (ACEXML_Char const *publicId)
{
  this->publicId_.reset (ACE::strnew (publicId));
}

void
ACEXML_InputSource::setSystemId (ACEXML_Char const *systemId)
{
  this->systemId_.reset (ACE::strnew (systemId));
}
