// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ArrayDef_i.h
//
// = DESCRIPTION
//    ArrayDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ARRAYDEF_I_H
#define TAO_ARRAYDEF_I_H

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

class TAO_IFRService_Export TAO_ArrayDef_i : public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_ArrayDef_i
  //
  // = DESCRIPTION
  //    Represents an IDL array type.
  //
public:
  TAO_ArrayDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_ArrayDef_i (void);
  // Destructor

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

  virtual CORBA::TypeCode_ptr type_i ()
;

  virtual CORBA::ULong length ()
;

  CORBA::ULong length_i ()
;

  virtual void length (
      CORBA::ULong length)
;

  void length_i (
      CORBA::ULong length)
;

  virtual CORBA::TypeCode_ptr element_type ()
;

  CORBA::TypeCode_ptr element_type_i ()
;

  virtual CORBA::IDLType_ptr element_type_def ()
;

  CORBA::IDLType_ptr element_type_def_i ()
;

  virtual void element_type_def (
      CORBA::IDLType_ptr element_type_def)
;

  void element_type_def_i (
      CORBA::IDLType_ptr element_type_def)
;

private:
  void destroy_element_type ()
;
  // Destroys an anonymous non-primitive element type.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ARRAYDEF_I_H */
