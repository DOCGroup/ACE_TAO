/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "IDLType_i.h"
#include "InterfaceDef_i.h"
#include "Servant_Factory.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(IFR_Service, Container_i, "$Id$")

TAO_Container_i::TAO_Container_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key)
{
}

TAO_Container_i::~TAO_Container_i (void)
{
}

void
TAO_Container_i::destroy (CORBA::Environment &ACE_TRY_ENV)
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
                this->repo_->servant_factory ()->create_idltype (entry_key,
                                                                 ACE_TRY_ENV);
              ACE_CHECK;

              auto_ptr<TAO_IDLType_i> safety (impl);

              IR::DefinitionKind def_kind = impl->def_kind (ACE_TRY_ENV);
              ACE_CHECK;

              switch (def_kind)
              {
                // These anonymous types were defined in IDL inside this
                // struct and have no 'contained-in' info of their own,
                // so they must be destroyed here.
                case IR::dk_String:
                case IR::dk_Wstring:
                case IR::dk_Fixed:
                case IR::dk_Array:
                case IR::dk_Sequence:
                  impl->destroy (ACE_TRY_ENV);
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

          TAO_Contained_i *impl =
            this->repo_->servant_factory ()->create_contained (defn_key,
                                                               ACE_TRY_ENV);
          ACE_CHECK;

          auto_ptr<TAO_Contained_i> safety (impl);

          impl->destroy (ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->repo_->config ()->remove_section (this->section_key_,
                                              "defns",
                                              1);
    }
}

IR::Contained_ptr
TAO_Container_i::lookup (const char *search_name,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->lookup_i (search_name,
                         ACE_TRY_ENV);
}

IR::Contained_ptr
TAO_Container_i::lookup_i (const char *search_name,
                           CORBA::Environment &ACE_TRY_ENV)
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
  IR::DefinitionKind def_kind = IR::dk_none;


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
            break;
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

              def_kind = ACE_static_cast (IR::DefinitionKind, kind);

              if (def_kind == IR::dk_Interface
                  || def_kind == IR::dk_Value)
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
                    break;
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
                            break;
                        }
                    }
                }
            }
        }

      return IR::Contained::_nil ();
    }

  this->repo_->config ()->get_integer_value (work_key,
                                             "def_kind",
                                             kind);

  ACE_TString id;
  this->repo_->config ()->get_string_value (work_key,
                                            "id",
                                            id);

  ACE_TString path;
  this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                            id.c_str (),
                                            path);

  def_kind = ACE_static_cast (IR::DefinitionKind, kind);

  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (def_kind,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::Contained::_nil ());

  return IR::Contained::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

IR::ContainedSeq *
TAO_Container_i::contents (IR::DefinitionKind limit_type,
                           CORBA::Boolean exclude_inherited,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (limit_type == IR::dk_none)
    return 0;

  ACE_Unbounded_Queue<IR::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  // Definitions

  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        0,
                                        defns_key);

  u_int count = 0;
  this->repo_->config ()->get_integer_value (defns_key,
                                             "count",
                                             count);

  for (u_int i = 0; i < count; ++i)
    {
      CORBA::String_var section_name = this->int_to_string (i);
      ACE_Configuration_Section_Key defn_key;
      int status =
        this->repo_->config ()->open_section (defns_key,
                                              section_name.in (),
                                              0,
                                              defn_key);

      if (status == 0)
        {
          u_int kind = 0;
          this->repo_->config ()->get_integer_value (defn_key,
                                                     "def_kind",
                                                     kind);

          IR::DefinitionKind def_kind =
            ACE_static_cast (IR::DefinitionKind, kind);

          if (limit_type == IR::dk_all
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

  // Base interfaces

  if (exclude_inherited == 0)
    {
      IR::DefinitionKind def_kind = this->def_kind (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (def_kind == IR::dk_Interface)
        {
          if (limit_type == IR::dk_Operation
              || limit_type == IR::dk_Attribute
              || limit_type == IR::dk_all)
            {
              TAO_InterfaceDef_i iface (this->repo_,
                                        this->section_key_);

              iface.inherited_contents (kind_queue,
                                        path_queue,
                                        limit_type,
                                        ACE_TRY_ENV);
              ACE_CHECK_RETURN (0);
            }
        }
    }

  size_t size = kind_queue.size ();

  IR::ContainedSeq *contents = 0;
  ACE_NEW_THROW_EX (contents,
                    IR::ContainedSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  IR::ContainedSeq_var retval = contents;

  retval->length (size);

  for (size_t j = 0; j < size; j++)
    {
      IR::DefinitionKind next_kind;
      kind_queue.dequeue_head (next_kind);

      ACE_TString next_path;
      path_queue.dequeue_head (next_path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (next_kind,
                                                        next_path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      IR::Contained_var next_cont =
        IR::Contained::_narrow (obj.in (),
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[j] = next_cont._retn ();
    }

  return retval._retn ();
}

IR::ContainedSeq *
TAO_Container_i::lookup_name (const char *search_name,
                              CORBA::Long levels_to_search,
                              IR::DefinitionKind limit_type,
                              CORBA::Boolean exclude_inherited,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Unbounded_Queue<IR::DefinitionKind> kind_queue;
  ACE_Unbounded_Queue<ACE_TString> path_queue;

  this->lookup_name_recursive (kind_queue,
                               path_queue,
                               search_name,
                               levels_to_search,
                               limit_type,
                               exclude_inherited,
                               ACE_TRY_ENV);

  size_t size = kind_queue.size ();

  IR::ContainedSeq *holder;
  ACE_NEW_THROW_EX (holder,
                    IR::ContainedSeq (size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  IR::ContainedSeq_var retval = holder;
  retval->length (size);

  for (size_t i = 0; i < size; i++)
    {
      IR::DefinitionKind next_kind;
      kind_queue.dequeue_head (next_kind);

      ACE_TString next_path;
      path_queue.dequeue_head (next_path);

      CORBA::Object_var obj =
        this->repo_->servant_factory ()->create_objref (next_kind,
                                                        next_path.c_str (),
                                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      IR::Contained_var next_cont =
        IR::Contained::_narrow (obj.in (),
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i] = next_cont._retn ();
    }

  return retval._retn ();
}

IR::Container::DescriptionSeq *
TAO_Container_i::describe_contents (IR::DefinitionKind limit_type,
                                    CORBA::Boolean exclude_inherited,
                                    CORBA::Long max_returned_objs,
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::ContainedSeq_var contents = this->contents (limit_type,
                                                  exclude_inherited,
                                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::ULong length = contents->length ();
  CORBA::ULong ret_len = 0;

  if (max_returned_objs == -1)
    ret_len = length;
  else
    ret_len = length < ACE_static_cast (CORBA::ULong, max_returned_objs)
              ? length
              : max_returned_objs;

  IR::Container::DescriptionSeq *desc_seq;
  ACE_NEW_THROW_EX (desc_seq,
                    IR::Container::DescriptionSeq (ret_len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  desc_seq->length (ret_len);

  IR::Container::DescriptionSeq_var retval = desc_seq;

  IR::Contained::Description_var desc;

  for (CORBA::ULong i = 0; i < ret_len; i++)
    {
      desc = contents[i]->describe (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      retval[i].contained_object =
        IR::Contained::_duplicate (contents[i]);

      retval[i].kind = desc->kind;

      retval[i].value = desc->value;
    }

  return retval._retn ();
}

IR::ModuleDef_ptr
TAO_Container_i::create_module (const char *id,
                                const char *name,
                                const char *version,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Module,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ModuleDef::_nil ());

  if (!legal_op)
    return IR::ModuleDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ModuleDef::_nil ());

  if (bad_params)
    return IR::ModuleDef::_nil ();

  ACE_Configuration_Section_Key defns_key;

  // Create/open section for members defined in this scope.
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Module);

  // For anonymous members.
  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (new_key,
                                        "refs",
                                        1,
                                        refs_key);

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Module,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ModuleDef::_nil ());

  return IR::ModuleDef::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

IR::ConstantDef_ptr
TAO_Container_i::create_constant (const char *id,
                                  const char *name,
                                  const char *version,
                                  IR::IDLType_ptr type,
                                  const CORBA::Any &value,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Constant,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ConstantDef::_nil ());

  if (!legal_op)
    return IR::ConstantDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ConstantDef::_nil ());

  if (bad_params)
    return IR::ConstantDef::_nil ();

  // Create/open section for members defined in this scope.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Constant);


  // Get the path to our type and store it.
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (type,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ConstantDef::_nil ());

  CORBA::String_var type_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "type_path",
                                            type_path.in ());

  // Store the value.
  ACE_Message_Block *mb = value._tao_get_cdr ();

  CORBA::TypeCode_var val_tc = value.type ();

  CORBA::TCKind kind = val_tc->kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ConstantDef::_nil ());

  switch (kind)
  {
    // The data for these types will be aligned to an 8-byte
    // boundary, while the rd_ptr may not.
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
    this->repo_->servant_factory ()->create_objref (IR::dk_Constant,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ConstantDef::_nil ());

  return IR::ConstantDef::_narrow (obj.in (),
                                   ACE_TRY_ENV);
}

IR::StructDef_ptr
TAO_Container_i::create_struct (const char *id,
                                const char *name,
                                const char *version,
                                const IR::StructMemberSeq &members,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Struct,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::StructDef::_nil ());

  if (!legal_op)
    return IR::StructDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::StructDef::_nil ());

  if (bad_params)
    return IR::StructDef::_nil ();

  // Create/open section for members defined in this scope.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Struct);

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

  // Create a section for each member. We just store the
  // member name and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      CORBA::String_var section_name = this->int_to_string (i);

      this->repo_->config ()->open_section (refs_key,
                                            section_name.in (),
                                            1,
                                            member_key);

      ACE_TString name (members[i].name);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                name);

      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (members[i].type_def,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (IR::StructDef::_nil ());

      CORBA::String_var member_path =
        PortableServer::ObjectId_to_string (oid.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path.in ());
    }

  IR::DefinitionKind def_kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::StructDef::_nil ());

  if (def_kind == IR::dk_Struct
      || def_kind == IR::dk_Union
      || def_kind == IR::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Struct,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::StructDef::_nil ());

  return IR::StructDef::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

IR::UnionDef_ptr
TAO_Container_i::create_union (const char *id,
                               const char *name,
                               const char *version,
                               IR::IDLType_ptr discriminator_type,
                               const IR::UnionMemberSeq &members,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Union,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::UnionDef::_nil ());

  if (!legal_op)
    return IR::UnionDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::UnionDef::_nil ());

  if (bad_params)
    return IR::UnionDef::_nil ();

  // Create new section, or open if it already exists.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Union);

  // Add a field to hold the path to the discriminator type.
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (discriminator_type,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::UnionDef::_nil ());

  CORBA::String_var disc_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "disc_path",
                                            disc_path.in ());

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

  // Create a section for each member. We store the member
  // name, its label value, and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            section_name.in (),
                                            1,
                                            member_key);

      ACE_TString name (members[i].name);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                name);

      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (members[i].type_def,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (IR::UnionDef::_nil ());

      CORBA::String_var member_path =
        PortableServer::ObjectId_to_string (oid.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path.in ());

      this->store_label (member_key,
                         members[i].label,
                         ACE_TRY_ENV);
    }

  IR::DefinitionKind def_kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::UnionDef::_nil ());

  if (def_kind == IR::dk_Struct
      || def_kind == IR::dk_Union
      || def_kind == IR::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Union,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::UnionDef::_nil ());

  return IR::UnionDef::_narrow (obj.in (),
                                ACE_TRY_ENV);
}

IR::EnumDef_ptr
TAO_Container_i::create_enum (const char *id,
                              const char *name,
                              const char *version,
                              const IR::EnumMemberSeq &members,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Enum,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::EnumDef::_nil ());

  if (!legal_op)
    return IR::EnumDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::EnumDef::_nil ());

  if (bad_params)
    return IR::EnumDef::_nil ();

  // Create new section, or open if it already exists.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Enum);

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
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (new_key,
                                            section_name.in (),
                                            1,
                                            member_key);

      ACE_TString member_name (members[i]);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                member_name);
    }

  IR::DefinitionKind def_kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::EnumDef::_nil ());

  if (def_kind == IR::dk_Struct
      || def_kind == IR::dk_Union
      || def_kind == IR::dk_Exception)
    {
      this->update_refs (path.c_str (),
                         name);
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Enum,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::EnumDef::_nil ());

  return IR::EnumDef::_narrow (obj.in (),
                               ACE_TRY_ENV);
}

IR::AliasDef_ptr
TAO_Container_i::create_alias (const char *id,
                               const char *name,
                               const char *version,
                               IR::IDLType_ptr original_type,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Alias,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AliasDef::_nil ());

  if (!legal_op)
    return IR::AliasDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AliasDef::_nil ());

  if (bad_params)
    return IR::AliasDef::_nil ();

  // Create new section.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Alias);

  // Get the path to our original type and store it.
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (original_type,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AliasDef::_nil ());

  CORBA::String_var unalias_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "original_type",
                                            unalias_path.in ());

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Alias,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::AliasDef::_nil ());

  return IR::AliasDef::_narrow (obj.in (),
                                ACE_TRY_ENV);
}

IR::InterfaceDef_ptr
TAO_Container_i::create_interface (const char *id,
                                   const char *name,
                                   const char *version,
                                   const IR::InterfaceDefSeq &base_interfaces,
                                   CORBA::Boolean is_abstract,
                                   CORBA::Boolean is_local,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Interface,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::InterfaceDef::_nil ());

  if (!legal_op)
    return IR::InterfaceDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::InterfaceDef::_nil ());

  if (bad_params)
    return IR::InterfaceDef::_nil ();

  // Create new section.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Interface);

  // Store the boolean attributes.
  this->repo_->config ()->set_integer_value (new_key,
                                             "is_abstract",
                                             is_abstract);

  this->repo_->config ()->set_integer_value (new_key,
                                             "is_local",
                                             is_local);

  CORBA::ULong length = base_interfaces.length ();

  if (length > 0)
    {
      // Open a section for the base interfaces.
      ACE_Configuration_Section_Key inherited_key;

      this->repo_->config ()->open_section (new_key,
                                            "inherited",
                                            1,
                                            inherited_key);

      // Store the path to each base interface.
      for (CORBA::ULong i = 0; i < length; i++)
        {
          PortableServer::ObjectId_var oid =
            this->repo_->ir_poa ()->reference_to_id (base_interfaces[i],
                                                     ACE_TRY_ENV);
          ACE_CHECK_RETURN (IR::InterfaceDef::_nil ());

          CORBA::String_var inherited_path =
            PortableServer::ObjectId_to_string (oid.in ());

          CORBA::String_var section_name = this->int_to_string (i);

          this->repo_->config ()->set_string_value (inherited_key,
                                                    section_name.in (),
                                                    inherited_path.in ());
        }
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Interface,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::InterfaceDef::_nil ());

  return IR::InterfaceDef::_narrow (obj.in (),
                                    ACE_TRY_ENV);
}

IR::ValueDef_ptr
TAO_Container_i::create_value (const char * /* id */,
                               const char * /* name */,
                               const char * /* version */,
                               CORBA::Boolean /* is_custom */,
                               CORBA::Boolean /* is_abstract */,
                               IR::ValueDef_ptr /* base_value */,
                               CORBA::Boolean /* is_truncatable */,
                               const IR::ValueDefSeq & /* abstract_base_values */,
                               const IR::InterfaceDefSeq & /* supported_interfaces */,
                               const IR::InitializerSeq & /* initializers */,
                               CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ValueBoxDef_ptr
TAO_Container_i::create_value_box (const char *id,
                                   const char *name,
                                   const char *version,
                                   IR::IDLType_ptr original_type_def,
                                   CORBA::Environment &ACE_TRY_ENV )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_ValueBox,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ValueBoxDef::_nil ());

  if (!legal_op)
    return IR::ValueBoxDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ValueBoxDef::_nil ());

  if (bad_params)
    return IR::ValueBoxDef::_nil ();

  // Create new section.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_ValueBox);

  // Get the path to our original type and store it.
  PortableServer::ObjectId_var oid =
    this->repo_->ir_poa ()->reference_to_id (original_type_def,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ValueBoxDef::_nil ());

  CORBA::String_var boxed_path =
    PortableServer::ObjectId_to_string (oid.in ());

  this->repo_->config ()->set_string_value (new_key,
                                            "boxed_type",
                                            boxed_path.in ());

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_ValueBox,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ValueBoxDef::_nil ());

  return IR::ValueBoxDef::_narrow (obj.in (),
                                   ACE_TRY_ENV);
}

IR::ExceptionDef_ptr
TAO_Container_i::create_exception (const char *id,
                                   const char *name,
                                   const char *version,
                                   const IR::StructMemberSeq &members,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Exception,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ExceptionDef::_nil ());

  if (!legal_op)
    return IR::ExceptionDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ExceptionDef::_nil ());

  if (bad_params)
    return IR::ExceptionDef::_nil ();

  // Create/open section for members defined in this scope.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Exception);

  ACE_Configuration_Section_Key refs_key;
  this->repo_->config ()->open_section (new_key,
                                        "refs",
                                        1,
                                        refs_key);

  CORBA::ULong count = members.length ();
  this->repo_->config ()->set_integer_value (refs_key,
                                             "count",
                                             count);

  // Create a section for each member. We just store the
  // member name and the path to its database entry.
  for (CORBA::ULong i = 0; i < count; i++)
    {
      ACE_Configuration_Section_Key member_key;
      CORBA::String_var section_name = this->int_to_string (i);
      this->repo_->config ()->open_section (refs_key,
                                            section_name.in (),
                                            1,
                                            member_key);

      ACE_TString name (members[i].name);
      this->repo_->config ()->set_string_value (member_key,
                                                "name",
                                                name);

      PortableServer::ObjectId_var oid =
        this->repo_->ir_poa ()->reference_to_id (members[i].type_def,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (IR::ExceptionDef::_nil ());

      CORBA::String_var member_path =
        PortableServer::ObjectId_to_string (oid.in ());

      this->repo_->config ()->set_string_value (member_key,
                                                "path",
                                                member_path.in ());
    }

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Exception,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::ExceptionDef::_nil ());

  return IR::ExceptionDef::_narrow (obj.in (),
                                    ACE_TRY_ENV);
}

IR::NativeDef_ptr
TAO_Container_i::create_native (const char *id,
                                const char *name,
                                const char *version,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean legal_op = this->valid_container (IR::dk_Native,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::NativeDef::_nil ());

  if (!legal_op)
    return IR::NativeDef::_nil ();

  CORBA::Boolean bad_params = this->pre_exist (id,
                                               name,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::NativeDef::_nil ());

  if (bad_params)
    return IR::NativeDef::_nil ();

  // Create new section.
  ACE_Configuration_Section_Key defns_key;
  this->repo_->config ()->open_section (this->section_key_,
                                        "defns",
                                        1,
                                        defns_key);

  // Common to all IR objects created in IR::Container.
  ACE_Configuration_Section_Key new_key;
  ACE_TString path = this->create_common (defns_key,
                                          new_key,
                                          id,
                                          name,
                                          version,
                                          "defns\\",
                                          IR::dk_Native);

  // Create the object reference.
  CORBA::Object_var obj =
    this->repo_->servant_factory ()->create_objref (IR::dk_Native,
                                                    path.c_str (),
                                                    ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR::NativeDef::_nil ());

  return IR::NativeDef::_narrow (obj.in (),
                                 ACE_TRY_ENV);
}

void
TAO_Container_i::lookup_name_recursive (
      ACE_Unbounded_Queue<IR::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Long levels_to_search,
      IR::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV
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

          IR::DefinitionKind def_kind =
            ACE_static_cast (IR::DefinitionKind, kind);

          if (limit_type == IR::dk_all || limit_type == def_kind)
            {
              ACE_TString name;
              this->repo_->config ()->get_string_value (defn_key,
                                                        "name",
                                                        name);

              if (!ACE_OS::strcmp (name.c_str (), search_name))
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

          if (levels_to_search == -1 || levels_to_search > 1)
            {
              TAO_Container_i *impl =
                this->repo_->servant_factory ()->create_container (
                                                     defn_key,
                                                     ACE_TRY_ENV
                                                   );
              ACE_CHECK;

              // This member may not be a container.
              if (impl == 0)
                continue;

              auto_ptr<TAO_Container_i> safety (impl);

              impl->lookup_name_recursive (kind_queue,
                                           path_queue,
                                           search_name,
                                           levels_to_search == -1
                                              ? -1
                                              : levels_to_search - 1,
                                           limit_type,
                                           exclude_inherited,
                                           ACE_TRY_ENV);
              ACE_CHECK;
            }
        }
    }

  IR::DefinitionKind kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK;

  if (kind == IR::dk_Interface || kind == IR::dk_Value)
    {
      if (limit_type == IR::dk_all || limit_type == IR::dk_Attribute)
        {
          this->lookup_attr (kind_queue,
                             path_queue,
                             search_name,
                             exclude_inherited);
        }

      if (limit_type == IR::dk_all || limit_type == IR::dk_Operation)
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
      ACE_Unbounded_Queue<IR::DefinitionKind> &kind_queue,
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
              kind_queue.enqueue_tail (IR::dk_Attribute);

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

              TAO_InterfaceDef_i impl (this->repo_,
                                       base_key);

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
      ACE_Unbounded_Queue<IR::DefinitionKind> &kind_queue,
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
              kind_queue.enqueue_tail (IR::dk_Operation);

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

              TAO_InterfaceDef_i impl (this->repo_,
                                       base_key);

              impl.lookup_attr (kind_queue,
                                path_queue,
                                search_name,
                                exclude_inherited);
            }
        }
    }
}

CORBA::Boolean
TAO_Container_i::name_exists (const char *name,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  int index = 0;
  ACE_TString section_name;

  // Check the members defined elsewhere.
  ACE_Configuration_Section_Key refs_key;

  if (this->repo_->config ()->open_section (this->section_key_,
                                            "refs",
                                            0,
                                            refs_key)
       == 0)
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

          ACE_TString member_name;
          this->repo_->config ()->get_string_value (member_key,
                                                    "name",
                                                    member_name);

          if (member_name == name)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (3,
                                                  CORBA::COMPLETED_NO),
                                1);
            }
        }
    }

  // Check the members defined in this scope.
  ACE_Configuration_Section_Key defns_key;
  if (this->repo_->config ()->open_section (this->section_key_,
                                            "defns",
                                            0,
                                            defns_key)
       == 0)
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

          ACE_TString defn_name;
          this->repo_->config ()->get_string_value (defn_key,
                                                    "name",
                                                    defn_name);

          if (defn_name == name)
            {
              ACE_THROW_RETURN (CORBA::BAD_PARAM (3,
                                                  CORBA::COMPLETED_NO),
                                1);
            }
        }
    }

  // If we are an interface, check the inherited members.
  IR::DefinitionKind def_kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (1);

  if (def_kind == IR::dk_Interface)
    {
      ACE_Configuration_Section_Key inherited_key;
      int status =
        this->repo_->config ()->open_section (this->section_key_,
                                              "inherited",
                                              0,
                                              inherited_key);

      // This section may not have been created.
      if (status == 0)
        {
          int index = 0;
          ACE_TString path;
          ACE_Configuration::VALUETYPE type;

          while (this->repo_->config ()->enumerate_values (inherited_key,
                                                           index++,
                                                           path,
                                                           type)
                  == 0)
            {
              ACE_Configuration_Section_Key base_key;
              this->repo_->config ()->expand_path (this->repo_->root_key (),
                                                   path,
                                                   base_key,
                                                   0);

              ACE_TString base_name;
              this->repo_->config ()->get_string_value (base_key,
                                                        "name",
                                                        base_name);

              if (base_name == name)
                {
                  ACE_THROW_RETURN (CORBA::BAD_PARAM (5,
                                                      CORBA::COMPLETED_NO),
                                    1);
                }
            }
        }
    }

  return 0;
}

CORBA::Boolean
TAO_Container_i::id_exists (const char *id,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The repo id must not already exist.
  ACE_TString holder;
  if (this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                id,
                                                holder)
       == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (2,
                                          CORBA::COMPLETED_NO),
                        1);
    }

  return 0;
}


CORBA::Boolean
TAO_Container_i::pre_exist (const char *id,
                            const char *name,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean exist = this->id_exists (id, ACE_TRY_ENV);
  ACE_CHECK_RETURN (1);

  if (exist)
    return 1;

  return name_exists (name, ACE_TRY_ENV);
}

ACE_TString
TAO_Container_i::create_common (
      ACE_Configuration_Section_Key sub_key,
      ACE_Configuration_Section_Key &new_key,
      const char *id,
      const char *name,
      const char *version,
      const char *sub_section,
      IR::DefinitionKind def_kind
    )
{
  u_int defn_count = 0;

  // If we have a count, it gets set, if not, it stays 0.
  this->repo_->config ()->get_integer_value (sub_key,
                                             "count",
                                             defn_count);

  CORBA::String_var section_name = this->int_to_string (defn_count);
  this->repo_->config ()->open_section (sub_key,
                                        section_name.in (),
                                        1,
                                        new_key);

  // Increment the count.
  this->repo_->config ()->set_integer_value (sub_key,
                                             "count",
                                             defn_count + 1);

  this->repo_->config ()->get_integer_value (sub_key,
                                             "count",
                                             defn_count);

  // Set the name attribute.
  this->repo_->config ()->set_string_value (new_key,
                                            "name",
                                            name);

  // Set the id attribute.
  this->repo_->config ()->set_string_value (new_key,
                                            "id",
                                            id);

  // Set the version attribute.
  this->repo_->config ()->set_string_value (new_key,
                                            "version",
                                            version);

  // Set the definition kind.
  this->repo_->config ()->set_integer_value (new_key,
                                             "def_kind",
                                             def_kind);

  // Get the container's absolute name, append the new name,
  // and set it in the new section.
  ACE_TString absolute_name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "absolute_name",
                                            absolute_name);

  absolute_name += "::";
  absolute_name += name;

  this->repo_->config ()->set_string_value (new_key,
                                            "absolute_name",
                                            absolute_name);

  // Get the container's path.
  ACE_TString container_id;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "id",
                                            container_id);

  this->repo_->config ()->set_string_value (new_key,
                                            "container_id",
                                            container_id);

  ACE_TString path;
  if (container_id == "") // This Container is the Repository.
    {
      path = "";
    }
  else
    {
      this->repo_->config ()->get_string_value (this->repo_->repo_ids_key (),
                                                container_id.c_str (),
                                                path);

      path += '\\';
    }

  path += sub_section;
  path += section_name.in ();

  // Store our path under our global repo id for fast lookup.
  this->repo_->config ()->set_string_value (this->repo_->repo_ids_key (),
                                            id,
                                            path);

  return path;
}

CORBA::Boolean
TAO_Container_i::valid_container (const IR::DefinitionKind op_kind,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IR::DefinitionKind my_kind = this->def_kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  int error_flag = 0;

  switch (my_kind)
  {
    case IR::dk_Repository:
    case IR::dk_Module:
      break;
    case IR::dk_Exception:
    case IR::dk_Struct:
    case IR::dk_Union:
      switch (op_kind)
      {
        case IR::dk_Struct:
        case IR::dk_Union:
        case IR::dk_Enum:
          break;
        default:
          error_flag = 1;
          break;
      }
      break;
    case IR::dk_Value:
    case IR::dk_Interface:
    case IR::dk_Home:
      switch (op_kind)
      {
        case IR::dk_Module:
        case IR::dk_Interface:
        case IR::dk_Value:
          error_flag = 1;
          break;
        default:
          break;
      }
      break;
    case IR::dk_Component:
      error_flag = 1;
      break;
    default:
      break;
  }

  if (error_flag == 1)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (4,
                                          CORBA::COMPLETED_NO),
                        0);
    }

  return 1;
}

void
TAO_Container_i::store_label (ACE_Configuration_Section_Key key,
                              const CORBA::Any &value,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::TypeCode_var tc = value.type ();

  CORBA::TCKind kind = tc->kind (ACE_TRY_ENV);
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
    this->repo_->config ()->set_string_value (key,
                                              "label",
                                              "default");
  else
    this->repo_->config ()->set_integer_value (key,
                                               "label",
                                               result);
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
  CORBA::String_var new_section_name = this->int_to_string (count);
  this->repo_->config ()->open_section (refs_key,
                                        new_section_name.in (),
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
