/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "InterfaceDef_i.h"
#include "ValueDef_i.h"
#include "ExtValueDef_i.h"
#include "IFR_Service_Utils.h"
#include "IFR_Service_Utils_T.h"

#include "tao/IFR_Client/IFR_ComponentsC.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID (IFRService, 
           Container_i, 
           "$Id$")

const char *TAO_Container_i::tmp_name_holder_ = 0;

TAO_Container_i::TAO_Container_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo)
{
}

TAO_Container_i::~TAO_Container_i (void)
{
}

void
TAO_Container_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Container_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int index = 0;
  int status;
  ACE_TString section_name;

  // Destroy and/or remove references.

  ACE_Configuration_Section_Key refs_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "refs",
                                          0,
                                          refs_key);
  // This section may not have been created.
  if (status == 0)
    {
      while (this->repo_->config ()->enumerate_sections (refs_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key member_key;
          this->repo_->config ()->open_section (refs_key,
                                                section_name.c_str (),
                                                0,
                                                member_key);

          ACE_TString path;
          this->repo_->config ()->get_string_value (member_key,
                                                    "path",
                                                    path);

          ACE_Configuration_Section_Key entry_key;
          status =
            this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                 path,
                                                 entry_key,
                                                 0);

          // This entry may already have been destroyed.
          if (status == 0)
            {
              TAO_IDLType_i *impl = 
                TAO_IFR_Service_Utils::path_to_idltype (path,
                                                        this->repo_);

              CORBA::DefinitionKind def_kind = 
                impl->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              switch (def_kind)
              {
                // These anonymous types were defined in IDL inside this
                // struct and have no 'contained-in' info of their own,
                // so they must be destroyed here.
                case CORBA::dk_String:
                case CORBA::dk_Wstring:
                case CORBA::dk_Fixed:
                case CORBA::dk_Array:
                case CORBA::dk_Sequence:
                  impl->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_CHECK;
                  break;
                default:
                  // No other types can be defined in this section of the struct.
                  break;
              }
            }
        }

      this->repo_->config ()->remove_section (this->section_key_,
                                              "refs",
                                              1);
    }

  ACE_TString id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            id);

  ACE_TString my_path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.c_str (),
                                            my_path);

  // Destroy definitions.

  ACE_Configuration_Section_Key defns_key;
  status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "defns",
                                          0,
                                          defns_key);

  // This section may not have been created.
  if (status == 0)
    {
      index = 0;

      while (this->repo_->config ()->enumerate_sections (defns_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key defn_key;
          this->repo_->config ()->open_section (defns_key,
                                                section_name.c_str (),
                                                0,
                                                defn_key);
          u_int kind = 0;
          this->repo_->config ()->get_integer_value (defn_key,
                                                     "def_kind",
                                                     kind);
          CORBA::DefinitionKind def_kind =
            ACE_static_cast (CORBA::DefinitionKind, kind);

          TAO_Contained_i *impl = this->repo_->select_contained (def_kind);
          impl->section_key (defn_key);

          impl->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->repo_->config ()->remove_section (this->section_key_,
                                              "defns",
                                              1);
    }
}

CORBA::Contained_ptr
TAO_Container_i::lookup (const char *search_name
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Contained::_nil ());

  return this->lookup_i (search_name
                         ACE_ENV_ARG_PARAMETER);
}

CORBA::Contained_ptr
TAO_Container_i::lookup_i (const char *search_name
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key iter_key;
  ACE_Configuration_Section_Key work_key;
  ACE_TString work_string;
  ACE_TString section_name;
  int status = 0;
  int index = 0;
  int pos = 0;
  int so_far_so_good = 0;
  u_int kind = 0;
  CORBA::DefinitionKind def_kind = CORBA::dk_none;


  if (ACE_OS::strncmp (search_name, "::", 2) == 0)
    {
      // We were given an absolute scoped name.
      work_string = search_name + 2;
      iter_key = this->repo_->root_key ();
    }
  else
    {
      // We were given a relative scoped name.
      work_string = search_name;
      iter_key = this->section_key_;
    }

  while (1)
    {
      pos = work_string.find ("::");
      so_far_so_good = 0;

      ACE_Configuration_Section_Key defns_key;
      status =
        this->repo_->config ()->open_section (iter_key,
                                              "defns",
                                              0,
                                              defns_key);
      if (status == 0)
        {
          index = 0;

          while (this->repo_->config ()->enumerate_sections (defns_key,
                                                             index++,
                                                             section_name)
                  == 0)
            {
              this->repo_->config ()->open_section (defns_key,
                                                    section_name.c_str (),
                                                    0,
                                                    work_key);

              ACE_TString defn_name;
              this->repo_->config ()->get_string_value (work_key,
                                                        "name",
                                                        defn_name);

              if (defn_name == work_string.substr (0, pos))
                {
                  iter_key = work_key;
                  so_far_so_good = 1;
                  break;
                }
            }
        }

      if (so_far_so_good)
        {
          if (pos != ACE_TString::npos)
            {
              work_string = work_string.substr (pos + 2);
              continue;
            }
          else
            {
              break;
            }
        }
      else
        {
          // Check for attributes and operations, but only if we
          // are on the last segment of the scoped name.
          if (pos == ACE_TString::npos)
            {
              this->repo_->config ()->get_integer_value (iter_key,
                                                         "def_kind",
                                                         kind);

              def_kind = ACE_static_cast (CORBA::DefinitionKind, kind);

              if (def_kind == CORBA::dk_Interface
                  || def_kind == CORBA::dk_Value)
                {
                  ACE_Configuration_Section_Key attrs_key;
                  status =
                    this->repo_->config ()->open_section (iter_key,
                                                          "attrs",
                                                          0,
                                                          attrs_key);

                  if (status == 0)
                    {
                      index = 0;
                      while (this->repo_->config ()->enumerate_sections (
                                                         attrs_key,
                                                         index++,
                                                         section_name
                                                       )
                              == 0)
                        {
                          this->repo_->config ()->open_section (
                                                      attrs_key,
                                                      section_name.c_str (),
                                                      0,
                                                      work_key
                                                    );

                          ACE_TString attr_name;
                          this->repo_->config ()->get_string_value (
                                                      work_key,
                                                      "name",
                                                      attr_name
                                                    );

                          // If we're here, working name has only one segment.
                          if (attr_name == work_string)
                            {
                              so_far_so_good = 1;
                              break;
                            }
                        }
                    }

                  if (so_far_so_good)
                    {
                      break;
                    }
                  else
                    {
                      ACE_Configuration_Section_Key ops_key;
                      status =
                        this->repo_->config ()->open_section (iter_key,
                                                              "ops",
                                                              0,
                                                              ops_key);

                      if (status == 0)
                        {
                          index = 0;

                          while (this->repo_->config ()->enumerate_sections (
                                                             ops_key,
                                                             index++,
                                                             section_name
                                                           )
                                  == 0)
                            {
                              this->repo_->config ()->open_section (
                                  ops_key,
                                  section_name.c_str (),
                                  0,
                                  work_key
                               );

                              ACE_TString op_name;
                              this->repo_->config ()->get_string_value (
                                                          work_key,
                                                          "name",
                                                          op_name
                                                        );

                              // If we're here, name has only one segment.
                              if (op_name == work_string)
                                {
                                  so_far_so_good = 1;
                                  break;
                                }
                            }

                          if (so_far_so_good)
                            {
                              break;
                            }
                        }
                    }
                }
            }
        }

      return CORBA::Contained::_nil ();
    }

  ACE_TString id;
  this->repo_->config ()->get_string_value (work_key,
                                            "id",
                                            id);

  ACE_TString path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.c_str (),
                                            path);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (path,
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Contained::_nil ());

  return CORBA::Contained::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ContainedSeq *
TAO_Container_i::contents (CORBA::DefinitionKind limit_type,
                           CORBA::Boolean exclude_inherited
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->contents_i (limit_type,
                           exclude_inherited
                           ACE_ENV_ARG_PARAMETER);
}

CORBA::ContainedSeq *
TAO_Container_i::contents_i (CORBA::DefinitionKind limit_type,
                             CORBA::Boolean exclude_inherited
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ContainedSeq *contents = 0;
  ACE_NEW_THROW_EX (contents,
                    CORBA::ContainedSeq,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ContainedSeq_var retval = contents;
  retval->length (0);

  if (limit_type == CORBA::dk_none)
    {
      return retval._retn ();
    }

  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  // Definitions

  ACE_Configuration_Section_Key defns_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "defns",
                                          0,
                                          defns_key);

  // If there are no contents (other than possible attributes or
  // operations), skip this part.
  if (status == 0)
    {
      u_int count = 0;
      this->repo_->config ()->get_integer_value (defns_key,
                                                 "count",
                                                 count);

      for (u_int i = 0; i < count; ++i)
        {
          ACE_Configuration_Section_Key defn_key;
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          status =
            this->repo_->config ()->open_section (defns_key,
                                                  stringified,
                                                  0,
                                                  defn_key);

          if (status == 0)
            {
              u_int kind = 0;
              this->repo_->config ()->get_integer_value (defn_key,
                                                         "def_kind",
                                                         kind);

              CORBA::DefinitionKind def_kind =
                ACE_static_cast (CORBA::DefinitionKind, kind);

              if (limit_type == CORBA::dk_all
                  || limit_type == def_kind)
                {
                  kind_queue.enqueue_tail (def_kind);

                  ACE_TString id;
                  this->repo_->config ()->get_string_value (defn_key,
                                                            "id",
                                                            id);

                  ACE_TString path;
                  this->repo_->config ()->get_string_value (
                                              this->repo_->repo_ids_key (),
                                              id.c_str (),
                                              path
                                            );

                  path_queue.enqueue_tail (path);
                }
            }
        }
    }

  // Base interfaces

  CORBA::DefinitionKind def_kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (def_kind == CORBA::dk_Interface)
    {
      if (limit_type == CORBA::dk_Operation
          || limit_type == CORBA::dk_Attribute
          || limit_type == CORBA::dk_all)
        {
          TAO_InterfaceDef_i iface (this->repo_);
          iface.section_key (this->section_key_);

          iface.interface_contents (kind_queue,
                                    path_queue,
                                    limit_type,
                                    exclude_inherited
                                    ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, kind_queue.size ());
  retval->length (size);

  for (CORBA::ULong j = 0; j < size; ++j)
    {
      CORBA::DefinitionKind next_kind;
      kind_queue.dequeue_head (next_kind);

      ACE_TString next_path;
      path_queue.dequeue_head (next_path);

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::create_objref (next_kind,
                                              next_path.c_str (),
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Contained_var next_cont =
        CORBA::Contained::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[j] = next_cont._retn ();
    }

  return retval._retn ();
}

CORBA::ContainedSeq *
TAO_Container_i::lookup_name (const char *search_name,
                              CORBA::Long levels_to_search,
                              CORBA::DefinitionKind limit_type,
                              CORBA::Boolean exclude_inherited
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->lookup_name_i (search_name,
                              levels_to_search,
                              limit_type,
                              exclude_inherited
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::ContainedSeq *
TAO_Container_i::lookup_name_i (const char *search_name,
                                CORBA::Long levels_to_search,
                                CORBA::DefinitionKind limit_type,
                                CORBA::Boolean exclude_inherited
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<CORBA::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->lookup_name_recursive (kind_queue,
                               path_queue,
                               search_name,
                               levels_to_search,
                               limit_type,
                               exclude_inherited
                               ACE_ENV_ARG_PARAMETER);

  CORBA::ULong size = ACE_static_cast (CORBA::ULong, kind_queue.size ());

  CORBA::ContainedSeq *holder;
  ACE_NEW_THROW_EX (holder,
                    CORBA::ContainedSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ContainedSeq_var retval = holder;
  retval->length (size);

  for (CORBA::ULong i = 0; i < size; ++i)
    {
      CORBA::DefinitionKind next_kind;
      kind_queue.dequeue_head (next_kind);

      ACE_TString next_path;
      path_queue.dequeue_head (next_path);

      CORBA::Object_var obj =
        TAO_IFR_Service_Utils::create_objref (next_kind,
                                              next_path.c_str (),
                                              this->repo_
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Contained_var next_cont =
        CORBA::Contained::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i] = next_cont._retn ();
    }

  return retval._retn ();
}

CORBA::Container::DescriptionSeq *
TAO_Container_i::describe_contents (CORBA::DefinitionKind limit_type,
                                    CORBA::Boolean exclude_inherited,
                                    CORBA::Long max_returned_objs
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_contents_i (limit_type,
                                    exclude_inherited,
                                    max_returned_objs
                                    ACE_ENV_ARG_PARAMETER);
}

CORBA::Container::DescriptionSeq *
TAO_Container_i::describe_contents_i (CORBA::DefinitionKind limit_type,
                                      CORBA::Boolean exclude_inherited,
                                      CORBA::Long max_returned_objs
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ContainedSeq_var contents = this->contents_i (limit_type,
                                                      exclude_inherited
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = contents->length ();
  CORBA::ULong ret_len = 0;

  if (max_returned_objs == -1)
    {
      ret_len = length;
    }
  else
    {
      ret_len = length < ACE_static_cast (CORBA::ULong, max_returned_objs)
                ? length
                : max_returned_objs;
    }

  CORBA::Container::DescriptionSeq *desc_seq;
  ACE_NEW_THROW_EX (desc_seq,
                    CORBA::Container::DescriptionSeq (ret_len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  desc_seq->length (ret_len);
  CORBA::Container::DescriptionSeq_var retval = desc_seq;
  CORBA::Contained::Description_var desc;

  ACE_Configuration_Section_Key contained_key;
  PortableServer::ObjectId_var oid;
  CORBA::String_var tmp;
  TAO_Contained_i *impl = 0;

  for (CORBA::ULong i = 0; i < ret_len; ++i)
    {
      ACE_TString contained_path (
          TAO_IFR_Service_Utils::reference_to_path (contents[i])
        );
      impl = TAO_IFR_Service_Utils::path_to_contained (contained_path,
                                                       this->repo_);

      desc = impl->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      retval[i].contained_object = CORBA::Contained::_duplicate (contents[i]);

      retval[i].kind = desc->kind;
      retval[i].value = desc->value;
    }

  return retval._retn ();
}

CORBA::ModuleDef_ptr
TAO_Container_i::create_module (const char *id,
                                const char *name,
                                const char *version
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ModuleDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ModuleDef::_nil ());

  return this->create_module_i (id,
                                name,
                                version
                                ACE_ENV_ARG_PARAMETER);
}

CORBA::ModuleDef_ptr
TAO_Container_i::create_module_i (const char *id,
                                  const char *name,
                                  const char *version
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // new_key is not needed when creating a module, but it is needed
  // by most of the other creation methods, so a blank one gets passed in.
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Module,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ModuleDef::_nil ());

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Module,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ModuleDef::_nil ());

  return CORBA::ComponentIR::ModuleDef::_narrow (obj.in ()
                                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::ConstantDef_ptr
TAO_Container_i::create_constant (const char *id,
                                  const char *name,
                                  const char *version,
                                  CORBA::IDLType_ptr type,
                                  const CORBA::Any &value
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ConstantDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ConstantDef::_nil ());

  return this->create_constant_i (id,
                                  name,
                                  version,
                                  type,
                                  value
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::ConstantDef_ptr
TAO_Container_i::create_constant_i (const char *id,
                                    const char *name,
                                    const char *version,
                                    CORBA::IDLType_ptr type,
                                    const CORBA::Any &value
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Constant,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ConstantDef::_nil ());

  // Get the path to our type and store it.
  char *type_path = TAO_IFR_Service_Utils::reference_to_path (type);

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path);

  // Store the value.
  ACE_Message_Block *mb = value._tao_get_cdr ();
  CORBA::TypeCode_var val_tc = value.type ();

  CORBA::TCKind kind = val_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ConstantDef::_nil ());

  switch (kind)
  {
    // The data for these types requires alignment to an 8-byte
    // boundary - the default for TAO CDR streams is 4.
    case CORBA::tk_double:
    case CORBA::tk_ulonglong:
    case CORBA::tk_longlong:
    case CORBA::tk_longdouble:
      mb->rd_ptr (ACE_ptr_align_binary (mb->rd_ptr (),
                                        ACE_CDR::MAX_ALIGNMENT));
      break;
    default:
      break;
  }

  // Align the read pointer with the base, so mb->length()
  // will be meaningful.
  mb->crunch ();

  this->repo_->config ()->set_binary_value (new_key,
                                            "value",
                                            mb->base (),
                                            mb->length ());

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Constant,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ConstantDef::_nil ());

  return CORBA::ConstantDef::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::StructDef_ptr
TAO_Container_i::create_struct (const char *id,
                                const char *name,
                                const char *version,
                                const CORBA::StructMemberSeq &members
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::StructDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::StructDef::_nil ());

  return this->create_struct_i (id,
                                name,
                                version,
                                members
                                ACE_ENV_ARG_PARAMETER);
}

CORBA::StructDef_ptr
TAO_Container_i::create_struct_i (const char *id,
                                  const char *name,
                                  const char *version,
                                  const CORBA::StructMemberSeq &members
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Struct,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::StructDef::_nil ());

  // Open a section for the struct's members.
  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (new_key,
                                        "refs",
                                        1,
                                        refs_key);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);
  char *member_path = 0;

  // Create a section for each member. We just store the
  // member name and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            stringified,
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].name.in ());

      member_path = 
        TAO_IFR_Service_Utils::reference_to_path (members[i].type_def.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path);
    }

  CORBA::DefinitionKind def_kind = 
    this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::StructDef::_nil ());

  if (def_kind == CORBA::dk_Struct
      || def_kind == CORBA::dk_Union
      || def_kind == CORBA::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Struct,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::StructDef::_nil ());

  return CORBA::StructDef::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::UnionDef_ptr
TAO_Container_i::create_union (const char *id,
                               const char *name,
                               const char *version,
                               CORBA::IDLType_ptr discriminator_type,
                               const CORBA::UnionMemberSeq &members
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::UnionDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::UnionDef::_nil ());

  return this->create_union_i (id,
                               name,
                               version,
                               discriminator_type,
                               members
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::UnionDef_ptr
TAO_Container_i::create_union_i (const char *id,
                                 const char *name,
                                 const char *version,
                                 CORBA::IDLType_ptr discriminator_type,
                                 const CORBA::UnionMemberSeq &members
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Union,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::UnionDef::_nil ());

  // Add a field to hold the path to the discriminator type.
  char *disc_path = 
    TAO_IFR_Service_Utils::reference_to_path (discriminator_type);

  this->repo_->config ()->set_string_value (new_key,
                                            "disc_path",
                                            disc_path);

  ACE_Configuration_Section_Key refs_key;

  // For the union's members.
  this->repo_->config ()->open_section (new_key,
                                        "refs",
                                        1,
                                        refs_key);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);
  char *member_path = 0;

  // Create a section for each member. We store the member
  // name, its label value, and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            stringified,
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].name.in ());

      member_path =
        TAO_IFR_Service_Utils::reference_to_path (members[i].type_def.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path);

      this->store_label (member_key,
                         members[i].label
                         ACE_ENV_ARG_PARAMETER);
    }

  CORBA::DefinitionKind def_kind = 
    this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::UnionDef::_nil ());

  if (def_kind == CORBA::dk_Struct
      || def_kind == CORBA::dk_Union
      || def_kind == CORBA::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Union,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::UnionDef::_nil ());

  return CORBA::UnionDef::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::EnumDef_ptr
TAO_Container_i::create_enum (const char *id,
                              const char *name,
                              const char *version,
                              const CORBA::EnumMemberSeq &members
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::EnumDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::EnumDef::_nil ());

  return this->create_enum_i (id,
                              name,
                              version,
                              members
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::EnumDef_ptr
TAO_Container_i::create_enum_i (const char *id,
                                const char *name,
                                const char *version,
                                const CORBA::EnumMemberSeq &members
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Enum,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::EnumDef::_nil ());

  // Store the member count of the enum.
  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (new_key,
                                             "count",
                                             count);

  // Create a section for each member, named for
  // its ordinal position in the enum,
  for (CORBA::ULong i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (new_key,
                                            stringified,
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].in ());
    }

  CORBA::DefinitionKind def_kind = 
    this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::EnumDef::_nil ());

  if (def_kind == CORBA::dk_Struct
      || def_kind == CORBA::dk_Union
      || def_kind == CORBA::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Enum,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::EnumDef::_nil ());

  return CORBA::EnumDef::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::AliasDef_ptr
TAO_Container_i::create_alias (const char *id,
                               const char *name,
                               const char *version,
                               CORBA::IDLType_ptr original_type
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AliasDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AliasDef::_nil ());

  return this->create_alias_i (id,
                               name,
                               version,
                               original_type
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::AliasDef_ptr
TAO_Container_i::create_alias_i (const char *id,
                                 const char *name,
                                 const char *version,
                                 CORBA::IDLType_ptr original_type
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Alias,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AliasDef::_nil ());

  // Get the path to our original type and store it.
  char *unalias_path = 
    TAO_IFR_Service_Utils::reference_to_path (original_type);

  this->repo_->config ()->set_string_value (new_key,
                                            "original_type",
                                            unalias_path);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Alias,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AliasDef::_nil ());

  return CORBA::AliasDef::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::InterfaceDef_ptr
TAO_Container_i::create_interface (const char *id,
                                   const char *name,
                                   const char *version,
                                   const CORBA::InterfaceDefSeq &base_interfaces
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::InterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return this->create_interface_i (id,
                                   name,
                                   version,
                                   base_interfaces
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::InterfaceDef_ptr
TAO_Container_i::create_interface_i (const char *id,
                                     const char *name,
                                     const char *version,
                                     const CORBA::InterfaceDefSeq &base_interfaces
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Interface,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  CORBA::ULong length = base_interfaces.length ();

  if (length > 0)
    {
      // Open a section for the base interfaces.
      ACE_Configuration_Section_Key inherited_key;
      this->repo_->config ()->open_section (new_key,
                                            "inherited",
                                            1,
                                            inherited_key);
      char *inherited_path = 0;

      // Store the path to each base interface.
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          inherited_path = 
            TAO_IFR_Service_Utils::reference_to_path (
                base_interfaces[i].in ()
              );
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (inherited_key,
                                                    stringified,
                                                    inherited_path);
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Interface,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::InterfaceDef::_nil ());

  return CORBA::InterfaceDef::_narrow (obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueDef_ptr
TAO_Container_i::create_value (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::InitializerSeq &initializers
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ValueDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueDef::_nil ());

  return this->create_value_i (id,
                               name,
                               version,
                               is_custom,
                               is_abstract,
                               base_value,
                               is_truncatable,
                               abstract_base_values,
                               supported_interfaces,
                               initializers
                               ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueDef_ptr
TAO_Container_i::create_value_i (
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces,
    const CORBA::InitializerSeq &initializers
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    this->create_value_common (this->def_kind (),
                               this->section_key_,
                               new_key,
                               id,
                               name,
                               version,
                               is_custom,
                               is_abstract,
                               base_value,
                               is_truncatable,
                               abstract_base_values,
                               supported_interfaces
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueDef::_nil ());

  TAO_IFR_Generic_Utils<CORBA::InitializerSeq>::set_initializers (
      initializers,
      this->repo_->config (),
      new_key
    );

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Value,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueDef::_nil ());

  return CORBA::ValueDef::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueBoxDef_ptr
TAO_Container_i::create_value_box (const char *id,
                                   const char *name,
                                   const char *version,
                                   CORBA::IDLType_ptr original_type_def
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ValueBoxDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueBoxDef::_nil ());

  return this->create_value_box_i (id,
                                   name,
                                   version,
                                   original_type_def
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueBoxDef_ptr
TAO_Container_i::create_value_box_i (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::IDLType_ptr original_type_def
                                     ACE_ENV_ARG_DECL )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_ValueBox,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueBoxDef::_nil ());

  // Get the path to our original type and store it.
  char *boxed_path = 
    TAO_IFR_Service_Utils::reference_to_path (original_type_def);

  this->repo_->config ()->set_string_value (new_key,
                                            "boxed_type",
                                            boxed_path);

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_ValueBox,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ValueBoxDef::_nil ());

  return CORBA::ValueBoxDef::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
}

CORBA::ExceptionDef_ptr
TAO_Container_i::create_exception (const char *id,
                                   const char *name,
                                   const char *version,
                                   const CORBA::StructMemberSeq &members
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExceptionDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExceptionDef::_nil ());

  return this->create_exception_i (id,
                                   name,
                                   version,
                                   members
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ExceptionDef_ptr
TAO_Container_i::create_exception_i (const char *id,
                                     const char *name,
                                     const char *version,
                                     const CORBA::StructMemberSeq &members
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Exception,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExceptionDef::_nil ());

  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (new_key,
                                        "refs",
                                        1,
                                        refs_key);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);
  char *member_path = 0;

  // Create a section for each member. We just store the
  // member name and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; ++i)
    {
      ACE_Configuration_Section_Key member_key;
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            stringified,
                                            1,
                                            member_key);

      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                members[i].name.in ());

      member_path = 
        TAO_IFR_Service_Utils::reference_to_path (members[i].type_def.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Exception,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExceptionDef::_nil ());

  return CORBA::ExceptionDef::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::NativeDef_ptr
TAO_Container_i::create_native (const char *id,
                                const char *name,
                                const char *version
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::NativeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::NativeDef::_nil ());

  return this->create_native_i (id,
                                name,
                                version
                                ACE_ENV_ARG_PARAMETER);
}

CORBA::NativeDef_ptr
TAO_Container_i::create_native_i (const char *id,
                                  const char *name,
                                  const char *version
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_Native,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::NativeDef::_nil ());

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Native,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::NativeDef::_nil ());

  return CORBA::NativeDef::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::AbstractInterfaceDef_ptr
TAO_Container_i::create_abstract_interface (
    const char *id,
    const char *name,
    const char *version,
    const CORBA::AbstractInterfaceDefSeq &base_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AbstractInterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AbstractInterfaceDef::_nil ());

  return this->create_abstract_interface_i (id,
                                            name,
                                            version,
                                            base_interfaces
                                            ACE_ENV_ARG_PARAMETER);
}

CORBA::AbstractInterfaceDef_ptr
TAO_Container_i::create_abstract_interface_i (
    const char *id,
    const char *name,
    const char *version,
    const CORBA::AbstractInterfaceDefSeq &base_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_AbstractInterface,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AbstractInterfaceDef::_nil ());

  CORBA::ULong length = base_interfaces.length ();

  if (length > 0)
    {
      // Open a section for the base interfaces.
      ACE_Configuration_Section_Key inherited_key;

      this->repo_->config ()->open_section (new_key,
                                            "inherited",
                                            1,
                                            inherited_key);
      char *inherited_path = 0;

      // Store the path to each base interface.
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          inherited_path = 
            TAO_IFR_Service_Utils::reference_to_path (base_interfaces[i]);

          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (inherited_key,
                                                    stringified,
                                                    inherited_path);
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_AbstractInterface,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::AbstractInterfaceDef::_nil ());

  return CORBA::AbstractInterfaceDef::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
}

CORBA::LocalInterfaceDef_ptr
TAO_Container_i::create_local_interface (
    const char *id,
    const char *name,
    const char *version,
    const CORBA::InterfaceDefSeq &base_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::LocalInterfaceDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::LocalInterfaceDef::_nil ());

  return this->create_local_interface_i (id,
                                         name,
                                         version,
                                         base_interfaces
                                         ACE_ENV_ARG_PARAMETER);
}

CORBA::LocalInterfaceDef_ptr
TAO_Container_i::create_local_interface_i (
    const char *id,
    const char *name,
    const char *version,
    const CORBA::InterfaceDefSeq &base_interfaces
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = 
    TAO_IFR_Service_Utils::create_common (this->def_kind (),
                                          CORBA::dk_LocalInterface,
                                          this->section_key_,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::LocalInterfaceDef::_nil ());

  CORBA::ULong length = base_interfaces.length ();

  if (length > 0)
    {
      // Open a section for the base interfaces.
      ACE_Configuration_Section_Key inherited_key;

      this->repo_->config ()->open_section (new_key,
                                            "inherited",
                                            1,
                                            inherited_key);
      char *inherited_path = 0;

      // Store the path to each base interface.
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          inherited_path =
            TAO_IFR_Service_Utils::reference_to_path (base_interfaces[i]);

          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (inherited_key,
                                                    stringified,
                                                    inherited_path);
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_LocalInterface,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::LocalInterfaceDef::_nil ());

  return CORBA::LocalInterfaceDef::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

CORBA::ExtValueDef_ptr 
TAO_Container_i::create_ext_value (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces,
      const CORBA::ExtInitializerSeq &initializers
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExtValueDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtValueDef::_nil ());

  return this->create_ext_value_i (id,
                                   name,
                                   version,
                                   is_custom,
                                   is_abstract,
                                   base_value,
                                   is_truncatable,
                                   abstract_base_values,
                                   supported_interfaces,
                                   initializers
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ExtValueDef_ptr 
TAO_Container_i::create_ext_value_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces,
      const CORBA::ExtInitializerSeq &initializers
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    this->create_value_common (this->def_kind (),
                               this->section_key_,
                               new_key,
                               id,
                               name,
                               version,
                               is_custom,
                               is_abstract,
                               base_value,
                               is_truncatable,
                               abstract_base_values,
                               supported_interfaces
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtValueDef::_nil ());

  /// This does not handle the initializer exceptions, so we do that below.
  TAO_IFR_Generic_Utils<CORBA::ExtInitializerSeq>::set_initializers (
      initializers,
      this->repo_->config (),
      this->section_key_
    );

  CORBA::ULong length = initializers.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key initializers_key, initializer_key;
      char *stringified = 0;

      this->repo_->config ()->open_section (new_key,
                                            "initializers",
                                            0,
                                            initializers_key);

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->open_section (initializers_key,
                                                stringified,
                                                0,
                                                initializer_key);

          TAO_ExtValueDef_i impl (this->repo_);
          impl.section_key (new_key);
          impl.exceptions (initializer_key,
                           "excepts",
                           initializers[i].exceptions);
        }
    }
    
  // Create the object reference.
  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::create_objref (CORBA::dk_Value,
                                          path.c_str (),
                                          this->repo_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtValueDef::_nil ());

  return CORBA::ExtValueDef::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
}

int
TAO_Container_i::same_as_tmp_name (const char *name)
{
  return ACE_OS::strcmp (TAO_Container_i::tmp_name_holder_, name) == 0;
}

void
TAO_Container_i::tmp_name_holder (const char *name)
{
  TAO_Container_i::tmp_name_holder_ = name;
}

void
TAO_Container_i::lookup_name_recursive (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL
    )
{
  ACE_Configuration_Section_Key defns_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "defns",
                                          0,
                                          defns_key);

  if (status == 0)
    {
      int index = 0;
      ACE_TString section_name;
      while (this->repo_->config ()->enumerate_sections (defns_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key defn_key;
          this->repo_->config ()->open_section (defns_key,
                                                section_name.c_str (),
                                                0,
                                                defn_key);

          u_int kind = 0;
          this->repo_->config ()->get_integer_value (defn_key,
                                                     "def_kind",
                                                     kind);

          CORBA::DefinitionKind def_kind =
            ACE_static_cast (CORBA::DefinitionKind, kind);

          ACE_TString id;
          this->repo_->config ()->get_string_value (defn_key,
                                                    "id",
                                                    id);

          ACE_TString path;
          this->repo_->config ()->get_string_value (
                                      this->repo_->repo_ids_key (),
                                      id.c_str (),
                                      path
                                    );

          if (limit_type == CORBA::dk_all || limit_type == def_kind)
            {
              ACE_TString name;
              this->repo_->config ()->get_string_value (defn_key,
                                                        "name",
                                                        name);

              if (ACE_OS::strcmp (name.c_str (), search_name) == 0)
                {
                  kind_queue.enqueue_tail (def_kind);

                  path_queue.enqueue_tail (path);
                }
            }

          if (levels_to_search == -1 || levels_to_search > 1)
            {
              TAO_Container_i *impl = 
                TAO_IFR_Service_Utils::path_to_container (path,
                                                          this->repo_);

              // This member may not be a container.
              if (impl == 0)
                {
                  continue;
                }

              impl->lookup_name_recursive (kind_queue,
                                           path_queue,
                                           search_name,
                                           levels_to_search == -1
                                              ? -1
                                              : levels_to_search - 1,
                                           limit_type,
                                           exclude_inherited
                                           ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
    }

  CORBA::DefinitionKind kind = this->def_kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (kind == CORBA::dk_Interface || kind == CORBA::dk_Value)
    {
      if (limit_type == CORBA::dk_all || limit_type == CORBA::dk_Attribute)
        {
          this->lookup_attr (kind_queue,
                             path_queue,
                             search_name,
                             exclude_inherited);
        }

      if (limit_type == CORBA::dk_all || limit_type == CORBA::dk_Operation)
        {
          this->lookup_op (kind_queue,
                           path_queue,
                           search_name,
                           exclude_inherited);
        }
    }
}

void
TAO_Container_i:: lookup_attr (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    )
{
  int index = 0;
  ACE_TString section_name;

  ACE_Configuration_Section_Key sub_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "attrs",
                                          0,
                                          sub_key);

  if (status == 0)
    {
      while (this->repo_->config ()->enumerate_sections (sub_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key member_key;
          this->repo_->config ()->open_section (sub_key,
                                                section_name.c_str (),
                                                0,
                                                member_key);

          ACE_TString name;
          this->repo_->config ()->get_string_value (member_key,
                                                    "name",
                                                    name);

          if (name == search_name)
            {
              kind_queue.enqueue_tail (CORBA::dk_Attribute);

              ACE_TString id;
              this->repo_->config ()->get_string_value (member_key,
                                                        "id",
                                                        id);

              ACE_TString path;
              this->repo_->config ()->get_string_value (
                                          this->repo_->repo_ids_key (),
                                          id.c_str (),
                                          path
                                        );

              path_queue.enqueue_tail (path);
            }
        }
    }

  if (exclude_inherited == 0)
    {
      ACE_Configuration_Section_Key bases_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "inherited",
                                              0,
                                              bases_key);

      if (status == 0)
        {
          ACE_Configuration::VALUETYPE type;
          index = 0;
          while (this->repo_->config ()->enumerate_values (
                                             bases_key,
                                             index++,
                                             section_name,
                                             type
                                           )
                  == 0)
            {
              ACE_TString path;
              this->repo_->config ()->get_string_value (
                                          bases_key,
                                          section_name.c_str (),
                                          path);

              ACE_Configuration_Section_Key base_key;
              this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                   path,
                                                   base_key,
                                                   0);

              TAO_InterfaceDef_i impl (this->repo_);
              impl.section_key (base_key);

              impl.lookup_attr (kind_queue,
                                path_queue,
                                search_name,
                                exclude_inherited);
            }
        }
    }
}

void
TAO_Container_i::lookup_op (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    )
{
  int index = 0;
  ACE_TString section_name;

  ACE_Configuration_Section_Key sub_key;
  int status =
    this->repo_->config ()->open_section (this->section_key_,
                                          "ops",
                                          0,
                                          sub_key);

  if (status == 0)
    {
      while (this->repo_->config ()->enumerate_sections (sub_key,
                                                         index++,
                                                         section_name)
              == 0)
        {
          ACE_Configuration_Section_Key member_key;
          this->repo_->config ()->open_section (sub_key,
                                                section_name.c_str (),
                                                0,
                                                member_key);

          ACE_TString name;
          this->repo_->config ()->get_string_value (member_key,
                                                    "name",
                                                    name);

          if (name == search_name)
            {
              kind_queue.enqueue_tail (CORBA::dk_Operation);

              ACE_TString id;
              this->repo_->config ()->get_string_value (member_key,
                                                        "id",
                                                        id);

              ACE_TString path;
              this->repo_->config ()->get_string_value (
                                          this->repo_->repo_ids_key (),
                                          id.c_str (),
                                          path
                                        );

              path_queue.enqueue_tail (path);
            }
        }
    }

  if (exclude_inherited == 0)
    {
      ACE_Configuration_Section_Key bases_key;
      status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "inherited",
                                              0,
                                              bases_key);

      if (status == 0)
        {
          ACE_Configuration::VALUETYPE type;
          index = 0;

          while (this->repo_->config ()->enumerate_values (
                                             bases_key,
                                             index++,
                                             section_name,
                                             type
                                           )
                  == 0)
            {
              ACE_TString path;
              this->repo_->config ()->get_string_value (
                                          bases_key,
                                          section_name.c_str (),
                                          path);

              ACE_Configuration_Section_Key base_key;
              this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                   path,
                                                   base_key,
                                                   0);

              TAO_InterfaceDef_i impl (this->repo_);
              impl.section_key (base_key);

              impl.lookup_op (kind_queue,
                                path_queue,
                                search_name,
                                exclude_inherited);
            }
        }
    }
}

void
TAO_Container_i::store_label (ACE_Configuration_Section_Key key,
                              const CORBA::Any &value
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var tc = value.type ();

  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_InputCDR cdr (value._tao_get_cdr (),
                    value._tao_byte_order ());

  u_int result = 0;
  int default_label = 0;

  switch (kind)
  {
    case CORBA::tk_octet:
      default_label = 1;
      break;
    case CORBA::tk_char:
    {
      CORBA::Char x;
      cdr.read_char (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_wchar:
    {
      CORBA::WChar x;
      cdr.read_wchar (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_boolean:
    {
      CORBA::Boolean x;
      cdr.read_boolean (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_short:
    {
      CORBA::Short x;
      cdr.read_short (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_ushort:
    {
      CORBA::UShort x;
      cdr.read_ushort (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_long:
    {
      CORBA::Long x;
      cdr.read_long (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_ulong:
    {
      CORBA::ULong x;
      cdr.read_ulong (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
#if !defined (ACE_LACKS_LONGLONG_T)
    case CORBA::tk_longlong:
    {
      CORBA::LongLong x;
      cdr.read_longlong (x);
      // We could lose data here.
      result = ACE_static_cast (u_int, x);
      break;
    }
#endif /* ACE_LACKS_LONGLONG_T */
    case CORBA::tk_ulonglong:
    {
      CORBA::ULongLong x;
      cdr.read_ulonglong (x);
      // We could lose data here.
      result = ACE_static_cast (u_int, x);
      break;
    }
    case CORBA::tk_enum:
    {
      CORBA::ULong x;
      cdr.read_ulong (x);
      result = ACE_static_cast (u_int, x);
      break;
    }
    default:
      break;
  }

  if (default_label == 1)
    {
      this->repo_->config ()->set_string_value (key,
                                                "label",
                                                "default");
    }
  else
    {
      this->repo_->config ()->set_integer_value (key,
                                                 "label",
                                                 result);
    }
}

void
TAO_Container_i::update_refs (const char *path,
                              const char *name)
{
  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "refs",
                                        1,
                                        refs_key);

  u_int count = 0;
  this->repo_->config ()->get_integer_value (refs_key,
                                             "count",
                                             count);

  int index = 0;
  ACE_TString section_name;
  while (this->repo_->config ()->enumerate_sections (refs_key,
                                                     index++,
                                                     section_name)
          == 0)
    {
      ACE_Configuration_Section_Key ref_key;
      this->repo_->config ()->open_section (refs_key,
                                            section_name.c_str (),
                                            0,
                                            ref_key);

      ACE_TString ref_name;
      this->repo_->config ()->get_string_value (ref_key,
                                                "name",
                                                ref_name);

      // If one of the names has been mangled by move(), fix it.
      if (ref_name.find (this->repo_->extension ()) != ACE_TString::npos)
        {
          this->repo_->config ()->set_string_value (ref_key,
                                                    "name",
                                                    name);

          this->repo_->config ()->set_string_value (ref_key,
                                                    "path",
                                                    path);

          return;
        }
    }

  // Add a new reference.
  ACE_Configuration_Section_Key new_key;
  char *stringified = TAO_IFR_Service_Utils::int_to_string (count);
  this->repo_->config ()->open_section (refs_key,
                                        stringified,
                                        1,
                                        new_key);

  this->repo_->config ()->set_string_value (new_key,
                                            "name",
                                            name);

  this->repo_->config ()->set_string_value (new_key,
                                            "path",
                                            path);

  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count + 1);
}

ACE_TString
TAO_Container_i::create_value_common (
    CORBA::DefinitionKind container_kind,
    ACE_Configuration_Section_Key container_key,
    ACE_Configuration_Section_Key new_key,
    const char *id,
    const char *name,
    const char *version,
    CORBA::Boolean is_custom,
    CORBA::Boolean is_abstract,
    CORBA::ValueDef_ptr base_value,
    CORBA::Boolean is_truncatable,
    const CORBA::ValueDefSeq &abstract_base_values,
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
{
  TAO_Container_i::tmp_name_holder_ = name;
  ACE_TString path;
  path = 
    TAO_IFR_Service_Utils::create_common (container_kind,
                                          CORBA::dk_Value,
                                          container_key,
                                          new_key,
                                          this->repo_,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          "defns"
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (path);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_custom",
                                             (CORBA::ULong) is_custom);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_abstract",
                                             (CORBA::ULong) is_abstract);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_truncatable",
                                             (CORBA::ULong) is_truncatable);
  if (!CORBA::is_nil (base_value))
    {
      const char *base_path = 
        TAO_IFR_Service_Utils::reference_to_path (base_value);
          
      // Get the servant's key into the temporary key holder, because
      // the name clash checker for base valuetypes is static, and has
      // no other way to know about a specific key.
      this->repo_->config ()->expand_path (
                                  this->repo_->root_key (),
                                  base_path,
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  0
                                );
      TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                          new_key,
                                          this->repo_,
                                          CORBA::dk_Value
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (path);
          
      const char *base_value_id = base_value->_interface_repository_id ();
      this->repo_->config ()->set_string_value (new_key,
                                                "base_value",
                                                base_value_id);
    }

  CORBA::ULong length = abstract_base_values.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key bases_key;
      this->repo_->config ()->open_section (this->section_key_,
                                            "abstract_bases",
                                            1,
                                            bases_key);
      this->repo_->config ()->set_integer_value (bases_key,
                                                 "count",
                                                 length);
                                 
      const char *base_id = 0;
      const char *base_path = 0;
      char *stringified = 0;

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          base_path = 
            TAO_IFR_Service_Utils::reference_to_path (
                abstract_base_values[i].in ()
              );
              
          // Get the servant's key into the temporary key holder, because
          // the name clash checker for base interfaces is static, and has
          // no other way to know about a specific key.
          this->repo_->config ()->expand_path (
                                      this->repo_->root_key (),
                                      base_path,
                                      TAO_IFR_Service_Utils::tmp_key_,
                                      0
                                    );
          TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                              new_key,
                                              this->repo_,
                                              CORBA::dk_Value
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (path);
          
          base_id = abstract_base_values[i]->_interface_repository_id ();
          stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (bases_key,
                                                    stringified,
                                                    base_id);
        }
    }

  length = supported_interfaces.length ();

  if (length > 0)
    {
      ACE_Configuration_Section_Key supported_key;
      this->repo_->config ()->open_section (new_key,
                                            "supported",
                                            1,
                                            supported_key);
      this->repo_->config ()->set_integer_value (supported_key,
                                                 "count",
                                                 length);
      const char *supported_id = 0;
      const char *supported_path = 0;
      CORBA::ULong kind = 0;
      CORBA::Boolean concrete_seen = 0;
      CORBA::DefinitionKind def_kind;
          
      for (CORBA::ULong i = 0; i < length; ++i)
        {
          supported_path = 
            TAO_IFR_Service_Utils::reference_to_path (
                supported_interfaces[i].in ()
              );
              
          // Get the servant's key into the temporary key holder, because
          // the name clash checker for base interfaces is static, and has
          // no other way to know about a specific key.
          this->repo_->config ()->expand_path (this->repo_->root_key (),
                                               supported_path,
                                               TAO_IFR_Service_Utils::tmp_key_,
                                               0);
          this->repo_->config ()->get_integer_value (
                                      TAO_IFR_Service_Utils::tmp_key_,
                                      "def_kind",
                                      kind);
          def_kind = ACE_static_cast (CORBA::DefinitionKind, kind);
          
          if (def_kind == CORBA::dk_Interface)
            {
              if (concrete_seen == 0)
                {
                  concrete_seen = 1;
                }
              else
                {
                  ACE_THROW_RETURN (CORBA::BAD_PARAM (12,
                                                      CORBA::COMPLETED_NO),
                                    path);
                }
            }                            
          
          TAO_IFR_Service_Utils::name_exists (&TAO_ValueDef_i::name_clash,
                                              new_key,
                                              this->repo_,
                                              CORBA::dk_Value
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (path);

          supported_id = 
            supported_interfaces[i]->_interface_repository_id ();
          char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
          this->repo_->config ()->set_string_value (supported_key,
                                                    stringified,
                                                    supported_id);
        }
    }
    
  return path;
}
