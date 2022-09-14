// -*- C++ -*-


//=============================================================================
/**
 *  @file    EnumDef_i.h
 *
 *  EnumDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ENUMDEF_I_H
#define TAO_ENUMDEF_I_H

#include "orbsvcs/IFRService/TypedefDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * Represents an OMG IDL enumeration definition.
 */
class TAO_IFRService_Export TAO_EnumDef_i : public virtual TAO_TypedefDef_i
{
public:
  /// Constructor
  TAO_EnumDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_EnumDef_i ();

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i ();

  virtual CORBA::EnumMemberSeq *members ();

  CORBA::EnumMemberSeq *members_i ();

  virtual void members (const CORBA::EnumMemberSeq &members);

  void members_i (const CORBA::EnumMemberSeq &members);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ENUMDEF_I_H */
