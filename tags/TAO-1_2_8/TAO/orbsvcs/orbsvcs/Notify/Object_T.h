/* -*- C++ -*- */
/**
 *  @file Object_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_OBJECT_T_H
#define TAO_NS_OBJECT_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Object.h"
#include "Refcountable.h"

/**
 * @class TAO_NS_Object_T
 *
 * @brief Base attributes of all EC participants.
 *
 */
template <class TYPE, class PARENT>
class TAO_NS_Object_T : public virtual TAO_NS_Object, public TAO_NS_Refcountable
{
public:
  /// Constuctor
  TAO_NS_Object_T (void);

  /// Destructor
  virtual ~TAO_NS_Object_T ();

  /// destroy <type>
  void destroy (TYPE* type ACE_ENV_ARG_DECL);

protected:
  /// = Data Members

  /// Parent
  PARENT* parent_;
};

#if defined (__ACE_INLINE__)
#include "Object_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Object_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_OBJECT_T_H */
