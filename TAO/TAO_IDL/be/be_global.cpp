
//=============================================================================
/**
 *  @file    be_global.cpp
 *
 *  $Id$
 *
 *  Stores global data specific to the compiler back end.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#include "be_global.h"
#include "be_codegen.h"
#include "be_generator.h"
#include "be_module.h"
#include "be_valuetype.h"
#include "be_interface.h"
#include "be_util.h"

#include "ast_predefined_type.h"
#include "ast_root.h"

#include "utl_identifier.h"
#include "utl_string.h"
#include "global_extern.h"
#include "idl_defines.h"

#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_ctype.h"

TAO_IDL_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : changing_standard_include_files_ (1),
    skel_export_macro_ (0),
    skel_export_include_ (0),
    skel_export_file_ (0),
    stub_export_macro_ (0),
    stub_export_include_ (0),
    stub_export_file_ (0),
    anyop_export_macro_ (0),
    anyop_export_include_ (0),
    exec_export_macro_ (0),
    exec_export_include_ (0),
    svnt_export_macro_ (0),
    svnt_export_include_ (0),
    conn_export_macro_ (0),
    conn_export_include_ (0),
    pch_include_ (0),
    pre_include_ (0),
    post_include_ (0),
    include_guard_ (0),
    safe_include_ (0),
    unique_include_ (0),
    core_versioning_begin_ ("\nTAO_BEGIN_VERSIONED_NAMESPACE_DECL\n"),
    core_versioning_end_   ("\nTAO_END_VERSIONED_NAMESPACE_DECL\n"),
    versioning_begin_ (),
    versioning_end_ (),
    client_hdr_ending_ (ACE::strnew ("C.h")),
    client_stub_ending_ (ACE::strnew ("C.cpp")),
    client_inline_ending_ (ACE::strnew ("C.inl")),
    server_hdr_ending_ (ACE::strnew ("S.h")),
    implementation_hdr_ending_ (ACE::strnew ("I.h")),
    implementation_skel_ending_ (ACE::strnew ("I.cpp")),
    impl_class_prefix_ (ACE::strnew ("")),
    impl_class_suffix_ (ACE::strnew ("_i")),
    server_template_hdr_ending_ (ACE::strnew ("S_T.h")),
    server_skeleton_ending_ (ACE::strnew ("S.cpp")),
    server_template_skeleton_ending_ (ACE::strnew ("S_T.cpp")),
    anyop_hdr_ending_ (ACE::strnew ("A.h")),
    anyop_src_ending_ (ACE::strnew ("A.cpp")),
    ciao_svnt_hdr_template_ending_ (ACE::strnew ("_svnt_T.h")),
    ciao_svnt_src_template_ending_ (ACE::strnew ("_svnt_T.cpp")),
    ciao_svnt_hdr_ending_ (ACE::strnew ("_svnt.h")),
    ciao_svnt_src_ending_ (ACE::strnew ("_svnt.cpp")),
    ciao_exec_hdr_ending_ (ACE::strnew ("_exec.h")),
    ciao_exec_src_ending_ (ACE::strnew ("_exec.cpp")),
    ciao_exec_stub_hdr_ending_ (ACE::strnew ("EC.h")),
    ciao_exec_idl_ending_ (ACE::strnew ("E.idl")),
    ciao_conn_hdr_ending_ (ACE::strnew ("_conn.h")),
    ciao_conn_src_ending_ (ACE::strnew ("_conn.cpp")),
    dds_typesupport_hdr_ending_ (ACE::strnew ("Support.h")),
    ciao_ami_conn_idl_ending_ (ACE::strnew ("A.idl")),
    ciao_ami_conn_impl_hdr_ending_ (ACE::strnew ("_conn_i.h")),
    ciao_ami_conn_impl_src_ending_ (ACE::strnew ("_conn_i.cpp")),
    ciao_container_type_ (ACE::strnew ("Session")),
    output_dir_ (0),
    stub_include_dir_ (0),
    skel_output_dir_ (0),
    anyop_output_dir_ (0),
    exec_output_dir_ (0),
    any_support_ (true),
    cdr_support_ (true),
    tc_support_ (true),
    obv_opt_accessor_ (0),
    gen_impl_files_ (false),
    gen_impl_debug_info_ (false),
    gen_copy_ctor_ (false),
    gen_assign_op_ (false),
    gen_thru_poa_collocation_ (true), // Default is thru_poa.
    gen_direct_collocation_ (false),
    gen_corba_e_ (false),
    gen_minimum_corba_ (false),
    gen_lwccm_ (false),
    gen_noeventccm_ (false),
    opt_tc_ (false),
    ami4ccm_call_back_ (false),
    ami_call_back_ (false),
    gen_amh_classes_ (false),
    gen_tie_classes_ (false),
    gen_smart_proxies_ (false),
    gen_inline_constants_ (true),
    gen_orb_h_include_ (true),
    gen_empty_anyop_header_ (false),
    lookup_strategy_ (TAO_PERFECT_HASH),
    dds_impl_ (DDS_NONE),
    opendds_sequence_suffix_ ("Seq"),
    void_type_ (0),
    ccmobject_ (0),
    messaging_ (0),
    messaging_exceptionholder_ (0),
    messaging_replyhandler_ (0),
    gen_anyop_files_ (false),
    gen_skel_files_ (true),
    gen_client_inline_ (true),
    gen_client_stub_ (true),
    gen_server_skeleton_ (true),
    gen_local_iface_anyops_ (true),
    use_clonable_in_args_ (false),
    gen_template_export_ (false),
    gen_ostream_operators_ (false),
    gen_custom_ending_ (true),
    gen_unique_guards_ (true),
    gen_ciao_svnt_ (false),
    gen_ciao_exec_idl_ (false),
    gen_ciao_exec_impl_ (false),
    gen_ciao_exec_reactor_impl_ (false),
    overwrite_not_exec_(false),
    gen_ciao_conn_impl_ (false),
    gen_dds_typesupport_idl_ (false),
    gen_ciao_valuefactory_reg_ (true),
    gen_stub_export_hdr_file_ (false),
    gen_skel_export_hdr_file_ (false),
    gen_svnt_export_hdr_file_ (false),
    gen_exec_export_hdr_file_ (false),
    gen_conn_export_hdr_file_ (false),
    tab_size_ (2),
    alt_mapping_ (false),
    in_facet_servant_ (false),
    gen_arg_traits_ (true),
    gen_anytypecode_adapter_ (false)
{
}

BE_GlobalData::~BE_GlobalData (void)
{
}

// To switch between changing or non-changing standard include files
// include files, so that #include statements can be
// generated with ""s or <>s respectively, for the standard include
// files (e.g. tao/corba.h)
void
BE_GlobalData::changing_standard_include_files (size_t changing)
{
  this->changing_standard_include_files_ = changing;
}

size_t
BE_GlobalData::changing_standard_include_files (void)
{
  return this->changing_standard_include_files_;
}

/************ Helper functions **************/
static const char*
be_change_idl_file_extension (UTL_String* idl_file,
                              const char *new_extension,
                              bool base_name_only = false,
                              bool for_anyop = false,
                              bool for_skel = false,
                              bool for_exec = false)
{
  // @@ This shouldn't happen anyway; but a better error handling
  // mechanism is needed.
  if (idl_file == 0 || new_extension == 0)
    {
      return 0;
    }

  static char fname[MAXPATHLEN];
  ACE_OS::memset (fname, 0, MAXPATHLEN);

  // Get the char* from the UTL_String.
  const char* string = idl_file->get_string ();

  // Get the base part of the filename, we try several extensions
  // before giving up.
  const char *base = 0;

  static const char* extensions[] = {
    ".idl",
    ".pidl",
    ".IDL",
    ".PIDL"
  };

  static int nextensions = sizeof(extensions)/sizeof(extensions[0]);

  for (int k = 0; k < nextensions; ++k)
    {
      base = ACE_OS::strstr (string, extensions[k]);

      if (base != 0)
        {
          break;
        }
    }

  if (base == 0)
    {
      return 0;
    }

  // Anyop * skel file output defaults to general output dir if not set.
  const char *output_path =
    be_util::get_output_path (for_anyop, for_skel, for_exec);

  if (!base_name_only && output_path != 0)
    {
      // Path info should also be added to fname.

      // Add path and "/".
      ACE_OS::sprintf (fname, "%s/", output_path);

      // Append the base part to fname.
      ACE_OS::strncpy (fname + ACE_OS::strlen (fname),
                       string,
                       base - string);
    }
  else
    {
      // Base_name_only or no putput_dir specified by user. JUST put the
      // base part to fname.
      ACE_OS::strncpy (fname, string, base - string);
    }

  // Turn '\' and '\\' into '/'.
  char* i = fname;

  for (char* j = fname; *j != 0; ++i, ++j)
    {
      if (*j == '\\')
        {
          *i = '/';

          if (*(j + 1) == '\\')
            {
              ++j;
            }
        }
      else
        {
          *i = *j;
        }
    }

  *i = 0;

  // Append the newextension.
  ACE_OS::strcat (fname, new_extension);
  return fname;
}

const char *
BE_GlobalData::be_get_client_hdr (UTL_String *idl_file_name,
                                  bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  ACE_CString fn (idl_file_name->get_string ());
  ACE_CString fn_ext = fn.substr (fn.length () - 5);
  bool orb_file = (fn_ext == ".pidl" || fn_ext == ".PIDL");

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  return be_change_idl_file_extension (idl_file_name,
                                       orb_file
                                         ? "C.h"
                                         : be_global->client_hdr_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_client_stub (UTL_String *idl_file_name)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_stub_ending ());
}

const char *
BE_GlobalData::be_get_client_inline (UTL_String *idl_file_name,
                                     bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_inline_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_hdr (UTL_String *idl_file_name,
                                  bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  ACE_CString fn (idl_file_name->get_string ());
  ACE_CString fn_ext = fn.substr (fn.length () - 5);
  bool orb_file = (fn_ext == ".pidl" || fn_ext == ".PIDL");

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  return be_change_idl_file_extension (idl_file_name,
                                       orb_file
                                         ? "S.h"
                                         : be_global->server_hdr_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_svnt_template_hdr (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  ACE_CString fn (idl_file_name->get_string ());
  ACE_CString fn_ext = fn.substr (fn.length () - 5);
  bool orb_file = (fn_ext == ".pidl" || fn_ext == ".PIDL");

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  return be_change_idl_file_extension (idl_file_name,
                                       orb_file
                                         ? "S.h"
                                         : be_global->ciao_svnt_header_template_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_implementation_hdr (UTL_String *idl_file_name,
                                          bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_hdr_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_skel (UTL_String *idl_file_name,
                                           bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_skel_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_hdr (UTL_String *idl_file_name,
                                           bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_hdr_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_server_skeleton (UTL_String *idl_file_name)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_skeleton_ending (),
                                       false,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_server_template_skeleton (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_skeleton_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_anyop_header (UTL_String *idl_file_name,
                                    bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->anyop_header_ending (),
                                       base_name_only,
                                       true);
}

const char *
BE_GlobalData::be_get_anyop_source (UTL_String *idl_file_name,
                                    bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->anyop_source_ending (),
                                       base_name_only,
                                       true);
}

const char *
BE_GlobalData::be_get_ciao_svnt_header (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_header_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_svnt_template_header (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_header_template_ending (),
                                       base_name_only);
}


const char *
BE_GlobalData::be_get_ciao_svnt_source (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_source_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_svnt_template_source (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_source_template_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_header (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_exec_header_ending (),
                                       base_name_only,
                                       false,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_ciao_exec_source (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_exec_source_ending (),
                                       base_name_only,
                                       false,
                                       false,
                                       true);
}

const char *
BE_GlobalData::be_get_ciao_exec_stub_header (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->ciao_exec_stub_header_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_idl (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->ciao_exec_idl_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_conn_header (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_conn_header_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_conn_source (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_conn_source_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_dds_typesupport_header (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->dds_typesupport_hdr_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_idl (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->ciao_ami_conn_idl_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_impl_hdr (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->ciao_ami_conn_impl_hdr_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_impl_src (
  UTL_String *idl_file_name,
  bool base_name_only)
{
  return
    be_change_idl_file_extension (
      idl_file_name,
      be_global->ciao_ami_conn_impl_src_ending (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_client_hdr_fname (
  bool base_name_only)
{
  return
    be_get_client_hdr (idl_global->stripped_filename (),
                       base_name_only);
}

const char *
BE_GlobalData::be_get_client_stub_fname (void)
{
  return
    be_get_client_stub (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_client_inline_fname (
  bool base_name_only)
{
  return
    be_get_client_inline (idl_global->stripped_filename (),
                          base_name_only);
}

const char *
BE_GlobalData::be_get_server_hdr_fname (
  bool base_name_only)
{
  return
    be_get_server_hdr (idl_global->stripped_filename (),
                       base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_hdr_fname (
  bool base_name_only)
{
  return
    be_get_implementation_hdr (idl_global->stripped_filename (),
                               base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_skel_fname (
  bool base_name_only)
{
  return
    be_get_implementation_skel (idl_global->stripped_filename (),
                                base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_hdr_fname (
  bool base_name_only)
{
  return
    be_get_server_template_hdr (idl_global->stripped_filename (),
                                base_name_only);
}

const char *
BE_GlobalData::be_get_server_skeleton_fname (void)
{
  return
    be_get_server_skeleton (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_implementation_skeleton_fname (void)
{
  return
    be_get_implementation_skel (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_server_template_skeleton_fname (
  bool base_name_only)
{
  return
    be_get_server_template_skeleton (idl_global->stripped_filename (),
                                     base_name_only);
}

const char *
BE_GlobalData::be_get_anyop_source_fname (
  bool base_name_only)
{
  return
    be_get_anyop_source (idl_global->stripped_filename (),
                         base_name_only);
}

const char *
BE_GlobalData::be_get_anyop_header_fname (
  bool base_name_only)
{
  return
    be_get_anyop_header (idl_global->stripped_filename (),
                         base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_svnt_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_svnt_header (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_tmpl_svnt_hdr_fname (
    bool base_name_only)
{
  return
    be_get_ciao_svnt_template_header (idl_global->stripped_filename (),
                                      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_svnt_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_svnt_source (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_tmpl_svnt_src_fname (
    bool base_name_only)
{
  return
    be_get_ciao_svnt_template_source (idl_global->stripped_filename (),
                                      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_header (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_source (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_stub_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_stub_header (
      idl_global->stripped_filename (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_exec_idl_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_idl (
      idl_global->stripped_filename (),
      base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_conn_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_conn_header (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_conn_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_conn_source (idl_global->stripped_filename (),
                             base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_idl_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_idl (idl_global->stripped_filename (),
                              base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_impl_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_impl_hdr (idl_global->stripped_filename (),
                                   base_name_only);
}

const char *
BE_GlobalData::be_get_ciao_ami_conn_impl_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_impl_src (idl_global->stripped_filename (),
                                   base_name_only);
}

const char*
BE_GlobalData::skel_export_macro (void) const
{
  if (this->skel_export_macro_ == 0)
    {
      return "";
    }

  return this->skel_export_macro_;
}

void
BE_GlobalData::skel_export_macro (const char *s)
{
  ACE::strdelete (this->skel_export_macro_);
  this->skel_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::skel_export_include (void) const
{
  return this->skel_export_include_;
}

void
BE_GlobalData::skel_export_include (const char *s)
{
  ACE::strdelete (this->skel_export_include_);
  this->skel_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::skel_export_file (void) const
{
  return this->skel_export_file_;
}

void
BE_GlobalData::skel_export_file (const char *s)
{
  ACE::strdelete (this->skel_export_file_);
  this->skel_export_file_ = ACE::strnew (s);
}

const char*
BE_GlobalData::stub_export_macro (void) const
{
  if (this->stub_export_macro_ == 0)
    {
      return "";
    }

  return this->stub_export_macro_;
}

void
BE_GlobalData::stub_export_macro (const char *s)
{
  ACE::strdelete (this->stub_export_macro_);
  this->stub_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::stub_export_include (void) const
{
  return this->stub_export_include_;
}

void
BE_GlobalData::stub_export_include (const char *s)
{
  ACE::strdelete (this->stub_export_include_);
  this->stub_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::stub_export_file (void) const
{
  return this->stub_export_file_;
}

void
BE_GlobalData::stub_export_file (const char *s)
{
  ACE::strdelete (this->stub_export_file_);
  this->stub_export_file_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_export_macro (void) const
{
  if (this->anyop_export_macro_ == 0)
    {
      return "";
    }

  return this->anyop_export_macro_;
}

void
BE_GlobalData::anyop_export_macro (const char *s)
{
  ACE::strdelete (this->anyop_export_macro_);
  this->anyop_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_export_include (void) const
{
  return this->anyop_export_include_;
}

void
BE_GlobalData::anyop_export_include (const char *s)
{
  ACE::strdelete (this->anyop_export_include_);
  this->anyop_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::exec_export_macro (void) const
{
  if (this->exec_export_macro_ == 0)
    {
      return "";
    }

  return this->exec_export_macro_;
}

void
BE_GlobalData::exec_export_macro (const char *s)
{
  ACE::strdelete (this->exec_export_macro_);
  this->exec_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::exec_export_include (void) const
{
  return this->exec_export_include_;
}

void
BE_GlobalData::exec_export_include (const char *s)
{
  ACE::strdelete (this->exec_export_include_);
  this->exec_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::svnt_export_macro (void) const
{
  if (this->svnt_export_macro_ == 0)
    {
      return "";
    }

  return this->svnt_export_macro_;
}

void
BE_GlobalData::svnt_export_macro (const char *s)
{
  ACE::strdelete (this->svnt_export_macro_);
  this->svnt_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::svnt_export_include (void) const
{
  return this->svnt_export_include_;
}

void
BE_GlobalData::svnt_export_include (const char *s)
{
  ACE::strdelete (this->svnt_export_include_);
  this->svnt_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::conn_export_macro (void) const
{
  if (this->conn_export_macro_ == 0)
    {
      return "";
    }

  return this->conn_export_macro_;
}

void
BE_GlobalData::conn_export_macro (const char *s)
{
  ACE::strdelete (this->conn_export_macro_);
  this->conn_export_macro_ = ACE::strnew (s);
}

const char*
BE_GlobalData::conn_export_include (void) const
{
  return this->conn_export_include_;
}

void
BE_GlobalData::conn_export_include (const char *s)
{
  ACE::strdelete (this->conn_export_include_);
  this->conn_export_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::pch_include (void) const
{
  return this->pch_include_;
}

void
BE_GlobalData::pch_include (const char *s)
{
  ACE::strdelete (this->pch_include_);
  this->pch_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::pre_include (void) const
{
  return this->pre_include_;
}

void
BE_GlobalData::pre_include (const char *s)
{
  ACE::strdelete (this->pre_include_);
  this->pre_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::post_include (void) const
{
  return this->post_include_;
}

void
BE_GlobalData::post_include (const char *s)
{
  ACE::strdelete (this->post_include_);
  this->post_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::include_guard (void) const
{
  return this->include_guard_;
}

void
BE_GlobalData::include_guard (const char *s)
{
  ACE::strdelete (this->include_guard_);
  this->include_guard_ = ACE::strnew (s);
}

const char*
BE_GlobalData::safe_include (void) const
{
  return this->safe_include_;
}

void
BE_GlobalData::safe_include (const char *s)
{
  ACE::strdelete (this->safe_include_);
  this->safe_include_ = ACE::strnew (s);
}

const char*
BE_GlobalData::unique_include (void) const
{
  return this->unique_include_;
}

void
BE_GlobalData::unique_include (const char *s)
{
  ACE::strdelete (this->unique_include_);
  this->unique_include_ = ACE::strnew (s);
}

void
BE_GlobalData::versioning_begin (const char * s)
{
  this->versioning_begin_ =
    ACE_CString ("\n\n")
    + ACE_CString (s)
    + ACE_CString ("\n\n");

  this->core_versioning_end_ += this->versioning_begin_;
  // Yes, "begin".
}

const char *
BE_GlobalData::versioning_begin (void) const
{
  return this->versioning_begin_.c_str ();
}

const char *
BE_GlobalData::core_versioning_begin (void) const
{
  return this->core_versioning_begin_.c_str ();
}

void
BE_GlobalData::versioning_end (const char * s)
{
  this->versioning_end_ =
    ACE_CString ("\n\n")
    + ACE_CString (s)
    + ACE_CString ("\n\n");

  this->core_versioning_begin_ =
    this->versioning_end_  // Yes, "end".
    + this->core_versioning_begin_;  // Initialized in constructor.
}

const char *
BE_GlobalData::versioning_end (void) const
{
  return this->versioning_end_.c_str ();
}

const char *
BE_GlobalData::core_versioning_end (void) const
{
  return this->core_versioning_end_.c_str ();
}

// Set the client_hdr_ending.
void
BE_GlobalData::client_hdr_ending (const char* s)
{
  ACE::strdelete (client_hdr_ending_);
  this->client_hdr_ending_ = ACE::strnew (s);
}

// Get the client_hdr_ending.
const char*
BE_GlobalData::client_hdr_ending (void) const
{
  return this->client_hdr_ending_;
}

void
BE_GlobalData::client_inline_ending  (const char* s)
{
  ACE::strdelete (client_inline_ending_);
  this->client_inline_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::client_inline_ending (void) const
{
  return this->client_inline_ending_;
}

// Set the client_stub_ending.
void
BE_GlobalData::client_stub_ending (const char* s)
{
  ACE::strdelete (this->client_stub_ending_);
  this->client_stub_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::client_stub_ending (void) const
{
  return this->client_stub_ending_;
}

void
BE_GlobalData::server_hdr_ending (const char* s)
{
  ACE::strdelete (this->server_hdr_ending_);
  this->server_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_hdr_ending (void) const
{
  return this->server_hdr_ending_;
}

void
BE_GlobalData::implementation_hdr_ending (const char* s)
{
  ACE::strdelete (this->implementation_hdr_ending_);
  this->implementation_hdr_ending_ = ACE::strnew (s);
}

void
BE_GlobalData::implementation_skel_ending (const char* s)
{
  ACE::strdelete (this->implementation_skel_ending_);
  this->implementation_skel_ending_ = ACE::strnew (s);
}

void
BE_GlobalData::impl_class_prefix (const char* s)
{
  ACE::strdelete (this->impl_class_prefix_);
  this->impl_class_prefix_ = ACE::strnew (s);
}

void
BE_GlobalData::impl_class_suffix (const char* s)
{
  ACE::strdelete (this->impl_class_suffix_);
  this->impl_class_suffix_ = ACE::strnew (s);
}

const char*
BE_GlobalData::impl_class_prefix (void) const
{
  return this->impl_class_prefix_;
}

const char*
BE_GlobalData::implementation_hdr_ending (void) const
{
  return this->implementation_hdr_ending_;
}

const char*
BE_GlobalData::impl_class_suffix (void) const
{
  return this->impl_class_suffix_;
}

const char*
BE_GlobalData::implementation_skel_ending (void) const
{
  return this->implementation_skel_ending_;
}

void
BE_GlobalData::server_template_hdr_ending (const char* s)
{
  ACE::strdelete (this->server_template_hdr_ending_);
  this->server_template_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_template_hdr_ending (void) const
{
  return this->server_template_hdr_ending_;
}

void
BE_GlobalData::server_skeleton_ending (const char* s)
{
  ACE::strdelete (this->server_skeleton_ending_);
  this->server_skeleton_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_skeleton_ending (void) const
{
  return this->server_skeleton_ending_;
}

void
BE_GlobalData::server_template_skeleton_ending (const char* s)
{
  ACE::strdelete (this->server_template_skeleton_ending_);
  this->server_template_skeleton_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_template_skeleton_ending (void) const
{
  return this->server_template_skeleton_ending_;
}

void
BE_GlobalData::anyop_header_ending (const char* s)
{
  ACE::strdelete (this->anyop_hdr_ending_);
  this->anyop_hdr_ending_ = ACE::strnew (s);
}

void
BE_GlobalData::use_clonable_in_args (bool clonable)
{
  this->use_clonable_in_args_ = clonable;
}

bool
BE_GlobalData::use_clonable_in_args (void) const
{
  return this->use_clonable_in_args_;
}

bool
BE_GlobalData::gen_template_export (void) const
{
  return this->gen_template_export_;
}

void
BE_GlobalData::gen_template_export (bool val)
{
  this->gen_template_export_ = val;
}

bool
BE_GlobalData::gen_ostream_operators (void) const
{
  return this->gen_ostream_operators_;
}

void
BE_GlobalData::gen_ostream_operators (bool val)
{
  this->gen_ostream_operators_ = val;
}

const char*
BE_GlobalData::anyop_header_ending (void) const
{
  return this->anyop_hdr_ending_;
}

void
BE_GlobalData::anyop_source_ending (const char* s)
{
  ACE::strdelete (this->anyop_src_ending_);
  this->anyop_src_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_source_ending (void) const
{
  return this->anyop_src_ending_;
}

void
BE_GlobalData::ciao_svnt_header_ending (const char* s)
{
  ACE::strdelete (this->ciao_svnt_hdr_ending_);
  this->ciao_svnt_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_svnt_header_ending (void) const
{
  return this->ciao_svnt_hdr_ending_;
}

void
BE_GlobalData::ciao_svnt_source_ending (const char* s)
{
  ACE::strdelete (this->ciao_svnt_src_ending_);
  this->ciao_svnt_src_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_svnt_source_ending (void) const
{
  return this->ciao_svnt_src_ending_;
}

void
BE_GlobalData::ciao_svnt_header_template_ending (const char* s)
{
  ACE::strdelete (this->ciao_svnt_hdr_template_ending_);
  this->ciao_svnt_hdr_template_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_svnt_header_template_ending (void) const
{
  return this->ciao_svnt_hdr_template_ending_;
}

void
BE_GlobalData::ciao_svnt_source_template_ending (const char* s)
{
  ACE::strdelete (this->ciao_svnt_src_template_ending_);
  this->ciao_svnt_src_template_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_svnt_source_template_ending (void) const
{
  return this->ciao_svnt_src_template_ending_;
}

void
BE_GlobalData::ciao_exec_header_ending (const char* s)
{
  ACE::strdelete (this->ciao_exec_hdr_ending_);
  this->ciao_exec_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_exec_header_ending (void) const
{
  return this->ciao_exec_hdr_ending_;
}

void
BE_GlobalData::ciao_exec_source_ending (const char* s)
{
  ACE::strdelete (this->ciao_exec_src_ending_);
  this->ciao_exec_src_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_exec_source_ending (void) const
{
  return this->ciao_exec_src_ending_;
}

void
BE_GlobalData::ciao_exec_stub_header_ending (const char* s)
{
  ACE::strdelete (this->ciao_exec_stub_hdr_ending_);
  this->ciao_exec_stub_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_exec_stub_header_ending (void) const
{
  return this->ciao_exec_stub_hdr_ending_;
}

void
BE_GlobalData::ciao_exec_idl_ending (const char* s)
{
  ACE::strdelete (this->ciao_exec_idl_ending_);
  this->ciao_exec_idl_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_exec_idl_ending (void) const
{
  return this->ciao_exec_idl_ending_;
}

void
BE_GlobalData::ciao_conn_header_ending (const char* s)
{
  ACE::strdelete (this->ciao_conn_hdr_ending_);
  this->ciao_conn_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_conn_header_ending (void) const
{
  return this->ciao_conn_hdr_ending_;
}

void
BE_GlobalData::ciao_conn_source_ending (const char* s)
{
  ACE::strdelete (this->ciao_conn_src_ending_);
  this->ciao_conn_src_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::ciao_conn_source_ending (void) const
{
  return this->ciao_conn_src_ending_;
}

void
BE_GlobalData::dds_typesupport_hdr_ending (const char* s)
{
  ACE::strdelete (this->dds_typesupport_hdr_ending_);
  this->dds_typesupport_hdr_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::dds_typesupport_hdr_ending (void) const
{
  return this->dds_typesupport_hdr_ending_;
}

void
BE_GlobalData::ciao_ami_conn_idl_ending (const char* s)
{
  ACE::strdelete (this->ciao_ami_conn_idl_ending_);
  this->ciao_ami_conn_idl_ending_ = ACE::strnew (s);
}

const char *
BE_GlobalData::ciao_ami_conn_idl_ending (void) const
{
  return this->ciao_ami_conn_idl_ending_;
}

void
BE_GlobalData::ciao_ami_conn_impl_hdr_ending (const char* s)
{
  ACE::strdelete (this->ciao_ami_conn_impl_hdr_ending_);
  this->ciao_ami_conn_impl_hdr_ending_ = ACE::strnew (s);
}

const char *
BE_GlobalData::ciao_ami_conn_impl_hdr_ending (void) const
{
  return this->ciao_ami_conn_impl_hdr_ending_;
}

void
BE_GlobalData::ciao_ami_conn_impl_src_ending (const char* s)
{
  ACE::strdelete (this->ciao_ami_conn_impl_src_ending_);
  this->ciao_ami_conn_impl_src_ending_ = ACE::strnew (s);
}

const char *
BE_GlobalData::ciao_ami_conn_impl_src_ending (void) const
{
  return this->ciao_ami_conn_impl_src_ending_;
}

void
BE_GlobalData::ciao_container_type (const char* s)
{
  ACE::strdelete (this->ciao_container_type_);
  this->ciao_container_type_ = ACE::strnew (s);
}

const char *
BE_GlobalData::ciao_container_type (void) const
{
  return this->ciao_container_type_;
}

void
BE_GlobalData::output_dir (const char* s)
{
  ACE::strdelete (this->output_dir_);
  this->output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::output_dir (void) const
{
  return this->output_dir_;
}
bool
BE_GlobalData::overwrite_not_exec (void) const
{
  return this->overwrite_not_exec_;
}

void
BE_GlobalData::overwrite_not_exec (bool val)
{
  this->overwrite_not_exec_ = val;
}

void
BE_GlobalData::skel_output_dir (const char* s)
{
  ACE::strdelete (this->skel_output_dir_);
  this->skel_output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::skel_output_dir (void) const
{
  return this->skel_output_dir_;
}

void
BE_GlobalData::stub_include_dir (const char* s)
{
  ACE::strdelete (this->stub_include_dir_);
  this->stub_include_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::stub_include_dir (void) const
{
  return this->stub_include_dir_;
}

void
BE_GlobalData::anyop_output_dir (const char* s)
{
  ACE::strdelete (this->anyop_output_dir_);
  this->anyop_output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_output_dir (void) const
{
  return this->anyop_output_dir_;
}

void
BE_GlobalData::exec_output_dir (const char* s)
{
  ACE::strdelete (this->exec_output_dir_);
  this->exec_output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::exec_output_dir (void) const
{
  return this->exec_output_dir_;
}
void
BE_GlobalData::any_support (bool val)
{
  this->any_support_ = val;
}

bool
BE_GlobalData::any_support (void) const
{
  return this->any_support_;
}

void
BE_GlobalData::cdr_support (bool val)
{
  this->cdr_support_ = val;
}

bool
BE_GlobalData::cdr_support (void) const
{
  return this->cdr_support_;
}

void
BE_GlobalData::tc_support (bool val)
{
  this->tc_support_ = val;
}

bool
BE_GlobalData::tc_support (void) const
{
  return this->tc_support_;
}

void
BE_GlobalData::obv_opt_accessor (bool val)
{
  this->obv_opt_accessor_ = val;
}

bool
BE_GlobalData::obv_opt_accessor (void) const
{
  return this->obv_opt_accessor_;
}

void
BE_GlobalData::gen_impl_files (bool val)
{
  this->gen_impl_files_ = val;
}

bool
BE_GlobalData::gen_impl_files (void) const
{
  return this->gen_impl_files_;
}

void
BE_GlobalData::gen_impl_debug_info (bool val)
{
  this->gen_impl_debug_info_ = val;
}

bool
BE_GlobalData::gen_impl_debug_info (void) const
{
  return this->gen_impl_debug_info_;
}

void
BE_GlobalData::gen_copy_ctor (bool val)
{
  this->gen_copy_ctor_ = val;
}

bool
BE_GlobalData::gen_copy_ctor (void) const
{
  return this->gen_copy_ctor_;
}

void
BE_GlobalData::gen_assign_op (bool val)
{
  this->gen_assign_op_ = val;
}

bool
BE_GlobalData::gen_assign_op (void) const
{
  return this->gen_assign_op_;
}

void
BE_GlobalData::gen_thru_poa_collocation (bool val)
{
  this->gen_thru_poa_collocation_ = val;
}

bool
BE_GlobalData::gen_thru_poa_collocation (void) const
{
  return this->gen_thru_poa_collocation_;
}

void
BE_GlobalData::gen_direct_collocation (bool val)
{
  this->gen_direct_collocation_ = val;
}

bool
BE_GlobalData::gen_direct_collocation (void) const
{
  return this->gen_direct_collocation_;
}

void
BE_GlobalData::gen_corba_e (bool val)
{
  this->gen_corba_e_ = val;
}

bool
BE_GlobalData::gen_corba_e (void) const
{
  return this->gen_corba_e_;
}

void
BE_GlobalData::gen_minimum_corba (bool val)
{
  this->gen_minimum_corba_ = val;
}

bool
BE_GlobalData::gen_minimum_corba (void) const
{
  return this->gen_minimum_corba_;
}

void
BE_GlobalData::gen_noeventccm (bool val)
{
  this->gen_noeventccm_ = val;
}

bool
BE_GlobalData::gen_noeventccm (void) const
{
  return this->gen_noeventccm_;
}

void
BE_GlobalData::gen_lwccm (bool val)
{
  this->gen_lwccm_ = val;
}

bool
BE_GlobalData::gen_lwccm (void) const
{
  return this->gen_lwccm_;
}


void
BE_GlobalData::opt_tc (bool val)
{
  this->opt_tc_ = val;
}

bool
BE_GlobalData::opt_tc (void) const
{
  return this->opt_tc_;
}

void
BE_GlobalData::ami4ccm_call_back (bool val)
{
  this->ami4ccm_call_back_ = val;
}

bool
BE_GlobalData::ami4ccm_call_back (void) const
{
  return this->ami4ccm_call_back_;
}

void
BE_GlobalData::ami_call_back (bool val)
{
  this->ami_call_back_ = val;
}

bool
BE_GlobalData::ami_call_back (void) const
{
  return this->ami_call_back_;
}

void
BE_GlobalData::gen_amh_classes (bool val)
{
  this->gen_amh_classes_ = val;
}

bool
BE_GlobalData::gen_amh_classes (void) const
{
  return this->gen_amh_classes_;
}

void
BE_GlobalData::gen_tie_classes (bool val)
{
  this->gen_tie_classes_ = val;
}

bool
BE_GlobalData::gen_tie_classes (void) const
{
  return this->gen_tie_classes_;
}

void
BE_GlobalData::gen_smart_proxies (bool val)
{
  this->gen_smart_proxies_ = val;
}

bool
BE_GlobalData::gen_smart_proxies (void) const
{
  return this->gen_smart_proxies_;
}

void
BE_GlobalData::gen_inline_constants (bool val)
{
  this->gen_inline_constants_ = val;
}

bool
BE_GlobalData::gen_inline_constants (void) const
{
  return this->gen_inline_constants_;
}

void
BE_GlobalData::gen_orb_h_include (bool val)
{
  this->gen_orb_h_include_ = val;
}

bool
BE_GlobalData::gen_orb_h_include (void) const
{
  return this->gen_orb_h_include_;
}

void
BE_GlobalData::gen_empty_anyop_header (bool val)
{
  this->gen_empty_anyop_header_ = val;
}

bool
BE_GlobalData::gen_empty_anyop_header (void) const
{
  return this->gen_empty_anyop_header_;
}

void
BE_GlobalData::lookup_strategy (LOOKUP_STRATEGY s)
{
  this->lookup_strategy_ = s;
}

BE_GlobalData::LOOKUP_STRATEGY
BE_GlobalData::lookup_strategy (void) const
{
  return this->lookup_strategy_;
}

void
BE_GlobalData::dds_impl (char const * const val)
{
  ACE_CString tmp (val, 0, false);

  if (tmp == "ndds")
    {
      this->dds_impl_ = NDDS;
    }
  else if (tmp == "opensplice")
    {
      this->dds_impl_ = OPENSPLICE;
    }
  else if (tmp == "opendds")
    {
      this->dds_impl_ = OPENDDS;
    }
  else if (tmp == "coredx")
    {
      this->dds_impl_ = COREDX;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%C: invalid or unknown ")
                  ACE_TEXT ("argument <%C> to -Wb,dds_impl\n"),
                  idl_global->prog_name (),
                  val));
    }
}

BE_GlobalData::DDS_IMPL
BE_GlobalData::dds_impl (void) const
{
  return this->dds_impl_;
}

void
BE_GlobalData::opendds_sequence_suffix (const char *val)
{
  this->opendds_sequence_suffix_ = val;
}

const char *
BE_GlobalData::opendds_sequence_suffix (void) const
{
  return this->opendds_sequence_suffix_.c_str ();
}

void
BE_GlobalData::destroy (void)
{
  ACE::strdelete (this->skel_export_macro_);
  this->skel_export_macro_ = 0;

  ACE::strdelete (this->skel_export_include_);
  this->skel_export_include_ = 0;

  ACE::strdelete (this->skel_export_file_);
  this->skel_export_file_ = 0;

  ACE::strdelete (this->stub_export_macro_);
  this->stub_export_macro_ = 0;

  ACE::strdelete (this->stub_export_include_);
  this->stub_export_include_ = 0;

  ACE::strdelete (this->stub_export_file_);
  this->stub_export_file_ = 0;

  ACE::strdelete (this->anyop_export_macro_);
  this->anyop_export_macro_ = 0;

  ACE::strdelete (this->anyop_export_include_);
  this->anyop_export_include_ = 0;

  ACE::strdelete (this->exec_export_macro_);
  this->exec_export_macro_ = 0;

  ACE::strdelete (this->exec_export_include_);
  this->exec_export_include_ = 0;

  ACE::strdelete (this->svnt_export_macro_);
  this->svnt_export_macro_ = 0;

  ACE::strdelete (this->svnt_export_include_);
  this->svnt_export_include_ = 0;

  ACE::strdelete (this->conn_export_macro_);
  this->conn_export_macro_ = 0;

  ACE::strdelete (this->conn_export_include_);
  this->conn_export_include_ = 0;

  ACE::strdelete (this->pch_include_);
  this->pch_include_ = 0;

  ACE::strdelete (this->pre_include_);
  this->pre_include_ = 0;

  ACE::strdelete (this->post_include_);
  this->post_include_ = 0;

  ACE::strdelete (this->include_guard_);
  this->include_guard_ = 0;

  ACE::strdelete (this->safe_include_);
  this->safe_include_ = 0;

  ACE::strdelete (this->unique_include_);
  this->unique_include_ = 0;

  ACE::strdelete (this->client_hdr_ending_);
  this->client_hdr_ending_ = 0;

  ACE::strdelete (this->client_stub_ending_);
  this->client_stub_ending_ = 0;

  ACE::strdelete (this->client_inline_ending_);
  this->client_inline_ending_ = 0;

  ACE::strdelete (this->server_hdr_ending_);
  this->server_hdr_ending_ = 0;

  ACE::strdelete (this->implementation_hdr_ending_);
  this->implementation_hdr_ending_ = 0;

  ACE::strdelete (this->implementation_skel_ending_);
  this->implementation_skel_ending_ = 0;

  ACE::strdelete (this->impl_class_prefix_);
  this->impl_class_prefix_ = 0;

  ACE::strdelete (this->impl_class_suffix_);
  this->impl_class_suffix_ = 0;

  ACE::strdelete (this->server_template_hdr_ending_);
  this->server_template_hdr_ending_ = 0;

  ACE::strdelete (this->server_skeleton_ending_);
  this->server_skeleton_ending_ = 0;

  ACE::strdelete (this->server_template_skeleton_ending_);
  this->server_template_skeleton_ending_ = 0;

  ACE::strdelete (this->anyop_hdr_ending_);
  this->anyop_hdr_ending_ = 0;

  ACE::strdelete (this->anyop_src_ending_);
  this->anyop_src_ending_ = 0;

  ACE::strdelete (this->ciao_svnt_hdr_ending_);
  this->ciao_svnt_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_svnt_src_ending_);
  this->ciao_svnt_src_ending_ = 0;

  ACE::strdelete (this->ciao_svnt_hdr_template_ending_);
  this->ciao_svnt_hdr_template_ending_ = 0;

  ACE::strdelete (this->ciao_svnt_src_template_ending_);
  this->ciao_svnt_src_template_ending_ = 0;

  ACE::strdelete (this->ciao_exec_hdr_ending_);
  this->ciao_exec_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_exec_src_ending_);
  this->ciao_exec_src_ending_ = 0;

  ACE::strdelete (this->ciao_exec_stub_hdr_ending_);
  this->ciao_exec_stub_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_exec_idl_ending_);
  this->ciao_exec_idl_ending_ = 0;

  ACE::strdelete (this->ciao_conn_hdr_ending_);
  this->ciao_conn_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_conn_src_ending_);
  this->ciao_conn_src_ending_ = 0;

  ACE::strdelete (this->dds_typesupport_hdr_ending_);
  this->dds_typesupport_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_ami_conn_idl_ending_);
  this->ciao_ami_conn_idl_ending_ = 0;

  ACE::strdelete (this->ciao_ami_conn_impl_hdr_ending_);
  this->ciao_ami_conn_impl_hdr_ending_ = 0;

  ACE::strdelete (this->ciao_ami_conn_impl_src_ending_);
  this->ciao_ami_conn_impl_src_ending_ = 0;

  ACE::strdelete (this->ciao_container_type_);
  this->ciao_container_type_ = 0;

  ACE::strdelete (this->output_dir_);
  this->output_dir_ = 0;

  ACE::strdelete (this->stub_include_dir_);
  this->stub_include_dir_ = 0;

  ACE::strdelete (this->skel_output_dir_);
  this->skel_output_dir_ = 0;

  ACE::strdelete (this->anyop_output_dir_);
  this->anyop_output_dir_ = 0;

  ACE::strdelete (this->exec_output_dir_);
  this->exec_output_dir_ = 0;

  if (0 != this->messaging_)
    {
      this->messaging_->destroy ();
      delete this->messaging_;
      this->messaging_ = 0;
    }

  if (0 != this->messaging_exceptionholder_)
    {
      this->messaging_exceptionholder_->destroy ();
      delete this->messaging_exceptionholder_;
      this->messaging_exceptionholder_ = 0;
    }

  if (0 != this->messaging_replyhandler_)
    {
      this->messaging_replyhandler_->destroy ();
      delete this->messaging_replyhandler_;
      this->messaging_replyhandler_ = 0;
    }

  if (0 != tao_cg)
    {
      tao_cg->destroy ();
    }
}

AST_PredefinedType *
BE_GlobalData:: void_type (void)
{
  if (0 == this->void_type_)
    {
      AST_Decl *d =
        idl_global->root ()->lookup_primitive_type (
          AST_Expression::EV_void);

      this->void_type_ = AST_PredefinedType::narrow_from_decl (d);
    }

  return this->void_type_;
}

be_interface *
BE_GlobalData::ccmobject (void)
{
  if (0 == this->ccmobject_)
    {
      Identifier *local_id = 0;
      ACE_NEW_RETURN (local_id,
                      Identifier ("CCMObject"),
                      0);
      UTL_ScopedName *local_name = 0;
      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (local_id, 0),
                      0);

      Identifier *module_id = 0;
      ACE_NEW_RETURN (module_id,
                      Identifier ("Components"),
                      0);
      UTL_ScopedName sn (module_id,
                         local_name);

      AST_Decl *d =
        idl_global->scopes ().top_non_null ()->lookup_by_name (&sn,
                                                               true);

      sn.destroy ();

      if (0 == d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_global::ccmobject - "
                             "lookup of CCMObject failed\n"),
                            0);
        }

      this->ccmobject_ = be_interface::narrow_from_decl (d);
    }

  return this->ccmobject_;
}

be_module *
BE_GlobalData::messaging (void)
{
  if (0 == this->messaging_)
    {
      Identifier *id = 0;
      UTL_ScopedName *sn = 0;

      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      0);

      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      0),
                      0);

      ACE_NEW_RETURN (this->messaging_,
                      be_module (sn),
                      0);

      this->messaging_->set_name (sn);
    }

  return this->messaging_;
}

be_valuetype *
BE_GlobalData::messaging_exceptionholder (void)
{
  if (0 == this->messaging_exceptionholder_)
    {
      Identifier *id = 0;
      be_module *msg = this->messaging ();
      idl_global->scopes ().push (msg);

      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      0);

      // Create a valuetype "ExceptionHolder"
      // from which we inherit.
      UTL_ScopedName *full_name = 0;
      ACE_NEW_RETURN (full_name,
                      UTL_ScopedName (id,
                                      0),
                      0);

      ACE_NEW_RETURN (id,
                      Identifier ("ExceptionHolder"),
                      0);

      UTL_ScopedName *local_name = 0;
      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (id,
                                      0),
                      0);

      full_name->nconc (local_name);

      ACE_NEW_RETURN (this->messaging_exceptionholder_,
                      be_valuetype (full_name,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0),
                      0);

      this->messaging_exceptionholder_->set_name (full_name);

      // Notice the valuetype "ExceptionHolder" that it is defined in the
      // "Messaging" module
      this->messaging_exceptionholder_->set_defined_in (msg);
      this->messaging_exceptionholder_->set_prefix_with_typeprefix (
                                            "omg.org"
                                          );

      idl_global->scopes ().pop ();

      // Notice the interface "ReplyHandler" that it is defined in the
      // "Messaging" module.
      this->messaging_exceptionholder_->set_defined_in (msg);
    }

  return this->messaging_exceptionholder_;
}

be_interface *
BE_GlobalData::messaging_replyhandler (void)
{
  if (0 == this->messaging_replyhandler_)
    {
      be_module *msg = this->messaging ();
      idl_global->scopes ().push (msg);

      Identifier *id = 0;
      UTL_ScopedName *local_name = 0;

      // Create a virtual module named "Messaging"
      // "and an interface "ReplyHandler"
      // from which we inherit.
      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      0);

      UTL_ScopedName *full_name = 0;
      ACE_NEW_RETURN (full_name,
                      UTL_ScopedName (id,
                                      0),
                      0);

      ACE_NEW_RETURN (id,
                      Identifier ("ReplyHandler"),
                      0);

      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (id,
                                      0),
                      0);

      full_name->nconc (local_name);

      ACE_NEW_RETURN (this->messaging_replyhandler_,
                      be_interface (full_name,
                                    0,  // inherited interfaces
                                    0,  // number of inherited interfaces
                                    0,  // ancestors
                                    0,  // number of ancestors
                                    0,  // not local
                                    0), // not abstract
                      0);

      this->messaging_replyhandler_->set_name (full_name);
      this->messaging_replyhandler_->set_prefix_with_typeprefix ("omg.org");

      idl_global->scopes ().pop ();

      // Notice the interface "ReplyHandler" that it is defined in the
      // "Messaging" module.
      this->messaging_replyhandler_->set_defined_in (msg);
    }

  return this->messaging_replyhandler_;
}

bool
BE_GlobalData::gen_anyop_files (void) const
{
  return this->gen_anyop_files_;
}

void
BE_GlobalData::gen_anyop_files (bool val)
{
  this->gen_anyop_files_ = val;
}

bool
BE_GlobalData::gen_skel_files (void) const
{
  return this->gen_skel_files_;
}

void
BE_GlobalData::gen_skel_files (bool val)
{
  this->gen_skel_files_ = val;
}

bool
BE_GlobalData::gen_client_inline (void) const
{
  return this->gen_client_inline_;
}

void
BE_GlobalData::gen_client_inline (bool val)
{
  this->gen_client_inline_ = val;
}

bool
BE_GlobalData::gen_client_stub (void) const
{
  return this->gen_client_stub_;
}

void
BE_GlobalData::gen_client_stub (bool val)
{
  this->gen_client_stub_ = val;
}

bool
BE_GlobalData::gen_server_skeleton (void) const
{
  return this->gen_server_skeleton_;
}

void
BE_GlobalData::gen_server_skeleton (bool val)
{
  this->gen_server_skeleton_ = val;
}

bool
BE_GlobalData::gen_local_iface_anyops (void) const
{
  return this->gen_local_iface_anyops_;
}

void
BE_GlobalData::gen_local_iface_anyops (bool val)
{
  this->gen_local_iface_anyops_ = val;
}

bool
BE_GlobalData::gen_custom_ending (void) const
{
  return this->gen_custom_ending_;
}

void
BE_GlobalData::gen_custom_ending (bool val)
{
  this->gen_custom_ending_ = val;
}

bool
BE_GlobalData::gen_unique_guards (void) const
{
  return this->gen_unique_guards_;
}

void
BE_GlobalData::gen_unique_guards (bool val)
{
  this->gen_unique_guards_ = val;
}

bool
BE_GlobalData::gen_ciao_svnt (void) const
{
  return this->gen_ciao_svnt_;
}

void
BE_GlobalData::gen_ciao_svnt (bool val)
{
  this->gen_ciao_svnt_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_idl (void) const
{
  return this->gen_ciao_exec_idl_;
}

void
BE_GlobalData::gen_ciao_exec_idl (bool val)
{
  this->gen_ciao_exec_idl_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_impl (void) const
{
  return this->gen_ciao_exec_impl_;
}

void
BE_GlobalData::gen_ciao_exec_impl (bool val)
{
  this->gen_ciao_exec_impl_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_reactor_impl (void) const
{
  return this->gen_ciao_exec_reactor_impl_;
}

void
BE_GlobalData::gen_ciao_exec_reactor_impl (bool val)
{
  this->gen_ciao_exec_reactor_impl_ = val;
}

bool
BE_GlobalData::gen_ciao_conn_impl (void) const
{
  return this->gen_ciao_conn_impl_;
}

void
BE_GlobalData::gen_ciao_conn_impl (bool val)
{
  this->gen_ciao_conn_impl_ = val;
}

bool
BE_GlobalData::gen_dds_typesupport_idl (void) const
{
  return this->gen_dds_typesupport_idl_;
}

void
BE_GlobalData::gen_dds_typesupport_idl (bool val)
{
  this->gen_dds_typesupport_idl_ = val;
}

bool
BE_GlobalData::gen_ciao_valuefactory_reg (void) const
{
  return this->gen_ciao_valuefactory_reg_;
}

void
BE_GlobalData::gen_ciao_valuefactory_reg (bool val)
{
  this->gen_ciao_valuefactory_reg_ = val;
}

bool
BE_GlobalData::gen_stub_export_hdr_file (void) const
{
  return this->gen_stub_export_hdr_file_;
}

void
BE_GlobalData::gen_stub_export_hdr_file (bool val)
{
  this->gen_stub_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_skel_export_hdr_file (void) const
{
  return this->gen_skel_export_hdr_file_;
}

void
BE_GlobalData::gen_skel_export_hdr_file (bool val)
{
  this->gen_skel_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_svnt_export_hdr_file (void) const
{
  return this->gen_svnt_export_hdr_file_;
}

void
BE_GlobalData::gen_svnt_export_hdr_file (bool val)
{
  this->gen_svnt_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_exec_export_hdr_file (void) const
{
  return this->gen_exec_export_hdr_file_;
}

void
BE_GlobalData::gen_exec_export_hdr_file (bool val)
{
  this->gen_exec_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_conn_export_hdr_file (void) const
{
  return this->gen_conn_export_hdr_file_;
}

void
BE_GlobalData::gen_conn_export_hdr_file (bool val)
{
  this->gen_conn_export_hdr_file_ = val;
}

bool
BE_GlobalData::alt_mapping (void) const
{
  return this->alt_mapping_;
}

void
BE_GlobalData::alt_mapping (bool val)
{
  this->alt_mapping_ = val;
}

bool
BE_GlobalData::in_facet_servant (void) const
{
  return this->in_facet_servant_;
}

void
BE_GlobalData::in_facet_servant (bool val)
{
  this->in_facet_servant_ = val;
}

bool
BE_GlobalData::gen_arg_traits (void) const
{
  return this->gen_arg_traits_;
}

void
BE_GlobalData::gen_arg_traits (bool val)
{
  this->gen_arg_traits_ = val;
}

bool
BE_GlobalData::gen_anytypecode_adapter (void) const
{
  return this->gen_anytypecode_adapter_;
}

void
BE_GlobalData::gen_anytypecode_adapter (bool val)
{
  this->gen_anytypecode_adapter_ = val;
}

unsigned long
BE_GlobalData::tab_size (void) const
{
  return this->tab_size_;
}

void
BE_GlobalData::tab_size (unsigned long val)
{
  this->tab_size_ = val;
}

ACE_CString
BE_GlobalData::spawn_options (void)
{
  return idl_global->idl_flags ();
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      // = Various 'h'eader_file_name_endings.
      case 'h':

        // <-hc Client's header file name ending>
        //      Default is "C.h".
        // <-hs Server's header file name ending>
        //       Default is "S.h".
        // <-hT Server's template hdr file name ending>
        //       Default is "S_T.h".
        // <-hI Server's implementation header file name ending>
        //       Default is "I.h".

        if (av[i][2] == 'c')
          {
            // Client stub's header file ending.
            // @@ No error handling done here.
            idl_global->append_idl_flag (av[i + 1]);
            be_global->client_hdr_ending (av[i + 1]);
            ++i;
          }
        else if (av[i][2] == 's')
          {
            // Server skeleton's header file.
            idl_global->append_idl_flag (av[i + 1]);
            be_global->server_hdr_ending (av[i + 1]);
            ++i;
          }
        else if (av[i][2] == 'T')
          {
            // Server template header ending.
            idl_global->append_idl_flag (av[i + 1]);
            be_global->server_template_hdr_ending (av[i + 1]);
            ++i;
         }
        else if (av[i][2] == 'I')
          {
            // Server implementation header ending.
            idl_global->append_idl_flag (av[i + 1]);
            be_global->implementation_hdr_ending (av[i + 1]);
            ++i;
          }
        else
          {
            // I expect 'c' or 's' or 'I' or 'T' after this.
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      case 'b':
        if (av[i][2] == '\0')
          {
            be_global->use_clonable_in_args(true);
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand")
                ACE_TEXT (" the '%C' option\n"),
                av[i]
              ));
          }
        break;
      // = Various 'c'lient side stub file_name_endings.
      case 'c':
        // <-cs Client stub's file name ending>
        //      Default is "C.cpp".
        // <-ci Client inline file name ending>
        //      Default is "C.inl".

        if (av[i][2] == 's')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->client_stub_ending (av[i + 1]);
            i++;
          }
        else if (av[i][2] == 'i')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->client_inline_ending (av[i + 1]);
            i++;
          }
        else
          {
            // I expect 's' or 'i' after 'c'.
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      // = Various 's'erver side skeleton file name endings.
      case 's':
        // <-ss Server's skeleton file name ending>
        //      Default is "S.cpp".
        // <-sT Server's template skeleton file name ending>
        //      Default is "S_T.cpp".
        // <-sI Server's implementation skeleton file name ending>
        //      Default is "I.cpp".

        if (av[i][2] == 's')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->server_skeleton_ending (av[i + 1]);
            ++i;
          }
        else if (av[i][2] == 'T')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->server_template_skeleton_ending (av[i + 1]);
            ++i;
          }
        else if (av[i][2] == 'I')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->implementation_skel_ending (av[i + 1]);
            ++i;
          }
        else
          {
            // I expect 's' or 'T' or or 't' after 's'.
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
        // Operation lookup strategy.
        // <perfect_hash>, <dynamic_hash> or <binary_search>
        // Default is perfect.
      case 'H':
        idl_global->append_idl_flag (av[i + 1]);

        if (av[i + 1] == 0 || av[i + 1][0] == '-')
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("no selection for -H option\n")));
          }
        else if (ACE_OS::strcmp (av[i+1], "dynamic_hash") == 0)
          {
            be_global->lookup_strategy (
                BE_GlobalData::TAO_DYNAMIC_HASH
              );
          }
        else if (ACE_OS::strcmp (av[i + 1], "perfect_hash") == 0)
          {
            be_global->lookup_strategy (
                BE_GlobalData::TAO_PERFECT_HASH
              );
          }
        else if (ACE_OS::strcmp (av[i + 1], "binary_search") == 0)
          {
            be_global->lookup_strategy (
                BE_GlobalData::TAO_BINARY_SEARCH
              );
          }
        else if (ACE_OS::strcmp (av[i + 1], "linear_search") == 0)
          {
            be_global->lookup_strategy (
                BE_GlobalData::TAO_LINEAR_SEARCH
              );
          }
        else
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%C: unknown operation lookup <%C>\n"),
                        av[0],
                        av[i + 1]));
          }

        ++i;
        break;
      // Switching between ""s and <>s when we generate
      // #include statements for the standard files (e.g. tao/corba.h)
      case 'i':
        if (av[i][2] == 'c')
          {
            be_global->changing_standard_include_files (1);
          }
        else if (av[i][2] == 'n')
          {
            be_global->changing_standard_include_files (0);
          }
        else if (av[i][2] == 'C')
          {
            if (av[i][3] == '\0')
              {
                be_global->stub_include_dir (av[i + 1]);
                ++i;
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand")
                    ACE_TEXT (" the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      // Path for the perfect hash generator(gperf) program. Default
      // is $ACE_ROOT/bin/ace_gperf.
      case 'g':
        if (av[i][2] == '\0')
          {
            idl_global->append_idl_flag (av[i + 1]);
            ACE_CString tmp (av[i + 1], 0, false);
#if defined (ACE_WIN32)
            // WIN32's CreateProcess needs the full executable name
            // when the gperf path is modified, but not for the default
            // path given above. Other platforms don't need the
            // executable name at all.
            tmp += "\\ace_gperf.exe";
#endif
            idl_global->gperf_path (tmp.fast_rep ());
            ++i;
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand")
                ACE_TEXT (" the '%C' option\n"),
                av[i]
              ));
          }

        break;
      // Directory where all the IDL-Compiler-Generated files are to
      // be kept. Default is the current directory from which the
      // <tao_idl> is called.
      case 'o':
        if (av[i][2] == '\0')
          {
            idl_global->append_idl_flag (av[i + 1]);
            int result = ACE_OS::mkdir (av[i + 1]);

            #if !defined (__BORLANDC__)
              if (result != 0 && errno != EEXIST)
            #else
              // The Borland RTL doesn't give EEXIST back, only EACCES in case
              // the directory exists, reported to Borland as QC 9495
              if (result != 0 && errno != EEXIST && errno != EACCES)
            #endif
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: unable to create directory %C")
                    ACE_TEXT (" specified by -o option\n"),
                    av[i + 1]
                  ));

                break;
              }

            be_global->output_dir (av[i + 1]);
            ++i;
          }
        else if (av[i][2] == 'A')
          {
            if (av[i][3] == '\0')
              {
                idl_global->append_idl_flag (av[i + 1]);
                int result = ACE_OS::mkdir (av[i + 1]);

                #if !defined (__BORLANDC__)
                  if (result != 0 && errno != EEXIST)
                #else
                  // The Borland RTL doesn't give EEXIST back, only EACCES in
                  // case the directory exists, reported to Borland as QC 9495
                  if (result != 0 && errno != EEXIST && errno != EACCES)
                #endif
                  {
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: unable to create directory %C")
                        ACE_TEXT (" specified by -oA option\n"),
                        av[i + 1]
                      ));

                    break;
                  }

                be_global->anyop_output_dir (av[i + 1]);
                ++i;
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand")
                    ACE_TEXT (" the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'E')
          {
            if (av[i][3] == '\0')
              {
                idl_global->append_idl_flag (av[i + 1]);
                int result = ACE_OS::mkdir (av[i + 1]);

                #if !defined (__BORLANDC__)
                  if (result != 0 && errno != EEXIST)
                #else
                  // The Borland RTL doesn't give EEXIST back, only EACCES in
                  // case the directory exists, reported to Borland as QC 9495
                  if (result != 0 && errno != EEXIST && errno != EACCES)
                #endif
                  {
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: unable to create directory %C")
                        ACE_TEXT (" specified by -oE option\n"),
                        av[i + 1]
                      ));

                    break;
                  }

                be_global->exec_output_dir (av[i + 1]);
                ++i;
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand")
                    ACE_TEXT (" the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'N')
           {
             if (av[i][3] == '\0')
               {
                 // Don't overwrite exec files.
                 be_global->overwrite_not_exec (true);
               }
             else
               {
                 ACE_ERROR ((
                     LM_ERROR,
                     ACE_TEXT ("IDL: I don't understand")
                     ACE_TEXT (" the '%C' option\n"),
                     av[i]
                   ));
               }
           }
        else if (av[i][2] == 'S')
          {
            if (av[i][3] == '\0')
              {
                idl_global->append_idl_flag (av[i + 1]);
                int result = ACE_OS::mkdir (av[i + 1]);

                #if !defined (__BORLANDC__)
                  if (result != 0 && errno != EEXIST)
                #else
                  // The Borland RTL doesn't give EEXIST back, only EACCES in
                  // case the directory exists, reported to Borland as QC 9495
                  if (result != 0 && errno != EEXIST && errno != EACCES)
                #endif
                  {
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: unable to create directory %C")
                        ACE_TEXT (" specified by -oS option\n"),
                        av[i + 1]
                      ));

                    break;
                  }

                be_global->skel_output_dir (av[i + 1]);
                ++i;
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand")
                    ACE_TEXT (" the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand")
                ACE_TEXT (" the '%C' option\n"),
                av[i]
              ));
          }

        break;
      case 'G':
        // Enable generation of ...
        if (av[i][2] == 'C')
          {
            // AMI with Call back.
            be_global->ami_call_back (true);
          }
        else if (av[i][2] == 'M')
          {
            // AMI4CCM calls implicit option 'C': AMI with Call back.
            be_global->ami_call_back (true);
             // Generate tie classes and files
            be_global->ami4ccm_call_back (true);
          }
        else if (av[i][2] == 'T')
          {
            // Generate tie classes and files
            be_global->gen_tie_classes (true);
          }
        else if (av[i][2] == 'H')
          {
            // AMH classes.
            be_global->gen_amh_classes (true);
          }
        else if (av[i][2] == 'X')
          {
            // Generate empty A.h file.
            be_global->gen_empty_anyop_header (true);
          }
        else if (av[i][2] == 'A')
          {
            // TAO-team-only, undocumented option to generate
            // Any operators into a separate set of files.
            be_global->gen_anyop_files (true);
          }
        else if (av[i][2] == 'c' && av[i][3] == 'n')
          {
            // CIAO connector impl code generation.
            be_global->gen_ciao_conn_impl (true);

            break;
          }
        else if (av[i][2] == 't' && av[i][3] == 's')
          {
            // DDS type support IDL generation.
            be_global->gen_dds_typesupport_idl (true);

            break;
          }
        else if (av[i][2] == 'e' && av[i][3] == 'x')
          {
            // CIAO executor impl code generation.
            be_global->gen_ciao_exec_impl (true);

            // should the reactor code be generated?
            if (av[i][4] == 'r')
              {
                be_global->gen_ciao_exec_reactor_impl (true);
              }
            break;
          }
        else if (av[i][2] == 's')
          {
            if (av[i][3] == 'p')
              {
                // Smart proxies.
                be_global->gen_smart_proxies (true);
              }
            else if (av[i][3] == 'e')
              {
                // Explicit sequence base class template export.
                be_global->gen_template_export (true);
              }
            else if (av[i][3] == 'v')
              {
                // CIAO servant code generation.
                be_global->gen_ciao_svnt (true);
              }
            else if (av[i][3] == 't' && av[i][4] == 'l')
              {
                // Generate code using STL types for strings
                // and sequences.
                be_global->alt_mapping (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }

            break;
          }
        else if (av[i][2] == 'x')
          {
            if (av[i][3] == 'h')
              {
                if (av[i][4] == 's')
                  {
                    if (av[i][5] == 't')
                      {
                        be_global->gen_stub_export_hdr_file (true);
                      }
                    else if (av[i][5] == 'k')
                      {
                        be_global->gen_skel_export_hdr_file (true);
                      }
                    else if (av[i][5] == 'v')
                      {
                        be_global->gen_svnt_export_hdr_file (true);
                      }
                    else
                      {
                        ACE_ERROR ((
                            LM_ERROR,
                            ACE_TEXT ("IDL: I don't understand ")
                            ACE_TEXT ("the '%s' option\n"),
                            av[i]
                          ));
                      }

                    break;
                  }
                else if (av[i][4] == 'e' && av[i][5] == 'x')
                  {
                    be_global->gen_exec_export_hdr_file (true);
                  }
                else if (av[i][4] == 'c' && av[i][5] == 'n')
                  {
                    be_global->gen_conn_export_hdr_file (true);
                  }
                else
                  {
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: I don't understand ")
                        ACE_TEXT ("the '%s' option\n"),
                        av[i]
                      ));
                  }

                break;
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%s' option\n"),
                    av[i]
                  ));
              }

            break;
          }
        else if (av[i][2] == 'u')
          {
            if (av[i][3] == 'c')
              {
                // Inline constants.
                be_global->gen_inline_constants (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }

            break;
          }
        else if (av[i][2] == 'c')
          {
            if (av[i][3] == 'e')
              {
                // CORBA/e.
                be_global->gen_corba_e (true);
              }
            else if (av[i][3] == 'l')
              {
                // CORBA/e.
                be_global->gen_lwccm (true);
              }
            else if (av[i][3] == 'm')
              {
                // NOEVENTS ccm, ccm without events .
                be_global->gen_noeventccm (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }
            break;
          }
        else if (av[i][2] == 'm')
          {
            if (av[i][3] == 'c')
              {
                // Minimum corba.
                be_global->gen_minimum_corba (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }
            break;
          }
        else if (av[i][2] == 't')
          {
            // Optimized typecode generation.
            be_global->opt_tc (1);
          }
        else if (av[i][2] == 'p')
          {
            // Generating Thru_POA collocated stubs.
            be_global->gen_thru_poa_collocation (true);
          }
        else if (av[i][2] == 'l')
          {
            if (av[i][3] == 'e' && av[i][4] == 'm')
              {
                be_global->gen_ciao_exec_idl (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%s' option\n"),
                    av[i]
                  ));
              }

            break;
          }
        else if (av[i][2] == 'd')
          {
            if ('\0' == av[i][3])
              {
                // Generating Direct collocated stubs.
                be_global->gen_direct_collocation (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'o')
          {
            if (av[i][3] == 's')
              {
                // Generating ostream operators for each type.
                this->gen_ostream_operators (true);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand ")
                    ACE_TEXT ("the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'I')
          {
            size_t options = ACE_OS::strlen (av[i]) - 3;
            size_t j;
            size_t k = i;
            // Generate implementation files.
            be_global->gen_impl_files (1);

            for (j = 0; j < options; ++j)
              {
                if (av[k][j + 3] == 's')
                  {
                    idl_global->append_idl_flag (av[i + 1]);
                    be_global->implementation_skel_ending (av[i + 1]);
                    ++i;
                  }
                else if (av[k][j + 3] == 'h')
                  {
                    idl_global->append_idl_flag (av[i + 1]);
                    be_global->implementation_hdr_ending (av[i + 1]);
                    ++i;
                  }
                else if (av[k][j + 3] == 'b')
                  {
                    idl_global->append_idl_flag (av[i + 1]);
                    be_global->impl_class_prefix (av[i + 1]);
                    ++i;
                  }
                else if (av[k][j + 3] == 'e')
                  {
                    idl_global->append_idl_flag (av[i + 1]);
                    be_global->impl_class_suffix (av[i + 1]);
                    ++i;
                  }
                else if (av[k][j + 3] == 'c')
                  {
                    be_global->gen_copy_ctor (true);
                  }
                else if (av[k][j + 3] == 'a')
                  {
                    be_global->gen_assign_op (true);
                  }
                else if (av[k][j + 3] == 'd')
                  {
                    be_global->gen_impl_debug_info (true);
                  }
                else if (ACE_OS::ace_isalpha (av[k][j + 3] ))
                  {
                    ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("IDL: I don't understand")
                        ACE_TEXT (" the '%C' option\n"),
                        av[i]
                      ));

                    idl_global->set_compile_flags (
                        idl_global->compile_flags ()
                        | IDL_CF_ONLY_USAGE
                      );
                  }
              }
          }
        else if (av[i][2] == 'a' && av[i][3] == 't' && av[i][4] == 'a')
          {
            // Generate the AnyTypeCode_Adapter version of the Any insert
            // policy - used with the sequences of basic types in the ORB.
            be_global->gen_anytypecode_adapter (true);
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      case 'S':
        // Suppress generation of...
        if (av[i][2] == 'a')
          {
            if (av[i][3] == 'l')
              {
                // Suppress Any support for local interfaces.
                be_global->gen_local_iface_anyops (false);
              }
            else if (av[i][3] == 't')
              {
                be_global->gen_arg_traits (false);
              }
            else
              {
                // Suppress all Any support.
                be_global->any_support (false);
              }
          }
        else if (av[i][2] == 'o' && av[i][3] == 'r' && av[i][4] == 'b' && '\0' == av[i][5])
          {
            be_global->gen_orb_h_include (false);
          }
        else if (av[i][2] == 'f' && av[i][3] == 'r')
          {
            // Suppress generation of valuetype factory registration
            // in CIAO servants.
            be_global->gen_ciao_valuefactory_reg (false);
          }
        else if (av[i][2] == 't')
          {
            // Suppress typecode generation
            // Anys must be suppressed as well.
            be_global->tc_support (false);
            be_global->any_support (false);
          }
        else if (av[i][2] == 'p')
          {
            // Suppress generating Thru_POA collocated stubs.
            be_global->gen_thru_poa_collocation (false);
          }
        else if (av[i][2] == 'd')
          {
            // sSppress generating Direct collocated stubs.
            be_global->gen_direct_collocation (false);
          }
        else if (av[i][2] == 'c')
          {
            if (av[i][3] == 'i')
              {
                // No stub inline.
                be_global->gen_client_inline (false);
              }
            else if (av[i][3] == 'c')
              {
                // No stub inline.
                be_global->gen_client_stub (false);
              }
            else if (av[i][3] == 'd' && av[i][4] == 'r')
              {
                // No cdr support.
                be_global->cdr_support (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'm')
          {
            // Turn off ccm preprocessing.
            idl_global->ignore_idl3 (true);
          }
        else if (av[i][2] == 'S')
          {
            if ('\0' == av[i][3])
              {
                // Disable skeleton file generation.
                be_global->gen_skel_files (false);
                be_global->gen_server_skeleton (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 's')
          {
            if (av[i][3] == 'c')
              {
                // No skeleton inline.
                be_global->gen_server_skeleton (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'e')
          {
            // Disable custom file endings for included idl/pidl
            // files from TAO specific include paths.
            be_global->gen_custom_ending (false);
          }
        else if (av[i][2] == 'g')
          {
            // Disable generation of unique guards.
            be_global->gen_unique_guards (false);
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      case 'T':
        if (av[i][2] == 'S')
          {
            unsigned long ul = ACE_OS::strtoul (av[i + 1], 0, 10);
            be_global->tab_size (ul);
            ++i;
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
          }

        break;
      default:
        ACE_ERROR ((
            LM_ERROR,
            ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
            av[i]
          ));

        idl_global->set_compile_flags (idl_global->compile_flags ()
                                       | IDL_CF_ONLY_USAGE);
        break;
    }
}

