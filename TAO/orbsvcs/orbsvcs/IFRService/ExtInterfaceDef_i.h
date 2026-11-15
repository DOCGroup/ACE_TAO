// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExtInterfaceDef_i.h
 *
 *  ExtInterfaceDef_i servant class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_EXTINTERFACEDEF_I_H
#define TAO_EXTINTERFACEDEF_I_H

#include "orbsvcs/IFRService/InterfaceDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/InterfaceAttrExtension_i.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Adds IDL3 attribute exception support to interface definitions.
class TAO_IFRService_Export TAO_ExtInterfaceDef_i
  : public virtual TAO_InterfaceDef_i,
    public virtual TAO_InterfaceAttrExtension_i
{
public:
  explicit TAO_ExtInterfaceDef_i (TAO_Repository_i *repo);

  ~TAO_ExtInterfaceDef_i () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTINTERFACEDEF_I_H */
