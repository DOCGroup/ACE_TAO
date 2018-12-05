// -*- C++ -*-

//=============================================================================
/**
 *  @file    IFR_Service_Utils_T.h
 *
 *  Template methods useful to IFR Service.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IFR_SERVICE_UTILS_T_H
#define TAO_IFR_SERVICE_UTILS_T_H

#include /**/ "ace/pre.h"

#include "ace/Configuration.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IFR_Client/IFR_ComponentsC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Repository_i;

template<typename T>
class TAO_IFR_Generic_Utils
{
public:
  static void destroy_special (const ACE_TCHAR *section_name,
                               TAO_Repository_i *repo,
                               ACE_Configuration_Section_Key &key);

  static void set_initializers (const T &initializers,
                                ACE_Configuration *config,
                                ACE_Configuration_Section_Key &key);
  /// Common to Container::create_value(), create_ext_value
  /// and ValueDef::initializers().
};

template<typename T_desc, typename T_impl>
class TAO_IFR_Desc_Utils
{
public:
  static void fill_desc_begin (T_desc &desc,
                               TAO_Repository_i *repo,
                               ACE_Configuration_Section_Key &key);
};

template<typename T_strseq>
class TAO_IFR_Strseq_Utils
{
public:
  static void fill_string_seq (const char *section_name,
                               ACE_Configuration *config,
                               ACE_Configuration_Section_Key &key,
                               T_strseq &seq);
};

template <typename T_desc_seq>
class TAO_Port_Desc_Seq_Utils
{
public:
  static void port_descriptions (T_desc_seq &desc_seq,
                                 ACE_Configuration *config,
                                 ACE_Configuration_Section_Key &key,
                                 const char *sub_section);

  static void get_is_multiple (T_desc_seq &desc_seq,
                               ACE_Configuration *config,
                               ACE_Configuration_Section_Key &key,
                               CORBA::ULong index);

  static void port_base_type (T_desc_seq &desc_seq,
                              ACE_TString &holder,
                              CORBA::ULong index);
};

template<typename T>
class TAO_Port_Utils
{
public:
  static T *create_entry (const char *id,
                          const char *name,
                          const char *version,
                          const char *sub_section,
                          CORBA::Container_ptr port_base_type,
                          CORBA::Boolean is_multiple,
                          TAO_Repository_i *repo,
                          CORBA::DefinitionKind port_kind,
                          ACE_Configuration_Section_Key &key);

  static void set_is_multiple (CORBA::Boolean is_multiple,
                               ACE_Configuration *config,
                               ACE_Configuration_Section_Key &key);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/IFRService/IFR_Service_Utils_T.cpp"
#endif /* defined REQUIRED SOURCE */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_IFR_SERVICE_UTILS_T_H */
