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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_global, "$Id$")

TAO_IDL_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
    : changing_standard_include_files_ (1),
      skel_export_macro_ (0),
      skel_export_include_ (0),
      stub_export_macro_ (0),
      stub_export_include_ (0),
      pch_include_ (0),
      pre_include_ (0),
      post_include_ (0),
      client_hdr_ending_ (ACE::strnew ("C.h")),
      client_stub_ending_ (ACE::strnew ("C.cpp")),
      client_inline_ending_ (ACE::strnew ("C.i")),
      server_hdr_ending_ (ACE::strnew ("S.h")),
      implementation_hdr_ending_ (ACE::strnew ("I.h")),
      implementation_skel_ending_ (ACE::strnew ("I.cpp")),
      impl_class_prefix_ (ACE::strnew ("")),
      impl_class_suffix_ (ACE::strnew ("_i")),
      server_template_hdr_ending_ (ACE::strnew ("S_T.h")),
      server_skeleton_ending_ (ACE::strnew ("S.cpp")),
      server_template_skeleton_ending_ (ACE::strnew ("S_T.cpp")),
      server_inline_ending_ (ACE::strnew ("S.i")),
      server_template_inline_ending_ (ACE::strnew ("S_T.i")),
      output_dir_ (0),
      any_support_ (I_TRUE),
      tc_support_ (I_TRUE),
#ifdef IDL_HAS_VALUETYPE
      obv_opt_accessor_ (0),
#endif /* IDL_HAS_VALUETYPE */
      gen_impl_files_ (I_FALSE),
      gen_copy_ctor_ (I_FALSE),
      gen_assign_op_ (I_FALSE),
      gen_thru_poa_collocation_ (I_TRUE), // Default is thru_poa.
      gen_direct_collocation_ (I_FALSE),
#ifdef ACE_HAS_EXCEPTIONS
      exception_support_ (I_TRUE),
#else
      exception_support_ (I_FALSE),
#endif /* ACE_HAS_EXCEPTIONS */
      use_raw_throw_ (I_FALSE),
      opt_tc_ (I_FALSE),
      ami_call_back_ (I_FALSE),
      gen_tie_classes_ (I_TRUE),
      gen_smart_proxies_ (I_FALSE),
      lookup_strategy_ (TAO_PERFECT_HASH)
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
                              int base_name_only = 0)
{
  // @@ This shouldn't happen anyway; but a better error handling
  // mechanism is needed.
  if (idl_file == 0 || new_extension == 0)
    return 0;

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
        break;
    }
  if (base == 0)
    return 0;

  if ((!base_name_only) && (be_global->output_dir () != 0))
    {
      // Path info should also be added to fname.

      // Add path and "/".
      ACE_OS::sprintf (fname, "%s/", be_global->output_dir ());

      // Append the base part to fname.
      ACE_OS::strncpy (fname + strlen (fname), string, base - string);
    }
  else
    // Base_name_only or no putput_dir specified by user. JUST put the
    // base part to fname.
    ACE_OS::strncpy (fname, string, base - string);

  // Turn '\' and '\\' into '/'.
  char* i = fname;
  for (char* j = fname; *j != 0; ++i, ++j)
    {
      if (*j == '\\')
        {
          *i = '/';
          if (*(j+1) == '\\')
            ++j;
        }
      else
        *i = *j;
    }
  *i = 0;

  // Append the newextension.
  ACE_OS::strcat (fname, new_extension);

  return fname;
}

const char *
BE_GlobalData::be_get_client_hdr (UTL_String *idl_file_name,
                                  int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_hdr_ending (),
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
                                     int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_inline_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_hdr (UTL_String *idl_file_name,
                                  int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_hdr_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_hdr (UTL_String *idl_file_name,
                                          int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_hdr_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_skel (UTL_String *idl_file_name,
                                           int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_skel_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_hdr (UTL_String *idl_file_name,
                                            int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_hdr_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_skeleton (UTL_String *idl_file_name)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_skeleton_ending ());
}

const char *
BE_GlobalData::be_get_server_template_skeleton (UTL_String *idl_file_name,
                                                int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_skeleton_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_inline (UTL_String *idl_file_name,
                                     int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_inline_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_inline (UTL_String *idl_file_name,
                                              int base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_inline_ending (),
                                       base_name_only);
}

const char *
BE_GlobalData::be_get_client_hdr_fname (int base_name_only)
{
  return be_get_client_hdr (idl_global->stripped_filename (),
                            base_name_only);
}

const char *
BE_GlobalData::be_get_client_stub_fname ()
{
  return be_get_client_stub (idl_global->stripped_filename ());
}

const char *
BE_GlobalData::be_get_client_inline_fname (int base_name_only)
{
  return be_get_client_inline (idl_global->stripped_filename (),
                               base_name_only);
}

const char *
BE_GlobalData::be_get_server_hdr_fname (int base_name_only)
{
  return be_get_server_hdr (idl_global->stripped_filename (),
                            base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_hdr_fname (int base_name_only)
{
  return be_get_implementation_hdr (idl_global->stripped_filename (),
                                    base_name_only);
}

const char *
BE_GlobalData::be_get_implementation_skel_fname (int base_name_only)
{
  return be_get_implementation_skel (idl_global->stripped_filename (),
                                     base_name_only);
}


const char *
BE_GlobalData::be_get_server_template_hdr_fname (int base_name_only)
{
  return be_get_server_template_hdr (idl_global->stripped_filename (),
                                     base_name_only);
}

const char *
BE_GlobalData::be_get_server_skeleton_fname ()
{
  return be_get_server_skeleton (idl_global->stripped_filename ());
}

/*
const char *
BE_GlobalData::be_get_implementation_hdr_fname ()
{
  return be_get_implementation_hdr (idl_global->stripped_filename ());
}
*/

const char *
BE_GlobalData::be_get_implementation_skeleton_fname ()
{
  return be_get_implementation_skel (idl_global->stripped_filename ());
}


const char *
BE_GlobalData::be_get_server_template_skeleton_fname (int base_name_only)
{
  return be_get_server_template_skeleton (idl_global->stripped_filename (),
                                          base_name_only);
}

const char *
BE_GlobalData::be_get_server_inline_fname (int base_name_only)
{
  return be_get_server_inline (idl_global->stripped_filename (),
                               base_name_only);
}

const char *
BE_GlobalData::be_get_server_template_inline_fname (int base_name_only)
{
  return be_get_server_template_inline (idl_global->stripped_filename (),
                                        base_name_only);
}

const char*
BE_GlobalData::skel_export_macro (void) const
{
  if (this->skel_export_macro_ == 0)
    return "";
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
    return "";
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
BE_GlobalData::server_template_inline_ending (const char* s)
{
  delete [] this->server_template_inline_ending_;
  this->server_template_inline_ending_ = ACE::strnew (s);
}

const char*
BE_GlobalData::server_template_inline_ending (void) const
{
  return this->server_template_inline_ending_;
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
BE_GlobalData::any_support (idl_bool val)
{
  this->any_support_ = val;
}

idl_bool
BE_GlobalData::any_support (void)
{
  return this->any_support_;
}

void
BE_GlobalData::tc_support (idl_bool val)
{
  this->tc_support_ = val;
}

idl_bool
BE_GlobalData::tc_support (void)
{
  return this->tc_support_;
}

#ifdef IDL_HAS_VALUETYPE
void
BE_GlobalData::obv_opt_accessor (idl_bool val)
{
  this->obv_opt_accessor_ = val;
}

idl_bool
BE_GlobalData::obv_opt_accessor (void)
{
  return this->obv_opt_accessor_;
}
#endif /* IDL_HAS_VALUETYPE */

void
BE_GlobalData::gen_impl_files (idl_bool val)
{
  this->gen_impl_files_ = val;
}

idl_bool
BE_GlobalData::gen_impl_files (void)
{
  return this->gen_impl_files_;
}

void
BE_GlobalData::gen_copy_ctor (idl_bool val)
{
  this->gen_copy_ctor_ = val;
}

idl_bool
BE_GlobalData::gen_copy_ctor (void)
{
  return this->gen_copy_ctor_;
}

void
BE_GlobalData::gen_assign_op (idl_bool val)
{
  this->gen_assign_op_ = val;
}

idl_bool
BE_GlobalData::gen_assign_op (void)
{
  return this->gen_assign_op_;
}

void
BE_GlobalData::gen_thru_poa_collocation (idl_bool val)
{
  this->gen_thru_poa_collocation_ = val;
}

idl_bool
BE_GlobalData::gen_thru_poa_collocation (void)
{
  return this->gen_thru_poa_collocation_;
}

void
BE_GlobalData::gen_direct_collocation (idl_bool val)
{
  this->gen_direct_collocation_ = val;
}

idl_bool
BE_GlobalData::gen_direct_collocation (void)
{
  return this->gen_direct_collocation_;
}

void
BE_GlobalData::exception_support (idl_bool val)
{
  this->exception_support_ = val;
}

idl_bool
BE_GlobalData::exception_support (void)
{
  return this->exception_support_;
}

void
BE_GlobalData::use_raw_throw (idl_bool val)
{
  this->use_raw_throw_ = val;
}

idl_bool
BE_GlobalData::use_raw_throw (void)
{
  return this->use_raw_throw_;
}

void
BE_GlobalData::opt_tc (idl_bool val)
{
  this->opt_tc_ = val;
}

idl_bool
BE_GlobalData::opt_tc (void)
{
  return this->opt_tc_;
}

void
BE_GlobalData::ami_call_back (idl_bool val)
{
  this->ami_call_back_ = val;
}

idl_bool
BE_GlobalData::ami_call_back (void)
{
  return this->ami_call_back_;
}

void
BE_GlobalData::gen_amh_classes (idl_bool val)
{
  this->gen_amh_classes_ = val;
}

idl_bool
BE_GlobalData::gen_amh_classes (void)
{
  return this->gen_amh_classes_;
}

void
BE_GlobalData::gen_tie_classes (idl_bool val)
{
  this->gen_tie_classes_ = val;
}

idl_bool
BE_GlobalData::gen_tie_classes (void)
{
  return this->gen_tie_classes_;
}

void
BE_GlobalData::gen_smart_proxies (idl_bool val)
{
  this->gen_smart_proxies_ = val;
}

idl_bool
BE_GlobalData::gen_smart_proxies (void)
{
  return this->gen_smart_proxies_;
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
}

