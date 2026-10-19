// -*- C++ -*-


//=============================================================================
/**
 *  @file    AbstractInterfaceDef_i.h
 *
 *  AbstractInterfaceDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ABSTRACTINTERFACEDEF_I_H
#define TAO_ABSTRACTINTERFACEDEF_I_H

#include "orbsvcs/IFRService/InterfaceDef_i.h"
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
 * Represents an abstract interface definition.
 */
class TAO_IFRService_Export TAO_AbstractInterfaceDef_i
  : public virtual TAO_InterfaceDef_i
{
public:
  /// Constructor
  TAO_AbstractInterfaceDef_i (TAO_Repository_i *repo);

  /// Destructor
  ~TAO_AbstractInterfaceDef_i () override;

  /// Return our definition kind.
  CORBA::DefinitionKind def_kind (
    ) override;

  CORBA::Boolean is_a (
      const char *interface_id
    ) override;

  CORBA::Boolean is_a_i (
      const char *interface_id
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ABSTRACTINTERFACEDEF_I_H */


