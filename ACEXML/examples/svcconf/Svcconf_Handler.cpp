// -*- C++ -*-  $Id$

#include "Svcconf_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Svcconf_Handler::ACEXML_Svcconf_Handler (void)
  : in_stream_def_ (0),
    in_module_ (0)
{
  // no-op
}

ACEXML_Svcconf_Handler::~ACEXML_Svcconf_Handler (void)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::characters (const ACEXML_Char *,
                                     int,
                                     int,
                                     ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endDocument (ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endElement (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *qName,
                                     ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // @@
  ACE_UNUSED_ARG (xmlenv);

  ACE_UNUSED_ARG (qName);
}

void
ACEXML_Svcconf_Handler::endPrefixMapping (const ACEXML_Char *,
                                           ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::ignorableWhitespace (const ACEXML_Char *,
                                              int,
                                              int,
                                              ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::processingInstruction (const ACEXML_Char *,
                                                const ACEXML_Char *,
                                                ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::setDocumentLocator (ACEXML_Locator *,
                                             ACEXML_Env &)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::skippedEntity (const ACEXML_Char *,
                                        ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startDocument (ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startElement (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       const ACEXML_Char *qName,
                                       ACEXML_Attributes *alist,
                                       ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS_String::strcmp (qName, "dynamic") == 0)
    {

    }
  else if (ACE_OS_String::strcmp (qName, "initializer") == 0)
    {

    }
  else if (ACE_OS_String::strcmp (qName, "static") == 0)
    {

    }
  else if (ACE_OS_String::strcmp (qName, "stream") == 0)
    {
      this->get_stream_id (alist, xmlenv);
      // @@ retrieve stream from Service_Repository here.
      return;
    }
  else if (ACE_OS_String::strcmp (qName, "streamdef") == 0)
    {
      this->in_stream_def_ = 1;
      this->get_stream_id (alist, xmlenv);
      return;
    }
  else if (ACE_OS_String::strcmp (qName, "module") == 0)
    {
      this->in_module_ = 1;

    }
  else if (ACE_OS_String::strcmp (qName, "resume") == 0)
    {

    }
  else if (ACE_OS_String::strcmp (qName, "suspend") == 0)
    {

    }
  else if (ACE_OS_String::strcmp (qName, "remove") == 0)
    {

    }
  else
    {

    }

  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT (" %s = \"%s\""),
                    alist->getQName (i), alist->getValue (i)));
      }
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT (">")));
}

void
ACEXML_Svcconf_Handler::startPrefixMapping (const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Svcconf_Handler::notationDecl (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Svcconf_Handler::resolveEntity (const ACEXML_Char *,
                                        const ACEXML_Char *,
                                        ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_Svcconf_Handler::error (ACEXML_SAXParseException &,
                                ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::fatalError (ACEXML_SAXParseException &,
                                     ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::warning (ACEXML_SAXParseException &,
                                  ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

int
ACEXML_Svcconf_Handler::get_stream_id (ACEXML_Attributes *alist,
                                       ACEXML_Env &xmlenv)
{
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS_String::strcmp (alist->getQName (i), "id") == 0)
          {
            this->stream_info_.stream_name (alist->getValue (i));
          }
        else
          {
            // @@ Exception...
            return -1;
          }
      }
  return 0;
}
