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
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

#include "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;

  class Environment;

};

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

  /// Is <object> collocated?
  virtual CORBA::Boolean is_collocated (CORBA::Object_ptr object
                                        ACE_ENV_ARG_DECL) const = 0;
};

#if defined (__ACE_INLINE__)
# include "tao/Collocation_Resolver.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_COLLOCATION_RESOLVER_H */
