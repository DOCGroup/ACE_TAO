// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    UnionDef_i.h
//
// = DESCRIPTION
//    UnionDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UNIONDEF_I_H
#define TAO_UNIONDEF_I_H

#include "orbsvcs/IFRService/TypedefDef_i.h"
#include "orbsvcs/IFRService/Container_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_UnionDef_i : public virtual TAO_TypedefDef_i,
                                             public virtual TAO_Container_i
{
  // = TITLE
  //    TAO_UnionDef_i
  //
  // = DESCRIPTION
  //    Represents an OMG IDL union definition.
  //
public:
  TAO_UnionDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_UnionDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind ()
;
  // Return our definition kind.

  virtual void destroy ()
;
  // Remove the repository entry and its contents.

  virtual void destroy_i ()
;

  virtual CORBA::TypeCode_ptr type ()
;
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i ()
;
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr discriminator_type ()
;

  CORBA::TypeCode_ptr discriminator_type_i ()
;

  virtual CORBA::IDLType_ptr discriminator_type_def ()
;

  CORBA::IDLType_ptr discriminator_type_def_i ()
;

  virtual void discriminator_type_def (
      CORBA::IDLType_ptr discriminator_type_def)
;

  void discriminator_type_def_i (
      CORBA::IDLType_ptr discriminator_type_def)
;

  virtual CORBA::UnionMemberSeq *members ()
;

  CORBA::UnionMemberSeq *members_i ()
;

  virtual void members (
      const CORBA::UnionMemberSeq &members)
;

  void members_i (
      const CORBA::UnionMemberSeq &members)
;

private:
  void fetch_label (
      const ACE_Configuration_Section_Key member_key,
      CORBA::UnionMember &member);
  // Used by members() to get each member's label value.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_UNIONDEF_I_H */
