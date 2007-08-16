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

#if (TAO_HAS_CORBALOC_PARSER == 1)

#include "ace/Service_Config.h"
#include "ace/Array.h"
#include "tao/Profile.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_CORBALOC_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_CORBALOC_Parser (void);

  /// The destructor
  virtual ~TAO_CORBALOC_Parser (void);

  /// = The IOR_Parser methods, please read the documentation in
  ///   IOR_Parser.h
  virtual bool match_prefix (const char *ior_string) const;

  /// Parse the ior-string that is passed.
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          );

private:

  /**
   * Make a stub from the MProfile that is created in
   * parse_string_mprofile_helper. Using this stub, create an object
   * reference which is sent to the application.
   */
  CORBA::Object_ptr make_stub_from_mprofile (CORBA::ORB_ptr orb,
                                             TAO_MProfile &mprofile
                                             );

  /// Gets the pointer to the key_string when the protocol used is RIR
  CORBA::Object_ptr parse_string_rir_helper (const char *corbaloc_name,
                                             CORBA::ORB_ptr orb
                                             );

  void make_canonical (const char *ior,
                       size_t ior_len,
                       ACE_CString &canonical_endpoint
                       );

  struct parsed_endpoint {
    parsed_endpoint ();
    ~parsed_endpoint ();
    TAO_Profile *profile_;
    char obj_key_sep_;
    ACE_CString prot_addr_;
  };
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CORBALOC_Parser.inl"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_CORBALOC_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_CORBALOC_Parser)

#endif /* TAO_HAS_CORBALOC_PARSER == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_CORBALOC_PARSER_H */
