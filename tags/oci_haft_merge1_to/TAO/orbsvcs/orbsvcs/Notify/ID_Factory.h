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
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Atomic_Op.h"
#include "Object.h"

/**
 * @class TAO_Notify_ID_Factory
 *
 * @brief A simple factory for generating ID's for objects created by Notify.
 *
 */
class TAO_Notify_Export TAO_Notify_ID_Factory
{
public:
  /// Constuctor
  TAO_Notify_ID_Factory (void);

  /// Destructor
  ~TAO_Notify_ID_Factory ();

  TAO_Notify_Object::ID id (void);

private:
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, TAO_Notify_Object::ID> seed_;
};


#if defined (__ACE_INLINE__)
#include "ID_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_ID_FACTORY_H */
