// $Id$

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
