// $Id$

#ifndef CATIOR_I_H
#define CATIOR_I_H

// ============================================================================
//
// = LIBRARY
//    TAO/Utils/catior
//
// = FILENAME
//    Catior_i.h
//
// = DESCRIPTION
//    Reads stringified IORs and outputs the encoded information.
//
// = AUTHORS
//      Jeff Hopper <jrhopper@cts.com>
//      SCIOP and Tagged component modifications by:
//      Jason Cohen, Lockheed Martin ATL <jcohen@atl.lmco.com>
//      Split into a separate library by:
//      Chad Elliott <elliott_c@ociweb.com>
//
// ============================================================================

#include "catior_export.h"
#include "ace/String_Base.h"
#include "ace/SStringfwd.h"
#include "tao/Basic_Types.h"
#include "tao/CDR.h"

class catior_Export Catior_i
{
public:
  /// Default constructor to initialize all data members.
  Catior_i();

  /// Decode the input IOR string into a human readable format stored in
  /// the supplied output string reference.  A return value of true
  /// indicates that there were no problems decoding the IOR.
  CORBA::Boolean decode (const ACE_CString& input, ACE_CString& output);

private:
  // These methods used to be functions in catior.cpp
  CORBA::Boolean catiiop (char* string);


  CORBA::Boolean catior (char const* str);

  CORBA::Boolean catpoop (char* string);

  CORBA::Boolean cat_tag_orb_type (TAO_InputCDR& stream);

  CORBA::Boolean cat_ibm_partner_version (TAO_InputCDR& stream);

  CORBA::Boolean cat_tao_tag_endpoints (TAO_InputCDR& stream);

  CORBA::Boolean cat_tag_alternate_endpoints (TAO_InputCDR& stream);

  CORBA::Boolean cat_tag_policies (TAO_InputCDR& stream);

  CORBA::Boolean cat_security_association (const CORBA::UShort& a);

  CORBA::Boolean cat_tag_ssl_sec_trans (TAO_InputCDR& cdr);

  CORBA::Boolean cat_octet_seq (const char* object_name,
                                TAO_InputCDR& stream);

  CORBA::Boolean cat_object_key (TAO_InputCDR& stream);

  ACE_CString _find_info (CORBA::ULong id);

  void displayHex (TAO_InputCDR & str);

  CORBA::Boolean cat_codeset_info (TAO_InputCDR& cdr);

  CORBA::Boolean cat_tagged_components (TAO_InputCDR& stream);

  CORBA::Boolean cat_multiple_components (TAO_InputCDR& stream);

  CORBA::Boolean cat_profile_helper (TAO_InputCDR& stream,
                                     const char* protocol);

  CORBA::Boolean cat_coiop_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_iiop_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_shmiop_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_uiop_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_sciop_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_nsk_profile_helper (TAO_InputCDR& stream,
                                         const char* protocol);

  CORBA::Boolean cat_nskpw_profile (TAO_InputCDR& stream);

  CORBA::Boolean cat_nskfs_profile (TAO_InputCDR& stream);

  void indent (void);

  size_t trace_depth_;
  ACE_CString buffer_;
};

#endif // CATIOR_I_H
