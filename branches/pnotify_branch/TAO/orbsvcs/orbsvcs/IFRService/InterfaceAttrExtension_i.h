/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    InterfaceAttrExtension_i.h
//
// = DESCRIPTION
//    InterfaceAttrExtension_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_INTERFACEATTREXTENSION_I_H
#define TAO_INTERFACEATTREXTENSION_I_H

#include "IRObject_i.h"
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

class TAO_IFRService_Export TAO_InterfaceAttrExtension_i 
  : public virtual TAO_IRObject_i
{
  // = TITLE
  //    TAO_InterfaceAttrExtension_i
  //
  // = DESCRIPTION
  //    IDL3 attribute extension mixin class.
  //
public:
  TAO_InterfaceAttrExtension_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_InterfaceAttrExtension_i (void);
  // Destructor

  virtual 
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
  describe_ext_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
  describe_ext_interface_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::ExtAttributeDef_ptr create_ext_attribute (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
    
  CORBA::ExtAttributeDef_ptr create_ext_attribute_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERFACEATTREXTENSION_I_H */

