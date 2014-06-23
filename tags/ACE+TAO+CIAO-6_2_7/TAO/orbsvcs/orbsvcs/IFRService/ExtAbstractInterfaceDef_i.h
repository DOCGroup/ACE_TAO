// -*- C++ -*-


//=============================================================================
/**
 *  @file    ExtAbstractInterfaceDef_i.h
 *
 *  $Id$
 *
 *  ExtAbstractInterfaceDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_EXTABSTRACTINTERFACEDEF_I_H
#define TAO_EXTABSTRACTINTERFACEDEF_I_H

#include "orbsvcs/IFRService/AbstractInterfaceDef_i.h"
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

/**
 * Mixes in IDL3 attribute exceptions.
 */
class TAO_IFRService_Export TAO_ExtAbstractInterfaceDef_i
  : public virtual TAO_AbstractInterfaceDef_i,
    public virtual TAO_InterfaceAttrExtension_i
{
public:
  /// Constructor
  TAO_ExtAbstractInterfaceDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ExtAbstractInterfaceDef_i (void);

  virtual void destroy (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTABSTRACTINTERFACEDEF_I_H */
