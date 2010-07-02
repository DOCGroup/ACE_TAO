/* -*- C++ -*- */
/**
 *  @file Find_Worker_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_FIND_WORKER_T_H
#define TAO_Notify_FIND_WORKER_T_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/Notify/Container_T.h"
#include "orbsvcs/Notify/Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Find_Worker_T
 *
 * @brief Helper to locate a TYPE given its ID.
 *
 */
template <class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION>
class TAO_Notify_Serv_Export TAO_Notify_Find_Worker_T : public TAO_ESF_Worker<TYPE>
{
  typedef TAO_Notify_Container_T<TYPE> CONTAINER;
  typedef TAO_ESF_Proxy_Collection<TYPE> COLLECTION;

public:
  /// Constructor
  TAO_Notify_Find_Worker_T (void);

  /// Find the Type.
  TYPE* find (const TAO_Notify_Object::ID id, CONTAINER& container);

  /// Find and resolve to the Interface.
  INTERFACE_PTR resolve (const TAO_Notify_Object::ID id, CONTAINER& container);

protected:
  ///= TAO_ESF_Worker method
  void work (TYPE* object);

  /// The id we're looking for.
  TAO_Notify_Object::ID id_;

  /// The result
  TYPE* result_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Find_Worker_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Find_Worker_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Find_Worker_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_FIND_WORKER_T_H */
