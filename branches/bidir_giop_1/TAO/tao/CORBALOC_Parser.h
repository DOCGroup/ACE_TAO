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

#include "tao/IOR_Parser.h"

// @@ Priyanka: notice how many includes I was able to remove.  In
// general you should try to minimize the number of #includes in your
// .h files.  In fact, I believe that once you remove the orb_ and
// mprofile_ fields below these two includes can go away too!
#include "tao/ORB.h"
#include "tao/MProfile.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

class TAO_Export TAO_CORBALOC_Parser : public TAO_IOR_Parser
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

  virtual int match_prefix (const char *ior_string) const;
  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h

  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Parse the ior-string that is passed.

private:

  // @@ Priyanka: do you really need this (see next comment)?  BTW, in
  // general we put private data members *after* private methods.
  CORBA::ORB_var orb_;
  // ORB

  // @@ Priyanka: this makes the class non-reentrant, only one thread
  //    can parse a string at a time.  The "Right Thing"[tm] is to
  //    pass the MProfile to whatever helper methods you have, so they
  //    can add stuff to it.  That way there is no resource
  //    contention.
  TAO_MProfile mprofile_;
  // One big mprofile which consists the profiles of all the endpoints.

  virtual int check_prefix (const char *endpoint);
  // Checks the prefix to see if it is RIR.

  virtual void parse_string_count_helper (const char * &corbaloc_name,
                                          CORBA::ULong &addr_list_length,
                                          CORBA::ULong &count_addr,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Helps count the length of the <obj_addr_list> and the number of
  // individual <obj_addr> in the <obj_addr_list>.

  virtual void
    parse_string_mprofile_helper (CORBA::String_var end_point,
                                  CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Creates a MProfile for the endpoint passed and each such mprofile
  // is added to the big mprofile <mprofile_> from which a pointer to
  // the Object represented by the key_string is obtained and passed
  // to the application.

  virtual CORBA::Object_ptr
    make_stub_from_mprofile (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Make a stub from the MProfile that is created in
  // parse_string_mprofile_helper. Using this stub, create an object
  // reference which is sent to the application.

  virtual CORBA::Object_ptr
    parse_string_rir_helper (const char * &corbaloc_name,
                             CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Gets the pointer to the key_string when the protocol used is RIR

  virtual void parse_string_assign_helper (CORBA::ULong &addr_list_length,
                                           ACE_CString &key_string,
                                           ACE_CString &cloc_name,
                                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Tokenizes the <obj_addr_list> using "," as the seperator. Assigns
  // individual endpoints to the elements of the ACE_Array_Base.

  virtual void
    assign_key_string(char * &cloc_name_ptr,
                      ACE_CString &key_string,
                      CORBA::ULong &addr_list_length,
                      CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Helps parse_string_assign_helper by assigning in the case when
  // the protocol name is present and we have to append jsut the key
  // string.

};

#if defined (__ACE_INLINE__)
# include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_CORBALOC_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_CORBALOC_Parser)

#include "ace/post.h"
#endif /* TAO_CORBALOC_PARSER_H */
