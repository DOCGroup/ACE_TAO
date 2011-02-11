// -*- C++ -*-

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
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;

  class ORB;
  typedef ORB *ORB_ptr;
}

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

  /// Return true if @a ior_string starts with a prefix known to this IOR
  /// parser
  virtual bool match_prefix (const char *ior_string) const = 0;

  /**
   * Parse the @a ior argument and return an object reference.
   * The call may raise the standard system exceptions (NO_MEMORY,
   * INV_OBJREF, etc.)
   */
  virtual CORBA::Object_ptr parse_string (const char *ior, CORBA::ORB_ptr orb)
    = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IOR_PARSER_H */
