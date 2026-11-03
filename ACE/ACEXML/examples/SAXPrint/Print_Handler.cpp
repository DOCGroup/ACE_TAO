// -*- C++ -*-

#include "Print_Handler.h"
#include "ace/Log_Msg.h"

ACEXML_Print_Handler::ACEXML_Print_Handler (ACEXML_Char const *fileName)
  : file_name_ (fileName == nullptr ? ACE_TEXT ("") : fileName),
    locator_ (nullptr)
{
}

void
ACEXML_Print_Handler::characters (ACEXML_Char const *cdata,
                                  size_t start,
                                  size_t length)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event characters () ** start: %u  end: %u ***************\n%s\n- End event characters () ---------------\n"),
              start, length, cdata));
}

void
ACEXML_Print_Handler::endDocument ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endDocument () ***************\n")));
}

void
ACEXML_Print_Handler::endElement (ACEXML_Char const *uri,
                                  ACEXML_Char const *name,
                                  ACEXML_Char const *qName)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endElement (%s, %s, %s) ***************\n"),
              uri, name, qName));
}

void
ACEXML_Print_Handler::endPrefixMapping (ACEXML_Char const *prefix)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endPrefixMapping (%s) ***************\n"),
              prefix));
}

void
ACEXML_Print_Handler::ignorableWhitespace (ACEXML_Char const *,
                                           int,
                                           int)
{
  //   ACE_DEBUG ((LM_DEBUG,
  //               ACE_TEXT ("* Event ignorableWhitespace () ***************\n")));
}

void
ACEXML_Print_Handler::processingInstruction (ACEXML_Char const *target,
                                             ACEXML_Char const *data)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event processingInstruction (%s, %s) ***************\n"),
              target, data));
}

void
ACEXML_Print_Handler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Event setDocumentLocator () ***************\n")));
}

void
ACEXML_Print_Handler::skippedEntity (ACEXML_Char const *name)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event skippedEntity (%s) ***************\n"),
              name));
}

void
ACEXML_Print_Handler::startDocument ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startDocument () ***************\n")));
}

void
ACEXML_Print_Handler::startElement (ACEXML_Char const *uri,
                                    ACEXML_Char const *name,
                                    ACEXML_Char const *qName,
                                    ACEXML_Attributes *alist)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startElement (%s, %s, %s) ***************\n"),
              uri, name, qName));

  if (alist != nullptr)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("    %s = \"%s\"\n"),
                    alist->getQName (i), alist->getValue (i)));
      }
}

void
ACEXML_Print_Handler::startPrefixMapping (ACEXML_Char const *prefix,
                                          ACEXML_Char const *uri)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startPrefixMapping () ***************\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Prefix = %s, URI = %s\n"), prefix, uri));
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Print_Handler::notationDecl (ACEXML_Char const *name,
                                    ACEXML_Char const *publicID,
                                    ACEXML_Char const *systemID)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event notationDecl: (%s) "),
              name));

  if (publicID == nullptr)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("SYSTEM %s\n"),
                systemID));
  else if (systemID == nullptr)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("PUBLIC %s\n"),
                publicID));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("PUBLIC %s %s\n"),
                publicID, systemID));
}

void
ACEXML_Print_Handler::unparsedEntityDecl (ACEXML_Char const *name,
                                          ACEXML_Char const *publicID,
                                          ACEXML_Char const *systemID,
                                          ACEXML_Char const *notationName)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Unparsed Entity: %s"),
              name));

  if (publicID == nullptr)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT (" SYSTEM %s"),
                systemID));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT (" PUBLIC %s %s"),
                publicID, systemID));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" NDATA %s\n"),
              notationName));
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Print_Handler::resolveEntity (ACEXML_Char const *,
                                     ACEXML_Char const *)
{
  // No-op.
  return nullptr;
}

// Methods inherit from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
ACEXML_Print_Handler::error (ACEXML_SAXParseException & ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == nullptr
               ? this->file_name_.c_str ()
               : this->locator_->getSystemId ()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_Print_Handler::fatalError (ACEXML_SAXParseException& ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == nullptr
               ? this->file_name_.c_str ()
               : this->locator_->getSystemId ()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_Print_Handler::warning (ACEXML_SAXParseException & ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == nullptr
               ? this->file_name_.c_str ()
               : this->locator_->getSystemId ()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}
