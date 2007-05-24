// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValueBox_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
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
  TAO_DynValueBox_i (void);

  ~TAO_DynValueBox_i (void);

  virtual CORBA::Any * get_boxed_value (void);

  virtual void set_boxed_value (const CORBA::Any & boxed);

  virtual DynamicAny::DynAny_ptr get_boxed_value_as_dyn_any (void);

  virtual void set_boxed_value_as_dyn_any (DynamicAny::DynAny_ptr boxed);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUEBOX_I_H */
