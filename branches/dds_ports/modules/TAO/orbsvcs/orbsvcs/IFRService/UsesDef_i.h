// -*- C++ -*-


//=============================================================================
/**
 *  @file    UsesDef_i.h
 *
 *  $Id$
 *
 *  UsesDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_USESDEF_I_H
#define TAO_USESDEF_I_H

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
 * @class TAO_UsesDef_i
 *
 * @brief TAO_UsesDef_i
 *
 * Represents an interface that is used by a component.
 */
class TAO_IFRService_Export TAO_UsesDef_i : public virtual TAO_Contained_i
{
public:
  /// Constructor
    TAO_UsesDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_UsesDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i (
    );

  virtual CORBA::InterfaceDef_ptr interface_type (
    );

  virtual void interface_type (
      CORBA::InterfaceDef_ptr interface_type
    );

  void interface_type_i (
      CORBA::InterfaceDef_ptr interface_type
    );

  CORBA::InterfaceDef_ptr interface_type_i (
    );

  virtual CORBA::Boolean is_multiple (
    );

  CORBA::Boolean is_multiple_i (
    );

  virtual void is_multiple (
      CORBA::Boolean is_multiple
    );

  void is_multiple_i (
      CORBA::Boolean is_multiple
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_USESDEF_I_H */
