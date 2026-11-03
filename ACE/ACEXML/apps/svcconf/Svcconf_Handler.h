//=============================================================================
/**
 *  @file    Svcconf_Handler.h
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
  ACE_Parsed_Info (ACE_Parsed_Info const &) = delete;
  ACE_Parsed_Info (ACE_Parsed_Info &&) = delete;
  ACE_Parsed_Info &operator= (ACE_Parsed_Info const &) = delete;
  ACE_Parsed_Info &operator= (ACE_Parsed_Info &&) = delete;

  /**
   * Set/get name of a parsed entity.
   */
  int name (ACEXML_Char const *n);
  ACEXML_Char const *name () const;

  /**
   * Set/get type of a dynamic node.
   */
  int service_type (int type);
  int service_type () const;

  /**
   * Set/Get active status.
   */
  int active (bool active);
  bool active () const;

  /**
   * Set/get initializer path.
   */
  int path (ACEXML_Char const *n);
  ACEXML_Char const *path () const;

  /**
   * Set/get initializer init function.
   */
  int init_func (ACEXML_Char const *n);
  ACEXML_Char const *init_func () const;

  /**
   * Set/get initializer init parameter.
   */
  int init_params (ACEXML_Char const *n);
  ACEXML_Char const *init_params () const;

  /**
   * Reset Parsed_Info.
   */
  void reset ();

protected:
  // Owning buffers allocated by ACE::strnew; freed by the destructor and reset().
  ACEXML_Char *name_;
  int service_type_;
  bool active_;
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
  ACEXML_Svcconf_Handler ();

  /*
   * Default destructor.
   */
  ~ACEXML_Svcconf_Handler () override = default;
  ACEXML_Svcconf_Handler (ACEXML_Svcconf_Handler const &) = delete;
  ACEXML_Svcconf_Handler (ACEXML_Svcconf_Handler &&) = delete;
  ACEXML_Svcconf_Handler &operator= (ACEXML_Svcconf_Handler const &) = delete;
  ACEXML_Svcconf_Handler &operator= (ACEXML_Svcconf_Handler &&) = delete;

  // Methods inherit from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  void characters (ACEXML_Char const *ch,
                   int start,
                   int length) override;

  /*
   * Receive notification of the end of a document.
   */
  void endDocument () override;

  /*
   * Receive notification of the end of an element.
   */
  void endElement (ACEXML_Char const *namespaceURI,
                   ACEXML_Char const *localName,
                   ACEXML_Char const *qName) override;

  /*
   * End the scope of a prefix-URI mapping.
   */
  void endPrefixMapping (ACEXML_Char const *prefix) override;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  void ignorableWhitespace (ACEXML_Char const *ch,
                            int start,
                            int length) override;

  /*
   * Receive notification of a processing instruction.
   */
  void processingInstruction (ACEXML_Char const *target,
                              ACEXML_Char const *data) override;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  void setDocumentLocator (ACEXML_Locator *locator) override;

  /*
   * Receive notification of a skipped entity.
   */
  void skippedEntity (ACEXML_Char const *name) override;

  /*
   * Receive notification of the beginning of a document.
   */
  void startDocument () override;

  /*
   * Receive notification of the beginning of an element.
   */
  void startElement (ACEXML_Char const *namespaceURI,
                     ACEXML_Char const *localName,
                     ACEXML_Char const *qName,
                     ACEXML_Attributes *atts) override;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  void startPrefixMapping (ACEXML_Char const *prefix,
                           ACEXML_Char const *uri) override;

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  void notationDecl (ACEXML_Char const *name,
                     ACEXML_Char const *publicId,
                     ACEXML_Char const *systemId) override;

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  void unparsedEntityDecl (ACEXML_Char const *name,
                           ACEXML_Char const *publicId,
                           ACEXML_Char const *systemId,
                           ACEXML_Char const *notationName) override;

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  ACEXML_InputSource *resolveEntity (ACEXML_Char const *publicId,
                                     ACEXML_Char const *systemId) override;

  // Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  void error (ACEXML_SAXParseException &exception) override;

  /*
   * Receive notification of a non-recoverable error.
   */
  void fatalError (ACEXML_SAXParseException &exception) override;

  /*
   * Receive notification of a warning.
   */
  void warning (ACEXML_SAXParseException &exception) override;

protected:
  /**
   * Get the only attribute in <stream> or <streamdef>.
   */
  int get_stream_id (ACEXML_Attributes *alist);

  /**
   * Get the only attribute in <resume>, <suspend>, <remove>
   */
  int get_id (ACEXML_Attributes *alist);

  /**
   * Get the dynamic tag attributes.
   */
  int get_dynamic_attrs (ACEXML_Attributes *alist);

  /**
   * Get the initializer tag attributes.
   */
  int get_initializer_attrs (ACEXML_Attributes *alist);

  /**
   * Get the static tag attributes.
   */
  int get_static_attrs (ACEXML_Attributes *alist);

private:
  /// We are parsing a stream definition
  bool in_stream_def_;

  /// We are defining a steam module
  bool in_module_;

  /// Non-owning; the service repository owns this record and manages stream_'s lifetime.
  ACE_Service_Type *stream_svc_type_;
  ACE_Stream_Type *stream_;

  //ACE_SHLIB_HANDLE dll_handle_;

  ACE_Parsed_Info parsed_info_;

  ACE_Parsed_Info stream_info_;

  ACEXML_Locator* locator_; // Non-owning; the parser owns the locator.
};

#if defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.inl"
#endif /* __ACEXML_INLINE__ */

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */

#endif /* ACEXML_SVCCONF_HANDLER_H */
