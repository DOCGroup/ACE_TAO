/* -*- C++ -*- */
/**
 *  @file Container_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_CONTAINER_T_H
#define TAO_NS_CONTAINER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Container.h"
#include "Object_T.h"
#include "orbsvcs/ESF/ESF_Worker.h"

/**
 * @class TAO_NS_Container_T
 *
 * @brief
 *
 */
template <class OBJECT>
class TAO_Notify_Export TAO_NS_Object_Find_Worker_T : public TAO_ESF_Worker<OBJECT>
{
  typedef TAO_ESF_Proxy_Collection<OBJECT> COLLECTION;

public:
  /// Constructor
  TAO_NS_Object_Find_Worker_T (TAO_NS_Object_Id id);

  /// Find
  OBJECT* find (COLLECTION* collection ACE_ENV_ARG_DECL);

protected:
  ///= TAO_ESF_Worker method
  void work (OBJECT* object ACE_ENV_ARG_DECL);

  /// The id we're looking for.
  TAO_NS_Object_Id id_;

  /// The result
  OBJECT* result_;
};

/**
 * @class TAO_NS_Container_T
 *
 * @brief A template class that manages a collection.
 * TYPE = type of collection, OBJECT = the object, PARENT = parent of object (grandparent of TYPE)
 *
 */
template <class TYPE, class OBJECT, class PARENT>
class TAO_Notify_Export TAO_NS_Container_T : public virtual TAO_NS_Object_T<OBJECT, PARENT>, public virtual TAO_NS_Container
{
  typedef TAO_NS_Object_Find_Worker_T<TYPE> FIND_WORKER;

public:
  /// Constuctor
  TAO_NS_Container_T (void);

  /// Destructor
  virtual ~TAO_NS_Container_T ();

  /// Insert object to this container.
  virtual void insert (TYPE* type ACE_ENV_ARG_DECL);

  /// Remove type from container_
  virtual void remove (TYPE* type ACE_ENV_ARG_DECL);

  /// Find the Type.
  TYPE* find (TAO_NS_Object_Id id ACE_ENV_ARG_DECL);

  /// Init this object.
  void init_collection (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

protected:
  typedef TAO_ESF_Proxy_Collection<TYPE> COLLECTION;

  /// The collection data structure that we add objects to.
  COLLECTION* collection_;
};

#if defined (__ACE_INLINE__)
#include "Container_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Container_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Container_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_CONTAINER_T_H */
