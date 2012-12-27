// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynUnion_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNUNION_I_H
#define TAO_DYNUNION_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicAny/DynCommon.h"
#include "tao/LocalObject.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynUnion_i
 *
 * Implementation of Dynamic Any type for Unions.
 */
class TAO_DynamicAny_Export TAO_DynUnion_i
  : public virtual DynamicAny::DynUnion,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_DynUnion_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynUnion_i (void);

  /// Constructor taking an Any argument.
  void init (const CORBA::Any& any);

  /// Constructor taking a typecode argument.
  void init (CORBA::TypeCode_ptr tc);

  // = LocalObject methods.
  static TAO_DynUnion_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynUnion.

  virtual DynamicAny::DynAny_ptr get_discriminator (void);

  virtual void set_discriminator (DynamicAny::DynAny_ptr d);

  virtual void set_to_default_member (void);

  virtual void set_to_no_active_member (void);

  virtual CORBA::Boolean has_no_active_member (void);

  virtual CORBA::TCKind discriminator_kind (void);

  virtual DynamicAny::DynAny_ptr member (void);

  virtual char * member_name (void);

  virtual CORBA::TCKind member_kind (void);

  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

private:
  /// Code common to the constructor from an Any arg and the member
  /// function from_any().
  void set_from_any (const CORBA::Any &any);

  /// Called by both versions of init().
  void init_common (void);

  /// Iterative check for label value match.
  CORBA::Boolean label_match (const CORBA::Any &my_any,
                              const CORBA::Any &other_any);

  /// Use copy() or assign() instead of these.
  TAO_DynUnion_i (const TAO_DynUnion_i &src);
  TAO_DynUnion_i &operator= (const TAO_DynUnion_i &src);

private:
  /// Just two components.
  DynamicAny::DynAny_var member_;
  DynamicAny::DynAny_var discriminator_;

  /// The active member's slot in the union type code.
  CORBA::ULong member_slot_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNUNION_I_H */
