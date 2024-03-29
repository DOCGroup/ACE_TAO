// -*- C++ -*-

#include "SAXPrint_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "SAXPrint_Handler.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_SAXPrint_Handler::ACEXML_SAXPrint_Handler (const ACEXML_Char* filename)
  : indent_ (0),  fileName_(ACE::strnew (filename)), locator_ (0)
{
}

ACEXML_SAXPrint_Handler::~ACEXML_SAXPrint_Handler ()
{
  delete [] this->fileName_;
}

void
ACEXML_SAXPrint_Handler::characters (const ACEXML_Char *cdata,
                                     size_t,
                                     size_t)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s"),
              cdata));
}

void
ACEXML_SAXPrint_Handler::endDocument ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void
ACEXML_SAXPrint_Handler::endElement (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *qName)
{
  this->dec_indent ();
  this->print_indent ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("</%s>"),
              qName));
}

void
ACEXML_SAXPrint_Handler::endPrefixMapping (const ACEXML_Char *)
{
//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("* Event endPrefixMapping (%s) ***************\n"),
//               prefix));
}

void
ACEXML_SAXPrint_Handler::ignorableWhitespace (const ACEXML_Char * cdata,
                                              int,
                                              int)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s"),
              cdata));
//   ACE_DEBUG ((LM_DEBUG,
  //               ACE_TEXT ("* Event ignorableWhitespace () ***************\n")));
}

void
ACEXML_SAXPrint_Handler::processingInstruction (const ACEXML_Char *target,
                                                const ACEXML_Char *data)
{
  this->print_indent ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<?%s %s>\n"),
              target, data));
}

void
ACEXML_SAXPrint_Handler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Event setDocumentLocator () ***************\n")));
}

void
ACEXML_SAXPrint_Handler::skippedEntity (const ACEXML_Char *name)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event skippedEntity (%s) ***************\n"),
              name));
}

void
ACEXML_SAXPrint_Handler::startDocument ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startDocument () ***************\n")));
}

void
ACEXML_SAXPrint_Handler::startElement (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       const ACEXML_Char *qName,
                                       ACEXML_Attributes *alist)
{
  this->print_indent ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("<%s"),
              qName));
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT (" %s = \"%s\""),
                    alist->getQName (i), alist->getValue (i)));
      }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (">")));
  this->inc_indent ();
}

void
ACEXML_SAXPrint_Handler::startPrefixMapping (const ACEXML_Char * ,
                                             const ACEXML_Char *)
{
//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("* Event startPrefixMapping () ***************\n")));
//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("Prefix = %s, URI = %s\n"), prefix, uri));
}

// *** Methods inherited from ACEXML_DTDHandler.

void
ACEXML_SAXPrint_Handler::notationDecl (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_SAXPrint_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *)
{
  // No-op.
}

// Methods inherited from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_SAXPrint_Handler::resolveEntity (const ACEXML_Char *,
                                        const ACEXML_Char *)
{
  // No-op.
  return 0;
}

// Methods inherited from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
ACEXML_SAXPrint_Handler::error (ACEXML_SAXParseException & ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_SAXPrint_Handler::fatalError (ACEXML_SAXParseException & ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_SAXPrint_Handler::warning (ACEXML_SAXParseException & ex)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_SAXPrint_Handler::print_indent ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
  for (size_t i = 0; i < this->indent_; ++i)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("  ")));
}
