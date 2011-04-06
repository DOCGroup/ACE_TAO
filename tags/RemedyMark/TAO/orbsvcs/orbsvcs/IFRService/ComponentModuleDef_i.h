// -*- C++ -*-


//=============================================================================
/**
 *  @file    ComponentModuleDef_i.h
 *
 *  $Id$
 *
 *  ComponentModuleDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_COMPONENTMODULEDEF_I_H
#define TAO_COMPONENTMODULEDEF_I_H

#include "orbsvcs/IFRService/ComponentContainer_i.h"
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
 * Represents a module definition that may contain Component-related
 * types.
 */
class TAO_IFRService_Export TAO_ComponentModuleDef_i
  : public virtual TAO_ComponentContainer_i
{
public:
  /// Constructor
  TAO_ComponentModuleDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ComponentModuleDef_i (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_MODULEDEF_I_H */

