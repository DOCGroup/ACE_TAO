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

#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * Callback SAX XML Handler for parsing XML.
 */
class Locator_XMLHandler : public ACEXML_DefaultHandler
{
public:

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

  typedef ACE_Vector<EnvVar> EnvList;

  struct Callback {
    virtual ~Callback() {}

    virtual void next_server (const ACE_CString& server_id,
      const ACE_CString& server_name, const ACE_CString& aname,
      const ACE_CString& startup_cmd, const EnvList& env_vars,
      const ACE_CString& working_dir, const ACE_CString& actmode,
      int start_limit, const ACE_CString& partial_ior,
      const ACE_CString& ior) = 0;

    virtual void next_activator (const ACE_CString& activator_name,
                                 long token,
                                 const ACE_CString& ior) = 0;
  };

  Locator_XMLHandler (Callback& cb);

  virtual void startElement (const ACEXML_Char* namespaceURI,
                             const ACEXML_Char* localName,
                             const ACEXML_Char* qName,
                             ACEXML_Attributes* atts);

  virtual void endElement (const ACEXML_Char* namespaceURI,
                           const ACEXML_Char* localName,
                           const ACEXML_Char* qName);

 private:

  // callback on completion of an element
  Callback& callback_;

  ACE_CString server_id_;
  ACE_TString server_name_;
  ACE_TString activator_name_;
  ACE_TString command_line_;
  ACE_TString activation_;
  ACE_TString working_dir_;
  ACE_TString server_object_ior_;
  ACE_TString partial_ior_;
  int start_limit_;
  EnvList env_vars_;
};

#endif /* Locator_XMLHandler_H */
