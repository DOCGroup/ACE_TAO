// $Id$

ACE_INLINE
CIAO::RTConfig_Handler::RTConfig_Handler (ACEXML_XMLReader *reader,
                                          RTConfiguration::RTORB_Resource_Info &rtr,
                                          RTConfiguration::Policy_Sets &ps
                                          ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (reader,
                             0,
                             0,
                             0,
                             0,
                             0
                             ACEXML_ENV_ARG_PARAMETER),
      state_ (START),
      rtresources (rtr),
      policysets (ps)
{
}

ACE_INLINE
CIAO::RTConfig_Handler::~RTConfig_Handler (void)
{
}

// =================================================================
