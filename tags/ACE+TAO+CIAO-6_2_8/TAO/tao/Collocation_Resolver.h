// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocation_Resolver.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_COLLOCATION_RESOLVER_H
#define TAO_COLLOCATION_RESOLVER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

/**
 * @class TAO_Collocation_Resolver
 *
 * @brief This class helps in resolving if a CORBA:Object is collocated.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Collocation_Resolver
  : public ACE_Service_Object
{
public:
  /// Destructor.
  virtual ~TAO_Collocation_Resolver (void);

  /// Is @a object collocated?
  virtual CORBA::Boolean is_collocated (CORBA::Object_ptr object) const = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COLLOCATION_RESOLVER_H */
