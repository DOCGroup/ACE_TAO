// $Id$

ACE_INLINE
CIAO::Assembly_Handler::Assembly_Handler (ACEXML_XMLReader *reader,
                                          Assembly_Spec *spec
                                          ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (reader,
                             0,
                             0,
                             0,
                             0,
                             0
                             ACEXML_ENV_ARG_PARAMETER),
      context_ (spec),
      state_ (START)
{
}

ACE_INLINE
CIAO::Assembly_Handler::~Assembly_Handler (void)
{
}

// =================================================================

ACE_INLINE
CIAO::Comp_File_Handler::Comp_File_Handler (Assembly_Spec *spec,
                                            ACEXML_XMLReader *parser,
                                            CIAO::XMLHelpers::Cascadable_DocHandler *p,
                                            const ACEXML_Char *namespaceURI,
                                            const ACEXML_Char *localName,
                                            const ACEXML_Char *qName,
                                            ACEXML_Attributes *atts
                                            ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (parser,
                             p,
                             namespaceURI,
                             localName,
                             qName,
                             atts
                             ACEXML_ENV_ARG_PARAMETER),
      element_count_ (0),
      context_ (spec),
      state_ (START)
{
}

ACE_INLINE
CIAO::Comp_File_Handler::~Comp_File_Handler (void)
{
}

// =================================================================

ACE_INLINE
CIAO::Partitioning_Handler::Partitioning_Handler (Assembly_Spec *spec,
                                                  ACEXML_XMLReader *parser,
                                                  CIAO::XMLHelpers::Cascadable_DocHandler *p,
                                                  const ACEXML_Char *namespaceURI,
                                                  const ACEXML_Char *localName,
                                                  const ACEXML_Char *qName,
                                                  ACEXML_Attributes *atts
                                                  ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (parser,
                             p,
                             namespaceURI,
                             localName,
                             qName,
                             atts
                             ACEXML_ENV_ARG_PARAMETER),
      element_count_ (0),
      context_ (spec),
      host_collocation_ (0),
      process_collocation_ (0),
      home_placement_ (0),
      comp_instance_ (0),
      state_ (START)
{
}

ACE_INLINE
CIAO::Partitioning_Handler::~Partitioning_Handler (void)
{
}

// =================================================================

ACE_INLINE
CIAO::Connections_Handler::Connections_Handler (Assembly_Spec *spec,
                                                ACEXML_XMLReader *parser,
                                                CIAO::XMLHelpers::Cascadable_DocHandler *p,
                                                const ACEXML_Char *namespaceURI,
                                                const ACEXML_Char *localName,
                                                const ACEXML_Char *qName,
                                                ACEXML_Attributes *atts
                                                ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
    : Cascadable_DocHandler (parser,
                             p,
                             namespaceURI,
                             localName,
                             qName,
                             atts
                             ACEXML_ENV_ARG_PARAMETER),
      element_count_ (0),
      context_ (spec),
      state_ (START)
{
}

ACE_INLINE
CIAO::Connections_Handler::~Connections_Handler (void)
{
}
