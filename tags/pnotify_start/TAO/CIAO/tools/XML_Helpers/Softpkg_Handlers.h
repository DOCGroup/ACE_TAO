// $Id$

//=============================================================================
/**
 *  @file Softpkg_Handlers.h
 *
 *  $Id$
 *
 *  This file turned out to host only one handler.  We'll still keep
 *  the name as Softpkg_Handlers, however.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_SOFTPKG_HANDLER_H
#define CIAO_SOFTPKG_HANDLER_H

#include "XML_Helpers_Export.h"
#include "Cascadable_DocHandler.h"

namespace CIAO
{
  /**
   * @class Svcconf_Handler
   *
   * @brief Svcconf_Handler is an example SAX event handler.
   *
   * This SAX event handler try to regenerate the XML document it
   * reads with correct indentation.
   */
  class CIAO_XML_HELPERS_Export Softpkg_Handler
    : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    /**
     * @enum
     *
     * @brief Softpkg type identifier
     */
    typedef enum
      {
        CSD,                    // CORBA's Component Softpkg Descriptor
        SSD                     // CIAO's Servent Softpkg Descriptor
      } Softpkg_Type;

    /**
     *
     */
    typedef enum
      {
        ROOT,
        DESCRIPTOR,
        IMPLEMENTATION,
        CIAODLL,
        CODE
      } Softpkg_State;

    /**
     * @struct Softpkg_Info
     *
     * @Brief Relevant information that we are interested in
     *
     * This Softpkg_Info combines information from both a component
     * softpkg descriptor and a servant softpkg descriptor.
     */
    typedef struct CIAO_XML_HELPERS_Export Struct_Softpkg_Info
    {
      /// Specify the pathname (should change to URI later) to the
      /// component descriptor file.
      ACE_TString descriptor_;

      /// Specify the pathname to the root softpkg (as defined in CCM.)
      ACE_TString csd_path_;

      /// Specify the pathname to component executor DLL.
      ACE_TString executor_UUID_;

      /// Specify the entrypoint to component executor DLL.
      ACE_TString executor_entrypt_;

      /// Specify the pathname to the servant softpkg (as defined in CIAO.)
      ACE_TString ssd_path_;

      /// Specify the pathname to component servant DLL.
      ACE_TString servant_UUID_;

      /// Specify the entrypoint to component servant DLL.
      ACE_TString servant_entrypt_;

      void dump (void) const;
    } Softpkg_Info;

  public:
    /**
     * Default constructor.
     */
    Softpkg_Handler (ACEXML_XMLReader *parser,
                     Softpkg_Info *info
                     ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Default destructor.
     */
    virtual ~Softpkg_Handler (void);

    // Methods inherit from ACEXML_ContentHandler.

    /**
     * Receive notification of character data.
     */
    virtual void characters (const ACEXML_Char *ch,
                             int start,
                             int length ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
      ;

    /**
     * Receive notification of the end of a document.
     */
    virtual void endDocument (ACEXML_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
      ;

    /**
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
      ;

    /**
     * Receive notification of the beginning of a document.
     */
    virtual void startDocument (ACEXML_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
      ;

    /**
     * Receive notification of the beginning of an element.
     */
    virtual void startElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName,
                               ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
      ;

  protected:

  private:
    /// if we need to trace the CDATA.
    bool rec_cdata_;

    /// Parse state
    Softpkg_State state_;

    /// Type of softpkg we are parsing.
    Softpkg_Type type_;

    /// Character info.
    ACE_TString characters_;

    /// Cached the Implementation UUID before it can be determined
    /// suitable for use.
    ACE_TString impluuid_;

    /// Cached the servant path before it can be determined
    /// suitable for use.
    ACE_TString ssd_;

    /// Cached the servant uuid before it can be determined
    /// suitable for use.
    ACE_TString ssd_id_;

    /// Cached the Implementation path before it can be determined
    /// suitable for use.
    ACE_TString path_;

    /// Cached the Implementation entry point before it can be
    /// determined suitable for use.
    ACE_TString entry_pt_;

    /// Parse result
    Softpkg_Info *softpkg_info_;
  };

}

#if defined (__ACE_INLINE__)
# include "Softpkg_Handlers.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_SOFTPKG_HANDLER_H */
