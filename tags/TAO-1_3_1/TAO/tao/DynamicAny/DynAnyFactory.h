/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO_DynamicAny
//
// = FILENAME
//    DynAnyFactory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =========================================================================

#ifndef TAO_DYNANYFACTORY_H
#define TAO_DYNANYFACTORY_H
#include "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynAnyFactory
  : public virtual DynamicAny::DynAnyFactory,
    public virtual TAO_Local_RefCounted_Object
{
  // = DESCRIPTION
  //   Implements the DynamicAnyFactory interface and provides several
  //   utility functions shared by all the Dynamic Any components.
  //
public:
  TAO_DynAnyFactory (void);
  // Constructor.

  static CORBA::TCKind unalias (CORBA_TypeCode_ptr tc
                                ACE_ENV_ARG_DECL);
  // Obtain the kind of object, after all aliasing has been removed.

  static CORBA_TypeCode_ptr strip_alias (CORBA_TypeCode_ptr tc
                                         ACE_ENV_ARG_DECL);
  // Same as above, but returns type code instead of TCKind. Caller
  // must release the return value.

  static DynamicAny::DynAny_ptr
     make_dyn_any (const CORBA_Any &any
                   ACE_ENV_ARG_DECL);
  // Create the correct type of DynAny object for <any>.

  static DynamicAny::DynAny_ptr
     make_dyn_any (CORBA::TypeCode_ptr tc
                   ACE_ENV_ARG_DECL);
  // Create the correct type of DynAny object for <tc>

  // = The DynamicAnyFactory methods.
  virtual DynamicAny::DynAny_ptr create_dyn_any (
      const CORBA::Any & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAnyFactory::InconsistentTypeCode
      ));

  virtual DynamicAny::DynAny_ptr create_dyn_any_from_type_code (
      CORBA::TypeCode_ptr type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAnyFactory::InconsistentTypeCode
      ));

private:
  // Not allowed.
  TAO_DynAnyFactory (const TAO_DynAnyFactory &src);
  TAO_DynAnyFactory &operator= (const TAO_DynAnyFactory &src);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_DYNANYFACTORY_H */
