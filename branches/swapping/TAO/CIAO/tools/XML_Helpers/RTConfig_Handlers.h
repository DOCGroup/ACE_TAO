// $Id$

//=============================================================================
/**
 *  @file RTConfig_Handlers.h
 *
 *  This file contains XML SAX content handler(s) for parsing CIAO's
 *  RT cad file extension descriptor.
 *
 *  These handlers assume the underlying XML parser is a *validating* parser.
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_RTCONFIG_HANDLERS_H
#define CIAO_RTCONFIG_HANDLERS_H

#include "Cascadable_DocHandler.h"
#include "XML_Utils.h"

namespace CIAO
{

  /**
   * @class Assembly_Handler
   *
   * @brief Assembly_Handler parse the root componentassembly descriptors
   *
   */
  class RTConfig_Handler : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    enum CS_States
      {
        START,                  // initial state
        RTRESOURCES,            // parsing an rtresources element
        TPWLANES,               // parsing a threadpoolwithlanes element
        CONNBANDS,              // parsing a connectionbands element
        RTPOLICYSET             // parsing rtpolicyset elements
      };

    /**
     * Default constructor.
     */
    RTConfig_Handler (ACEXML_XMLReader *parser,
                      RTConfiguration::RTORB_Resource_Info &rtr,
                      RTConfiguration::Policy_Sets &ps
                      ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Default destructor.
     */
    virtual ~RTConfig_Handler (void);

    // Methods inherit from ACEXML_ContentHandler.

    /**
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the beginning of an element.
     */
    virtual void startElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName,
                               ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  protected:
    /// Internal state of the handler.
    CS_States state_;

    /// parse the threadpool attributes
    void parse_threadpool_attrs (ACEXML_Attributes *atts
                                 ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// parse the threadpoolwithlanes attributes
    void parse_threadpoollanes_attrs (ACEXML_Attributes *atts
                                      ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// parse and add the threadpoollane attributes
    void add_threadpoollane (ACEXML_Attributes *atts
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// parse and add the priorityband attributes
    void add_priorityband (ACEXML_Attributes *atts
                           ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// parse and create a new policyset from attributes
    void create_new_policyset (ACEXML_Attributes *atts
                               ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// parse and set up the priority model configuration information
    void parse_priority_model_config (ACEXML_Attributes *atts,
                                      RTConfiguration::Priority_Model_Config &config
                                      ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

  private:
    // Current context when filling RTConfiguration::ThreadPoolLanes_Configuration
    RTConfiguration::ThreadPoolLanes_Configuration_var tpl_config_;

    // Current context when filling RTConfiguration::PriorityBands_Configuration
    RTConfiguration::PriorityBands_Configuration_var bands_config_;

    // Current context when filling RTConfiguration::Policy_Set
    RTConfiguration::Policy_Set_var policy_set_;

    // RTORB related info.
    RTConfiguration::RTORB_Resource_Info &rtresources;

    // Sequence of named policy sets.
    RTConfiguration::Policy_Sets &policysets;
  };

}

#if defined (__ACE_INLINE__)
# include "RTConfig_Handlers.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_RTCONFIG_HANDLERS_H */
