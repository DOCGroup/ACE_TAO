// -*- C++ -*-
//=============================================================================
/**
 *  @file   XML_ContentHandler.h
 *
 *  $Id$
 *
 *  @brief  Callback SAX XML Handler for parsing XML.
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
 * Callback SAX XML Handler for parsing XML.
 *
 */
class XML_ContentHandler : public ACEXML_DefaultHandler
{
public:

  // XML ELEMENT names
  static const char* ROOT_TAG;
  static const char* SERVER_INFO_TAG;
  static const char* SERVER_TAG;
  static const char* STARTUP_OPTIONS_TAG;
  static const char* COMMAND_LINE_TAG;
  static const char* WORKING_DIR_TAG;
  static const char* ACTIVATION_TAG;
  static const char* ENVIRONMENT_TAG;
  static const char* PARTIAL_IOR_TAG;
  static const char* IOR_TAG;

  struct Callback {
    virtual ~Callback() {}
    virtual void next_server (const ACE_CString& server_name, const ACE_CString& startup_cmd, 
      const ACE_CString& env_vars, const ACE_CString& working_dir,
      const ACE_CString& actmode, const ACE_CString& partial_ior, const ACE_CString& ior) = 0;
  };

  XML_ContentHandler (Callback& cb);

  virtual void startElement (const ACEXML_Char* namespaceURI,
                             const ACEXML_Char* localName,
                             const ACEXML_Char* qName,
                             ACEXML_Attributes* atts ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual void endElement (const ACEXML_Char* namespaceURI,
                           const ACEXML_Char* localName,
                           const ACEXML_Char* qName ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char* ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual void ignorableWhitespace (const ACEXML_Char*,
                                    int,
                                    int ACEXML_ENV_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((ACEXML_SAXException))
  {
    tag_state_ = tsInvalid;
  }

 private:
  // reset all attributes
  void reset_server_info (void);

 private:

  // callback on completion of an element
  Callback& callback_;

  // Holds the server_name
  ACE_CString server_name_;

  // Holds the command_line that is to be used to activate the server
  ACE_CString command_line_;

  ACE_CString activation_;

  // Directory where it should be running
  ACE_CString working_dir_;

  // Environment variables if any
  ACE_CString env_vars_;

  // IOR of the server
  ACE_CString server_object_ior_;

  // Hostname
  ACE_CString partial_ior_;

  // current state
  enum TagState {
    tsInvalid,
    tsServerInfo,
    tsServer,
    tsStartupOptions,
    tsCommandLine,
    tsWorkingDir,
    tsActivation,
    tsEnvironment,
    tsLocation,
    tsIOR
  };

  TagState tag_state_;
};

#endif /* XML_CONTENTHANDLER_H */
