// $Id$

ACE_INLINE const ACEXML_Char *
CIAO::XMLHelpers::Cascadable_DocHandler::namespaceURI () const
{
  return this->namespaceURI_;
}

ACE_INLINE const ACEXML_Char *
CIAO::XMLHelpers::Cascadable_DocHandler::localName () const
{
  return this->localName_;
}

ACE_INLINE const ACEXML_Char *
CIAO::XMLHelpers::Cascadable_DocHandler::qName () const
{
  return this->qName_;
}

ACE_INLINE void
CIAO::XMLHelpers::Cascadable_DocHandler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
}

ACE_INLINE void
CIAO::XMLHelpers::Cascadable_DocHandler::error (ACEXML_SAXParseException & ex
                                                ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->print_warning (ACE_TEXT ("Error"),
                       ex
                       ACEXML_ENV_ARG_PARAMETER);
}

ACE_INLINE void
CIAO::XMLHelpers::Cascadable_DocHandler::fatalError (ACEXML_SAXParseException & ex
                                                     ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->print_warning (ACE_TEXT ("Fatal Error"),
                       ex
                       ACEXML_ENV_ARG_PARAMETER);
}

ACE_INLINE void
CIAO::XMLHelpers::Cascadable_DocHandler::warning (ACEXML_SAXParseException & ex
                                                  ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  this->print_warning (ACE_TEXT ("Warning"),
                       ex
                       ACEXML_ENV_ARG_PARAMETER);
}
