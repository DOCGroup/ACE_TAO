// $Id$

#include "Servant_Factory.h"
#include "concrete_classes.h"
#include "IFR_ComponentsS.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, Servant_Factory, "$Id$")

IFR_Servant_Factory::IFR_Servant_Factory (TAO_Repository_i *repo)
  : repo_ (repo)
{
}

TAO_IDLType_i *
IFR_Servant_Factory::create_idltype (
    ACE_Configuration_Section_Key servant_key
    ACE_ENV_ARG_DECL
  )
{
  u_int def_kind = 0;
  this->repo_->config ()->get_integer_value (servant_key,
                                             "def_kind",
                                             def_kind);

  switch (ACE_static_cast (CORBA::DefinitionKind, def_kind))
  {
    case CORBA::dk_Interface:
    {
      TAO_InterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_InterfaceDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_AbstractInterface:
    {
      TAO_AbstractInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AbstractInterfaceDef_i (this->repo_,
                                                    servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_LocalInterface:
    {
      TAO_LocalInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_LocalInterfaceDef_i (this->repo_,
                                                 servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Alias:
    {
      TAO_AliasDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AliasDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Struct:
    {
      TAO_StructDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StructDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Union:
    {
      TAO_UnionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UnionDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Enum:
    {
      TAO_EnumDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_EnumDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Primitive:
    {
      TAO_PrimitiveDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PrimitiveDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_String:
    {
      TAO_StringDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StringDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Sequence:
    {
      TAO_SequenceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_SequenceDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Array:
    {
      TAO_ArrayDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ArrayDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Wstring:
    {
      TAO_WstringDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_WstringDef_i (this->repo_,
                                          servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Fixed:
    {
      TAO_FixedDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_FixedDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Value:
    {
      TAO_ValueDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_ValueBox:
    {
      TAO_ValueBoxDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueBoxDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Native:
    {
      TAO_NativeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_NativeDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Component:
    {
      TAO_ComponentDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ComponentDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Home:
    {
      TAO_HomeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_HomeDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    default:
      return 0;
  }
}

TAO_Contained_i *
IFR_Servant_Factory::create_contained (
    ACE_Configuration_Section_Key servant_key
    ACE_ENV_ARG_DECL
  )
{
  u_int def_kind = 0;
  this->repo_->config ()->get_integer_value (servant_key,
                                             "def_kind",
                                             def_kind);

  switch (ACE_static_cast (CORBA::DefinitionKind, def_kind))
  {
    case CORBA::dk_Interface:
    {
      TAO_InterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_InterfaceDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_AbstractInterface:
    {
      TAO_AbstractInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AbstractInterfaceDef_i (this->repo_,
                                                    servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_LocalInterface:
    {
      TAO_LocalInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_LocalInterfaceDef_i (this->repo_,
                                                 servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Alias:
    {
      TAO_AliasDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AliasDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Struct:
    {
      TAO_StructDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StructDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Union:
    {
      TAO_UnionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UnionDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Enum:
    {
      TAO_EnumDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_EnumDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Module:
    {
      TAO_ModuleDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ModuleDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Exception:
    {
      TAO_ExceptionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ExceptionDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Constant:
    {
      TAO_ConstantDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ConstantDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_ValueMember:
    {
      TAO_ValueMemberDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueMemberDef_i (this->repo_,
                                              servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Uses:
    {
      TAO_UsesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UsesDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Provides:
    {
      TAO_ProvidesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ProvidesDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_PrimaryKey:
    {
      TAO_PrimaryKeyDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PrimaryKeyDef_i (this->repo_,
                                             servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Emits:
    {
      TAO_EmitsDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_EmitsDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Publishes:
    {
      TAO_PublishesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PublishesDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Consumes:
    {
      TAO_ConsumesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ConsumesDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Attribute:
    {
      TAO_AttributeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AttributeDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Operation:
    {
      TAO_OperationDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_OperationDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Value:
    {
      TAO_ValueDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_ValueBox:
    {
      TAO_ValueBoxDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueBoxDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Native:
    {
      TAO_NativeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_NativeDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Component:
    {
      TAO_ComponentDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ComponentDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Home:
    {
      TAO_HomeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_HomeDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    default:
      return 0;
  }
}

TAO_Container_i *
IFR_Servant_Factory::create_container (
    ACE_Configuration_Section_Key servant_key
    ACE_ENV_ARG_DECL
  )
{
  u_int def_kind = 0;
  this->repo_->config ()->get_integer_value (servant_key,
                                             "def_kind",
                                             def_kind);

  switch (ACE_static_cast (CORBA::DefinitionKind, def_kind))
  {
    case CORBA::dk_Interface:
    {
      TAO_InterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_InterfaceDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_AbstractInterface:
    {
      TAO_AbstractInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AbstractInterfaceDef_i (this->repo_,
                                                    servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_LocalInterface:
    {
      TAO_LocalInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_LocalInterfaceDef_i (this->repo_,
                                                 servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Struct:
    {
      TAO_StructDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StructDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Union:
    {
      TAO_UnionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UnionDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Module:
    {
      TAO_ModuleDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ModuleDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Exception:
    {
      TAO_ExceptionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ExceptionDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Value:
    {
      TAO_ValueDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Component:
    {
      TAO_ComponentDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ComponentDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    case CORBA::dk_Home:
    {
      TAO_HomeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_HomeDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return impl;
    }
    default:
      return 0;
  }
}

PortableServer::Servant
IFR_Servant_Factory::create_tie (
    ACE_Configuration_Section_Key servant_key,
    PortableServer::POA_ptr poa
    ACE_ENV_ARG_DECL
  )
{
  u_int def_kind = 0;
  this->repo_->config ()->get_integer_value (servant_key,
                                             "def_kind",
                                             def_kind);

  switch (ACE_static_cast (CORBA::DefinitionKind, def_kind))
  {
    case CORBA::dk_Attribute:
    {
      TAO_AttributeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AttributeDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_AttributeDef_i> safety (impl);

      POA_CORBA_AttributeDef_tie<TAO_AttributeDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_AttributeDef_tie<TAO_AttributeDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Constant:
    {
      TAO_ConstantDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ConstantDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ConstantDef_i> safety (impl);

      POA_CORBA_ConstantDef_tie<TAO_ConstantDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ConstantDef_tie<TAO_ConstantDef_i> (impl,
                                                                      poa,
                                                                      1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Exception:
    {
      TAO_ExceptionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ExceptionDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ExceptionDef_i> safety (impl);

      POA_CORBA_ExceptionDef_tie<TAO_ExceptionDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ExceptionDef_tie<TAO_ExceptionDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Interface:
    {
      TAO_InterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_InterfaceDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_InterfaceDef_i> safety (impl);

      POA_CORBA_InterfaceDef_tie<TAO_InterfaceDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_InterfaceDef_tie<TAO_InterfaceDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_AbstractInterface:
    {
      TAO_AbstractInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AbstractInterfaceDef_i (this->repo_,
                                                    servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_AbstractInterfaceDef_i> safety (impl);

      POA_CORBA_AbstractInterfaceDef_tie<TAO_AbstractInterfaceDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (
          impl_tie,
          POA_CORBA_AbstractInterfaceDef_tie<TAO_AbstractInterfaceDef_i> (impl,
                                                                          poa,
                                                                          1),
          CORBA::NO_MEMORY ()
        );
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_LocalInterface:
    {
      TAO_LocalInterfaceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_LocalInterfaceDef_i (this->repo_,
                                                 servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_LocalInterfaceDef_i> safety (impl);

      POA_CORBA_LocalInterfaceDef_tie<TAO_LocalInterfaceDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (
          impl_tie,
          POA_CORBA_LocalInterfaceDef_tie<TAO_LocalInterfaceDef_i> (impl,
                                                                    poa,
                                                                    1),
          CORBA::NO_MEMORY ()
        );
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Module:
    {
      TAO_ModuleDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ModuleDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ModuleDef_i> safety (impl);

      POA_CORBA_ModuleDef_tie<TAO_ModuleDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ModuleDef_tie<TAO_ModuleDef_i> (impl,
                                                                  poa,
                                                                  1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Operation:
    {
      TAO_OperationDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_OperationDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_OperationDef_i> safety (impl);

      POA_CORBA_OperationDef_tie<TAO_OperationDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_OperationDef_tie<TAO_OperationDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Alias:
    {
      TAO_AliasDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_AliasDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_AliasDef_i> safety (impl);

      POA_CORBA_AliasDef_tie<TAO_AliasDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_AliasDef_tie<TAO_AliasDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Struct:
    {
      TAO_StructDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StructDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_StructDef_i> safety (impl);

      POA_CORBA_StructDef_tie<TAO_StructDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_StructDef_tie<TAO_StructDef_i> (impl,
                                                                  poa,
                                                                  1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Union:
    {
      TAO_UnionDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UnionDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_UnionDef_i> safety (impl);

      POA_CORBA_UnionDef_tie<TAO_UnionDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_UnionDef_tie<TAO_UnionDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Enum:
    {
      TAO_EnumDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_EnumDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_EnumDef_i> safety (impl);

      POA_CORBA_EnumDef_tie<TAO_EnumDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_EnumDef_tie<TAO_EnumDef_i> (impl,
                                                              poa,
                                                              1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Primitive:
    {
      TAO_PrimitiveDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PrimitiveDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_PrimitiveDef_i> safety (impl);

      POA_CORBA_PrimitiveDef_tie<TAO_PrimitiveDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_PrimitiveDef_tie<TAO_PrimitiveDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_String:
    {
      TAO_StringDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_StringDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_StringDef_i> safety (impl);

      POA_CORBA_StringDef_tie<TAO_StringDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_StringDef_tie<TAO_StringDef_i> (impl,
                                                                  poa,
                                                                  1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Sequence:
    {
      TAO_SequenceDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_SequenceDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_SequenceDef_i> safety (impl);

      POA_CORBA_SequenceDef_tie<TAO_SequenceDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_SequenceDef_tie<TAO_SequenceDef_i> (impl,
                                                                      poa,
                                                                      1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Array:
    {
      TAO_ArrayDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ArrayDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ArrayDef_i> safety (impl);

      POA_CORBA_ArrayDef_tie<TAO_ArrayDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ArrayDef_tie<TAO_ArrayDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Wstring:
    {
      TAO_WstringDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_WstringDef_i (this->repo_,
                                          servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_WstringDef_i> safety (impl);

      POA_CORBA_WstringDef_tie<TAO_WstringDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_WstringDef_tie<TAO_WstringDef_i> (impl,
                                                                    poa,
                                                                    1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Fixed:
    {
      TAO_FixedDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_FixedDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_FixedDef_i> safety (impl);

      POA_CORBA_FixedDef_tie<TAO_FixedDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_FixedDef_tie<TAO_FixedDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Value:
    {
      TAO_ValueDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ValueDef_i> safety (impl);

      POA_CORBA_ValueDef_tie<TAO_ValueDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ValueDef_tie<TAO_ValueDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_ValueBox:
    {
      TAO_ValueBoxDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueBoxDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ValueBoxDef_i> safety (impl);

      POA_CORBA_ValueBoxDef_tie<TAO_ValueBoxDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_ValueBoxDef_tie<TAO_ValueBoxDef_i> (impl,
                                                                      poa,
                                                                      1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_ValueMember:
    {
      TAO_ValueMemberDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ValueMemberDef_i (this->repo_,
                                              servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ValueMemberDef_i> safety (impl);

      POA_CORBA_ValueMemberDef_tie<TAO_ValueMemberDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (
          impl_tie,
          POA_CORBA_ValueMemberDef_tie<TAO_ValueMemberDef_i> (impl,
                                                              poa,
                                                              1),
          CORBA::NO_MEMORY ()
        );
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Native:
    {
      TAO_NativeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_NativeDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_NativeDef_i> safety (impl);

      POA_CORBA_NativeDef_tie<TAO_NativeDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_CORBA_NativeDef_tie<TAO_NativeDef_i> (impl,
                                                                  poa,
                                                                  1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Component:
    {
      TAO_ComponentDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ComponentDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ComponentDef_i> safety (impl);

      POA_IR::ComponentDef_tie<TAO_ComponentDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::ComponentDef_tie<TAO_ComponentDef_i> (impl,
                                                                      poa,
                                                                      1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Home:
    {
      TAO_HomeDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_HomeDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_HomeDef_i> safety (impl);

      POA_IR::HomeDef_tie<TAO_HomeDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::HomeDef_tie<TAO_HomeDef_i> (impl,
                                                            poa,
                                                            1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Factory:
    {
      TAO_FactoryDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_FactoryDef_i (this->repo_,
                                          servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_FactoryDef_i> safety (impl);

      POA_IR::FactoryDef_tie<TAO_FactoryDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::FactoryDef_tie<TAO_FactoryDef_i> (impl,
                                                                  poa,
                                                                  1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Finder:
    {
      TAO_FinderDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_FinderDef_i (this->repo_,
                                         servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_FinderDef_i> safety (impl);

      POA_IR::FinderDef_tie<TAO_FinderDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::FinderDef_tie<TAO_FinderDef_i> (impl,
                                                                poa,
                                                                1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_PrimaryKey:
    {
      TAO_PrimaryKeyDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PrimaryKeyDef_i (this->repo_,
                                             servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_PrimaryKeyDef_i> safety (impl);

      POA_IR::PrimaryKeyDef_tie<TAO_PrimaryKeyDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::PrimaryKeyDef_tie<TAO_PrimaryKeyDef_i> (impl,
                                                                        poa,
                                                                        1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Emits:
    {
      TAO_EmitsDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_EmitsDef_i (this->repo_,
                                        servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_EmitsDef_i> safety (impl);

      POA_IR::EmitsDef_tie<TAO_EmitsDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::EmitsDef_tie<TAO_EmitsDef_i> (impl,
                                                              poa,
                                                              1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Publishes:
    {
      TAO_PublishesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_PublishesDef_i (this->repo_,
                                            servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_PublishesDef_i> safety (impl);

      POA_IR::PublishesDef_tie<TAO_PublishesDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::PublishesDef_tie<TAO_PublishesDef_i> (impl,
                                                                      poa,
                                                                      1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Consumes:
    {
      TAO_ConsumesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ConsumesDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ConsumesDef_i> safety (impl);

      POA_IR::ConsumesDef_tie<TAO_ConsumesDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::ConsumesDef_tie<TAO_ConsumesDef_i> (impl,
                                                                    poa,
                                                                    1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Provides:
    {
      TAO_ProvidesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_ProvidesDef_i (this->repo_,
                                           servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_ProvidesDef_i> safety (impl);

      POA_IR::ProvidesDef_tie<TAO_ProvidesDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::ProvidesDef_tie<TAO_ProvidesDef_i> (impl,
                                                                    poa,
                                                                    1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    case CORBA::dk_Uses:
    {
      TAO_UsesDef_i *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO_UsesDef_i (this->repo_,
                                       servant_key),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_UsesDef_i> safety (impl);

      POA_IR::UsesDef_tie<TAO_UsesDef_i> *impl_tie = 0;
      ACE_NEW_THROW_EX (impl_tie,
                        POA_IR::UsesDef_tie<TAO_UsesDef_i> (impl,
                                                            poa,
                                                            1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      safety.release ();
      return impl_tie;
    }
    default:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
  }
}

CORBA::Object_ptr
IFR_Servant_Factory::create_objref (CORBA::DefinitionKind def_kind,
                                    const char *obj_id
                                    ACE_ENV_ARG_DECL)
{
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (obj_id);

  // Can plug in prefix tricks here.
  ACE_TString repo_id ("IDL:omg.org/");

  switch (def_kind)
  {
    case CORBA::dk_Attribute:
      repo_id += "CORBA/AttributeDef:";
      break;
    case CORBA::dk_Constant:
      repo_id += "CORBA/ConstantDef:";
      break;
    case CORBA::dk_Exception:
      repo_id += "CORBA/ExceptionDef:";
      break;
    case CORBA::dk_Interface:
      repo_id += "CORBA/InterfaceDef:";
      break;
    case CORBA::dk_AbstractInterface:
      repo_id += "CORBA/AbstractInterfaceDef:";
      break;
    case CORBA::dk_LocalInterface:
      repo_id += "CORBA/LocalInterfaceDef:";
      break;
    case CORBA::dk_Module:
      repo_id += "CORBA/ModuleDef:";
      break;
    case CORBA::dk_Operation:
      repo_id += "CORBA/OperationDef:";
      break;
    case CORBA::dk_Typedef:
      repo_id += "CORBA/TypedefDef:";
      break;
    case CORBA::dk_Alias:
      repo_id += "CORBA/AliasDef:";
      break;
    case CORBA::dk_Struct:
      repo_id += "CORBA/StructDef:";
      break;
    case CORBA::dk_Union:
      repo_id += "CORBA/UnionDef:";
      break;
    case CORBA::dk_Enum:
      repo_id += "CORBA/EnumDef:";
      break;
    case CORBA::dk_Primitive:
      repo_id += "CORBA/PrimitiveDef:";
      break;
    case CORBA::dk_String:
      repo_id += "CORBA/StringDef:";
      break;
    case CORBA::dk_Sequence:
      repo_id += "CORBA/SequenceDef:";
      break;
    case CORBA::dk_Array:
      repo_id += "CORBA/ArrayDef:";
      break;
    case CORBA::dk_Wstring:
      repo_id += "CORBA/WstringDef:";
      break;
    case CORBA::dk_Fixed:
      repo_id += "CORBA/FixedDef:";
      break;
    case CORBA::dk_Value:
      repo_id += "CORBA/ValueDef:";
      break;
    case CORBA::dk_ValueBox:
      repo_id += "CORBA/ValueBoxDef:";
      break;
    case CORBA::dk_ValueMember:
      repo_id += "CORBA/ValueMemberDef:";
      break;
    case CORBA::dk_Native:
      repo_id += "CORBA/NativeDef:";
      break;
    case CORBA::dk_Component:
      repo_id += "IR/ComponentDef:";
      break;
    case CORBA::dk_Home:
      repo_id += "HomeDef:";
      break;
    case CORBA::dk_Factory:
      repo_id += "IR/FactoryDef:";
      break;
    case CORBA::dk_Finder:
      repo_id += "IR/FinderDef:";
      break;
    case CORBA::dk_PrimaryKey:
      repo_id += "IR/PrimaryKeyDef:";
      break;
    case CORBA::dk_Emits:
      repo_id += "IR/EmitsDef:";
      break;
    case CORBA::dk_Publishes:
      repo_id += "IR/PublishesDef:";
      break;
    case CORBA::dk_Consumes:
      repo_id += "IR/ConsumesDef:";
      break;
    case CORBA::dk_Provides:
      repo_id += "IR/ProvidesDef:";
      break;
    case CORBA::dk_Uses:
      repo_id += "IR/UsesDef:";
      break;
    default:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
  }

  // Can plug in version tricks here.
  repo_id += "1.0";

  return this->repo_->ir_poa ()->create_reference_with_id (oid.in (),
                                                           repo_id.c_str ()
                                                           ACE_ENV_ARG_PARAMETER);
}
