// -*- C++ -*-  $Id$

ACE_INLINE
CIAO::Softpkg_Handler::Softpkg_Handler (ACEXML_XMLReader *parser,
                                        CIAO::Softpkg_Handler::Softpkg_Info *info
                                        ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (parser,
                             0,
                             0,
                             0,
                             0,
                             0
                             ACEXML_ENV_ARG_PARAMETER),
      rec_cdata_ (false),
      state_ (CIAO::Softpkg_Handler::ROOT),
      softpkg_info_ (info)
{
  // no-op
}

ACE_INLINE
CIAO::Softpkg_Handler::~Softpkg_Handler (void)
{
  // no-op
}
