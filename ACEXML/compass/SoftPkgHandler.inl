// $Id$

const ACEXML_SoftPkg*
ACEXML_SoftPkgHandler::get_softpkg (void) const
{
  return this->softpkg_;
}

void
ACEXML_SoftPkgHandler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_SoftPkgHandler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

void
ACEXML_SoftPkgHandler::endPrefixMapping (const ACEXML_Char *
                                         ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_SoftPkgHandler::ignorableWhitespace (const ACEXML_Char *cdata,
                                            int,
                                            int
                                            ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->cdata_.length())
    this->cdata_ += cdata;
}

void
ACEXML_SoftPkgHandler::processingInstruction (const ACEXML_Char *,
                                              const ACEXML_Char *
                                              ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

void
ACEXML_SoftPkgHandler::setDocumentLocator (ACEXML_Locator * locator)
{
  this->locator_ = locator;
}

void
ACEXML_SoftPkgHandler::skippedEntity (const ACEXML_Char *
                                      ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
ACEXML_SoftPkgHandler::startPrefixMapping (const ACEXML_Char *,
                                           const ACEXML_Char *
                                           ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{

}

// *** Methods inherited from ACEXML_DTDHandler.

void
ACEXML_SoftPkgHandler::notationDecl (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_SoftPkgHandler::unparsedEntityDecl (const ACEXML_Char *,
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
ACEXML_SoftPkgHandler::resolveEntity (const ACEXML_Char *,
                                      const ACEXML_Char *
                                      ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}
