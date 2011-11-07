// -*- C++ -*-


//=============================================================================
/**
 *  @file    AttributeDef_i.h
 *
 *  $Id$
 *
 *  AttributeDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ATTRIBUTEDEF_I_H
#define TAO_ATTRIBUTEDEF_I_H

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
 * @class TAO_AttributeDef_i
 *
 * @brief TAO_AttributeDef_i
 *
 * Represents the information that describes an attribute
 * of an interface.
 */
class TAO_IFRService_Export TAO_AttributeDef_i : public virtual TAO_Contained_i
{
public:
  /// Constructor
  TAO_AttributeDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_AttributeDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i (
    );

  virtual CORBA::TypeCode_ptr type (
    );

  CORBA::TypeCode_ptr type_i (
    );

  virtual CORBA::IDLType_ptr type_def (
    );

  CORBA::IDLType_ptr type_def_i (
    );

  virtual void type_def (
      CORBA::IDLType_ptr type_def
    );

  void type_def_i (
      CORBA::IDLType_ptr type_def
    );

  virtual CORBA::AttributeMode mode (
    );

  CORBA::AttributeMode mode_i (
    );

  virtual void mode (
      CORBA::AttributeMode mode
    );

  void mode_i (
      CORBA::AttributeMode mode
    );

  /// Common code for InterfaceDef to call in
  /// making its own description.
  void make_description (
      CORBA::AttributeDescription &ad
    );

  // These two are called by Contained::move().
  CORBA::ExceptionDefSeq *get_exceptions (
    );

  CORBA::ExceptionDefSeq *put_exceptions (
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ATTRIBUTEDEF_I_H */
