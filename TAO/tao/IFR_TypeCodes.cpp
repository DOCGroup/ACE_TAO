// -*- C++ -*-
// $Id$

#include "tao/corbafwd.h"

ACE_RCSID(tao, IFR_TypeCodes, "$Id$")

// These Interface Repository typecodes and constants are in
// the CORBA namespace. The typecodes are set to 0 here. If the 
// IFR_Client library is built and linked, the library initializer 
// will set them to their correct values. The constants are set to
// the correct values in this file, since it doesn't take up any
// more space to do that.

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Identifier, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ScopedName, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_RepositoryId, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_IRObject, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Visibility, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (const CORBA::Visibility)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::Visibility, PRIVATE_MEMBER, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Visibility)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const CORBA::Visibility, PUBLIC_MEMBER, 1)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ValueModifier, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (const ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const ValueModifier, VM_NONE, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const ValueModifier, VM_CUSTOM, 1)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const ValueModifier, VM_ABSTRACT, 2)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const ValueModifier)
TAO_NAMESPACE_BEGIN (CORBA)
TAO_NAMESPACE_DEFINE (const ValueModifier, VM_TRUNCATABLE, 3)
TAO_NAMESPACE_END
