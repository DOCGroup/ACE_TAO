// $Id$

//=============================================================================
/**
 *  @file    Svcconf_Handler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_SVCCONF_HANDLER_H
#define ACEXML_SVCCONF_HANDLER_H

#include "ACEXML/common/DefaultHandler.h"
#include "ace/Service_Types.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

class ACE_Parsed_Info
{
public:
  ACE_Parsed_Info ();
  ~ACE_Parsed_Info ();

  /**
   * Set/get name of a parsed entity.
   */
  int name (const ACEXML_Char *n);
  const ACEXML_Char *name (void);

  /**
   * Set/get type of a dynamic node.
   */
  int service_type (int type);
  int service_type (void);

  /**
   * Set/Get active status.
   */
  int active (int a);
  int active (void);

  /**
   * Set/get initializer path.
   */
  int path (const ACEXML_Char *n);
  const ACEXML_Char *path (void);

  /**
   * Set/get initializer init function.
   */
  int init_func (const ACEXML_Char *n);
  const ACEXML_Char *init_func (void);

  /**
   * Set/get initializer init parameter.
   */
  int init_params (const ACEXML_Char *n);
  const ACEXML_Char *init_params (void);

  /**
   * Reset Parsed_Info.
   */
  void reset (void);

protected:
  ACEXML_Char *name_;
  int service_type_;
  int active_;
  ACEXML_Char *path_;
  ACEXML_Char *init_func_;
  ACEXML_Char *init_params_;
};

/**
 * @class ACEXML_Svcconf_Handler
 *
 * @brief ACEXML_Svcconf_Handler is an example SAX event handler.
 *
 * This SAX event handler try to regenerate the XML document it
 * reads with correct indentation.
 */
class ACEXML_Svcconf_Handler : public ACEXML_DefaultHandler
{
public:
  /*
   * Default constructor.
   */
  ACEXML_Svcconf_Handler (void);

  /*
   * Default destructor.
   */
  virtual ~ACEXML_Svcconf_Handler (void);

  // Methods inherit from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument (ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) ;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  virtual void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  // Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

protected:
  /**
   * Get the only attribute in <stream> or <streamdef>.
   */
  int get_stream_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL);

  /**
   * Get the only attribute in <resume>, <suspend>, <remove>
   */
  int get_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL);

  /**
   * Get the dynamic tag attributes.
   */
  int get_dynamic_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL);

  /**
   * Get the initializer tag attributes.
   */
  int get_initializer_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL);

  /**
   * Get the static tag attributes.
   */
  int get_static_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL);

private:
  /// We are parsing a stream definition
  int in_stream_def_;

  /// We are defining a steam module
  int in_module_;

  /// @a stream_ holds the actually Stream_Type object managed by @a stream_svc_type_.
  ACE_Service_Type *stream_svc_type_;
  ACE_Stream_Type *stream_;

  //ACE_SHLIB_HANDLE dll_handle_;

  ACE_Parsed_Info parsed_info_;

  ACE_Parsed_Info stream_info_;

  ACEXML_Locator* locator_;
};

#if defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.inl"
#endif /* __ACEXML_INLINE__ */

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */

#endif /* ACEXML_SVCCONF_HANDLER_H */
