// $Id$

#ifndef TAO_IFR_SERVICE_UTILS_T_CPP
#define TAO_IFR_SERVICE_UTILS_T_CPP

#include "IFR_Service_Utils_T.h"
#include "IFR_Service_Utils.h"

ACE_RCSID (IFRService,
           IFR_Service_Utils_T,
           "$Id$")

template<typename T>
void
TAO_IFR_Generic_Utils<T>::destroy_special (const char *section_name,
                                           TAO_Repository_i *repo,
                                           ACE_Configuration_Section_Key &key
                                           ACE_ENV_ARG_DECL)
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
      impl.destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
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

      config->get_string_value (key,
                                "base_type",
                                holder);
      TAO_Port_Desc_Seq_Utils<T_desc_seq>::port_base_type (desc_seq,
                                                           holder,
                                                           i);

      TAO_Port_Desc_Seq_Utils<T_desc_seq>::get_is_multiple (desc_seq,
                                                            config,
                                                            key,
                                                            i);
    }
}

template<typename T_desc_seq>
void 
TAO_Port_Desc_Seq_Utils<T_desc_seq>::get_is_multiple (
    T_desc_seq &desc_seq,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key,
    CORBA::ULong index
  )
{
  // All types except UsesDescription have no is_multiple member.
  ACE_UNUSED_ARG (desc_seq);
  ACE_UNUSED_ARG (config);
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (index);
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
                                 ACE_Configuration_Section_Key &key
                                 ACE_ENV_ARG_DECL)
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
                                          sub_section
                                          ACE_ENV_ARG_PARAMETER);

  repo->config ()->set_string_value (
                       new_key,
                       "base_type",
                       port_base_type->_interface_repository_id ()
                     );

  TAO_Port_Utils<T>::set_is_multiple (is_multiple,
                                      repo->config (),
                                      new_key);

  CORBA::Object_var obj = 
    TAO_IFR_Service_Utils::path_to_ir_object (path,
                                              repo
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (T::_nil ());

  return T::_narrow (obj.in ()
                     ACE_ENV_ARG_PARAMETER);
}

template<typename T>
void
TAO_Port_Utils<T>::set_is_multiple (CORBA::Boolean is_multiple,
                                    ACE_Configuration *config,
                                    ACE_Configuration_Section_Key &key)
{
  /// Do nothing for eveything except UsesDef.
  ACE_UNUSED_ARG (is_multiple);
  ACE_UNUSED_ARG (config);
  ACE_UNUSED_ARG (key);
}

#endif /* TAO_IFR_SERVICE_UTILS_T_CPP */

