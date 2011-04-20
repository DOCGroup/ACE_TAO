// -*- C++ -*-


//=============================================================================
/**
 *  @file    HomeDef_i.h
 *
 *  $Id$
 *
 *  HomeDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_HOMEDEF_I_H
#define TAO_HOMEDEF_I_H

#include "orbsvcs/IFRService/ExtInterfaceDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/IFR_ComponentsS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_HomeDef_i
  : public virtual TAO_ExtInterfaceDef_i
{
  // = TITLE
  //    HomTAO_HomeDef_ieDef_i
  //
  // = DESCRIPTION
  //    Represents a home definition, containing factory and finder.
  //
public:
  /// Constructor
  TAO_HomeDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_HomeDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (void);

  /// Remove the repository entry.
  virtual void destroy (void);

  virtual void destroy_i (void);

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (void);

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i (void);

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type (void);

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i (void);

  virtual CORBA::ComponentIR::HomeDef_ptr base_home (void);

  CORBA::ComponentIR::HomeDef_ptr base_home_i (void);

  virtual void base_home (CORBA::ComponentIR::HomeDef_ptr base_home);

  void base_home_i (CORBA::ComponentIR::HomeDef_ptr base_home);

  virtual CORBA::InterfaceDefSeq *supported_interfaces (void);

  CORBA::InterfaceDefSeq *supported_interfaces_i (void);

  virtual void supported_interfaces (
      const CORBA::InterfaceDefSeq &supported_interfaces);

  void supported_interfaces_i (
      const CORBA::InterfaceDefSeq &supported_interfaces);

  virtual CORBA::ComponentIR::ComponentDef_ptr managed_component (void);

  CORBA::ComponentIR::ComponentDef_ptr managed_component_i (void);

  virtual void managed_component (
      CORBA::ComponentIR::ComponentDef_ptr managed_component);

  void managed_component_i (
      CORBA::ComponentIR::ComponentDef_ptr managed_component);

  virtual CORBA::ValueDef_ptr primary_key (void);

  CORBA::ValueDef_ptr primary_key_i (void);

  virtual void primary_key (CORBA::ValueDef_ptr primary_key);

  void primary_key_i (CORBA::ValueDef_ptr primary_key);

  virtual CORBA::ComponentIR::FactoryDef_ptr create_factory (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions);

  CORBA::ComponentIR::FactoryDef_ptr create_factory_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions);

  virtual CORBA::ComponentIR::FinderDef_ptr create_finder (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions);

  CORBA::ComponentIR::FinderDef_ptr create_finder_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions);

private:
  // Common code for create_factory_i and create_finder_i.
  CORBA::Object_ptr create_common (
      CORBA::DefinitionKind created_kind,
      const char *id,
      const char *name,
      const char *version,
      const char *sub_section,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions);

  void fill_op_desc_seq (ACE_Configuration_Section_Key &key,
                         CORBA::OpDescriptionSeq &ods,
                         const char *sub_section);

  void fill_op_desc (ACE_Configuration_Section_Key &key,
                     CORBA::OperationDescription &od,
                     const char *sub_section);

  void fill_param_desc_seq (ACE_Configuration_Section_Key &key,
                            CORBA::ParDescriptionSeq &pds,
                            const char *sub_section);

  void fill_param_desc (ACE_Configuration_Section_Key &key,
                        CORBA::ParameterDescription &pd,
                        const char *sub_section);

  void fill_exc_desc_seq (ACE_Configuration_Section_Key &key,
                          CORBA::ExcDescriptionSeq &eds,
                          const char *sub_section);

  void fill_exc_desc (ACE_Configuration_Section_Key &key,
                      CORBA::ExceptionDescription &ed,
                      const char *sub_section);

  void fill_attr_desc_seq (ACE_Configuration_Section_Key &key,
                           CORBA::ExtAttrDescriptionSeq &eads,
                           const char *sub_section);

  void fill_attr_desc (ACE_Configuration_Section_Key &key,
                       CORBA::ExtAttributeDescription &ead,
                       const char *sub_section);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HOMEDEF_I_H */
