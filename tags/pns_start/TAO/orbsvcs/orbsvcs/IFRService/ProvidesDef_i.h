/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ProvidesDef_i.h
//
// = DESCRIPTION
//    ProvidesDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PROVIDESDEF_I_H
#define TAO_PROVIDESDEF_I_H

#include "Contained_i.h"
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

class TAO_IFRService_Export TAO_ProvidesDef_i : public virtual TAO_Contained_i
{
  // = TITLE
  //    TAO_ProvidesDef_i
  //
  // = DESCRIPTION
  //    Represents an interface that is provided by a component.
  //
public:
    TAO_ProvidesDef_i (TAO_Repository_i *repoy);
  // Constructor

  virtual ~TAO_ProvidesDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA::Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::InterfaceDef_ptr interface_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::InterfaceDef_ptr interface_type_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void interface_type (
      CORBA::InterfaceDef_ptr interface_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));   

  void interface_type_i (
      CORBA::InterfaceDef_ptr interface_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));   
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_PROVIDESDEF_I_H */


