/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    AbstractInterfaceDef_i.h
//
// = DESCRIPTION
//    AbstractInterfaceDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ABSTRACTINTERFACEDEF_I_H
#define TAO_ABSTRACTINTERFACEDEF_I_H

#include "InterfaceDef_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_AbstractInterfaceDef_i 
	: public virtual TAO_InterfaceDef_i
{
  // = TITLE
  //    TAO_AbstractInterfaceDef_i
  //
  // = DESCRIPTION
  //    Represents an abstract interface definition.
  //
public:
  TAO_AbstractInterfaceDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_AbstractInterfaceDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA::Boolean is_a (
      const char *interface_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean is_a_i (
      const char *interface_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ABSTRACTINTERFACEDEF_I_H */


