// $Id$

// ============================================================================
//
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_global.cpp
//
// = DESCRIPTION
//    Stores global data specific to the compiler back end.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "be_global.h"
#include "be_codegen.h"
#include "be_generator.h"
#include "be_module.h"
#include "be_valuetype.h"
#include "be_interface.h"
#include "ast_predefined_type.h"
#include "utl_identifier.h"
#include "utl_string.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_ctype.h"

ACE_RCSID (be,
           be_global,
           "$Id$")

TAO_IDL_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : changing_standard_include_files_ (1),
    skel_export_macro_ (0),
    skel_export_include_ (0),
    stub_export_macro_ (0),
    stub_export_include_ (0),
    anyop_export_macro_ (0),
    anyop_export_include_ (0),
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
    server_inline_ending_ (ACE::strnew ("S.inl")),
    anyop_hdr_ending_ (ACE::strnew ("A.h")),
    anyop_src_ending_ (ACE::strnew ("A.cpp")),
    output_dir_ (0),
    skel_output_dir_ (0),
    anyop_output_dir_ (0),
    any_support_ (true),
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
    opt_tc_ (false),
    ami_call_back_ (false),
    gen_amh_classes_ (false),
    gen_tie_classes_ (false),
    gen_smart_proxies_ (false),
    gen_inline_constants_ (true),
    gen_dcps_type_support_ (false),
    gen_dcps_type_support_only_ (false),
    gen_orb_h_include_ (true),
    gen_empty_anyop_header_ (false),
    lookup_strategy_ (TAO_PERFECT_HASH),
    void_type_ (0),
    ccmobject_ (0),
    messaging_ (0),
    messaging_exceptionholder_ (0),
    exceptionholder_ (0),
    messaging_replyhandler_ (0),
    gen_anyop_files_ (false),
    gen_skel_files_ (true),
    gen_client_inline_ (true),
    gen_server_inline_ (true),
    gen_client_stub_ (true),
    gen_server_skeleton_ (true),
    gen_local_iface_anyops_ (true),
    use_clonable_in_args_ (false),
    gen_template_export_ (false),
    gen_ostream_operators_ (false),
    gen_custom_ending_ (true)
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
                              bool for_skel = false)
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
  const char *output_path = 0;

  if (for_anyop && 0 != be_global->anyop_output_dir ())
    {
      output_path = be_global->anyop_output_dir ();
    }
  else if (for_skel && 0 != be_global->skel_output_dir ())
    {
      output_path = be_global->skel_output_dir ();
    }
  else
    {
      output_path = be_global->output_dir ();
    }

  if (!base_name_only && output_path != 0)
    {
      // Path info should also be added to fname.

      // Add path and "/".
      ACE_OS::sprintf (fname, "%s/", output_path);

      // Append the base part to fname.
      ACE_OS::strncpy (fname + ACE_OS::strlen (fname), string, base - string);
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

          if (*(j+1) == '\\')
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
  if (!orb_file && !be_global->gen_custom_ending () &&
      idl_global->validate_orb_include (idl_file_name))
    orb_file = true;

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
  if (!orb_file && !be_global->gen_custom_ending () &&
      idl_global->validate_orb_include (idl_file_name))
    orb_file = true;

  return be_change_idl_file_extension (idl_file_name,
                                       orb_file
                                         ? "S.h"
                                         : be_global->server_hdr_ending (),
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
BE_GlobalData::be_get_server_inline (UTL_String *idl_file_name,
                                     bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_inline_ending (),
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
BE_GlobalData::be_get_client_hdr_fname (bool base_name_only)
{
  return be_get_client_hdr (idl_global->stripped_filename (),
                            base_name_only);
}

const char *
BE_GlobalData::be_get_client_stub_fname (void)
{
  return be_get_client_stub (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_client_inline_fname (bool base_name_only)
{
  return be_get_client_inline (idl_global->stripped_filename (),
                               base_name_only);
}

const char *
BE_GlobalData::be_get_server_hdr_fname (bool base_name_only)
{
  return be_get_server_hdr (idl_global->stripped_filename (),
                            base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_hdr_fname (bool base_name_only)
{
  return be_get_implementation_hdr (idl_global->stripped_filename (),
                                    base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_skel_fname (bool base_name_only)
{
  return be_get_implementation_skel (idl_global->stripped_filename (),
                                     base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_hdr_fname (bool base_name_only)
{
  return be_get_server_template_hdr (idl_global->stripped_filename (),
                                     base_name_only);
}

const char *
BE_GlobalData::be_get_server_skeleton_fname (void)
{
  return be_get_server_skeleton (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_implementation_skeleton_fname (void)
{
  return be_get_implementation_skel (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_server_template_skeleton_fname (bool base_name_only)
{
  return be_get_server_template_skeleton (idl_global->stripped_filename (),
                                          base_name_only);
}

const char *
BE_GlobalData::be_get_server_inline_fname (bool base_name_only)
{
  return be_get_server_inline (idl_global->stripped_filename (),
                               base_name_only);
}

const char *
BE_GlobalData::be_get_anyop_source_fname (bool base_name_only)
{
  return be_get_anyop_source (idl_global->stripped_filename (),
                              base_name_only);
}

const char *
BE_GlobalData::be_get_anyop_header_fname (bool base_name_only)
{
  return be_get_anyop_header (idl_global->stripped_filename (),
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
  this->skel_export_macro_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::skel_export_include (void) const
{
  return this->skel_export_include_;
}

void
BE_GlobalData::skel_export_include (const char *s)
{
  this->skel_export_include_ = ACE_OS::strdup (s);
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
  this->stub_export_macro_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::stub_export_include (void) const
{
  return this->stub_export_include_;
}

void
BE_GlobalData::stub_export_include (const char *s)
{
  this->stub_export_include_ = ACE_OS::strdup (s);
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
  this->anyop_export_macro_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::anyop_export_include (void) const
{
  return this->anyop_export_include_;
}

void
BE_GlobalData::anyop_export_include (const char *s)
{
  this->anyop_export_include_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::pch_include (void) const
{
  return this->pch_include_;
}

void
BE_GlobalData::pch_include (const char *s)
{
  this->pch_include_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::pre_include (void) const
{
  return this->pre_include_;
}

void
BE_GlobalData::pre_include (const char *s)
{
  this->pre_include_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::post_include (void) const
{
  return this->post_include_;
}

void
BE_GlobalData::post_include (const char *s)
{
  this->post_include_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::include_guard (void) const
{
  return this->include_guard_;
}

void
BE_GlobalData::include_guard (const char *s)
{
  this->include_guard_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::safe_include (void) const
{
  return this->safe_include_;
}

void
BE_GlobalData::safe_include (const char *s)
{
  this->safe_include_ = ACE_OS::strdup (s);
}

const char*
BE_GlobalData::unique_include (void) const
{
  return this->unique_include_;
}

void
BE_GlobalData::unique_include (const char *s)
{
  this->unique_include_ = ACE_OS::strdup (s);
}

void
BE_GlobalData::versioning_begin (const char * s)
{
  this->versioning_begin_ =
    ACE_CString ("\n\n")
    + ACE_CString (s)
    + ACE_CString ("\n\n");

  this->core_versioning_end_ += this->versioning_begin_;  // Yes, "begin".
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
  delete [] this->client_hdr_ending_;
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
  delete [] this->client_inline_ending_;
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
  delete [] this->client_stub_ending_;
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
  delete [] this->server_hdr_ending_;
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
  delete [] this->implementation_hdr_ending_;
  this->implementation_hdr_ending_ = ACE::strnew (s);
}

void
BE_GlobalData::implementation_skel_ending (const char* s)
{
  delete [] this->implementation_skel_ending_;
  this->implementation_skel_ending_ = ACE::strnew (s);
}

void
BE_GlobalData::impl_class_prefix (const char* s)
{
  delete [] this->impl_class_prefix_;
  this->impl_class_prefix_ = ACE::strnew (s);
}

void
BE_GlobalData::impl_class_suffix (const char* s)
{
  delete [] this->impl_class_suffix_;
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
  delete [] this->server_template_hdr_ending_;
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
  delete [] this->server_skeleton_ending_;
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
  delete [] this->server_template_skeleton_ending_;
  this->server_template_skeleton_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_template_skeleton_ending (void) const
{
  return this->server_template_skeleton_ending_;
}

void
BE_GlobalData::server_inline_ending (const char* s)
{
  delete [] this->server_inline_ending_;
  this->server_inline_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_inline_ending (void) const
{
  return this->server_inline_ending_;
}

void
BE_GlobalData::anyop_header_ending (const char* s)
{
  delete [] this->anyop_hdr_ending_;
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
  delete [] this->anyop_src_ending_;
  this->anyop_src_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_source_ending (void) const
{
  return this->anyop_src_ending_;
}

void
BE_GlobalData::output_dir (const char* s)
{
  delete [] this->output_dir_;
  this->output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::output_dir (void) const
{
  return this->output_dir_;
}

void
BE_GlobalData::skel_output_dir (const char* s)
{
  delete [] this->skel_output_dir_;
  this->skel_output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::skel_output_dir (void) const
{
  return this->skel_output_dir_;
}

void
BE_GlobalData::anyop_output_dir (const char* s)
{
  delete [] this->anyop_output_dir_;
  this->anyop_output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::anyop_output_dir (void) const
{
  return this->anyop_output_dir_;
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
BE_GlobalData::gen_dcps_type_support (bool val)
{
  this->gen_dcps_type_support_ = val;
}

bool
BE_GlobalData::gen_dcps_type_support (void) const
{
  return this->gen_dcps_type_support_;
}

void
BE_GlobalData::gen_dcps_type_support_only (bool val)
{
  this->gen_dcps_type_support_only_ = val;
}

bool
BE_GlobalData::gen_dcps_type_support_only (void) const
{
  return this->gen_dcps_type_support_only_;
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
BE_GlobalData::destroy (void)
{
  ACE_OS::free (this->pch_include_);
  this->pch_include_ = 0;

  ACE_OS::free (this->pre_include_);
  this->pre_include_ = 0;

  ACE_OS::free (this->post_include_);
  this->post_include_ = 0;

  ACE_OS::free (this->include_guard_);
  this->include_guard_ = 0;

  ACE_OS::free (this->safe_include_);
  this->safe_include_ = 0;

  ACE_OS::free (this->unique_include_);
  this->unique_include_ = 0;

  delete [] this->client_hdr_ending_;
  this->client_hdr_ending_ = 0;

  delete [] this->client_stub_ending_;
  this->client_stub_ending_ = 0;

  delete [] this->client_inline_ending_;
  this->client_inline_ending_ = 0;

  delete [] this->server_hdr_ending_;
  this->server_hdr_ending_ = 0;

  delete [] this->implementation_hdr_ending_;
  this->implementation_hdr_ending_ = 0;

  delete [] this->implementation_skel_ending_;
  this->implementation_skel_ending_ = 0;

  delete [] this->impl_class_prefix_;
  this->impl_class_prefix_ = 0;

  delete [] this->impl_class_suffix_;
  this->impl_class_suffix_ = 0;

  delete [] this->server_template_hdr_ending_;
  this->server_template_hdr_ending_ = 0;

  delete [] this->server_skeleton_ending_;
  this->server_skeleton_ending_ = 0;

  delete [] this->server_template_skeleton_ending_;
  this->server_template_skeleton_ending_ = 0;

  delete [] this->server_inline_ending_;
  this->server_inline_ending_ = 0;

  delete [] this->anyop_hdr_ending_;
  this->anyop_hdr_ending_ = 0;

  delete [] this->anyop_src_ending_;
  this->anyop_src_ending_ = 0;

  delete [] this->output_dir_;
  this->output_dir_ = 0;

  delete [] this->anyop_output_dir_;
  this->anyop_output_dir_ = 0;

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
        idl_global->scopes ().bottom ()->lookup_primitive_type (
                                              AST_Expression::EV_void
                                            );
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
                                                               true,
                                                               true,
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

be_valuetype *
BE_GlobalData::exceptionholder (void) const
{
  return this->exceptionholder_;
}

void
BE_GlobalData::exceptionholder (be_valuetype *val)
{
  this->exceptionholder_ = val;
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
BE_GlobalData::gen_server_inline (void) const
{
  return this->gen_server_inline_;
}

void
BE_GlobalData::gen_server_inline (bool val)
{
  this->gen_server_inline_ = val;
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
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
                ACE_TEXT (" the '%s' option\n"),
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
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
        // <-si Server's inline file name ending>
        //      Default is "S.inl".
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
        else if (av[i][2] == 'i')
          {
            idl_global->append_idl_flag (av[i + 1]);
            be_global->server_inline_ending (av[i + 1]);
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
            // I expect 's' or 'T' or 'i' or 't' after 's'.
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
                        ACE_TEXT ("%s: unknown operation lookup <%s>\n"),
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
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
                ACE_TEXT (" the '%s' option\n"),
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
                    ACE_TEXT ("IDL: unable to create directory %s")
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
                        ACE_TEXT ("IDL: unable to create directory %s")
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
                    ACE_TEXT (" the '%s' option\n"),
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
                        ACE_TEXT ("IDL: unable to create directory %s")
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
                    ACE_TEXT (" the '%s' option\n"),
                    av[i]
                  ));
              }
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand")
                ACE_TEXT (" the '%s' option\n"),
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
                    ACE_TEXT ("the '%s' option\n"),
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
                    ACE_TEXT ("the '%s' option\n"),
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
        else if (av[i][2] == 'd')
          {
            if (av[i][3] == 'c')
              {
                if (av[i][4] == 'p' && av[i][5] =='s')
                  {
                    if ('\0' == av[i][6])
                      {
                        // DDS DCPS type support.
                        be_global->gen_dcps_type_support (true);
                      }
                    else if (av[i][6] == 'o' && av[i][7] == 'n'
                      && av[i][8] == 'l' && av[i][9] == 'y' && '\0' == av[i][10])
                      {
                        // DDS DCPS type only support
                        be_global->gen_dcps_type_support (true);
                        be_global->gen_dcps_type_support_only (true);
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
              }
            else if ('\0' == av[i][3])
              {
                // Generating Direct collocated stubs.
                be_global->gen_direct_collocation (true);
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
                    ACE_TEXT ("the '%s' option\n"),
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
                        ACE_TEXT (" the '%s' option\n"),
                        av[i]
                      ));

                    idl_global->set_compile_flags (
                        idl_global->compile_flags ()
                        | IDL_CF_ONLY_USAGE
                      );
                  }
              }
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
            else
              {
                // Suppress all Any support.
                be_global->any_support (false);
              }
          }
        else if (av[i][2] == 'o' && av[i][3] == 'r' && av[i][4] =='b' && '\0' == av[i][5])
          {
            be_global->gen_orb_h_include (false);
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
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
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
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 's')
          {
            if (av[i][3] == 'i')
              {
                // No skeleton inline.
                be_global->gen_server_inline (false);
              }
            else if (av[i][3] == 'c')
              {
                // No skeleton inline.
                be_global->gen_server_skeleton (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                    av[i]
                  ));
              }
          }
        else if (av[i][2] == 'e')
          {
            // disable custom file endings for included idl/pidl
            // files from TAO specific include paths.
            be_global->gen_custom_ending (false);
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                av[i]
              ));
          }

        break;
      default:
        ACE_ERROR ((
            LM_ERROR,
            ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
            av[i]
          ));

        idl_global->set_compile_flags (idl_global->compile_flags ()
                                       | IDL_CF_ONLY_USAGE);
        break;
    }
}

// Prepare an argument for a BE
void
BE_GlobalData::prep_be_arg (char *s)
{
  static const char arg_macro[]            = "export_macro=";
  static const char arg_include[]          = "export_include=";
  static const char skel_arg_macro[]       = "skel_export_macro=";
  static const char skel_arg_include[]     = "skel_export_include=";
  static const char stub_arg_macro[]       = "stub_export_macro=";
  static const char stub_arg_include[]     = "stub_export_include=";
  static const char anyop_arg_macro[]      = "anyop_export_macro=";
  static const char anyop_arg_include[]    = "anyop_export_include=";
  static const char arg_pch_include[]      = "pch_include=";
  static const char arg_pre_include[]      = "pre_include=";
  static const char arg_post_include[]     = "post_include=";
  static const char arg_versioning_begin[] = "versioning_begin=";
  static const char arg_versioning_end[]   = "versioning_end=";
  static const char obv_opt_accessor[]     = "obv_opt_accessor";
  static const char include_guard[]        = "include_guard=";
  static const char safe_include[]         = "safe_include=";
  static const char unique_include[]       = "unique_include=";

  char* last = 0;

  for (char* arg = ACE_OS::strtok_r (s, ",", &last);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ",", &last))
    {
      if (ACE_OS::strstr (arg, arg_macro) == arg)
        {
          char* val = arg + sizeof (arg_macro) - 1;
          be_global->skel_export_macro (val);
          be_global->stub_export_macro (val);
          be_global->anyop_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, arg_include) == arg)
        {
          char* val = arg + sizeof (arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_macro) == arg)
        {
          char* val = arg + sizeof (skel_arg_macro) - 1;
          be_global->skel_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_include) == arg)
        {
          char* val = arg + sizeof (skel_arg_include) - 1;
          be_global->skel_export_include (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_macro) == arg)
        {
          char* val = arg + sizeof (stub_arg_macro) - 1;
          be_global->stub_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_include) == arg)
        {
          char* val = arg + sizeof (stub_arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, anyop_arg_macro) == arg)
        {
          char* val = arg + sizeof (anyop_arg_macro) - 1;
          be_global->anyop_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, anyop_arg_include) == arg)
        {
          char* val = arg + sizeof (anyop_arg_include) - 1;
          be_global->anyop_export_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pch_include) == arg)
        {
          char* val = arg + sizeof (arg_pch_include) - 1;
          be_global->pch_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pre_include) == arg)
        {
          char* val = arg + sizeof (arg_pre_include) - 1;
          be_global->pre_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_post_include) == arg)
        {
          char* val = arg + sizeof (arg_post_include) - 1;
          be_global->post_include (val);
        }
      else if (ACE_OS::strstr (arg, include_guard) == arg)
        {
          char* val = arg + sizeof (include_guard) - 1;
          be_global->include_guard (val);
        }
      else if (ACE_OS::strstr (arg, safe_include) == arg)
        {
          char* val = arg + sizeof (safe_include) - 1;
          be_global->safe_include (val);
        }
      else if (ACE_OS::strstr (arg, unique_include) == arg)
        {
          char* val = arg + sizeof (unique_include) - 1;
          be_global->unique_include (val);
        }
      else if (ACE_OS::strstr (arg, obv_opt_accessor) == arg)
        {
          be_global->obv_opt_accessor (1);
        }
      else if (ACE_OS::strstr (arg, arg_versioning_begin) == arg)
        {
          char const * const val = arg + sizeof (arg_versioning_begin) - 1;
          be_global->versioning_begin (val);
        }
      else if (ACE_OS::strstr (arg, arg_versioning_end) == arg)
        {
          char const * const val = arg + sizeof (arg_versioning_end) - 1;
          be_global->versioning_end (val);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%s%s%s%s"),
                      idl_global->prog_name (),
                      ACE_TEXT (": invalid or unknown argument <"),
                      arg,
                      ACE_TEXT ("> to back end\n")));
        }
    }
}

void
BE_GlobalData::arg_post_proc (void)
{
  // Let us try to use Perfect Hashing Operation Lookup Strategy. Let
  // us check whether things are fine with GPERF.
#if defined (ACE_HAS_GPERF) && !defined (ACE_USES_WCHAR)
  // If Perfect Hashing or Binary Search or Linear Search strategies
  // have been selected, let us make sure that it exists and will
  // work.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      // Testing whether GPERF works or no.
      if (idl_global->check_gperf () == -1)
        {
          // If gperf_path is an absolute path, try to call this
          // again with
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("TAO_IDL: warning, GPERF could not be executed\n")
              ACE_TEXT ("Perfect Hashing or Binary/Linear Search cannot be")
              ACE_TEXT (" done without GPERF\n")
              ACE_TEXT ("Now, using Dynamic Hashing..\n")
              ACE_TEXT ("To use Perfect Hashing or Binary/Linear")
              ACE_TEXT (" Search strategy\n")
              ACE_TEXT ("\t-Build gperf at $ACE_ROOT/apps/gperf/src\n")
              ACE_TEXT ("\t-Set the environment variable $ACE_ROOT")
              ACE_TEXT (" appropriately or add $ACE_ROOT/bin to the PATH\n")
              ACE_TEXT ("\t-Refer to Operation Lookup section in the TAO IDL")
              ACE_TEXT (" User Guide ($TAO_ROOT/docs/compiler.html)")
              ACE_TEXT (" for more details\n")
            ));

          // Switching over to Dynamic Hashing.
          be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
        }
    }
#else /* Not ACE_HAS_GPERF */
  // If GPERF is not there, we cannot use PERFECT_HASH strategy. Let
  // us go for DYNAMIC_HASH.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
    }
#endif /* ACE_HAS_GPERF */

  // Make sure that we are not suppressing TypeCode generation and asking for
  // optimized typecode support at the same time.
  if (!be_global->tc_support () && be_global->opt_tc ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Bad Combination -St and -Gt\n")));
    }
}

void
BE_GlobalData::usage (void) const
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,export_macro=<macro name>\t\t\tsets export macro ")
      ACE_TEXT ("for all files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,export_include=<include path>\t\tsets export include ")
      ACE_TEXT ("file for all files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,stub_export_macro=<macro name>\t\tsets export ")
      ACE_TEXT ("macro for client files only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,stub_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for client only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,skel_export_macro=<macro name>\t\tsets export ")
      ACE_TEXT ("macro for server files only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,skel_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for server only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,anyop_export_macro=<macro name>\t\tsets export macro ")
      ACE_TEXT ("for typecode/Any operator files only, when -GA option ")
      ACE_TEXT ("is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,anyop_export_include=<include path>\tsets export ")
      ACE_TEXT ("include file typecode/Any operator files only, when -GA ")
      ACE_TEXT ("option is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,pch_include=<include path>\t\t\tsets include ")
      ACE_TEXT ("file for precompiled header mechanism\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,pre_include=<include path>\t\t\tsets include ")
      ACE_TEXT ("file generate before any other includes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,post_include=<include path>\t\tsets include ")
      ACE_TEXT ("file generated at the end of the file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,include_guard=<include path>\t\tguard to prevent ")
      ACE_TEXT ("the generated client header file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,safe_include=<include path>\t\tinclude that should ")
      ACE_TEXT ("be used instead of the own generated client header file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,unique_include=<include path>\t\tinclude that should ")
      ACE_TEXT ("be generated as only contents of the generated client ")
      ACE_TEXT ("header file.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,obv_opt_accessor\t\t\t\toptimizes access to base class ")
      ACE_TEXT ("data in valuetypes\n")
    ));
#if (defined (ACE_HAS_VERSIONED_NAMESPACE)      \
     && ACE_HAS_VERSIONED_NAMESPACE == 1)       \
  || (defined (TAO_HAS_VERSIONED_NAMESPACE)      \
     && TAO_HAS_VERSIONED_NAMESPACE == 1)
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,versioning_begin\t\t\tSet text that opens a ")
      ACE_TEXT ("a \"versioned\" namespace\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,versioning_end\t\t\tSet text that closes a ")
      ACE_TEXT ("a \"versioned\" namespace\n")
    ));
#endif  /* ACE_HAS_VERSIONED_NAMESPACE || TAO_HAS_VERSIONED_NAMESPACE */
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -b\t\t\tUse a clonable argument type for oneway methods.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ci\t\t\tClient inline file name ending. Default is C.inl\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -cs\t\t\tClient stub's file name ending.")
      ACE_TEXT (" Default is C.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -g <gperf_path>\tPath for the GPERF program.")
      ACE_TEXT (" Default is $ACE_ROOT/bin/ace_gperf\n")));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GC \t\t\tGenerate the AMI classes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GH \t\t\tGenerate the AMH classes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gce \t\t\tGenerate code optimized for CORBA/e\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gmc \t\t\tGenerate code optimized for Minimum CORBA\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gd \t\t\tGenerate the code for direct collocation. Default ")
      ACE_TEXT ("is thru-POA collocation\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gdcps \t\tGenerate code supporting DDS DCPS type ")
      ACE_TEXT ("definitions.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gos \t\t\tGenerate std::ostream insertion operators.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gdcpsonly \t\t\tGenerate code only supporting DDS DCPS ")
      ACE_TEXT ("type serializer definitions.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GI[h|s|b|e|c|a|d]\tGenerate Implementation Files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\th - Implementation header file name ending.")
      ACE_TEXT (" Default is I.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\ts - Implementation skeleton file name ending.")
      ACE_TEXT (" Default is I.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\tb - Prefix to the implementation class names.")
      ACE_TEXT (" Default is 'no prefix'\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\te - Suffix to the implementation class names.")
      ACE_TEXT (" Default is _i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\tc - Generate copy constructors in the servant")
      ACE_TEXT (" implementation template files (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\ta - Generate assignment operators in the servant")
      ACE_TEXT (" implementation template files (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\td - Generate debug (source file/line#) information.")
      ACE_TEXT (" (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gp \t\t\tGenerate the code for thru-POA collocation")
      ACE_TEXT (" (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gsp\t\t\tGenerate the code for Smart Proxies\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gt\t\t\tenable optimized TypeCode support")
      ACE_TEXT (" (unopt by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GT\t\t\tgenerate tie class (and file)")
      ACE_TEXT (" generation (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("    \t\t\tNo effect if TypeCode generation is suppressed\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GA\t\t\tgenerate Any operator and type codes in *A.{h,cpp}")
      ACE_TEXT (" (generated in *C.{h,cpp} by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Guc\t\t\tgenerate uninlined constant if declared ")
      ACE_TEXT ("in a module (inlined by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gse\t\t\tgenerate explicit export of sequence's ")
      ACE_TEXT ("template base class (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hc\t\t\tClient's header file name ending.")
      ACE_TEXT (" Default is C.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hs\t\t\tServer's header file name ending.")
      ACE_TEXT (" Default is S.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hT\t\t\tServer's template hdr file name ending.")
      ACE_TEXT (" Default is S_T.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H perfect_hash\tTo force perfect hashed operation")
      ACE_TEXT (" lookup strategy (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H dynamic_hash\tTo force dynamic hashed operation")
      ACE_TEXT (" lookup strategy. Default is perfect hashing\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H linear_search\tTo force linear search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H binary_search\tTo force binary search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -in \t\t\tTo generate <>s for standard #include'd")
      ACE_TEXT (" files (non-changing files)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ic \t\t\tTo generate \"\"s for standard #include'd")
      ACE_TEXT (" files (changing files)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <output_dir>\tOutput directory for the generated files.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -oS <output_dir>\tOutput directory for the generated ")
      ACE_TEXT ("skeleton files. Default is -o value or current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -oA <output_dir>\tOutput directory for the generated anyop")
      ACE_TEXT ("files. Default is -o value or current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -si\t\t\tServer's inline file name ending.")
      ACE_TEXT (" Default is S.inl\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ss\t\t\tServer's skeleton file name ending.")
      ACE_TEXT (" Default is S.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -sT\t\t\tServer's template skeleton file name ending.")
      ACE_TEXT (" Default is S_T.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sa\t\t\tsuppress Any support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sal\t\t\tsuppress Any support")
      ACE_TEXT (" for local interfaces")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -St\t\t\tsuppress TypeCode support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sp\t\t\tsuppress generating Thru POA collocated")
      ACE_TEXT (" stubs (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sd\t\t\tsuppress generating Direct collocated")
      ACE_TEXT (" stubs (disable by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sm\t\t\tdisable IDL3 equivalent IDL preprocessing")
      ACE_TEXT (" (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -SS\t\t\tsuppress generating skeleton implementation")
      ACE_TEXT (" and inline file (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sci\t\t\tsuppress generating client inline file")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Ssi\t\t\tsuppress generating server inline file")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sorb\t\t\tsuppress generating include of ORB.h")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Se\t\t\tdisable custom header file name endings")
      ACE_TEXT (" for files\n\t\t\t")
      ACE_TEXT ("that are found in TAO specific include directories,\n\t\t\t")
      ACE_TEXT ("(i.e. $TAO_ROOT, $TAO_ROOT/tao, $TAO_ROOT/orbsvcs,\n\t\t\t")
      ACE_TEXT ("$TAO_ROOT/CIAO, $TAO_ROOT/CIAO/ciao, $TAO_ROOT/CIAO/ccm)\n\t\t\t")
      ACE_TEXT (" (enabled by default)\n")
    ));
}

AST_Generator *
BE_GlobalData::generator_init (void)
{
  tao_cg = TAO_CODEGEN::instance ();
  tao_cg->config_visitor_factory ();

  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  be_generator,
                  0);

  return gen;
}
