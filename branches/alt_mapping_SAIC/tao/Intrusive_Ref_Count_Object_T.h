// -*- C++ -*-

//=============================================================================
/**
 *  @file    Intrusive_Ref_Count_Object_T.h
 *
 *  $Id$
 *
 *  @authors Yan Dai  <dai_y@ociweb.com>
 */
//=============================================================================

#ifndef TAO_INTRUSIVE_REF_COUNT_OBJECT_T_H
#define TAO_INTRUSIVE_REF_COUNT_OBJECT_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"
#include /**/ "tao/Intrusive_Ref_Count_Base_T.h"

#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Intrusive_Ref_Count_Object<ACE_LOCK>
 *
 * @brief Template class as wrapper of a non reference counted data type but provide
 *        intrusive reference-counting feature by inherited from TAO_Intrusive_Ref_Count_Base.
 *        This makes the parameterized type data be smart pointer by using a
 *        TAO_Intrusive_Ref_Count_Handle<X> to an this wrapper object.
 */
template <class OBJ, class ACE_LOCK>
class TAO_Intrusive_Ref_Count_Object : public TAO_Intrusive_Ref_Count_Base <ACE_LOCK>
{
public:

  /// take ownership of obj.
  TAO_Intrusive_Ref_Count_Object (OBJ* obj);
  virtual ~TAO_Intrusive_Ref_Count_Object (void);

  OBJ* get () const;

private:

  // Prevent default constructor used.
  TAO_Intrusive_Ref_Count_Object (void);

  // Prevent copying/assignment.
  TAO_Intrusive_Ref_Count_Object (const TAO_Intrusive_Ref_Count_Object&);
  TAO_Intrusive_Ref_Count_Object& operator= (const TAO_Intrusive_Ref_Count_Object&);

  OBJ* obj_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Intrusive_Ref_Count_Object_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Intrusive_Ref_Count_Object_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Intrusive_Ref_Count_Object_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_INTRUSIVE_REF_COUNT_OBJECT_T_H */
