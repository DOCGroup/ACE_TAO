// -*- C++ -*-


//=============================================================================
/**
 *  @file    StructDef_i.h
 *
 *  $Id$
 *
 *  StructDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_STRUCTDEF_I_H
#define TAO_STRUCTDEF_I_H

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

class TAO_IFRService_Export TAO_StructDef_i : public virtual TAO_TypedefDef_i,
                                              public virtual TAO_Container_i
{
  // = TITLE
  //    TAO_StructDef_i
  //
  // = DESCRIPTION
  //    Represents an OMG IDL structure definition.
  //
public:
  /// Constructor
  TAO_StructDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_StructDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// Remove the repository entry and its contents.
  virtual void destroy ();

  virtual void destroy_i ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i ();

  virtual CORBA::StructMemberSeq *members ();

  CORBA::StructMemberSeq *members_i ();

  virtual void members (const CORBA::StructMemberSeq &members);

  void members_i (const CORBA::StructMemberSeq &members);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_STRUCTDEF_I_H */
