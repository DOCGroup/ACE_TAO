// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    WstringDef_i.h
//
// = DESCRIPTION
//    WstringDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_WSTRINGDEF_I_H
#define TAO_WSTRINGDEF_I_H

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

class TAO_IFRService_Export TAO_WstringDef_i : public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_WstringDef_i
  //
  // = DESCRIPTION
  //    Represents a bounded wstring (unbounded wstrings
  //    are included in PrimitiveDef).
  //
public:
  TAO_WstringDef_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_WstringDef_i (void);
  // Destructor.

  virtual CORBA::DefinitionKind def_kind ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr type ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's virtual function.

  virtual CORBA::TypeCode_ptr type_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's virtual function.

  virtual CORBA::ULong bound ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong bound_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void bound (
      CORBA::ULong bound)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void bound_i (
      CORBA::ULong bound)

    ACE_THROW_SPEC ((CORBA::SystemException));
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_WSTRINGDEF_I_H */

