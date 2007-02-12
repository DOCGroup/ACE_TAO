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
;
  // Return our definition kind.

  virtual void destroy ()
;
  // Remove the repository entry.

  virtual void destroy_i ()
;

  virtual CORBA::TypeCode_ptr type ()
;
  // From IDLType_i's virtual function.

  virtual CORBA::TypeCode_ptr type_i ()
;
  // From IDLType_i's virtual function.

  virtual CORBA::ULong bound ()
;

  CORBA::ULong bound_i ()
;

  virtual void bound (
      CORBA::ULong bound)
;

  void bound_i (
      CORBA::ULong bound)
;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_WSTRINGDEF_I_H */

