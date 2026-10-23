// -*- C++ -*-
//=============================================================================
/**
 *  @file   Locator_XMLHandler.h
 *
 *  @author Justin Michel <michel_j@ociweb.com>
 */
//=============================================================================

#ifndef Locator_XMLHandler_H
#define Locator_XMLHandler_H

#include "ACEXML/common/DefaultHandler.h"
#include "tao/ORB.h"
#include "Server_Info.h"
#include "Activator_Info.h"

#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class XML_Backing_Store;
/**
 * Callback SAX XML Handler for parsing Locator XML.
 */
class Locator_XMLHandler : public ACEXML_DefaultHandler
{
public:
  using NameValue = std::pair<ACE_CString, ACE_CString>;
  using NameValues = std::vector<NameValue>;
  // XML ELEMENT names
  static const ACE_TCHAR* ROOT_TAG;
  static const ACE_TCHAR* SERVER_INFO_TAG;
  static const ACE_TCHAR* ENVIRONMENT_TAG;
  static const ACE_TCHAR* ACTIVATOR_INFO_TAG;

  static const ACE_TCHAR* SERVER_TAG;
  static const ACE_TCHAR* POANAME_TAG;
  static const ACE_TCHAR* JACORB_TAG;
  static const ACE_TCHAR* ACTNAME_TAG;
  static const ACE_TCHAR* CMDLINE_TAG;
  static const ACE_TCHAR* DIR_TAG;
  static const ACE_TCHAR* MODE_TAG;
  static const ACE_TCHAR* LIMIT_TAG;
  static const ACE_TCHAR* PARTIOR_TAG;
  static const ACE_TCHAR* IOR_TAG;
  static const ACE_TCHAR* STARTED_TAG;
  static const ACE_TCHAR* PEER_TAG;
  static const ACE_TCHAR* PID_TAG;
  static const ACE_TCHAR* KEYNAME_TAG;
  static const ACE_TCHAR* ALTKEY_TAG;

  struct EnvVar {
    ACE_TString name;
    ACE_TString value;
    bool operator==(const EnvVar&) const; // To allow Vector explicit instantiation
    bool operator!=(const EnvVar&) const; // To allow Vector explicit instantiation
  };

  using EnvList = std::vector<EnvVar>;
  using PeerList = std::vector<ACE_CString>;

  /// constructor
  /// @param repo the repo to update based on XML
  Locator_XMLHandler (XML_Backing_Store& repo);

  /// provide implementation for handling a new XML element
  void startElement (const ACEXML_Char* namespaceURI,
                     const ACEXML_Char* localName,
                     const ACEXML_Char* qName,
                     ACEXML_Attributes* atts) override;

  /// provide implementation for handling terminating an XML element
  void endElement (const ACEXML_Char* namespaceURI,
                   const ACEXML_Char* localName,
                   const ACEXML_Char* qName) override;

 private:
  /// the repository
  XML_Backing_Store& repo_;
  Server_Info *si_;
  bool server_started_;
  NameValues extra_params_;
  EnvList env_vars_;
  PeerList peer_list_;
};

#endif /* Locator_XMLHandler_H */
