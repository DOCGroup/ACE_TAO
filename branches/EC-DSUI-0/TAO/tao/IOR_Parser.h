
//=============================================================================
/**
 *  @file   IOR_Parser.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_IOR_PARSER_H
#define TAO_IOR_PARSER_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/Exception.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

/**
 * @class TAO_IOR_Parser
 *
 * @brief Defines the interface for the pluggable IOR parser components
 *
 * The ORB is able to dynamically load the list of IOR formats it
 * understands.  That allow us to integrate formats such as
 * corbaname:, http: or ftp: only when required.
 * This class provides a uniform interface for all the IOR parsers
 */
class TAO_Export TAO_IOR_Parser : public ACE_Service_Object
{
public:
  /// The destructor
  virtual ~TAO_IOR_Parser (void);

  /// Return 1 if <ior_string> starts with a prefix known to this IOR
  /// parser
  virtual int match_prefix (const char *ior_string) const = 0;

  /**
   * Parse the <ior> argument and return an object reference.
   * The call may raise the standard system exceptions (NO_MEMORY,
   * INV_OBJREF, etc.)
   */
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#if defined (__ACE_INLINE__)
# include "tao/IOR_Parser.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_IOR_PARSER_H */
