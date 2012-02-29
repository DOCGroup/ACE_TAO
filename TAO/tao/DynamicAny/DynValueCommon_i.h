// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValueCommon_i.h
 *
 *  $Id$
 *
 *  @author Simon Massey <simon dot massey at prismtech dot com>
 */
//=============================================================================


#ifndef TAO_DYNVALUECOMMON_I_H
#define TAO_DYNVALUECOMMON_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynAny_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynValueCommon_i
 *
 * @brief Implementation of the DynValueCommon class.
 */
class TAO_DynamicAny_Export TAO_DynValueCommon_i
  : public virtual DynamicAny::DynValueCommon,
    public virtual TAO_DynAny_i
{
public:
  /// Constructor.
  TAO_DynValueCommon_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynValueCommon_i (void);

  // = LocalObject methods.
  static TAO_DynValueCommon_i *_narrow (CORBA::Object_ptr obj);

  /// Called by DynValue and DynValueBox init() functions.
  void init_common (void);

  // = Functions specific to DynValueCommon.
  virtual CORBA::Boolean is_null (void);

  virtual void set_to_null (void);

  virtual void set_to_value (void) = 0;

private:
  // = Use copy() or assign() instead of these.
  TAO_DynValueCommon_i (const TAO_DynValueCommon_i &src);
  TAO_DynValueCommon_i &operator= (const TAO_DynValueCommon_i &src);

  /// Check if the typecode is acceptable. Needs to be provided by DynValue or DynValueBox
  virtual void check_typecode (CORBA::TypeCode_ptr tc)=0;

protected:
  CORBA::Boolean is_null_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUECOMMON_I_H */
