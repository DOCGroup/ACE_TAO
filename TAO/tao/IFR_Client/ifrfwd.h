// -*- C++ -*-

//=============================================================================
/**
 *  @file    ifrfwd.h
 *
 *  $Id$
 *
 *  Forward declare the basic types used in the IR
 *  implementation. This file should be included in
 *  ORB file headers that use one or more IR namespace
 *  types. The corresponding source files will include
 *  InterfaceC.h.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_IFRFWD_H
#define TAO_IFRFWD_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ifr_client_export.h"
#include "tao/corbafwd.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_IFR_Client_Export

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// Forward declarations of some data types are needed.
class IR_Contained;
class IR_Contained_var;
class IR_Contained_out;
typedef IR_Contained *IR_Contained_ptr;

class IR_ContainedSeq;
class IR_ContainedSeq_var;
class IR_ContainedSeq_out;

class IR_Repository;
class IR_Repository_var;
class IR_Repository_out;
typedef IR_Repository *IR_Repository_ptr;

class IR_ComponentRepository;
class IR_ComponentRepository_var;
class IR_ComponentRepository_out;
typedef IR_ComponentRepository *IR_ComponentRepository_ptr;

class IR_Container;
class IR_Container_var;
class IR_Container_out;
typedef IR_Container *IR_Container_ptr;

class IR_ModuleDef;
class IR_ModuleDef_var;
class IR_ModuleDef_out;
typedef IR_ModuleDef *IR_ModuleDef_ptr;

class IR_ConstantDef;
class IR_ConstantDef_var;
class IR_ConstantDef_out;
typedef IR_ConstantDef *IR_ConstantDef_ptr;

class IR_IDLType;
class IR_IDLType_var;
class IR_IDLType_out;
typedef IR_IDLType *IR_IDLType_ptr;

class IR_StructDef;
class IR_StructDef_var;
class IR_StructDef_out;
typedef IR_StructDef *IR_StructDef_ptr;

class IR_UnionDef;
class IR_UnionDef_var;
class IR_UnionDef_out;
typedef IR_UnionDef *IR_UnionDef_ptr;

class IR_EnumDef;
class IR_EnumDef_var;
class IR_EnumDef_out;
typedef IR_EnumDef *IR_EnumDef_ptr;

class IR_AliasDef;
class IR_AliasDef_var;
class IR_AliasDef_out;
typedef IR_AliasDef *IR_AliasDef_ptr;

class IR_InterfaceDef;
class IR_InterfaceDef_var;
class IR_InterfaceDef_out;
typedef IR_InterfaceDef *IR_InterfaceDef_ptr;

class IR_ExceptionDef;
class IR_ExceptionDef_var;
class IR_ExceptionDef_out;
typedef IR_ExceptionDef *IR_ExceptionDef_ptr;

class IR_NativeDef;
class IR_NativeDef_var;
class IR_NativeDef_out;
typedef IR_NativeDef *IR_NativeDef_ptr;

class IR_InterfaceDefSeq;
class IR_InterfaceDefSeq_var;
class IR_InterfaceDefSeq_out;

class IR_ValueDef;
class IR_ValueDef_var;
class IR_ValueDef_out;
typedef IR_ValueDef *IR_ValueDef_ptr;

class IR_ValueDefSeq;
class IR_ValueDefSeq_var;
class IR_ValueDefSeq_out;

class IR_ValueBoxDef;
class IR_ValueBoxDef_var;
class IR_ValueBoxDef_out;
typedef IR_ValueBoxDef *IR_ValueBoxDef_ptr;

class IR_ComponentDef;
class IR_ComponentDef_var;
class IR_ComponentDef_out;
typedef IR_ComponentDef *IR_ComponentDef_ptr;

class IR_ComponentDefSeq;
class IR_ComponentDefSeq_var;
class IR_ComponentDefSeq_out;

class IR_ProvidesDef;
class IR_ProvidesDef_var;
class IR_ProvidesDef_out;
typedef IR_ProvidesDef *IR_ProvidesDef_ptr;

class IR_ProvidesDefSeq;
class IR_ProvidesDefSeq_var;
class IR_ProvidesDefSeq_out;

class IR_UsesDef;
class IR_UsesDef_var;
class IR_UsesDef_out;
typedef IR_UsesDef *IR_UsesDef_ptr;

class IR_UsesDefSeq;
class IR_UsesDefSeq_var;
class IR_UsesDefSeq_out;

class IR_HomeDef;
class IR_HomeDef_var;
class IR_HomeDef_out;
typedef IR_HomeDef *IR_HomeDef_ptr;

class IR_HomeDefSeq;
class IR_HomeDefSeq_var;
class IR_HomeDefSeq_out;

class IR_EventDef;
class IR_EventDef_var;
class IR_EventDef_out;
typedef IR_EventDef *IR_EventDef_ptr;

class IR_EmitsDef;
class IR_EmitsDef_var;
class IR_EmitsDef_out;
typedef IR_EmitsDef *IR_EmitsDef_ptr;

class IR_EmitsDefSeq;
class IR_EmitsDefSeq_var;
class IR_EmitsDefSeq_out;

class IR_PublishesDef;
class IR_PublishesDef_var;
class IR_PublishesDef_out;
typedef IR_PublishesDef *IR_PublishesDef_ptr;

class IR_PublishesDefSeq;
class IR_PublishesDefSeq_var;
class IR_PublishesDefSeq_out;

class IR_ConsumesDef;
class IR_ConsumesDef_var;
class IR_ConsumesDef_out;
typedef IR_ConsumesDef *IR_ConsumesDef_ptr;

class IR_ConsumesDefSeq;
class IR_ConsumesDefSeq_var;
class IR_ConsumesDefSeq_out;

class IR_FactoryDef;
class IR_FactoryDef_var;
class IR_FactoryDef_out;
typedef IR_FactoryDef *IR_FactoryDef_ptr;

class IR_FactoryDefSeq;
class IR_FactoryDefSeq_var;
class IR_FactoryDefSeq_out;

class IR_FinderDef;
class IR_FinderDef_var;
class IR_FinderDef_out;
typedef IR_FinderDef *IR_FinderDef_ptr;

class IR_FinderDefSeq;
class IR_FinderDefSeq_var;
class IR_FinderDefSeq_out;

class IR_PrimaryKeyDef;
class IR_PrimaryKeyDef_var;
class IR_PrimaryKeyDef_out;
typedef IR_PrimaryKeyDef *IR_PrimaryKeyDef_ptr;

struct IR_StructMember;
class IR_StructMember_var;
class IR_StructMember_out;

class IR_StructMemberSeq;
class IR_StructMemberSeq_var;
class IR_StructMemberSeq_out;

struct IR_Initializer;
class IR_Initializer_var;
class IR_Initializer_out;

class IR_InitializerSeq;
class IR_InitializerSeq_var;
class IR_InitializerSeq_out;

struct IR_UnionMember;
class IR_UnionMember_var;
class IR_UnionMember_out;

class IR_UnionMemberSeq;
class IR_UnionMemberSeq_var;
class IR_UnionMemberSeq_out;

class IR_EnumMemberSeq;
class IR_EnumMemberSeq_var;
class IR_EnumMemberSeq_out;

struct IR_ModuleDescription;
class IR_ModuleDescription_var;
class IR_ModuleDescription_out;

struct IR_ConstantDescription;
class IR_ConstantDescription_var;
class IR_ConstantDescription_out;

class IR_TypedefDef;
class IR_TypedefDef_var;
class IR_TypedefDef_out;
typedef IR_TypedefDef *IR_TypedefDef_ptr;

struct IR_TypeDescription;
class IR_TypeDescription_var;
class IR_TypeDescription_out;

class IR_PrimitiveDef;
class IR_PrimitiveDef_var;
class IR_PrimitiveDef_out;
typedef IR_PrimitiveDef *IR_PrimitiveDef_ptr;

class IR_StringDef;
class IR_StringDef_var;
class IR_StringDef_out;
typedef IR_StringDef *IR_StringDef_ptr;

class IR_WstringDef;
class IR_WstringDef_var;
class IR_WstringDef_out;
typedef IR_WstringDef *IR_WstringDef_ptr;

class IR_FixedDef;
class IR_FixedDef_var;
class IR_FixedDef_out;
typedef IR_FixedDef *IR_FixedDef_ptr;

class IR_SequenceDef;
class IR_SequenceDef_var;
class IR_SequenceDef_out;
typedef IR_SequenceDef *IR_SequenceDef_ptr;

class IR_ArrayDef;
class IR_ArrayDef_var;
class IR_ArrayDef_out;
typedef IR_ArrayDef *IR_ArrayDef_ptr;

struct IR_ExceptionDescription;
class IR_ExceptionDescription_var;
class IR_ExceptionDescription_out;

class IR_ExceptionDefSeq;
class IR_ExceptionDefSeq_var;
class IR_ExceptionDefSeq_out;

class IR_ExcDescriptionSeq;
class IR_ExcDescriptionSeq_var;
class IR_ExcDescriptionSeq_out;

class IR_AttributeDef;
class IR_AttributeDef_var;
class IR_AttributeDef_out;
typedef IR_AttributeDef *IR_AttributeDef_ptr;

struct IR_AttributeDescription;
class IR_AttributeDescription_var;
class IR_AttributeDescription_out;

struct IR_ParameterDescription;
class IR_ParameterDescription_var;
class IR_ParameterDescription_out;

class IR_ParDescriptionSeq;
class IR_ParDescriptionSeq_var;
class IR_ParDescriptionSeq_out;

class IR_ContextIdSeq;
class IR_ContextIdSeq_var;
class IR_ContextIdSeq_out;

class IR_OperationDef;
class IR_OperationDef_var;
class IR_OperationDef_out;
typedef IR_OperationDef *IR_OperationDef_ptr;

struct IR_OperationDescription;
class IR_OperationDescription_var;
class IR_OperationDescription_out;

struct IR_ParameterDescription;
class IR_ParameterDescription_var;
class IR_ParameterDescription_out;

class IR_RepositoryIdSeq;
class IR_RepositoryIdSeq_var;
class IR_RepositoryIdSeq_out;

class IR_OpDescriptionSeq;
class IR_OpDescriptionSeq_var;
class IR_OpDescriptionSeq_out;

class IR_AttrDescriptionSeq;
class IR_AttrDescriptionSeq_var;
class IR_AttrDescriptionSeq_out;

struct IR_InterfaceDescription;
class IR_InterfaceDescription_var;
class IR_InterfaceDescription_out;

struct IR_ValueMember;
class IR_ValueMember_var;
class IR_ValueMember_out;

class IR_ValueMemberSeq;
class IR_ValueMemberSeq_var;
class IR_ValueMemberSeq_out;

class IR_ValueMemberDef;
class IR_ValueMemberDef_var;
class IR_ValueMemberDef_out;
typedef IR_ValueMemberDef *IR_ValueMemberDef_ptr;

struct IR_ValueDescription;
class IR_ValueDescription_var;
class IR_ValueDescription_out;

struct IR_ProvidesDescription;
class IR_ProvidesDescription_var;
class IR_ProvidesDescription_out;

struct IR_UsesDescription;
class IR_UsesDescription_var;
class IR_UsesDescription_out;

class IR_ProvidesDescSeq;
class IR_ProvidesDescSeq_var;
class IR_ProvidesDescSeq_out;

class IR_UsesDescSeq;
class IR_UsesDescSeq_var;
class IR_UsesDescSeq_out;

struct IR_EventDescription;
class IR_EventDescription_var;
class IR_EventDescription_out;

struct IR_ValueDescription;
class IR_ValueDescription_var;
class IR_ValueDescription_out;

struct IR_ComponentDescription;
class IR_ComponentDescription_var;
class IR_ComponentDescription_out;

struct IR_PrimaryKeyDescription;
class IR_PrimaryKeyDescription_var;
class IR_PrimaryKeyDescription_out;

struct IR_HomeDescription;
class IR_HomeDescription_var;
class IR_HomeDescription_out;

TAO_NAMESPACE  IR
{
  // = TITLE
  //    Provides the IR namespace.
  //
  // = DESCRIPTION
  //    This class allows the use of IR::Foo, as well as IR_Foo
  //    for all standard types Foo.

  typedef char * Identifier;
  typedef CORBA::String_var Identifier_var;
  typedef CORBA::String_out Identifier_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Identifier;

  typedef char * ScopedName;
  typedef CORBA::String_var ScopedName_var;
  typedef CORBA::String_out ScopedName_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ScopedName;

  typedef char * RepositoryId;
  typedef CORBA::String_var RepositoryId_var;
  typedef CORBA::String_out RepositoryId_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_RepositoryId;

  typedef char * VersionSpec;
  typedef CORBA::String_var VersionSpec_var;
  typedef CORBA::String_out VersionSpec_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_VersionSpec;

  typedef char * ContextIdentifier;
  typedef CORBA::String_var ContextIdentifier_var;
  typedef CORBA::String_out ContextIdentifier_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ContextIdentifier;

  typedef IR_DefinitionKind DefinitionKind;
  typedef IR_DefinitionKind &DefinitionKind_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_DefinitionKind;


  typedef IR_Contained Contained;
  typedef IR_Contained *Contained_ptr;
  typedef IR_Contained_var Contained_var;
  typedef IR_Contained_out Contained_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Contained;

  typedef IR_ContainedSeq ContainedSeq;
  typedef IR_ContainedSeq_var ContainedSeq_var;
  typedef IR_ContainedSeq_out ContainedSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ContainedSeq;

  typedef IR_Repository Repository;
  typedef IR_Repository *Repository_ptr;
  typedef IR_Repository_var Repository_var;
  typedef IR_Repository_out Repository_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Repository;

  typedef IR_ComponentRepository ComponentRepository;
  typedef IR_ComponentRepository *ComponentRepository_ptr;
  typedef IR_ComponentRepository_var ComponentRepository_var;
  typedef IR_ComponentRepository_out ComponentRepository_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ComponentRepository;

  typedef IR_Container Container;
  typedef IR_Container *Container_ptr;
  typedef IR_Container_var Container_var;
  typedef IR_Container_out Container_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Container;

  typedef IR_ModuleDef ModuleDef;
  typedef IR_ModuleDef *ModuleDef_ptr;
  typedef IR_ModuleDef_var ModuleDef_var;
  typedef IR_ModuleDef_out ModuleDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ModuleDef;

  typedef IR_ConstantDef ConstantDef;
  typedef IR_ConstantDef *ConstantDef_ptr;
  typedef IR_ConstantDef_var ConstantDef_var;
  typedef IR_ConstantDef_out ConstantDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConstantDef;

  typedef IR_IDLType IDLType;
  typedef IR_IDLType *IDLType_ptr;
  typedef IR_IDLType_var IDLType_var;
  typedef IR_IDLType_out IDLType_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_IDLType;

  typedef IR_StructDef StructDef;
  typedef IR_StructDef *StructDef_ptr;
  typedef IR_StructDef_var StructDef_var;
  typedef IR_StructDef_out StructDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_StructDef;

  typedef IR_UnionDef UnionDef;
  typedef IR_UnionDef *UnionDef_ptr;
  typedef IR_UnionDef_var UnionDef_var;
  typedef IR_UnionDef_out UnionDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UnionDef;

  typedef IR_EnumDef EnumDef;
  typedef IR_EnumDef *EnumDef_ptr;
  typedef IR_EnumDef_var EnumDef_var;
  typedef IR_EnumDef_out EnumDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EnumDef;

  typedef IR_AliasDef AliasDef;
  typedef IR_AliasDef *AliasDef_ptr;
  typedef IR_AliasDef_var AliasDef_var;
  typedef IR_AliasDef_out AliasDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_AliasDef;

  typedef IR_InterfaceDef InterfaceDef;
  typedef IR_InterfaceDef *InterfaceDef_ptr;
  typedef IR_InterfaceDef_var InterfaceDef_var;
  typedef IR_InterfaceDef_out InterfaceDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InterfaceDef;

  typedef IR_ExceptionDef ExceptionDef;
  typedef IR_ExceptionDef *ExceptionDef_ptr;
  typedef IR_ExceptionDef_var ExceptionDef_var;
  typedef IR_ExceptionDef_out ExceptionDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ExceptionDef;

  typedef IR_NativeDef NativeDef;
  typedef IR_NativeDef *NativeDef_ptr;
  typedef IR_NativeDef_var NativeDef_var;
  typedef IR_NativeDef_out NativeDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_NativeDef;

  typedef IR_InterfaceDefSeq InterfaceDefSeq;
  typedef IR_InterfaceDefSeq_var InterfaceDefSeq_var;
  typedef IR_InterfaceDefSeq_out InterfaceDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InterfaceDefSeq;

  typedef IR_ValueDef ValueDef;
  typedef IR_ValueDef *ValueDef_ptr;
  typedef IR_ValueDef_var ValueDef_var;
  typedef IR_ValueDef_out ValueDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueDef;

  typedef IR_ValueDefSeq ValueDefSeq;
  typedef IR_ValueDefSeq_var ValueDefSeq_var;
  typedef IR_ValueDefSeq_out ValueDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueDefSeq;

  typedef IR_ValueBoxDef ValueBoxDef;
  typedef IR_ValueBoxDef *ValueBoxDef_ptr;
  typedef IR_ValueBoxDef_var ValueBoxDef_var;
  typedef IR_ValueBoxDef_out ValueBoxDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueBoxDef;

  typedef IR_ComponentDef ComponentDef;
  typedef IR_ComponentDef *ComponentDef_ptr;
  typedef IR_ComponentDef_var ComponentDef_var;
  typedef IR_ComponentDef_out ComponentDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ComponentDef;

  typedef IR_ComponentDefSeq ComponentDefSeq;
  typedef IR_ComponentDefSeq_var ComponentDefSeq_var;
  typedef IR_ComponentDefSeq_out ComponentDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ComponentDefSeq;

  typedef IR_ProvidesDef ProvidesDef;
  typedef IR_ProvidesDef *ProvidesDef_ptr;
  typedef IR_ProvidesDef_var ProvidesDef_var;
  typedef IR_ProvidesDef_out ProvidesDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ProvidesDef;

  typedef IR_ProvidesDefSeq ProvidesDefSeq;
  typedef IR_ProvidesDefSeq_var ProvidesDefSeq_var;
  typedef IR_ProvidesDefSeq_out ProvidesDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ProvidesDefSeq;

  typedef IR_UsesDef UsesDef;
  typedef IR_UsesDef *UsesDef_ptr;
  typedef IR_UsesDef_var UsesDef_var;
  typedef IR_UsesDef_out UsesDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UsesDef;

  typedef IR_UsesDescription UsesDescription;
  typedef IR_UsesDescription_var UsesDescription_var;
  typedef IR_UsesDescription_out UsesDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_IR_UsesDescription;


  typedef IR_UsesDefSeq UsesDefSeq;
  typedef IR_UsesDefSeq_var UsesDefSeq_var;
  typedef IR_UsesDefSeq_out UsesDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UsesDefSeq;

  typedef IR_HomeDef HomeDef;
  typedef IR_HomeDef *HomeDef_ptr;
  typedef IR_HomeDef_var HomeDef_var;
  typedef IR_HomeDef_out HomeDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_HomeDef;

  typedef IR_HomeDefSeq HomeDefSeq;
  typedef IR_HomeDefSeq_var HomeDefSeq_var;
  typedef IR_HomeDefSeq_out HomeDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_HomeDefSeq;

  typedef IR_EventDef EventDef;
  typedef IR_EventDef *EventDef_ptr;
  typedef IR_EventDef_var EventDef_var;
  typedef IR_EventDef_out EventDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EventDef;

  typedef IR_EmitsDef EmitsDef;
  typedef IR_EmitsDef *EmitsDef_ptr;
  typedef IR_EmitsDef_var EmitsDef_var;
  typedef IR_EmitsDef_out EmitsDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EmitsDef;

  typedef IR_EmitsDefSeq EmitsDefSeq;
  typedef IR_EmitsDefSeq_var EmitsDefSeq_var;
  typedef IR_EmitsDefSeq_out EmitsDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EmitsDefSeq;

  typedef IR_PublishesDef PublishesDef;
  typedef IR_PublishesDef *PublishesDef_ptr;
  typedef IR_PublishesDef_var PublishesDef_var;
  typedef IR_PublishesDef_out PublishesDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PublishesDef;

  typedef IR_PublishesDefSeq PublishesDefSeq;
  typedef IR_PublishesDefSeq_var PublishesDefSeq_var;
  typedef IR_PublishesDefSeq_out PublishesDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PublishesDefSeq;

  typedef IR_ConsumesDef ConsumesDef;
  typedef IR_ConsumesDef *ConsumesDef_ptr;
  typedef IR_ConsumesDef_var ConsumesDef_var;
  typedef IR_ConsumesDef_out ConsumesDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConsumesDef;

  typedef IR_ConsumesDefSeq ConsumesDefSeq;
  typedef IR_ConsumesDefSeq_var ConsumesDefSeq_var;
  typedef IR_ConsumesDefSeq_out ConsumesDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConsumesDefSeq;

  typedef IR_FactoryDef FactoryDef;
  typedef IR_FactoryDef *FactoryDef_ptr;
  typedef IR_FactoryDef_var FactoryDef_var;
  typedef IR_FactoryDef_out FactoryDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FactoryDef;

  typedef IR_FactoryDefSeq FactoryDefSeq;
  typedef IR_FactoryDefSeq_var FactoryDefSeq_var;
  typedef IR_FactoryDefSeq_out FactoryDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FactoryDefSeq;

  typedef IR_FinderDef FinderDef;
  typedef IR_FinderDef *FinderDef_ptr;
  typedef IR_FinderDef_var FinderDef_var;
  typedef IR_FinderDef_out FinderDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FinderDef;

  typedef IR_FinderDefSeq FinderDefSeq;
  typedef IR_FinderDefSeq_var FinderDefSeq_var;
  typedef IR_FinderDefSeq_out FinderDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FinderDefSeq;

  typedef IR_PrimaryKeyDef PrimaryKeyDef;
  typedef IR_PrimaryKeyDef *PrimaryKeyDef_ptr;
  typedef IR_PrimaryKeyDef_var PrimaryKeyDef_var;
  typedef IR_PrimaryKeyDef_out PrimaryKeyDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PrimaryKeyDef;

  typedef IR_StructMember StructMember;
  typedef IR_StructMember_var StructMember_var;
  typedef IR_StructMember_out StructMember_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_StructMember;

  typedef IR_StructMemberSeq StructMemberSeq;
  typedef IR_StructMemberSeq_var StructMemberSeq_var;
  typedef IR_StructMemberSeq_out StructMemberSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_StructMemberSeq;

  typedef IR_Initializer Initializer;
  typedef IR_Initializer_var Initializer_var;
  typedef IR_Initializer_out Initializer_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_Initializer;

  typedef IR_InitializerSeq InitializerSeq;
  typedef IR_InitializerSeq_var InitializerSeq_var;
  typedef IR_InitializerSeq_out InitializerSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InitializerSeq;

  typedef IR_UnionMember UnionMember;
  typedef IR_UnionMember_var UnionMember_var;
  typedef IR_UnionMember_out UnionMember_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UnionMember;

  typedef IR_UnionMemberSeq UnionMemberSeq;
  typedef IR_UnionMemberSeq_var UnionMemberSeq_var;
  typedef IR_UnionMemberSeq_out UnionMemberSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UnionMemberSeq;

  typedef IR_EnumMemberSeq EnumMemberSeq;
  typedef IR_EnumMemberSeq_var EnumMemberSeq_var;
  typedef IR_EnumMemberSeq_out EnumMemberSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EnumMemberSeq;

  typedef IR_ModuleDescription ModuleDescription;
  typedef IR_ModuleDescription_var ModuleDescription_var;
  typedef IR_ModuleDescription_out ModuleDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ModuleDescription;

  typedef IR_ConstantDescription ConstantDescription;
  typedef IR_ConstantDescription_var ConstantDescription_var;
  typedef IR_ConstantDescription_out ConstantDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ConstantDescription;

  typedef IR_TypedefDef TypedefDef;
  typedef IR_TypedefDef *TypedefDef_ptr;
  typedef IR_TypedefDef_var TypedefDef_var;
  typedef IR_TypedefDef_out TypedefDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TypedefDef;

  typedef IR_TypeDescription TypeDescription;
  typedef IR_TypeDescription_var TypeDescription_var;
  typedef IR_TypeDescription_out TypeDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_TypeDescription;

  typedef IR_PrimitiveDef PrimitiveDef;
  typedef IR_PrimitiveDef *PrimitiveDef_ptr;
  typedef IR_PrimitiveDef_var PrimitiveDef_var;
  typedef IR_PrimitiveDef_out PrimitiveDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PrimitiveDef;

  typedef IR_StringDef StringDef;
  typedef IR_StringDef *StringDef_ptr;
  typedef IR_StringDef_var StringDef_var;
  typedef IR_StringDef_out StringDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_StringDef;

  typedef IR_WstringDef WstringDef;
  typedef IR_WstringDef *WstringDef_ptr;
  typedef IR_WstringDef_var WstringDef_var;
  typedef IR_WstringDef_out WstringDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_WstringDef;

  typedef IR_FixedDef FixedDef;
  typedef IR_FixedDef *FixedDef_ptr;
  typedef IR_FixedDef_var FixedDef_var;
  typedef IR_FixedDef_out FixedDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_FixedDef;

  typedef IR_SequenceDef SequenceDef;
  typedef IR_SequenceDef *SequenceDef_ptr;
  typedef IR_SequenceDef_var SequenceDef_var;
  typedef IR_SequenceDef_out SequenceDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_SequenceDef;

  typedef IR_ArrayDef ArrayDef;
  typedef IR_ArrayDef *ArrayDef_ptr;
  typedef IR_ArrayDef_var ArrayDef_var;
  typedef IR_ArrayDef_out ArrayDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ArrayDef;

  typedef IR_ExceptionDescription ExceptionDescription;
  typedef IR_ExceptionDescription_var ExceptionDescription_var;
  typedef IR_ExceptionDescription_out ExceptionDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ExceptionDescription;

  typedef IR_ExceptionDefSeq ExceptionDefSeq;
  typedef IR_ExceptionDefSeq_var ExceptionDefSeq_var;
  typedef IR_ExceptionDefSeq_out ExceptionDefSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ExceptionDefSeq;

  typedef IR_ExcDescriptionSeq ExcDescriptionSeq;
  typedef IR_ExcDescriptionSeq_var ExcDescriptionSeq_var;
  typedef IR_ExcDescriptionSeq_out ExcDescriptionSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ExcDescriptionSeq;

  typedef IR_AttributeMode AttributeMode;
  typedef IR_AttributeMode &AttributeMode_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_AttributeMode;

  typedef IR_AttributeDef AttributeDef;
  typedef IR_AttributeDef *AttributeDef_ptr;
  typedef IR_AttributeDef_var AttributeDef_var;
  typedef IR_AttributeDef_out AttributeDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_AttributeDef;

  typedef IR_AttributeDescription AttributeDescription;
  typedef IR_AttributeDescription_var AttributeDescription_var;
  typedef IR_AttributeDescription_out AttributeDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_AttributeDescription;

  typedef IR_PrimitiveKind PrimitiveKind;
  typedef IR_PrimitiveKind &PrimitiveKind_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PrimitiveKind;

  typedef IR_ParameterMode ParameterMode;
  typedef IR_ParameterMode &ParameterMode_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ParameterMode;

  typedef IR_ParameterDescription ParameterDescription;
  typedef IR_ParameterDescription_var ParameterDescription_var;
  typedef IR_ParameterDescription_out ParameterDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ParameterDescription;

  typedef IR_ParDescriptionSeq ParDescriptionSeq;
  typedef IR_ParDescriptionSeq_var ParDescriptionSeq_var;
  typedef IR_ParDescriptionSeq_out ParDescriptionSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ParDescriptionSeq;

  typedef IR_ContextIdSeq ContextIdSeq;
  typedef IR_ContextIdSeq_var ContextIdSeq_var;
  typedef IR_ContextIdSeq_out ContextIdSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ContextIdSeq;

  typedef IR_OperationDef OperationDef;
  typedef IR_OperationDef *OperationDef_ptr;
  typedef IR_OperationDef_var OperationDef_var;
  typedef IR_OperationDef_out OperationDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_OperationDef;

  typedef IR_OperationMode OperationMode;
  typedef IR_OperationMode &OperationMode_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_OperationMode;

  typedef IR_OperationDescription OperationDescription;
  typedef IR_OperationDescription_var OperationDescription_var;
  typedef IR_OperationDescription_out OperationDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_OperationDescription;

  typedef IR_RepositoryIdSeq RepositoryIdSeq;
  typedef IR_RepositoryIdSeq_var RepositoryIdSeq_var;
  typedef IR_RepositoryIdSeq_out RepositoryIdSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_RepositoryIdSeq;

  typedef IR_OpDescriptionSeq OpDescriptionSeq;
  typedef IR_OpDescriptionSeq_var OpDescriptionSeq_var;
  typedef IR_OpDescriptionSeq_out OpDescriptionSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_OpDescriptionSeq;

  typedef IR_AttrDescriptionSeq AttrDescriptionSeq;
  typedef IR_AttrDescriptionSeq_var AttrDescriptionSeq_var;
  typedef IR_AttrDescriptionSeq_out AttrDescriptionSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_AttrDescriptionSeq;

  typedef IR_InterfaceDescription InterfaceDescription;
  typedef IR_InterfaceDescription_var InterfaceDescription_var;
  typedef IR_InterfaceDescription_out InterfaceDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_InterfaceDescription;

  typedef IR_ValueMember ValueMember;
  typedef IR_ValueMember_var ValueMember_var;
  typedef IR_ValueMember_out ValueMember_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueMember;

  typedef IR_ValueMemberSeq ValueMemberSeq;
  typedef IR_ValueMemberSeq_var ValueMemberSeq_var;
  typedef IR_ValueMemberSeq_out ValueMemberSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueMemberSeq;

  typedef IR_ValueMemberDef ValueMemberDef;
  typedef IR_ValueMemberDef *ValueMemberDef_ptr;
  typedef IR_ValueMemberDef_var ValueMemberDef_var;
  typedef IR_ValueMemberDef_out ValueMemberDef_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueMemberDef;

  typedef IR_ValueDescription ValueDescription;
  typedef IR_ValueDescription_var ValueDescription_var;
  typedef IR_ValueDescription_out ValueDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ValueDescription;

  typedef IR_ProvidesDescription ProvidesDescription;
  typedef IR_ProvidesDescription_var ProvidesDescription_var;
  typedef IR_ProvidesDescription_out ProvidesDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ProvidesDescription;

  typedef IR_UsesDescription UsesDescription;
  typedef IR_UsesDescription_var UsesDescription_var;
  typedef IR_UsesDescription_out UsesDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UsesDescription;

  typedef IR_ProvidesDescSeq ProvidesDescSeq;
  typedef IR_ProvidesDescSeq_var ProvidesDescSeq_var;
  typedef IR_ProvidesDescSeq_out ProvidesDescSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ProvidesDescSeq;

  typedef IR_UsesDescSeq UsesDescSeq;
  typedef IR_UsesDescSeq_var UsesDescSeq_var;
  typedef IR_UsesDescSeq_out UsesDescSeq_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_UsesDescSeq;

  typedef IR_EventDescription EventDescription;
  typedef IR_EventDescription_var EventDescription_var;
  typedef IR_EventDescription_out EventDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_EventDescription;

  typedef IR_ComponentDescription ComponentDescription;
  typedef IR_ComponentDescription_var ComponentDescription_var;
  typedef IR_ComponentDescription_out ComponentDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_ComponentDescription;

  typedef IR_PrimaryKeyDescription PrimaryKeyDescription;
  typedef IR_PrimaryKeyDescription_var PrimaryKeyDescription_var;
  typedef IR_PrimaryKeyDescription_out PrimaryKeyDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_PrimaryKeyDescription;

  typedef IR_HomeDescription HomeDescription;
  typedef IR_HomeDescription_var HomeDescription_var;
  typedef IR_HomeDescription_out HomeDescription_out;
  TAO_NAMESPACE_STORAGE_CLASS CORBA::TypeCode_ptr _tc_HomeDescription;

}
TAO_NAMESPACE_CLOSE // module IR



#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_IFRFWD_H */
