// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   CORBANAME_Parser.h
//
// = AUTHOR
//   Priyanka Gontla (pgontla@uci.edu)
//
// ============================================================================

#ifndef TAO_CORBANAME_PARSER_H
#define TAO_CORBANAME_PARSER_H
#include "ace/pre.h"

#include "tao/IOR_Parser.h"
#include "ior_corbaname_export.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IOR_CORBANAME_Export TAO_CORBANAME_Parser : public TAO_IOR_Parser
{
  // = TITLE
  //   Implements the <corbaname:> IOR format
  //
  // = DESCRIPTION
  //   This class implements the <corbaname:> IOR format.
  //   It is dynamically loaded by the ORB and used to get reference
  //   to a naming service at the given address and port and then resolve an
  //   object in that context.
  //
public:
  TAO_CORBANAME_Parser (void);
  // Constructor

  virtual ~TAO_CORBANAME_Parser (void);
  // The destructor

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:
  virtual CORBA::Object_ptr
    parse_string_dynamic_request_helper (CosNaming::NamingContextExt_var &naming_context,
                                         ACE_CString &key_string,
                                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined (__ACE_INLINE__)
# include "CORBANAME_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_FACTORY_DECLARE (TAO_IOR_CORBANAME, TAO_CORBANAME_Parser)

#include "ace/post.h"
#endif /* TAO_CORBANAME_PARSER_H */
