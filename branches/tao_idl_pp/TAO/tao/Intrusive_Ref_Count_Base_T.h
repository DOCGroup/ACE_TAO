// -*- C++ -*-

//=============================================================================
/**
 *  @file    Intrusive_Ref_Count_Base_T.h
 *
 *  $Id$
 *
 *  @authors Tim Bradley  <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_INTRUSIVE_REF_COUNT_BASE_T_H
#define TAO_INTRUSIVE_REF_COUNT_BASE_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Intrusive_Ref_Count_Base<ACE_LOCK>
 *
 * @brief Template base class to provide intrusive reference-counting
 *        to subclasses.  This makes the subclass capable of using a
 *        TAO_Intrusive_Ref_Count_Handle<X> class as a smart-pointer
 *        to an X object.  In this case, X is a sub-class of this class,
 *        TAO_Intrusive_Ref_Count_Base<ACE_LOCK>.  The ACE_LOCK type is
 *        used to protect the atomic reference count data member.
 *
 */
template <class ACE_LOCK>
class TAO_Intrusive_Ref_Count_Base
{
public:

  virtual ~TAO_Intrusive_Ref_Count_Base (void);

  void _add_ref (void);
  void _remove_ref (void);


protected:

  TAO_Intrusive_Ref_Count_Base (void);


private:

  ACE_Atomic_Op<ACE_LOCK, long> ref_count_;

  // Prevent copying/assignment.
  TAO_Intrusive_Ref_Count_Base (const TAO_Intrusive_Ref_Count_Base&);
  TAO_Intrusive_Ref_Count_Base& operator= (const TAO_Intrusive_Ref_Count_Base&);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Intrusive_Ref_Count_Base_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Intrusive_Ref_Count_Base_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Intrusive_Ref_Count_Base_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_INTRUSIVE_REF_COUNT_BASE_T_H */
