// $Id$

#ifndef TAO_IFR_SERVICE_UTILS_T_CPP
#define TAO_IFR_SERVICE_UTILS_T_CPP

#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IFRService/Repository_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T>
void
TAO_IFR_Generic_Utils<T>::destroy_special (const char *section_name,
                                           TAO_Repository_i *repo,
                                           ACE_Configuration_Section_Key &key)
{
  ACE_Configuration_Section_Key sub_key;
  int status =
    repo->config ()->open_section (key,
                                   section_name,
                                   0,
                                   sub_key);

  if (status != 0)
    {
      /// Nothing to destroy.
      return;
    }

  CORBA::ULong count = 0;
  repo->config ()->get_integer_value (sub_key,
                                      "count",
                                      count);

  char *stringified = 0;
  ACE_Configuration_Section_Key special_key;
  ACE_TString holder;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      repo->config ()->open_section (sub_key,
                                     stringified,
                                     0,
                                     special_key);
      T impl (repo);
      impl.section_key (special_key);
      impl.destroy_i ();
    }
}

template<typename T>
void
TAO_IFR_Generic_Utils<T>::set_initializers (
    const T &initializers,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key
  )
{
  CORBA::ULong length = initializers.length ();

  if (length == 0)
    {
      return;
    }

  ACE_Configuration_Section_Key initializers_key;
  config->open_section (key,
                        "initializers",
                        1,
                        initializers_key);
  config->set_integer_value (initializers_key,
                             "count",
                             length);

  CORBA::ULong arg_count = 0;
  char *arg_path = 0;
  ACE_Configuration_Section_Key initializer_key;
  ACE_Configuration_Section_Key params_key;
  ACE_Configuration_Section_Key arg_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->open_section (initializers_key,
                            stringified,
                            1,
                            initializer_key);
      config->set_string_value (initializer_key,
                                "name",
                                initializers[i].name.in ());

      arg_count = initializers[i].members.length ();

      if (arg_count > 0)
        {
          config->open_section (initializer_key,
                                "params",
                                1,
                                params_key);
          config->set_integer_value (params_key,
                                     "count",
                                     arg_count);

          for (CORBA::ULong j = 0; j < arg_count; ++j)
            {
              char *stringified =
                TAO_IFR_Service_Utils::int_to_string (j);
              config->open_section (params_key,
                                    stringified,
                                    1,
                                    arg_key);
              config->set_string_value (
                  arg_key,
                  "arg_name",
                  initializers[i].members[j].name.in ()
                );
              arg_path =
                TAO_IFR_Service_Utils::reference_to_path (
                    initializers[i].members[j].type_def.in ()
                  );
              config->set_string_value (arg_key,
                                        "arg_path",
                                        arg_path);
            }
        }
    }
}

template<typename T_desc, typename T_impl>
void
TAO_IFR_Desc_Utils<T_desc,T_impl>::fill_desc_begin (
    T_desc &desc,
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key &key
  )
{
  T_impl impl (repo);
  impl.section_key (key);

  desc.name = impl.name_i ();

  desc.id = impl.id_i ();

  ACE_TString holder;
  repo->config ()->get_string_value (key,
                                     "container_id",
                                     holder);
  desc.defined_in = holder.fast_rep ();

  desc.version = impl.version_i ();
}

template<typename T_strseq>
void
TAO_IFR_Strseq_Utils<T_strseq> ::fill_string_seq (
    const char *section_name,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key,
    T_strseq &seq
  )
{
  ACE_Configuration_Section_Key section_key;
  int status = config->open_section (key,
                                     section_name,
                                     0,
                                     section_key);

  if (status != 0)
    {
      seq.length (0);
      return;
    }

  CORBA::ULong count = 0;
  config->get_integer_value (section_key,
                             "count",
                             count);
  seq.length (count);
  char *stringified = 0;
  ACE_TString holder;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->get_string_value (section_key,
                                stringified,
                                holder);
      seq[i] = holder.fast_rep ();
    }
}

template<typename T_desc_seq>
void
TAO_Port_Desc_Seq_Utils<T_desc_seq>::port_descriptions (
    T_desc_seq &desc_seq,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key,
    const char *sub_section
  )
{
  ACE_Configuration_Section_Key sub_key;
  int status = config->open_section (key,
                                     sub_section,
                                     0,
                                     sub_key);

  if (status != 0)
    {
      desc_seq.length (0);
      return;
    }

  CORBA::ULong count = 0;
  config->get_integer_value (sub_key,
                             "count",
                             count);
  desc_seq.length (count);
  ACE_Configuration_Section_Key desc_key;
  char *stringified = 0;
  ACE_TString holder;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->open_section (sub_key,
                            stringified,
                            0,
                            desc_key);

      config->get_string_value (desc_key,
                                "name",
                                holder);
      desc_seq[i].name = holder.c_str ();

      config->get_string_value (desc_key,
                                "id",
                                holder);
      desc_seq[i].id = holder.c_str ();

      /// Seems to me that this field should refer to the component
      /// where the port is defined - NOT where the base type is defined.
      config->get_string_value (key,
                                "id",
                                holder);
      desc_seq[i].defined_in = holder.c_str ();

      config->get_string_value (desc_key,
                                "version",
                                holder);
      desc_seq[i].version = holder.c_str ();

      config->get_string_value (desc_key,
                                "base_type",
                                holder);
      TAO_Port_Desc_Seq_Utils<T_desc_seq>::port_base_type (desc_seq,
                                                           holder,
                                                           i);

      TAO_Port_Desc_Seq_Utils<T_desc_seq>::get_is_multiple (desc_seq,
                                                            config,
                                                            desc_key,
                                                            i);
    }
}

template<typename T_desc_seq>
void
TAO_Port_Desc_Seq_Utils<T_desc_seq>::get_is_multiple (
    T_desc_seq & /* desc_seq */,
    ACE_Configuration * /* config */,
    ACE_Configuration_Section_Key & /* key */,
    CORBA::ULong /* index */
  )
{
  // All types except UsesDescription have no is_multiple member.
}

template<typename T_desc_seq>
void
TAO_Port_Desc_Seq_Utils<T_desc_seq>::port_base_type (T_desc_seq &desc_seq,
                                                     ACE_TString &holder,
                                                     CORBA::ULong index)
{
  desc_seq[index].interface_type = holder.fast_rep ();
}

template<typename T>
T *
TAO_Port_Utils<T>::create_entry (const char *id,
                                 const char *name,
                                 const char *version,
                                 const char *sub_section,
                                 CORBA::Container_ptr port_base_type,
                                 CORBA::Boolean is_multiple,
                                 TAO_Repository_i *repo,
                                 CORBA::DefinitionKind port_kind,
                                 ACE_Configuration_Section_Key &key)
{
  TAO_Container_i::tmp_name_holder (name);
  ACE_Configuration_Section_Key new_key;
  ACE_TString path =
    TAO_IFR_Service_Utils::create_common (CORBA::dk_Component,
                                          port_kind,
                                          key,
                                          new_key,
                                          repo,
                                          id,
                                          name,
                                          &TAO_Container_i::same_as_tmp_name,
                                          version,
                                          sub_section);

  const char *tmp =
    TAO_IFR_Service_Utils::reference_to_path (port_base_type);

  ACE_Configuration_Section_Key base_type_key;
  repo->config ()->expand_path (repo->root_key (),
                                tmp,
                                base_type_key,
                                0);

  ACE_TString holder;
  repo->config ()->get_string_value (base_type_key,
                                     "id",
                                     holder);

  repo->config ()->set_string_value (
                       new_key,
                       "base_type",
                       holder);

  TAO_Port_Utils<T>::set_is_multiple (is_multiple,
                                      repo->config (),
                                      new_key);

  CORBA::Object_var obj =
    TAO_IFR_Service_Utils::path_to_ir_object (path,
                                              repo);

  return T::_narrow (obj.in ());
}

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x660) && (__BORLANDC__ <= 0x680)
// Borland gives warnings about argument not used on the construct as used
// for the other compilers. This has been reported to Borland, adding
// a workaround to suppress these warnings so that the real important ones
// are not missed.
template<typename T>
void
TAO_Port_Utils<T>::set_is_multiple (CORBA::Boolean is_multiple,
                                    ACE_Configuration * config,
                                    ACE_Configuration_Section_Key &key)
{
  ACE_UNUSED_ARG (is_multiple);
  ACE_UNUSED_ARG (config);
  ACE_UNUSED_ARG (key);
  // Do nothing for everything except UsesDef.
}
#else
template<typename T>
void
TAO_Port_Utils<T>::set_is_multiple (CORBA::Boolean /* is_multiple */,
                                    ACE_Configuration * /* config */,
                                    ACE_Configuration_Section_Key & /* key */)
{
  // Do nothing for everything except UsesDef.
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_IFR_SERVICE_UTILS_T_CPP */
