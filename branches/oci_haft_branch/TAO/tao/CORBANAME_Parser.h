
//=============================================================================
/**
 *  @file   CORBANAME_Parser.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla (pgontla@uci.edu)
 */
//=============================================================================


#ifndef TAO_CORBANAME_PARSER_H
#define TAO_CORBANAME_PARSER_H
#include /**/ "ace/pre.h"

#include "tao/IOR_Parser.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

/**
 * @class TAO_CORBANAME_Parser
 *
 * @brief Implements the <corbaname:> IOR format
 *
 * This class implements the <corbaname:> IOR format.
 * It is dynamically loaded by the ORB and used to get reference
 * to a naming service at the given address and port and then resolve an
 * object in that context.
 */
class TAO_Export TAO_CORBANAME_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_CORBANAME_Parser (void);

  /// The destructor
  virtual ~TAO_CORBANAME_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:
  virtual CORBA::Object_ptr
    parse_string_dynamic_request_helper (CORBA::Object_ptr naming_context,
                                         ACE_CString &key_string
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined (__ACE_INLINE__)
# include "CORBANAME_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_CORBANAME_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_CORBANAME_Parser)

#include /**/ "ace/post.h"
#endif /* TAO_CORBANAME_PARSER_H */
