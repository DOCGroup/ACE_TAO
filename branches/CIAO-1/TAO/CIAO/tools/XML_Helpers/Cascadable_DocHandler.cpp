// $Id$

#include "Cascadable_DocHandler.h"

#if !defined (__ACE_INLINE__)
# include "Cascadable_DocHandler.i"
#endif /* __ACE_INLINE__ */

CIAO::XMLHelpers::Cascadable_DocHandler::Cascadable_DocHandler (ACEXML_XMLReader *parser,
                                                                Cascadable_DocHandler *parent,
                                                                const ACEXML_Char *namespaceURI,
                                                                const ACEXML_Char *localName,
                                                                const ACEXML_Char *qName,
                                                                ACEXML_Attributes *
                                                                ACEXML_ENV_ARG_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : namespaceURI_ (ACE::strnew (namespaceURI)),
      localName_ (ACE::strnew (localName)),
      qName_ (ACE::strnew (qName)),
      parser_ (parser),
      parent_ (parent),
      child_ (0)
{
}

CIAO::XMLHelpers::Cascadable_DocHandler::~Cascadable_DocHandler ()
{
  delete[] this->namespaceURI_;
  delete[] this->localName_;
  delete[] this->qName_;
  delete[] this->child_;
}

void
CIAO::XMLHelpers::Cascadable_DocHandler::destroy ()
{
  delete this;
}


void
CIAO::XMLHelpers::Cascadable_DocHandler::push_handler (Cascadable_DocHandler *new_handler,
                                                       ACEXML_Attributes *atts
                                                       ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // This method should be invoked from this->startElement ().

  this->child_ = new_handler;
  this->parser_->setContentHandler (new_handler);
  this->parser_->setDTDHandler (new_handler);
  this->parser_->setEntityResolver (new_handler);
  this->parser_->setErrorHandler (new_handler);

  new_handler->startElement (new_handler->namespaceURI (),
                             new_handler->localName (),
                             new_handler->qName (),
                             atts
                             ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK;
}

void
CIAO::XMLHelpers::Cascadable_DocHandler::pop_handler (const ACEXML_Char *namespaceURI,
                                                      const ACEXML_Char *localName,
                                                      const ACEXML_Char *qName
                                                      ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // This method should be invoked from this->child_->endElement ().

  this->parser_->setContentHandler (this);
  this->parser_->setDTDHandler (this);
  this->parser_->setEntityResolver (this);
  this->parser_->setErrorHandler (this);

  // endElement should harvest the parse result stored in the
  // this->child_ somehow because it's the last chance to do so.
  this->endElement (namespaceURI,
                    localName,
                    qName
                    ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK;

  this->child_->destroy ();
  this->child_ = 0;
}

CIAO::XMLHelpers::Skip_DocHandler::Skip_DocHandler (ACEXML_XMLReader *parser,
                                                    Cascadable_DocHandler *parent,
                                                    const ACEXML_Char *namespaceURI,
                                                    const ACEXML_Char *localName,
                                                    const ACEXML_Char *qName,
                                                    ACEXML_Attributes *attrs
                                                    ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
  // The exception stuff will not work for platform without native exception.
    : element_count_ (0),
      Cascadable_DocHandler (parser,
                             parent,
                             namespaceURI,
                             localName,
                             qName,
                             attrs
                             ACEXML_ENV_ARG_PARAMETER)
{
}

CIAO::XMLHelpers::Skip_DocHandler::~Skip_DocHandler ()
{
}

void
CIAO::XMLHelpers::Skip_DocHandler::endElement (const ACEXML_Char *namespaceURI,
                                               const ACEXML_Char *localName,
                                               const ACEXML_Char *qName
                                               ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  --this->element_count_;
  if (this->element_count_ == 0)
    {
      this->parent_->pop_handler (namespaceURI,
                                  localName,
                                  qName
                                  ACEXML_ENV_ARG_PARAMETER);
      ACEXML_CHECK;
    }
  else
    {
      cout << "End Skip level " << this->element_count_ << endl;
    }
}

void
CIAO::XMLHelpers::Skip_DocHandler::startElement (const ACEXML_Char *namespaceURI,
                                                 const ACEXML_Char *localName,
                                                 const ACEXML_Char *qName,
                                                 ACEXML_Attributes *atts
                                                 ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ++this->element_count_;
  cout << "Start Skip level " << this->element_count_ << endl;
}
