/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ProvidesDef_i.h
 *
 *  ProvidesDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PROVIDESDEF_I_H
#define TAO_PROVIDESDEF_I_H

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
 * @class TAO_ProvidesDef_i
 *
 * @brief TAO_ProvidesDef_i
 *
 * Represents an interface that is provided by a component.
 */
class TAO_IFRService_Export TAO_ProvidesDef_i : public virtual TAO_Contained_i
{
public:
  /// Constructor
    TAO_ProvidesDef_i (TAO_Repository_i *repoy);

  /// Destructor
  ~TAO_ProvidesDef_i (void) override;

  /// Return our definition kind.
  CORBA::DefinitionKind def_kind () override;

  /// From Contained_i's pure virtual function.
  CORBA::Contained::Description *describe () override;

  /// From Contained_i's pure virtual function.
  CORBA::Contained::Description *describe_i () override;

  virtual CORBA::InterfaceDef_ptr interface_type ();

  CORBA::InterfaceDef_ptr interface_type_i ();

  virtual void interface_type (CORBA::InterfaceDef_ptr interface_type);

  void interface_type_i (CORBA::InterfaceDef_ptr interface_type);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PROVIDESDEF_I_H */


