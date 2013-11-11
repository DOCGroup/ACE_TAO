// -*- C++ -*-


//=============================================================================
/**
 *  @file    ExtValueDef_i.h
 *
 *  $Id$
 *
 *  ExtValueDef_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_EXTVALUEDEF_I_H
#define TAO_EXTVALUEDEF_I_H

#include "orbsvcs/IFRService/ValueDef_i.h"
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
 * @class TAO_ExtValueDef_i
 *
 * @brief TAO_ExtValueDef_i
 *
 * Adds member and methods to TAO_ValueDef_i to get IDL3
 * attribute exceptions.
 */
class TAO_IFRService_Export TAO_ExtValueDef_i : public virtual TAO_ValueDef_i
{
public:
  /// Constructor
  TAO_ExtValueDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ExtValueDef_i (void);

  virtual CORBA::ExtInitializerSeq *ext_initializers (
    );

  CORBA::ExtInitializerSeq *ext_initializers_i (
    );

  virtual void ext_initializers (
      const CORBA::ExtInitializerSeq &ext_initializers
    );

  void ext_initializers_i (
      const CORBA::ExtInitializerSeq &ext_initializers
    );

  virtual CORBA::ExtValueDef::ExtFullValueDescription *describe_ext_value (
    );

  CORBA::ExtValueDef::ExtFullValueDescription *describe_ext_value_i (
    );

  virtual CORBA::ExtAttributeDef_ptr create_ext_attribute (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions
    );

  CORBA::ExtAttributeDef_ptr create_ext_attribute_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode,
      const CORBA::ExceptionDefSeq &get_exceptions,
      const CORBA::ExceptionDefSeq &set_exceptions
    );

  //// Public because it's also called from
  //// TAO_Container_i::create_ext_value_i.
  void exceptions (ACE_Configuration_Section_Key &key,
                   const char *sub_section,
                   const CORBA::ExcDescriptionSeq &exceptions);

private:
  /// Common code for the set/get operations with attributes and initializers.

  void fill_exceptions (CORBA::ExcDescriptionSeq &exceptions,
                        ACE_Configuration_Section_Key &key,
                        const char *sub_section);

  void exceptions (ACE_Configuration_Section_Key &key,
                   const char *sub_section,
                   const CORBA::ExceptionDefSeq &exceptions);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_EXTVALUEDEF_I_H */
