// -*- C++ -*-
//=============================================================================
/**
 *  @file   XML_ContentHandler.h
 *
 *  $Id$
 *
 *  @brief  This is a content handler which helps update the existing
 *  xml file used for store server configuration.
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef XML_CONTENTHANDLER_H
#define XML_CONTENTHANDLER_H

#include "ACEXML/common/DefaultHandler.h"

/**
 * @class XML_ContentHandler
 *
 * @brief Implementation Repository
 *
 * This provides the interface to update the Server Configuration
 * which is in an xml file.
 *
 */
class XML_ContentHandler : public ACEXML_DefaultHandler
{
public:
  // = Constructor and destructor
  XML_ContentHandler (const char *server_name);

  virtual ~XML_ContentHandler (void);

  // Methods inherit from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument ( ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) ;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument ( ACEXML_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  virtual void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  int get_startup_information (ACE_CString &logical_server_name,
                               ACE_CString &startup_command,
                               // ImplementationRepository::EnvironmentList
                               //&environment_vars,
                               ACE_CString &working_dir,
                               //ImplementationRepository::ActivationMode
                               ACE_CString &activation);

  int get_running_information (ACE_CString POA_name,
                               ACE_CString &location,
                               ACE_CString &server_object_ior);

  void update_running_information (ACE_CString POA_name,
                                   ACE_CString location,
                                   ACE_CString server_object_ior);

  void set_startup_value (ACE_CString POA_name,
                          int new_value);

  void get_startup_value (ACE_CString POA_name,
                          int &new_value);


 private:

  // Holds the server_name
  ACE_CString server_name_;

  // Holds the command_line that is to be used to activate the
  // server
  ACE_CString command_line_;


  ACE_CString activation_;

  // Directory where it should be running
  ACE_CString working_dir_;

  // Environment variables if any
  ACE_CString environment_vars_;

  // IOR of the server
  ACE_CString server_object_ior_;

  // Hostname
  ACE_CString location_;

  // Checks to see if that particular entry exists.
  int found_server_entry_;
  int command_line_entry_;
  int working_dir_entry_;
  int environment_vars_entry_;
  int activation_entry_;
  int server_object_ior_entry_;
  int location_entry_;
  int startup_value_;
};

#endif /* XML_CONTENTHANDLER_H */
