// -*- C++ -*-


//=============================================================================
/**
 *  @file    ValueBoxDef_i.h
 *
 *  $Id$
 *
 *  ValueBoxDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_VALUEBOXDEF_I_H
#define TAO_VALUEBOXDEF_I_H

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
 * Represents an value box definition.
 */
class TAO_IFRService_Export TAO_ValueBoxDef_i : public virtual TAO_TypedefDef_i
{
public:
public:
  /// Constructor
  TAO_ValueBoxDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ValueBoxDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type ();

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i ();

  virtual CORBA::IDLType_ptr original_type_def ();

  CORBA::IDLType_ptr original_type_def_i ();

  virtual void original_type_def (
      CORBA::IDLType_ptr original_type_def);

  void original_type_def_i (
      CORBA::IDLType_ptr original_type_def);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_VALUEBOXDEF_I_H */
