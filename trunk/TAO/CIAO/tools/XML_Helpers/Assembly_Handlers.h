// $Id$

//=============================================================================
/**
 *  @file Assembly_Handlers.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_ASSEMBLY_HANDLERS_H
#define CIAO_ASSEMBLY_HANDLERS_H

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
  class Assembly_Handler : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    enum CS_States
      {
        START,
        COMPONENTFILES_DONE,
        PLACEMENT_DONE,
        CONNECTION_DONE,
        DONE
      };

    /**
     * Default constructor.
     */
    Assembly_Handler (ACEXML_XMLReader *parser,
                      Assembly_Spec *spec
                      ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Default destructor.
     */
    virtual ~Assembly_Handler (void);

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
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
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
    Assembly_Spec *context_;

    CS_States state_;

  private:
  };

  /**
   * @class Comp_File_Handler
   *
   * @brief This DocHandler handles the "componentfiles" tag
   */
  class Comp_File_Handler : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    // @@ CF_States is not used for now.
    enum CF_States
      {
        START
      };

    /// Constructor.
    Comp_File_Handler (Assembly_Spec *spec,
                       ACEXML_XMLReader *parser,
                       CIAO::XMLHelpers::Cascadable_DocHandler *parent,
                       const ACEXML_Char *namespaceURI,
                       const ACEXML_Char *localName,
                       const ACEXML_Char *qName,
                       ACEXML_Attributes *atts
                       ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// Destructor
    virtual ~Comp_File_Handler ();

    /**
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the beginning of an element.
     */
    virtual void startElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName,
                               ACEXML_Attributes *atts
                               ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  protected:
    long element_count_;

    Assembly_Spec *context_;

    ACE_CString id_;
    ACE_CString fileinarchive_;

    CF_States state_;
  };

  /**
   * @class Partitioning_Handler
   *
   * @brief This DocHandler handles the "partitioning" tag
   */
  class Partitioning_Handler : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    // @@ Do we need PH_States?
    enum PH_States
      {
        START,
        HOMEPLACEMENT,
        COMPONENT_INSTANTIATION
      };

    enum CHAR_States
      {
        IDLE,
        DESTINATION
      };

    /// Constructor.
    Partitioning_Handler (Assembly_Spec *spec,
                          ACEXML_XMLReader *parser,
                          CIAO::XMLHelpers::Cascadable_DocHandler *parent,
                          const ACEXML_Char *namespaceURI,
                          const ACEXML_Char *localName,
                          const ACEXML_Char *qName,
                          ACEXML_Attributes *atts
                          ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// Destructor
    virtual ~Partitioning_Handler ();

    /*
     * Receive notification of character data.
     */
    virtual void characters (const ACEXML_Char *ch,
                             int start,
                             int length
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the beginning of an element.
     */
    virtual void startElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName,
                               ACEXML_Attributes *atts
                               ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  protected:
    long element_count_;

    Assembly_Spec *context_;

    /// Temporary holder when building componentinstantiation spec.
    Assembly_Placement::hostcollocation *host_collocation_;

    /// Temporary holder when building componentinstantiation spec.
    Assembly_Placement::processcollocation *process_collocation_;

    /// Temporary holder when building homeplacement spec.
    Assembly_Placement::homeplacement *home_placement_;

    /// Temporary holder when building componentinstantiation spec.
    Assembly_Placement::componentinstantiation *comp_instance_;

    // State of this partitioning handler.
    PH_States state_;

    ACE_CString characters_;

    /// Temporary holder when building registration information
    Assembly_Placement::componentinstantiation::Register_Info comp_register_info_;
 };

  /**
   * @class Connections_Handler
   *
   * @brief This DocHandler handles the "connections" tag
   */
  class Connections_Handler : public CIAO::XMLHelpers::Cascadable_DocHandler
  {
  public:
    enum CH_States
      {
        START,                  // parsing destination info
        SOURCE                  // parsing source I/F resolution method
      };

    /// Constructor.
    Connections_Handler (Assembly_Spec *spec,
                         ACEXML_XMLReader *parser,
                         CIAO::XMLHelpers::Cascadable_DocHandler *parent,
                         const ACEXML_Char *namespaceURI,
                         const ACEXML_Char *localName,
                         const ACEXML_Char *qName,
                         ACEXML_Attributes *atts
                         ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException));

    /// Destructor
    virtual ~Connections_Handler ();

    /*
     * Receive notification of character data.
     */
    virtual void characters (const ACEXML_Char *ch,
                             int start,
                             int length
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the end of an element.
     */
    virtual void endElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName
                             ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    /**
     * Receive notification of the beginning of an element.
     */
    virtual void startElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName,
                               ACEXML_Attributes *atts
                               ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  protected:
    // Create a new blank Connect_Info.
    void reset_info (ACEXML_Attributes *atts
                     ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    long element_count_;

    Assembly_Spec *context_;

    // Connection Info currently being built.
    Assembly_Connection::Connect_Info info_;

    CH_States state_;

    // Resolver info cache.
    ACE_CString resolver_info_;

    // Resolver cache.
    Assembly_Connection::IF_Resolver_Info *resolver_;

    ACE_CString characters_;
  };
}

#if defined (__ACE_INLINE__)
# include "Assembly_Handlers.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_ASSEMBLY_HANDLERS_H */
