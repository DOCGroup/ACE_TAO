// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    PrimitiveDef_i.h
//
// = DESCRIPTION
//    PrimitiveDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PRIMITIVEDEF_I_H
#define TAO_PRIMITIVEDEF_I_H

#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_PrimitiveDef_i : public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_PrimitiveDef_i
  //
  // = DESCRIPTION
  //    Represents any of the OMG IDL primitive types
  //
public:
  TAO_PrimitiveDef_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_PrimitiveDef_i (void);
  // Destructor.

  virtual CORBA::DefinitionKind def_kind ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // May not be called on a primitive kind - raises BAD_INV_ORDER.

  virtual void destroy_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Must implement this here because it's pure virtual in the
  // base class.

  virtual CORBA::TypeCode_ptr type ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PrimitiveKind kind ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PrimitiveKind kind_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PRIMITIVEDEF_I_H */
