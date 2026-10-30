/* -*- C++ -*- */
/**
 *  @file Find_Worker_T.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
template <class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION>
class TAO_Notify_Serv_Export TAO_Notify_Find_Worker_T : public TAO_ESF_Worker<TYPE>
{
  using CONTAINER = TAO_Notify_Container_T<TYPE>;
  using COLLECTION = TAO_ESF_Proxy_Collection<TYPE>;

public:
  /// Constructor
  TAO_Notify_Find_Worker_T ();

  TAO_Notify_Find_Worker_T (TAO_Notify_Find_Worker_T const &) = delete;
  TAO_Notify_Find_Worker_T (TAO_Notify_Find_Worker_T &&) = delete;
  TAO_Notify_Find_Worker_T &operator= (TAO_Notify_Find_Worker_T const &) = delete;
  TAO_Notify_Find_Worker_T &operator= (TAO_Notify_Find_Worker_T &&) = delete;

  /// Find the Type.
  TYPE* find (TAO_Notify_Object::ID const id, CONTAINER& container);

  /// Find and resolve to the Interface.
  INTERFACE_PTR resolve (TAO_Notify_Object::ID const id, CONTAINER& container);

protected:
  ///= TAO_ESF_Worker method
  void work (TYPE* object);

  /// The id we're looking for.
  TAO_Notify_Object::ID id_ {};

  /// The result
  TYPE* result_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Find_Worker_T.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Find_Worker_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_FIND_WORKER_T_H */
