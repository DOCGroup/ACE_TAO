/* -*- C++ -*- */
// $Id$

#include "IRObject_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "Contained_i.h"

#include "tao/Object_KeyC.h"
#include "tao/PortableServer/POA.h"

ACE_RCSID (IFR_Service, 
           IRObject_i, 
           "$Id$")

TAO_IRObject_i::TAO_IRObject_i (TAO_Repository_i *repo)
  : repo_ (repo),
    section_key_ (ACE_Configuration_Section_Key ())
{
}

TAO_IRObject_i::~TAO_IRObject_i (void)
{
}

void
TAO_IRObject_i::section_key (ACE_Configuration_Section_Key &key)
{
  this->section_key_ = key;
}

void
TAO_IRObject_i::update_key (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::ObjectId_var oid =
    this->repo_->poa_current ()->get_object_id ();
  ACE_CHECK;

  CORBA::String_var oid_string =
    PortableServer::ObjectId_to_string (oid.in ());

  if (oid_string[0U] == '\0')
    {
      this->section_key_ = this->repo_->root_key ();
      return;
    }

  int status = 
    this->repo_->config ()->expand_path (this->repo_->root_key (),
                                         oid_string.in (),
                                         this->section_key_,
                                         0);

  if (status != 0)
    {
      // If we're here, destroy() has been called.
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }
}

char *
TAO_IRObject_i::int_to_string (CORBA::ULong number) const
{
  char retval[9];

  ACE_OS::sprintf (retval, "%8.8X", number);

  return CORBA::string_dup (retval);
}

char *
TAO_IRObject_i::reference_to_path (CORBA::IRObject_ptr obj)
{
  PortableServer::ObjectId object_id;
  TAO_ObjectKey_var object_key = obj->_key ();
  int status = TAO_POA::parse_ir_object_key (object_key.in (),
                                             object_id);
  if (status != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "reference_to_path - parse_ir_object_key failed\n"));
      return 0;
    }

  return PortableServer::ObjectId_to_string (object_id);
}

CORBA::DefinitionKind
TAO_IRObject_i::path_to_def_kind (ACE_TString &path)
{
  int status = 
    this->repo_->config ()->expand_path (this->repo_->root_key (),
                                         path,
                                         this->tmp_key_,
                                         0);

  if (status != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_def_kind - bad path: '%s'\n",
                  path.c_str ()));
      return CORBA::dk_none;
    }

  u_int kind = 0;
  this->repo_->config ()->get_integer_value (this->tmp_key_,
                                             "def_kind",
                                             kind);
  return ACE_static_cast (CORBA::DefinitionKind,
                          kind);
}

CORBA::DefinitionKind 
TAO_IRObject_i::reference_to_def_kind (CORBA::IRObject_ptr obj)
{
  CORBA::String_var tmp = this->reference_to_path (obj);
  ACE_TString path (tmp.in ());
  return this->path_to_def_kind (path);
}

TAO_IDLType_i *
TAO_IRObject_i::path_to_idltype (ACE_TString &path)
{
  CORBA::DefinitionKind def_kind = this->path_to_def_kind (path);
  TAO_IDLType_i *retval = this->repo_->select_idltype (def_kind);

  if (retval == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_idltype - not an IDLType: '%s'\n",
                  path.c_str ()));
      return 0;
    }

  retval->section_key_ = this->tmp_key_;
  return retval;
}

TAO_Contained_i *
TAO_IRObject_i::path_to_contained (ACE_TString &path)
{
  CORBA::DefinitionKind def_kind = this->path_to_def_kind (path);
  TAO_Contained_i *retval = this->repo_->select_contained (def_kind);

  if (retval == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_contained - not a contained type: '%s'\n",
                  path.c_str ()));
      return 0;
    }

  retval->section_key_ = this->tmp_key_;
  return retval;
}

TAO_Container_i *
TAO_IRObject_i::path_to_container (ACE_TString &path)
{
  CORBA::DefinitionKind def_kind = this->path_to_def_kind (path);
  TAO_Container_i *retval = this->repo_->select_container (def_kind);

  if (retval != 0)
    {
      retval->section_key_ = this->tmp_key_;
    }

  return retval;
}

CORBA::Object_ptr
TAO_IRObject_i::path_to_ir_object (ACE_TString &path
                                   ACE_ENV_ARG_DECL)
{
  CORBA::DefinitionKind def_kind = this->path_to_def_kind (path);

  return this->create_objref (def_kind,
                              path.c_str ()
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_IRObject_i::create_objref (CORBA::DefinitionKind def_kind,
                               const char *obj_id
                               ACE_ENV_ARG_DECL)
{
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
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 
                        CORBA::Object::_nil ());
  }

  repo_id += "1.0";

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (obj_id);

  PortableServer::POA_ptr poa = this->repo_->select_poa (def_kind);

  return poa->create_reference_with_id (oid.in (),
                                        repo_id.c_str ()
                                        ACE_ENV_ARG_PARAMETER);
}

