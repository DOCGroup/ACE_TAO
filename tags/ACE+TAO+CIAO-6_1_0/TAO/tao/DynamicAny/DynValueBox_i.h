// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValueBox_i.h
 *
 *  $Id$
 *
 *  @author Simon Massey <simon dot massey at prismtech dot com>
 */
//=============================================================================


#ifndef TAO_DYNVALUEBOX_I_H
#define TAO_DYNVALUEBOX_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynValueCommon_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynValueBox_i
 *
 * @brief Implementation of the DynValueBox class.
 */
class TAO_DynamicAny_Export TAO_DynValueBox_i
  : public virtual DynamicAny::DynValueBox,
    public virtual TAO_DynValueCommon_i
{
public:
  /// Constructor.
  TAO_DynValueBox_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynValueBox_i (void);

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc);

  /// Initialize using an Any.
  void init (const CORBA::Any& any);

  // = LocalObject methods.
  static TAO_DynValueBox_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynValueBox.
  virtual CORBA::Any * get_boxed_value (void);

  virtual void set_boxed_value (const CORBA::Any & boxed);

  virtual DynamicAny::DynAny_ptr get_boxed_value_as_dyn_any (void);

  virtual void set_boxed_value_as_dyn_any (DynamicAny::DynAny_ptr boxed);

  // = DynAny common functions not implemented in class TAO_DynCommon.
  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

  // = DynValueCommon needed to be provided here
  virtual void set_to_value (void);

private:
  /// Check if the typecode is acceptable.
  void check_typecode (CORBA::TypeCode_ptr tc);

  /// Code common to the constructor from an Any arg and the member
  /// function from_any().
  void set_from_any (const CORBA::Any &any);

  // = Use copy() or assign() instead of these.
  TAO_DynValueBox_i (const TAO_DynValueBox_i &src);
  TAO_DynValueBox_i &operator= (const TAO_DynValueBox_i &src);

  /// The boxed component of DynValueBox is another DynAny.
  DynamicAny::DynAny_var boxed_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUEBOX_I_H */
