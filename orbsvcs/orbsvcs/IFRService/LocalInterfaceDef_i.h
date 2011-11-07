// -*- C++ -*-


//=============================================================================
/**
 *  @file    LocalInterfaceDef_i.h
 *
 *  $Id$
 *
 *  LocalInterfaceDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_LOCALINTERFACEDEF_I_H
#define TAO_LOCALINTERFACEDEF_I_H

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
 * @class TAO_LocalInterfaceDef_i
 *
 * @brief TAO_LocalInterfaceDef_i
 *
 * Represents an local interface definition.
 */
class TAO_IFRService_Export TAO_LocalInterfaceDef_i : public virtual TAO_InterfaceDef_i
{
public:
  /// Constructor
  TAO_LocalInterfaceDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_LocalInterfaceDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (void);

  virtual CORBA::Boolean is_a (const char *interface_id);

  CORBA::Boolean is_a_i (const char *interface_id);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_LOCALINTERFACEDEF_I_H */
