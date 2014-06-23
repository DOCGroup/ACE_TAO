// -*- C++ -*-

//=============================================================================
/**
 *  @file    InterfaceAttrExtension_i.h
 *
 *  $Id$
 *
 *  InterfaceAttrExtension_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_INTERFACEATTREXTENSION_I_H
#define TAO_INTERFACEATTREXTENSION_I_H

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  /// Constructor
  TAO_InterfaceAttrExtension_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_InterfaceAttrExtension_i (void);

  virtual
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
  describe_ext_interface (void);

  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
  describe_ext_interface_i (void);

  virtual CORBA::ExtAttributeDef_ptr create_ext_attribute (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions);

  CORBA::ExtAttributeDef_ptr create_ext_attribute_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERFACEATTREXTENSION_I_H */
