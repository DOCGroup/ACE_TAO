// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   CORBALOC_Parser.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//   Priyanka Gontla (pgontla@uci.edu)
//
// ============================================================================

#ifndef TAO_CORBALOC_PARSER_H
#define TAO_CORBALOC_PARSER_H
#include "ace/pre.h"

#include "tao/ORB.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"
#include "tao/IOR_Parser.h"
#include "tao/Parser_Registry.h"

#include "ace/SString.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Read_Buffer.h"
#include "ace/Auto_Ptr.h"
#include "ace/Arg_Shifter.h"

#include "tao/Object.h"
#include "tao/Typecode.h"
#include "tao/NVList.h"
#include "tao/Stub.h"
#include "tao/DynAny_i.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/TAO_Internal.h"
#include "tao/CDR.h"
#include "tao/IOR_LookupTable.h"
#include "tao/Object_Adapter.h"
#include "tao/POA.h"
#include "tao/Request.h"
#include "tao/MProfile.h"

#include "tao/IOR_Parser.h"
#include "tao/Pluggable.h"

#include "ior_corbaloc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IOR_CORBALOC_Export TAO_CORBALOC_Parser : public TAO_IOR_Parser
{
  // = TITLE
  //   Implements the <corbaloc:> IOR format
  //
  // = DESCRIPTION
  //   This class implements the <corbaloc:> IOR format.
  //   It is dynamically loaded by the ORB and used to parse the
  //   string to separate the individual <obj_addr> from the list of object
  //   addresses <obj_addr_list>.
  //
public:
  TAO_CORBALOC_Parser (void);
  // Constructor

  virtual ~TAO_CORBALOC_Parser (void);
  // The destructor

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;

  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:
  virtual int check_prefix (const char *endpoint);
  // Checks the prefix (IIOP or RIR for now);

  virtual void parse_string_count_helper (const char * &corbaloc_name,
                                          CORBA::ULong &addr_list_length,
                                          CORBA::ULong &count_addr,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Helps count the length of the <obj_addr_list> and the number of
  // individual <obj_addr> in the <obj_addr_list>.
  
  virtual CORBA::Object_ptr parse_string_mprofile_helper (ACE_Array_Base <char *> &addr,
                                                          CORBA::ULong &count_addr,
                                                          CORBA::ORB_ptr orb,
                                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Creates a MProfile for the endpoints in the <obj_addr_list> and
  // finally returns a pointer to the Object represented by the key_string.

  virtual CORBA::Object_ptr parse_string_rir_helper (const char * &corbaloc_name,
                                                     CORBA::ORB_ptr orb,
                                                     CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Gets the pointer to the key_string when the protocol used is RIR
  
  virtual void parse_string_assign_helper (ACE_Array_Base <char *> &addr,
                                           CORBA::ULong &addr_list_length,
                                           ACE_CString &key_string,
                                           ACE_CString &cloc_name,
                                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Tokenizes the <obj_addr_list> using "," as the seperator. Assigns
  // individual endpoints to the elements of the ACE_Array_Base.

  virtual void assign_iiop_prefix_key_string (ACE_Array_Base <char *> &addr,
                                              char * &cloc_name_ptr,
                                              ACE_CString &key_string,
                                              CORBA::ULong &current_addr,
                                              CORBA::ULong &addr_list_length);
  // Helps parse_string_assign_helper by assigning in the case when 
  // the protocol name is missing and we have to append the default
  // protocol <iiop:> and the key string.

  virtual void assign_key_string(ACE_Array_Base <char *> &addr,
                                 char * &cloc_name_ptr,
                                 ACE_CString &key_string,
                                 CORBA::ULong &current_addr,
                                 CORBA::ULong &addr_list_length);
  // Helps parse_string_assign_helper by assigning in the case when 
  // the protocol name is present and we have to append jsut the key
  // string.


};

#if defined (__ACE_INLINE__)
# include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_FACTORY_DECLARE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#include "ace/post.h"
#endif /* TAO_CORBALOC_PARSER_H */
