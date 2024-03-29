// -*- C++ -*-


//=============================================================================
/**
 *  @file    ConstantDef_i.h
 *
 *  ConstantDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONSTANTDEF_I_H
#define TAO_CONSTANTDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
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
 * @class TAO_ConstantDef_i
 *
 * @brief TAO_ConstantDef_i
 *
 * Represents a named constant.
 */
class TAO_IFRService_Export TAO_ConstantDef_i : public virtual TAO_Contained_i
{
public:
  /// Constructor
    TAO_ConstantDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ConstantDef_i ();

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind ();

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe ();

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i ();

  virtual CORBA::TypeCode_ptr type ();

  CORBA::TypeCode_ptr type_i ();

  virtual CORBA::IDLType_ptr type_def ();

  CORBA::IDLType_ptr type_def_i ();

  virtual void type_def (CORBA::IDLType_ptr type_def);

  void type_def_i (CORBA::IDLType_ptr type_def);

  virtual CORBA::Any *value ();

  CORBA::Any *value_i ();

  virtual void value (const CORBA::Any &value);

  void value_i (const CORBA::Any &value);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONSTANTDEF_I_H */
