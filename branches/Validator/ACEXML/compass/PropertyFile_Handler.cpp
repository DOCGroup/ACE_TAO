// $Id$

#include "PropertyFile_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "PropertyFile_Handler.inl"
#endif /* __ACEXML_INLINE__ */

static const ACEXML_Char empty_string[] = {0};

ACEXML_PropertyFile_Handler::ACEXML_PropertyFile_Handler (const ACEXML_Char* filename)
  : fileName_(ACE::strnew (filename)), locator_ (0), property_ (0)
{
  // no-op
}

ACEXML_PropertyFile_Handler::~ACEXML_PropertyFile_Handler (void)
{
  delete [] this->fileName_;
  delete this->property_;
}

const ACEXML_Property*
ACEXML_PropertyFile_Handler::get_property (void) const
{
  return this->property_;
}

void
ACEXML_PropertyFile_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_PropertyFile_Handler::startElement (const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *qName,
                                           ACEXML_Attributes *alist
                                           ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->name_ = qName;
  if (ACE_OS::strcmp (qName, "properties") == 0 ||
      ACE_OS::strcmp (qName, "description") == 0)
    return;
  else if (ACE_OS::strcmp (qName, "struct") == 0 ||
           ACE_OS::strcmp (qName, "simple") == 0)
    {
      this->attname_.clear();
      this->atttype_.clear();
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS::strcmp (alist->getQName (i), "name") == 0)
            this->attname_ = alist->getValue (i);
          else if (ACE_OS::strcmp (alist->getQName (i), "type") == 0)
            this->atttype_ = alist->getValue (i);
        }
      if (!this->property_ && this->atttype_ == "ACEXML_EC_Property")
        ACE_NEW (this->property_, ACEXML_EC_Property);
    }
  else if (ACE_OS::strcmp (qName, "value") == 0)
    this->cdata_.clear();
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
ACEXML_PropertyFile_Handler::endElement (const ACEXML_Char *,
                                         const ACEXML_Char *,
                                         const ACEXML_Char *qName
                                         ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->name_ != qName)
    {
      ACE_ERROR ((LM_ERROR, "Mismatched element %s encountered\n", qName));
      ACEXML_SAXParseException* exception = 0;
      ACE_NEW_NORETURN (exception,
                        ACEXML_SAXParseException ("Mismatched Element"));
      ACEXML_ENV_RAISE (exception);
    }
  if (this->name_ == "description")
    this->cdata_.clear();
  else if (this->name_ == "value")
    this->value_ = this->cdata_;
  else if (this->name_ == "simple")
    {
      if (this->atttype_ == "string")
        {
          if (this->property_->set (this->attname_, this->value_) != 0)
            {
              ACE_ERROR ((LM_ERROR, "Property %s = %s invalid\n",
                          this->attname_.c_str(), this->value.c_str()));
              ACEXML_SAXParseException* exception = 0;
              ACE_NEW_NORETURN (exception,
                                ACEXML_SAXParseException ("Invalid Property"));
              ACEXML_ENV_RAISE (exception);
            }
        }
      else if (this->atttype_ == "long")
        {
          long value = ACE_OS::strtol (this->value_.c_str(), 10);
          if (this->property_->set (this->attname_, value) != 0)
             ACE_ERROR ((LM_ERROR, "Property %s = %ld invalid\n",
                         this->attname_.c_str(), value));
          ACEXML_SAXParseException* exception = 0;
          ACE_NEW_NORETURN (exception,
                            ACEXML_SAXParseException ("Invalid Property"));
          ACEXML_ENV_RAISE (exception);
        }
      this->value_.clear();
      this->cdata_.clear();
    }
  return;
}

void
ACEXML_PropertyFile_Handler::characters (const ACEXML_Char *cdata,
                                         int, int
                                         ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->cdata_.length())
    this->cdata_ += cdata;
  this->cdata_ = cdata;
}

void
ACEXML_PropertyFile_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

void
ACEXML_PropertyFile_Handler::endPrefixMapping (const ACEXML_Char *
                                               ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_PropertyFile_Handler::ignorableWhitespace (const ACEXML_Char *,
                                                  int,
                                                  int
                                                  ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

void
ACEXML_PropertyFile_Handler::processingInstruction (const ACEXML_Char *,
                                                    const ACEXML_Char *
                                                    ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

void
ACEXML_PropertyFile_Handler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
}

void
ACEXML_PropertyFile_Handler::skippedEntity (const ACEXML_Char *
                                            ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_PropertyFile_Handler::startPrefixMapping (const ACEXML_Char *,
                                                 const ACEXML_Char *
                                                 ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

// *** Methods inherited from ACEXML_DTDHandler.

void
ACEXML_PropertyFile_Handler::notationDecl (const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *
                                           ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_PropertyFile_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                                 const ACEXML_Char *,
                                                 const ACEXML_Char *,
                                                 const ACEXML_Char *
                                                 ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// Methods inherited from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_PropertyFile_Handler::resolveEntity (const ACEXML_Char *,
                                            const ACEXML_Char *
                                            ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherited from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
ACEXML_PropertyFile_Handler::error (ACEXML_SAXParseException & ex
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
ACEXML_PropertyFile_Handler::fatalError (ACEXML_SAXParseException & ex
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
ACEXML_PropertyFile_Handler::warning (ACEXML_SAXParseException & ex
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
