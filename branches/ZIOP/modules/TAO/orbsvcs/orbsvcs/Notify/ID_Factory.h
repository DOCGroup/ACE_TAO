/* -*- C++ -*- */
/**
 *  @file ID_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_ID_FACTORY_H
#define TAO_Notify_ID_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"

#include "orbsvcs/Notify/Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ID_Factory
 *
 * @brief A simple factory for generating ID's for objects created by Notify.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ID_Factory
{
public:
  /// Constuctor
  TAO_Notify_ID_Factory (void);

  /// Destructor
  ~TAO_Notify_ID_Factory ();

  TAO_Notify_Object::ID id (void);

  void set_last_used (const TAO_Notify_Object::ID id);

private:
  // Can't use atomic op, because we added the set_last_used() method.
  TAO_Notify_Object::ID seed_;
  TAO_SYNCH_MUTEX mtx_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/ID_Factory.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_ID_FACTORY_H */
