//=============================================================================
/**
 *  @file    Default_Collocation_Resolver.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_DEFAULT_COLLOCATION_RESOLVER_H
#define TAO_DEFAULT_COLLOCATION_RESOLVER_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Collocation_Resolver.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_Default_Collocation_Resolver
 *
 * @brief Simple resolver used to decide if a CORBA::Object is
 * collocated.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Default_Collocation_Resolver :
  public TAO_Collocation_Resolver
{
public:

  /// Is <object> collocated?
  virtual CORBA::Boolean is_collocated (CORBA::Object_ptr object
                                        TAO_ENV_ARG_DECL) const;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Collocation_Resolver)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Collocation_Resolver)

#if defined (__ACE_INLINE__)
# include "tao/Default_Collocation_Resolver.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_DEFAULT_COLLOCATION_RESOLVER_H */
