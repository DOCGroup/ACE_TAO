/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ComponentModuleDef_i.h
//
// = DESCRIPTION
//    ComponentModuleDef_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_COMPONENTMODULEDEF_I_H
#define TAO_COMPONENTMODULEDEF_I_H

#include "ComponentContainer_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ModuleDef_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_ComponentModuleDef_i 
  : public virtual TAO_ComponentContainer_i,
    public virtual TAO_ModuleDef_i
{
  // = TITLE
  //    TAO_ComponentModuleDef_i
  //
  // = DESCRIPTION
  //    Represents a module definition that may contain Component-related
  //    types.
  //
public:
  TAO_ComponentModuleDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_ComponentModuleDef_i (void);
  // Destructor
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_MODULEDEF_I_H */

