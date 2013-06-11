// -*- C++ -*-
//=============================================================================
/**
 *  @file   Locator_XMLHandler.h
 *
 *  $Id$
 *
 *  @author Justin Michel <michel_j@ociweb.com>
 */
//=============================================================================

#ifndef Locator_XMLHandler_H
#define Locator_XMLHandler_H

#include "ACEXML/common/DefaultHandler.h"

#include "tao/ORB.h"

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
  typedef std::pair<ACE_CString, ACE_CString> NameValue;
  typedef std::vector<NameValue> NameValues;
  // XML ELEMENT names
  static const ACE_TCHAR* ROOT_TAG;
  static const ACE_TCHAR* SERVER_INFO_TAG;
  static const ACE_TCHAR* ENVIRONMENT_TAG;
  static const ACE_TCHAR* ACTIVATOR_INFO_TAG;

  struct EnvVar {
    ACE_TString name;
    ACE_TString value;
    bool operator==(const EnvVar&) const; // To allow Vector explicit instantiation
    bool operator!=(const EnvVar&) const; // To allow Vector explicit instantiation
  };

  typedef std::vector<EnvVar> EnvList;

  /// constructor
  /// @param repo the repo to update based on XML
  /// @param orb the orb (used to create server object)
  Locator_XMLHandler (XML_Backing_Store& repo, CORBA::ORB_ptr orb);

  /// provide implementation for handling a new XML element
  virtual void startElement (const ACEXML_Char* namespaceURI,
                             const ACEXML_Char* localName,
                             const ACEXML_Char* qName,
                             ACEXML_Attributes* atts);

  /// provide implementation for handling terminating an XML element
  virtual void endElement (const ACEXML_Char* namespaceURI,
                           const ACEXML_Char* localName,
                           const ACEXML_Char* qName);

 private:
  /// the repository
  XML_Backing_Store& repo_;

  /// the server related parameters
  ACE_CString server_id_;
  ACE_TString server_name_;
  bool jacorb_server_;
  ACE_TString activator_name_;
  ACE_TString command_line_;
  ACE_TString activation_;
  ACE_TString working_dir_;
  ACE_TString server_object_ior_;
  ACE_TString partial_ior_;
  int start_limit_;
  bool server_started_;
  NameValues extra_params_;
  unsigned int repo_id_;
  unsigned int repo_type_;
  EnvList env_vars_;

  /// the orb
  CORBA::ORB_var orb_;
};

#endif /* Locator_XMLHandler_H */
