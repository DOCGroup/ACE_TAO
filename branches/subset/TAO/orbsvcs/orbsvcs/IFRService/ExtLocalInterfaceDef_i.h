/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ExtLocalInterfaceDef_i.h
//
// = DESCRIPTION
//    ExtLocalInterfaceDef_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_EXTLOCALINTERFACEDEF_I_H
#define TAO_EXTLOCALINTERFACEDEF_I_H

#include "LocalInterfaceDef_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "InterfaceAttrExtension_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  TAO_ExtLocalInterfaceDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_ExtLocalInterfaceDef_i (void);
  // Destructor

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTLOCALINTERFACEDEF_I_H */


