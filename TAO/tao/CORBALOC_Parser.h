// -*- C++ -*-

//=============================================================================
/**
 *  @file   CORBALOC_Parser.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Priyanka Gontla (pgontla@uci.edu)
 */
//=============================================================================


#ifndef TAO_CORBALOC_PARSER_H
#define TAO_CORBALOC_PARSER_H
#include /**/ "ace/pre.h"

#include "tao/IOR_Parser.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

class TAO_MProfile;

/**
 * @class TAO_CORBALOC_Parser
 *
 * @brief Implements the <corbaloc:> IOR format
 *
 * This class implements the <corbaloc:> IOR format.
 * It is dynamically loaded by the ORB and used to parse the
 * string to separate the individual <obj_addr> from the list of object
 * addresses <obj_addr_list>.
 */
class TAO_Export TAO_CORBALOC_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_CORBALOC_Parser (void);

  /// The destructor
  virtual ~TAO_CORBALOC_Parser (void);

  /// = The IOR_Parser methods, please read the documentation in
  ///   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;

  /// Parse the ior-string that is passed.
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Checks the prefix to see if it is RIR.
  virtual int check_prefix (const char *endpoint
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Helps count the length of the <obj_addr_list> and the number of
  /// individual <obj_addr> in the <obj_addr_list>.
  virtual void parse_string_count_helper (const char * corbaloc_name,
                                          CORBA::ULong &addr_list_length,
                                          CORBA::ULong &count_addr
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Creates a MProfile for the endpoint passed and each such mprofile
   * is added to the big mprofile <mprofile_> from which a pointer to
   * the Object represented by the key_string is obtained and passed
   * to the application.
   */
  virtual void parse_string_mprofile_helper (const char * end_point,
                                             CORBA::ORB_ptr orb,
                                             TAO_MProfile &mprofile
                                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Make a stub from the MProfile that is created in
   * parse_string_mprofile_helper. Using this stub, create an object
   * reference which is sent to the application.
   */
  virtual CORBA::Object_ptr
    make_stub_from_mprofile (CORBA::ORB_ptr orb,
                             TAO_MProfile &mprofile
                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Gets the pointer to the key_string when the protocol used is RIR
  virtual CORBA::Object_ptr
    parse_string_rir_helper (const char * &corbaloc_name,
                             CORBA::ORB_ptr orb
                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Tokenizes the <obj_addr_list> using "," as the seperator. Assigns
  /// individual endpoints to the elements of the ACE_Array_Base.
  virtual void parse_string_assign_helper (
                                           ACE_CString &key_string,
                                           ACE_CString &cloc_name,
                                           CORBA::ORB_ptr orb,
                                           TAO_MProfile &mprofile
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Helps parse_string_assign_helper by assigning in the case when
   * the protocol name is present and we have to append jsut the key
   * string.
   */
  virtual void
    assign_key_string(char * &cloc_name_ptr,
                      ACE_CString &key_string,
                      CORBA::ULong &addr_list_length,
                      CORBA::ORB_ptr orb,
                      TAO_MProfile &mprofile
                      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#if defined (__ACE_INLINE__)
# include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_CORBALOC_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_CORBALOC_Parser)

#include /**/ "ace/post.h"
#endif /* TAO_CORBALOC_PARSER_H */
