// $Id$

#include "ACEXML/compass/SoftPkgHandler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/compass/SoftPkgHandler.inl"
#endif /* __ACEXML_INLINE__ */

static const ACEXML_Char empty_string[] = {0};

ACEXML_SoftPkgHandler::ACEXML_SoftPkgHandler (const ACEXML_Char* filename)
  : fileName_(ACE::strnew (filename)), locator_ (0), softpkg_ (0)
{
  // no-op
}

ACEXML_SoftPkgHandler::~ACEXML_SoftPkgHandler (void)
{
  delete [] this->fileName_;
  delete this->softpkg_;
}

void
ACEXML_SoftPkgHandler::startElement (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *qName,
                                     ACEXML_Attributes *alist
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACEXML_SAXParseException* exception = 0;
  if (!this->softpkg_)
    {
      ACE_NEW_NORETURN (this->softpkg_, ACEXML_SoftPkg);
      if (!this->softpkg_)
        {
          ACE_ERROR ((LM_ERROR, "Unable to allocate memory for %s\n",
                      qName));
          return;
        }
    }
  if (ACE_OS::strcmp (qName, "softpkg") == 0
      || ACE_OS::strcmp (qName, "os") == 0
      || ACE_OS::strcmp (qName, "processor") == 0
      || ACE_OS::strcmp (qName, "compiler") == 0
      || ACE_OS::strcmp (qName, "programminglanguage") == 0
      || ACE_OS::strcmp (qName, "fileinarchive") == 0
      || ACE_OS::strcmp (qName, "localfile") == 0)
    {
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (this->softpkg_->set (qName, alist->getQName (i),
                                   alist->getValue (i)) < 0)
            {
              ACE_ERROR ((LM_ERROR, "Unable to set value for element %s\n",
                          qName));
              ACE_NEW_NORETURN (exception,
                                ACEXML_SAXParseException ("Set value"));
              ACEXML_ENV_RAISE (exception);
            }
        }
    }
  else if (ACE_OS::strcmp (qName, "dependency") == 0 ||
           ACE_OS::strcmp (qName, "implementation") == 0 ||
           ACE_OS::strcmp (qName, "code") == 0)
    {
      if (this->softpkg_->start_element (qName, alist->getValue (0U)) < 0)
        {
          ACE_ERROR ((LM_ERROR, "Invalid element %s encountered\n",
                      qName));
          ACE_NEW_NORETURN (exception,
                            ACEXML_SAXParseException ("Invalid Element"));
          ACEXML_ENV_RAISE (exception);
        }
    }
  else if (ACE_OS::strcmp (qName, "pkgtype") == 0
           || ACE_OS::strcmp (qName, "title") == 0
           || ACE_OS::strcmp (qName, "description") == 0
           || ACE_OS::strcmp (qName, "propertyfile") == 0
           || ACE_OS::strcmp (qName, "entrypoint") == 0)
    return;
  else
    {
      ACE_ERROR ((LM_ERROR, "Unknown element %s encountered\n", qName));
      ACEXML_SAXParseException* exception = 0;
      ACE_NEW_NORETURN (exception,
                        ACEXML_SAXParseException ("Unknown Element"));
      ACEXML_ENV_RAISE (exception);
    }
  return;
}

void
ACEXML_SoftPkgHandler::endElement (const ACEXML_Char *,
                                   const ACEXML_Char *,
                                   const ACEXML_Char *qName
                                   ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS::strcmp (qName, "pkgtype") == 0
      || ACE_OS::strcmp (qName, "title") == 0
      || ACE_OS::strcmp (qName, "description") == 0
      || ACE_OS::strcmp (qName, "propertyfile") == 0
      || ACE_OS::strcmp (qName, "entrypoint") == 0)
    {
      if (this->cdata_.length())
        {
          ACEXML_String temp;
          if (this->softpkg_->set (qName, temp, this->cdata_) < 0)
            {
              ACE_ERROR ((LM_ERROR, "Invalid content in %s\n", qName));
              ACEXML_SAXParseException* exception = 0;
              ACE_NEW_NORETURN (exception,
                                ACEXML_SAXParseException ("Invalid Content"));
              ACEXML_ENV_RAISE (exception);
            }
          this->cdata_.clear();
        }
//       else
//         {
//           ACE_ERROR ((LM_ERROR, "Empty element %s encountered\n", qName));
//           ACEXML_SAXParseException* exception = 0;
//           ACE_NEW_NORETURN (exception,
//                             ACEXML_SAXParseException ("Empty Element"));
//           ACEXML_ENV_RAISE (exception);
//         }
    }
  else if (ACE_OS::strcmp (qName, "dependency") == 0 ||
           ACE_OS::strcmp (qName, "implementation") == 0 ||
           ACE_OS::strcmp (qName, "code") == 0)
    {
      this->softpkg_->end_element (qName);
    }
  else if (ACE_OS::strcmp (qName, "softpkg") == 0
           || ACE_OS::strcmp (qName, "os") == 0
           || ACE_OS::strcmp (qName, "processor") == 0
           || ACE_OS::strcmp (qName, "compiler") == 0
           || ACE_OS::strcmp (qName, "programminglanguage") == 0
           || ACE_OS::strcmp (qName, "fileinarchive") == 0
           || ACE_OS::strcmp (qName, "localfile") == 0)

    return;
  else
    {
      ACE_ERROR ((LM_ERROR, "Unknown element %s encountered\n", qName));
      ACEXML_SAXParseException* exception = 0;
      ACE_NEW_NORETURN (exception,
                        ACEXML_SAXParseException ("Unknown Element"));
      ACEXML_ENV_RAISE (exception);
    }
  return;
}

void
ACEXML_SoftPkgHandler::characters (const ACEXML_Char *cdata,
                                   int, int
                                   ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->cdata_.length())
    this->cdata_ += cdata;
  else
    this->cdata_ = cdata;
}


// Methods inherited from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
ACEXML_SoftPkgHandler::error (ACEXML_SAXParseException & ex
                              ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0
               ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_SoftPkgHandler::fatalError (ACEXML_SAXParseException & ex
                                   ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0
               ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_SoftPkgHandler::warning (ACEXML_SAXParseException & ex
                                ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0
               ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}
