// -*- C++ -*-

//=============================================================================
/**
 *  @file    Intrusive_Ref_Count_Handle_T.h
 *
 *  $Id$
 *
 *  @authors Tim Bradley  <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_INTRUSIVE_REF_COUNT_HANDLE_T_H
#define TAO_INTRUSIVE_REF_COUNT_HANDLE_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Intrusive_Ref_Count_Handle<T>
 *
 * @brief Template class for smart-pointer to (intrusively) ref-counted object.
 *
 *  This class behaves just like a xxx_var type behaves.  The only significant
 *  difference is that this class provides a "bool is_nil() const" method,
 *  and xxx_var types don't (they use the "bool CORBA::is_nil(xxx_ptr ptr)"
 *  method instead). For example,
 *
 *  typedef TAO_Intrusive_Ref_Count_Handle<PortableServer::ServantBase>
 *      MyServantBase_var;
 *
 *  The MyServantBase_var and the PortableServer::ServantBase_var are
 *  nearly idenitical.  The only difference is that the MyServantBase_var
 *  has a "isNil()" method that indicates whether or not the smart pointer
 *  is in the 'nil' state or not.
 *
 *  This class can be used to "safely" deal with an instance of a servant.
 *  For example, we can use a single variable
 *  TAO_Intrusive_Ref_Count_Handle<Foo_i>
 *
 *      typedef TAO_Intrusive_Ref_Count_Handle<Foo_i> Foo_i_var;
 *      Foo_i_var servant_;
 *
 *  instead of using two variables
 *
 *      PortableServer::ServantBase_var servant_holder_;
 *      Foo_i* servant_;

 *  to deal with the servant memory.
 *
 *  The Foo_i_var type does everything that the PortableServer::ServantBase_var
 *  type does. In addition, the Foo_i_var type can provide access to the servant
 *  as derived class via the arrow operator.
 */
template <typename T>
class TAO_Intrusive_Ref_Count_Handle
{
public:

  /// Default Constructor - enters the "nil" state.
  TAO_Intrusive_Ref_Count_Handle (void);

  /// Ctor - By default, takes ownership of passed-in "copy" of reference
  ///        to T.  But the second argument (bool) can be changed from
  ///        the default value of 'true' to the non-default value of 'false'.
  ///        The second argument dictates whether or not this handle object
  ///        should take ownership of the passed-in pointer to the T object.
  ///        By default, it takes ownership, leaving the reference counter
  ///        of the T object unchanged.  When it is instructed to not take
  ///        ownership (false value for second arg), then the reference
  ///        counter of the T object will be incremented so that this
  ///        handle object has its own "copy".
  TAO_Intrusive_Ref_Count_Handle (T* p, bool take_ownership = true);

  /// Copy Constructor - claims a "copy" of rhs object's reference to T.
  TAO_Intrusive_Ref_Count_Handle (const TAO_Intrusive_Ref_Count_Handle& b);

  /// Destructor
  ~TAO_Intrusive_Ref_Count_Handle (void);

  /// Assignment Operator with T* argument.
  /// Takes ownership of passed-in "copy" of reference to T.
  TAO_Intrusive_Ref_Count_Handle& operator= (T* p);

  /// Assignment Operator with const TAO_Smart_Ptr<T>& argument.
  /// Claims a "copy" of rhs object's reference to T.
  TAO_Intrusive_Ref_Count_Handle& operator=
                                   (const TAO_Intrusive_Ref_Count_Handle& b);

  /// Const Accessor to underlying pointer (T*) using arrow (->) operator.
  T* operator->() const;

  /// Returns true if underlying pointer is NULL (0).
  /// Returns false otherwise.
  bool is_nil (void) const;

  /// Used to pass the underlying pointer as an "IN" argument to a method.
  T* in (void) const;

  /// Used to pass the underlying pointer as an "IN/OUT" argument to a method.
  T*& inout (void);

  /// Used to pass the underlying pointer as an "OUT" argument to a method.
  T*& out (void);

  /// Used to take-away the underlying pointer from this smart pointer object.
  /// Caller becomes responsibe for the returned "copy" to the reference.
  /// Always leaves the smart pointer in the "nil" state upon return.
  T* _retn (void);

  /// Equality operator allows the refcounted object to be used generically
  /// as a contained object
  bool operator== (const TAO_Intrusive_Ref_Count_Handle& h) const;

private:

  /// Claim a "copy" of the reference-counted object by adding
  /// one to its reference counter.  Do nothing if this smart pointer
  /// object is currently in the "nil" state.
  void claim (void);

  /// Drop our "copy" of the reference-counted object by removing
  /// one from its reference counter.  Do nothing if this smart pointer
  /// object is currently in the "nil" state.
  /// Note that this method will always leave this smart pointer
  /// in the "nil" state upon its return.
  void drop (void);

  /// The underlying pointer to the (intrusively) reference-counted object.
  /// Set to 0 when this smart pointer is in the "nil" state.  Otherwise,
  /// this smart pointer always owns a (reference-counted) "copy" of the
  /// object pointed to by the ptr_ data member.
  T* ptr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Intrusive_Ref_Count_Handle_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Intrusive_Ref_Count_Handle_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Intrusive_Ref_Count_Handle_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_INTRUSIVE_REF_COUNT_HANDLE_T_H */
