// $Id$

#include "concrete_classes.h"
#include "IFR_Service_Utils_T.h"
#include "ComponentRepository_i.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService,
           tmplinst,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<CORBA::DefinitionKind>;
template class ACE_Unbounded_Queue<CORBA::DefinitionKind>;
template class ACE_Unbounded_Queue_Iterator<CORBA::DefinitionKind>;
template class ACE_Node<ACE_Configuration_Section_Key>;
template class ACE_Unbounded_Queue<ACE_Configuration_Section_Key>;
template class ACE_Unbounded_Queue_Iterator<ACE_Configuration_Section_Key>;

template class POA_CORBA::Repository_tie<TAO_Repository_i>;
template class POA_CORBA::ComponentIR::Repository_tie<TAO_ComponentRepository_i>;
template class POA_CORBA::PrimitiveDef_tie<TAO_PrimitiveDef_i>;
template class POA_CORBA::StringDef_tie<TAO_StringDef_i>;
template class POA_CORBA::WstringDef_tie<TAO_WstringDef_i>;
template class POA_CORBA::SequenceDef_tie<TAO_SequenceDef_i>;
template class POA_CORBA::ArrayDef_tie<TAO_ArrayDef_i>;
template class POA_CORBA::FixedDef_tie<TAO_FixedDef_i>;
template class POA_CORBA::EnumDef_tie<TAO_EnumDef_i>;
template class POA_CORBA::AliasDef_tie<TAO_AliasDef_i>;
template class POA_CORBA::NativeDef_tie<TAO_NativeDef_i>;
template class POA_CORBA::ValueBoxDef_tie<TAO_ValueBoxDef_i>;
template class POA_CORBA::UnionDef_tie<TAO_UnionDef_i>;
template class POA_CORBA::StructDef_tie<TAO_StructDef_i>;
template class POA_CORBA::ComponentIR::ConsumesDef_tie<TAO_ConsumesDef_i>;
template class POA_CORBA::ComponentIR::PublishesDef_tie<TAO_PublishesDef_i>;
template class POA_CORBA::ComponentIR::EmitsDef_tie<TAO_EmitsDef_i>;
template class POA_CORBA::ComponentIR::EventDef_tie<TAO_EventDef_i>;
template class POA_CORBA::ComponentIR::ProvidesDef_tie<TAO_ProvidesDef_i>;
template class POA_CORBA::ComponentIR::UsesDef_tie<TAO_UsesDef_i>;
template class POA_CORBA::ValueMemberDef_tie<TAO_ValueMemberDef_i>;
template class POA_CORBA::ComponentIR::FactoryDef_tie<TAO_FactoryDef_i>;
template class POA_CORBA::ComponentIR::FinderDef_tie<TAO_FinderDef_i>;
template class POA_CORBA::AttributeDef_tie<TAO_AttributeDef_i>;
template class POA_CORBA::ConstantDef_tie<TAO_ConstantDef_i>;
template class POA_CORBA::ComponentIR::ComponentDef_tie<TAO_ComponentDef_i>;
template class POA_CORBA::ComponentIR::HomeDef_tie<TAO_HomeDef_i>;
template class POA_CORBA::ValueDef_tie<TAO_ValueDef_i>;
template class POA_CORBA::ExceptionDef_tie<TAO_ExceptionDef_i>;
template class POA_CORBA::ModuleDef_tie<TAO_ModuleDef_i>;
template class POA_CORBA::OperationDef_tie<TAO_OperationDef_i>;
template class POA_CORBA::InterfaceDef_tie<TAO_InterfaceDef_i>;
template class POA_CORBA::AbstractInterfaceDef_tie<TAO_AbstractInterfaceDef_i>;
template class POA_CORBA::LocalInterfaceDef_tie<TAO_LocalInterfaceDef_i>;

template class auto_ptr<TAO_Repository_i>;
template class ACE_Auto_Basic_Ptr<TAO_Repository_i>;
template class auto_ptr<TAO_ComponentRepository_i>;
template class ACE_Auto_Basic_Ptr<TAO_ComponentRepository_i>;
template class auto_ptr<TAO_PrimitiveDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_PrimitiveDef_i>;
template class auto_ptr<TAO_StringDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_StringDef_i>;
template class auto_ptr<TAO_WstringDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_WstringDef_i>;
template class auto_ptr<TAO_SequenceDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_SequenceDef_i>;
template class auto_ptr<TAO_ArrayDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ArrayDef_i>;
template class auto_ptr<TAO_FixedDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_FixedDef_i>;
template class auto_ptr<TAO_EnumDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_EnumDef_i>;
template class auto_ptr<TAO_AliasDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_AliasDef_i>;
template class auto_ptr<TAO_NativeDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_NativeDef_i>;
template class auto_ptr<TAO_ValueBoxDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ValueBoxDef_i>;
template class auto_ptr<TAO_UnionDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_UnionDef_i>;
template class auto_ptr<TAO_StructDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_StructDef_i>;
template class auto_ptr<TAO_ConsumesDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ConsumesDef_i>;
template class auto_ptr<TAO_PublishesDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_PublishesDef_i>;
template class auto_ptr<TAO_EmitsDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_EmitsDef_i>;
template class auto_ptr<TAO_EventDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_EventDef_i>;
template class auto_ptr<TAO_ProvidesDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ProvidesDef_i>;
template class auto_ptr<TAO_UsesDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_UsesDef_i>;
template class auto_ptr<TAO_ValueMemberDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ValueMemberDef_i>;
template class auto_ptr<TAO_FactoryDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_FactoryDef_i>;
template class auto_ptr<TAO_FinderDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_FinderDef_i>;
template class auto_ptr<TAO_AttributeDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_AttributeDef_i>;
template class auto_ptr<TAO_ConstantDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ConstantDef_i>;
template class auto_ptr<TAO_ComponentDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ComponentDef_i>;
template class auto_ptr<TAO_HomeDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_HomeDef_i>;
template class auto_ptr<TAO_ValueDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ValueDef_i>;
template class auto_ptr<TAO_ExceptionDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ExceptionDef_i>;
template class auto_ptr<TAO_ModuleDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_ModuleDef_i>;
template class auto_ptr<TAO_OperationDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_OperationDef_i>;
template class auto_ptr<TAO_InterfaceDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_InterfaceDef_i>;
template class auto_ptr<TAO_AbstractInterfaceDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_AbstractInterfaceDef_i>;
template class auto_ptr<TAO_LocalInterfaceDef_i>;
template class ACE_Auto_Basic_Ptr<TAO_LocalInterfaceDef_i>;
template class auto_ptr<TAO_Container_i>;
template class ACE_Auto_Basic_Ptr<TAO_Container_i>;
template class auto_ptr<TAO_Contained_i>;
template class ACE_Auto_Basic_Ptr<TAO_Contained_i>;
template class auto_ptr<TAO_IDLType_i>;
template class ACE_Auto_Basic_Ptr<TAO_IDLType_i>;

template class ACE_Auto_Basic_Ptr<char>;

template class TAO_IFR_Generic_Utils<TAO_AttributeDef_i>;
template class TAO_IFR_Generic_Utils<TAO_OperationDef_i>;
template class TAO_IFR_Generic_Utils<TAO_ProvidesDef_i>;
template class TAO_IFR_Generic_Utils<TAO_UsesDef_i>;
template class TAO_IFR_Generic_Utils<TAO_EmitsDef_i>;
template class TAO_IFR_Generic_Utils<TAO_PublishesDef_i>;
template class TAO_IFR_Generic_Utils<TAO_ConsumesDef_i>;
template class TAO_IFR_Generic_Utils<TAO_FinderDef_i>;
template class TAO_IFR_Generic_Utils<TAO_FactoryDef_i>;
template class TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::ProvidesDescriptionSeq>;
template class TAO_Port_Utils<CORBA::ComponentIR::ProvidesDef>;
template class TAO_Port_Utils<CORBA::ComponentIR::EmitsDef>;
template class TAO_Port_Utils<CORBA::ComponentIR::PublishesDef>;
template class TAO_Port_Utils<CORBA::ComponentIR::ConsumesDef>;
template class TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>;
template class TAO_IFR_Strseq_Utils<CORBA::ContextIdSeq>;
template class TAO_IFR_Desc_Utils<CORBA::ComponentIR::HomeDescription, TAO_HomeDef_i>;
template class TAO_IFR_Desc_Utils<CORBA::ExceptionDescription, TAO_ExceptionDef_i>;
template class TAO_IFR_Desc_Utils<CORBA::ExtAttributeDescription, TAO_AttributeDef_i>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<CORBA::DefinitionKind>
#pragma instantiate ACE_Unbounded_Queue<CORBA::DefinitionKind>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::DefinitionKind>
#pragma instantiate ACE_Node<ACE_Configuration_Section_Key>
#pragma instantiate ACE_Unbounded_Queue<ACE_Configuration_Section_Key>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Configuration_Section_Key>

#pragma instantiate POA_CORBA::Repository_tie<TAO_Repository_i>
#pragma instantiate POA_CORBA::ComponentIR::Repository_tie<TAO_ComponentRepository_i>
#pragma instantiate POA_CORBA::PrimitiveDef_tie<TAO_PrimitiveDef_i>
#pragma instantiate POA_CORBA::StringDef_tie<TAO_StringDef_i>
#pragma instantiate POA_CORBA::WstringDef_tie<TAO_WstringDef_i>
#pragma instantiate POA_CORBA::SequenceDef_tie<TAO_SequenceDef_i>
#pragma instantiate POA_CORBA::ArrayDef_tie<TAO_ArrayDef_i>
#pragma instantiate POA_CORBA::FixedDef_tie<TAO_FixedDef_i>
#pragma instantiate POA_CORBA::EnumDef_tie<TAO_EnumDef_i>
#pragma instantiate POA_CORBA::AliasDef_tie<TAO_AliasDef_i>
#pragma instantiate POA_CORBA::NativeDef_tie<TAO_NativeDef_i>
#pragma instantiate POA_CORBA::ValueBoxDef_tie<TAO_ValueBoxDef_i>
#pragma instantiate POA_CORBA::UnionDef_tie<TAO_UnionDef_i>
#pragma instantiate POA_CORBA::StructDef_tie<TAO_StructDef_i>
#pragma instantiate POA_CORBA::ComponentIR::ConsumesDef_tie<TAO_ConsumesDef_i>
#pragma instantiate POA_CORBA::ComponentIR::PublishesDef_tie<TAO_PublishesDef_i>
#pragma instantiate POA_CORBA::ComponentIR::EmitsDef_tie<TAO_EmitsDef_i>
#pragma instantiate POA_CORBA::ComponentIR::EventDef_tie<TAO_PrimaryKeyDef_i>
#pragma instantiate POA_CORBA::ComponentIR::ProvidesDef_tie<TAO_ProvidesDef_i>
#pragma instantiate POA_CORBA::ComponentIR::UsesDef_tie<TAO_UsesDef_i>
#pragma instantiate POA_CORBA::ValueMemberDef_tie<TAO_ValueMemberDef_i>
#pragma instantiate POA_CORBA::ComponentIR::FactoryDef_tie<TAO_FactoryDef_i>
#pragma instantiate POA_CORBA::ComponentIR::FinderDef_tie<TAO_FinderDef_i>
#pragma instantiate POA_CORBA::AttributeDef_tie<TAO_AttributeDef_i>
#pragma instantiate POA_CORBA::ConstantDef_tie<TAO_ConstantDef_i>
#pragma instantiate POA_CORBA::ComponentIR::ComponentDef_tie<TAO_ComponentDef_i>
#pragma instantiate POA_CORBA::ComponentIR::HomeDef_tie<TAO_HomeDef_i>
#pragma instantiate POA_CORBA::ValueDef_tie<TAO_ValueDef_i>
#pragma instantiate POA_CORBA::ExceptionDef_tie<TAO_ExceptionDef_i>
#pragma instantiate POA_CORBA::ModuleDef_tie<TAO_ModuleDef_i>
#pragma instantiate POA_CORBA::OperationDef_tie<TAO_OperationDef_i>
#pragma instantiate POA_CORBA::InterfaceDef_tie<TAO_>
#pragma instantiate POA_CORBA::AbstractInterfaceDef_tie<TAO_AbstractInterfaceDef_i>
#pragma instantiate POA_CORBA::LocalInterfaceDef_tie<TAO_LocalInterfaceDef_i>

#pragma instantiate auto_ptr<TAO_Repository_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Repository_i>
#pragma instantiate auto_ptr<TAO_ComponentRepository_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ComponentRepository_i>
#pragma instantiate auto_ptr<TAO_PrimitiveDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_PrimitiveDef_i>
#pragma instantiate auto_ptr<TAO_StringDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_StringDef_i>
#pragma instantiate auto_ptr<TAO_WstringDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_WstringDef_i>
#pragma instantiate auto_ptr<TAO_SequenceDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_SequenceDef_i>
#pragma instantiate auto_ptr<TAO_ArrayDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ArrayDef_i>
#pragma instantiate auto_ptr<TAO_FixedDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_FixedDef_i>
#pragma instantiate auto_ptr<TAO_EnumDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_EnumDef_i>
#pragma instantiate auto_ptr<TAO_AliasDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_AliasDef_i>
#pragma instantiate auto_ptr<TAO_NativeDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_NativeDef_i>
#pragma instantiate auto_ptr<TAO_ValueBoxDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ValueBoxDef_i>
#pragma instantiate auto_ptr<TAO_UnionDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_UnionDef_i>
#pragma instantiate auto_ptr<TAO_StructDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_StructDef_i>
#pragma instantiate auto_ptr<TAO_ConsumesDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ConsumesDef_i>
#pragma instantiate auto_ptr<TAO_PublishesDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_PublishesDef_i>
#pragma instantiate auto_ptr<TAO_EmitsDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_EmitsDef_i>
#pragma instantiate auto_ptr<TAO_EventDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_EventDef_i>
#pragma instantiate auto_ptr<TAO_ProvidesDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ProvidesDef_i>
#pragma instantiate auto_ptr<TAO_UsesDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_UsesDef_i>
#pragma instantiate auto_ptr<TAO_ValueMemberDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ValueMemberDef_i>
#pragma instantiate auto_ptr<TAO_FactoryDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_FactoryDef_i>
#pragma instantiate auto_ptr<TAO_FinderDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_FinderDef_i>
#pragma instantiate auto_ptr<TAO_AttributeDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_AttributeDef_i>
#pragma instantiate auto_ptr<TAO_ConstantDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ConstantDef_i>
#pragma instantiate auto_ptr<TAO_ComponentDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ComponentDef_i>
#pragma instantiate auto_ptr<TAO_HomeDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_HomeDef_i>
#pragma instantiate auto_ptr<TAO_ValueDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ValueDef_i>
#pragma instantiate auto_ptr<TAO_ExceptionDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ExceptionDef_i>
#pragma instantiate auto_ptr<TAO_ModuleDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_ModuleDef_i>
#pragma instantiate auto_ptr<TAO_OperationDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_OperationDef_i>
#pragma instantiate auto_ptr<TAO_InterfaceDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_InterfaceDef_i>
#pragma instantiate auto_ptr<TAO_AbstractInterfaceDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_AbstractInterfaceDef_i>
#pragma instantiate auto_ptr<TAO_LocalInterfaceDef_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_LocalInterfaceDef_i>
#pragma instantiate auto_ptr<TAO_Container_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Container_i>
#pragma instantiate auto_ptr<TAO_Contained_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Contained_i>
#pragma instantiate auto_ptr<TAO_IDLType_i>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_IDLType_i>

#pragma instantiate ACE_Auto_Basic_Ptr<char>

#pragma instantiate TAO_IFR_Generic_Utils<TAO_AttributeDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_OperationDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_ProvidesDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_UsesDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_EmitsDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_PublishesDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_ConsumesDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_FinderDef_i>
#pragma instantiate TAO_IFR_Generic_Utils<TAO_FactoryDef_i>
#pragma instantiate TAO_Port_Desc_Seq_Utils<CORBA::ComponentIR::ProvidesDescriptionSeq>
#pragma instantiate TAO_Port_Utils<CORBA::ComponentIR::ProvidesDef>
#pragma instantiate TAO_Port_Utils<CORBA::ComponentIR::EmitsDef>
#pragma instantiate TAO_Port_Utils<CORBA::ComponentIR::PublishesDef>
#pragma instantiate TAO_Port_Utils<CORBA::ComponentIR::ConsumesDef>
#pragma instantiate TAO_IFR_Strseq_Utils<CORBA::RepositoryIdSeq>
#pragma instantiate TAO_IFR_Strseq_Utils<CORBA::ContextIdSeq>
#pragma instantiate TAO_IFR_Desc_Utils<CORBA::ComponentIR::HomeDescription, TAO_HomeDef_i>
#pragma instantiate TAO_IFR_Desc_Utils<CORBA::ExceptionDescription, TAO_ExceptionDef_i>
#pragma instantiate TAO_IFR_Desc_Utils<CORBA::ExtAttributeDescription, TAO_AttributeDef_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
