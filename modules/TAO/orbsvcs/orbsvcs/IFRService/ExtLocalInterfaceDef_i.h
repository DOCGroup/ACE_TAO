// -*- C++ -*-


//=============================================================================
/**
 *  @file    ExtLocalInterfaceDef_i.h
 *
 *  $Id$
 *
 *  ExtLocalInterfaceDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_EXTLOCALINTERFACEDEF_I_H
#define TAO_EXTLOCALINTERFACEDEF_I_H

#include "orbsvcs/IFRService/LocalInterfaceDef_i.h"
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

class TAO_IFRService_Export TAO_ExtLocalInterfaceDef_i
  : public virtual TAO_LocalInterfaceDef_i,
    public virtual TAO_InterfaceAttrExtension_i
{
  // = TITLE
  //    TAO_ExtLocaltInterfaceDef_i
  //
  // = DESCRIPTION
  //    Mixes in IDL3 attribute exceptions.
  //
public:
  /// Constructor
  TAO_ExtLocalInterfaceDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ExtLocalInterfaceDef_i (void);

  virtual void destroy (
    );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTLOCALINTERFACEDEF_I_H */

