// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValueCommon_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
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
  TAO_DynValueCommon_i (void);

  ~TAO_DynValueCommon_i (void);

  virtual CORBA::Boolean is_null (void);

  virtual void set_to_null (void);

  virtual void set_to_value (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUECOMMON_I_H */
