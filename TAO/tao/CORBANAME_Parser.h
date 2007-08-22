// -*- C++ -*-

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

#if (TAO_HAS_CORBANAME_PARSER == 1)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_CORBANAME_Parser : public TAO_IOR_Parser
{
public:

  /// The destructor
  virtual ~TAO_CORBANAME_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual bool match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb);

 private:
   virtual CORBA::Object_ptr
   parse_string_dynamic_request_helper (CORBA::Object_ptr naming_context,
                                        ACE_CString &key_string
                                        );
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_CORBANAME_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_CORBANAME_Parser)

#endif /* TAO_HAS_CORBANAME_PARSER == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_CORBANAME_PARSER_H */
