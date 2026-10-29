
//=============================================================================
/**
 *  @file    be_global.cpp
 *
 *  Stores global data specific to the compiler back end.
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

#include <string_view>

#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_ctype.h"

TAO_IDL_BE_Export BE_GlobalData *be_global = nullptr;

BE_GlobalData::BE_GlobalData ()
  : core_versioning_begin_ (core_versioned_ns_begin),
    core_versioning_end_ (core_versioned_ns_end),
    anyops_versioning_begin_ (core_versioning_begin_ + "namespace CORBA {\n"),
    anyops_versioning_end_ ("\n}" + core_versioning_end_)
{
}

BE_GlobalData::~BE_GlobalData () = default;

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
BE_GlobalData::changing_standard_include_files ()
{
  return this->changing_standard_include_files_;
}

/************ Helper functions **************/
static std::string
be_change_idl_file_extension (UTL_String* idl_file,
                              std::optional<std::string_view> new_extension,
                              bool base_name_only = false,
                              bool for_anyop = false,
                              bool for_skel = false,
                              bool for_exec = false)
{
  if (idl_file == nullptr || !new_extension)
    {
      return {};
    }

  std::optional<std::string_view> const input_view = idl_file->get_string_view ();
  if (!input_view)
    {
      return {};
    }

  std::string_view const input = *input_view;

  // Get the base part of the filename, we try several extensions
  // before giving up.
  std::string_view::size_type base = std::string_view::npos;

  static constexpr std::string_view extensions[] = {
    ".idl",
    ".pidl",
    ".IDL",
    ".PIDL"
  };

  constexpr size_t nextensions = sizeof (extensions) / sizeof (extensions[0]);

  for (size_t k = 0; k < nextensions; ++k)
    {
      base = input.find (extensions[k]);

      if (base != std::string_view::npos)
        {
          break;
        }
    }

  if (base == std::string_view::npos)
    {
      return {};
    }

  // Anyop * skel file output defaults to general output dir if not set.
  std::string_view const output_path =
    be_util::get_output_path (for_anyop, for_skel, for_exec);

  std::string fname;
  if (!base_name_only && !output_path.empty ())
    {
      fname = output_path;
      fname.push_back ('/');
    }

  for (std::string_view::size_type i = 0; i < base; ++i)
    {
      if (input[i] == '\\')
        {
          fname.push_back ('/');
          if (i + 1 < base && input[i + 1] == '\\')
            {
              ++i;
            }
        }
      else
        {
          fname.push_back (input[i]);
        }
    }

  fname.append (new_extension->data (), new_extension->size ());
  return fname;
}

static std::optional<std::string_view>
optional_string_view (std::optional<std::string> const& value)
{
  return value ? std::optional<std::string_view> (*value) : std::nullopt;
}

std::string
BE_GlobalData::be_get_client_hdr (UTL_String *idl_file_name,
                                  bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  std::string_view const fn = idl_file_name->get_string_view ().value ();
  bool orb_file = fn.size () >= 5
    && (fn.compare (fn.size () - 5, 5, ".pidl") == 0
        || fn.compare (fn.size () - 5, 5, ".PIDL") == 0);

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  std::optional<std::string_view> const ending =
    orb_file ? std::optional<std::string_view> ("C.h") : be_global->client_hdr_ending ();

  return be_change_idl_file_extension (idl_file_name,
                                       ending,
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_client_stub (UTL_String *idl_file_name)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_stub_ending ());
}

std::string
BE_GlobalData::be_get_client_inline (UTL_String *idl_file_name,
                                     bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->client_inline_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_server_hdr (UTL_String *idl_file_name,
                                  bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  std::string_view const fn = idl_file_name->get_string_view ().value ();
  bool orb_file = fn.size () >= 5
    && (fn.compare (fn.size () - 5, 5, ".pidl") == 0
        || fn.compare (fn.size () - 5, 5, ".PIDL") == 0);

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  std::optional<std::string_view> const ending =
    orb_file ? std::optional<std::string_view> ("S.h") : be_global->server_hdr_ending ();

  return be_change_idl_file_extension (idl_file_name,
                                       ending,
                                       base_name_only,
                                       false,
                                       true);
}

std::string
BE_GlobalData::be_get_svnt_template_hdr (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  // User-defined file extensions don't apply to .pidl files.
  std::string_view const fn = idl_file_name->get_string_view ().value ();
  bool orb_file = fn.size () >= 5
    && (fn.compare (fn.size () - 5, 5, ".pidl") == 0
        || fn.compare (fn.size () - 5, 5, ".PIDL") == 0);

  if (!orb_file && !be_global->gen_custom_ending ()
      && FE_Utils::validate_orb_include (idl_file_name))
    {
      orb_file = true;
    }

  std::optional<std::string_view> const ending =
    orb_file ? std::optional<std::string_view> ("S.h") :
    be_global->ciao_svnt_header_template_ending ();

  return be_change_idl_file_extension (idl_file_name,
                                       ending,
                                       base_name_only,
                                       false,
                                       true);
}

std::string
BE_GlobalData::be_get_implementation_hdr (UTL_String *idl_file_name,
                                          bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_hdr_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_implementation_skel (UTL_String *idl_file_name,
                                           bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->implementation_skel_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_server_template_hdr (UTL_String *idl_file_name,
                                           bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_hdr_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

std::string
BE_GlobalData::be_get_server_skeleton (UTL_String *idl_file_name)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_skeleton_ending (),
                                       false,
                                       false,
                                       true);
}

std::string
BE_GlobalData::be_get_server_template_skeleton (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->server_template_skeleton_ending (),
                                       base_name_only,
                                       false,
                                       true);
}

std::string
BE_GlobalData::be_get_anyop_header (UTL_String *idl_file_name,
                                    bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->anyop_header_ending (),
                                       base_name_only,
                                       true);
}

std::string
BE_GlobalData::be_get_anyop_source (UTL_String *idl_file_name,
                                    bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->anyop_source_ending (),
                                       base_name_only,
                                       true);
}

std::string
BE_GlobalData::be_get_ciao_svnt_header (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_header_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_svnt_template_header (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_header_template_ending (),
                                       base_name_only);
}


std::string
BE_GlobalData::be_get_ciao_svnt_source (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_source_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_svnt_template_source (UTL_String *idl_file_name,
                                                bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_svnt_source_template_ending (),
                                       base_name_only);
}

std::string
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

std::string
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

std::string
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

std::string
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

std::string
BE_GlobalData::be_get_ciao_conn_header (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_conn_header_ending (),
                                       base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_conn_source (UTL_String *idl_file_name,
                                        bool base_name_only)
{
  return be_change_idl_file_extension (idl_file_name,
                                       be_global->ciao_conn_source_ending (),
                                       base_name_only);
}

std::string
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

std::string
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

std::string
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

std::string
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

std::string
BE_GlobalData::be_get_client_hdr_fname (
  bool base_name_only)
{
  return
    be_get_client_hdr (idl_global->stripped_filename (),
                       base_name_only);
}

std::string
BE_GlobalData::be_get_client_stub_fname ()
{
  return
    be_get_client_stub (idl_global->stripped_filename ());
}

std::string
BE_GlobalData::be_get_client_inline_fname (
  bool base_name_only)
{
  return
    be_get_client_inline (idl_global->stripped_filename (),
                          base_name_only);
}

std::string
BE_GlobalData::be_get_server_hdr_fname (
  bool base_name_only)
{
  return
    be_get_server_hdr (idl_global->stripped_filename (),
                       base_name_only);
}

std::string
BE_GlobalData::be_get_implementation_hdr_fname (
  bool base_name_only)
{
  return
    be_get_implementation_hdr (idl_global->stripped_filename (),
                               base_name_only);
}

std::string
BE_GlobalData::be_get_implementation_skel_fname (
  bool base_name_only)
{
  return
    be_get_implementation_skel (idl_global->stripped_filename (),
                                base_name_only);
}

std::string
BE_GlobalData::be_get_server_template_hdr_fname (
  bool base_name_only)
{
  return
    be_get_server_template_hdr (idl_global->stripped_filename (),
                                base_name_only);
}

std::string
BE_GlobalData::be_get_server_skeleton_fname ()
{
  return
    be_get_server_skeleton (idl_global->stripped_filename ());
}

std::string
BE_GlobalData::be_get_implementation_skeleton_fname ()
{
  return
    be_get_implementation_skel (idl_global->stripped_filename ());
}

std::string
BE_GlobalData::be_get_server_template_skeleton_fname (
  bool base_name_only)
{
  return
    be_get_server_template_skeleton (idl_global->stripped_filename (),
                                     base_name_only);
}

std::string
BE_GlobalData::be_get_anyop_source_fname (
  bool base_name_only)
{
  return
    be_get_anyop_source (idl_global->stripped_filename (),
                         base_name_only);
}

std::string
BE_GlobalData::be_get_anyop_header_fname (
  bool base_name_only)
{
  return
    be_get_anyop_header (idl_global->stripped_filename (),
                         base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_svnt_hdr_fname (bool base_name_only)
{
  return
    be_get_ciao_svnt_header (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_tmpl_svnt_hdr_fname (bool base_name_only)
{
  return
    be_get_ciao_svnt_template_header (idl_global->stripped_filename (),
                                      base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_svnt_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_svnt_source (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_tmpl_svnt_src_fname (
    bool base_name_only)
{
  return
    be_get_ciao_svnt_template_source (idl_global->stripped_filename (),
                                      base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_exec_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_header (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_exec_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_source (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_exec_stub_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_stub_header (
      idl_global->stripped_filename (),
      base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_exec_idl_fname (
  bool base_name_only)
{
  return
    be_get_ciao_exec_idl (
      idl_global->stripped_filename (),
      base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_conn_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_conn_header (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_conn_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_conn_source (idl_global->stripped_filename (),
                             base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_ami_conn_idl_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_idl (idl_global->stripped_filename (),
                              base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_ami_conn_impl_hdr_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_impl_hdr (idl_global->stripped_filename (),
                                   base_name_only);
}

std::string
BE_GlobalData::be_get_ciao_ami_conn_impl_src_fname (
  bool base_name_only)
{
  return
    be_get_ciao_ami_conn_impl_src (idl_global->stripped_filename (),
                                   base_name_only);
}

std::string_view
BE_GlobalData::skel_export_macro () const
{
  return this->skel_export_macro_;
}

void
BE_GlobalData::skel_export_macro (std::string_view s)
{
  this->skel_export_macro_ = s;
}

std::string_view
BE_GlobalData::skel_export_include () const
{
  return this->skel_export_include_;
}

void
BE_GlobalData::skel_export_include (std::string_view s)
{
  this->skel_export_include_ = s;
}

std::string_view
BE_GlobalData::skel_export_file () const
{
  return this->skel_export_file_;
}

void
BE_GlobalData::skel_export_file (std::string_view s)
{
  this->skel_export_file_ = s;
}

std::string_view
BE_GlobalData::stub_export_macro () const
{
  return this->stub_export_macro_;
}

void
BE_GlobalData::stub_export_macro (std::string_view s)
{
  this->stub_export_macro_ = s;
}

std::string_view
BE_GlobalData::stub_export_include () const
{
  return this->stub_export_include_;
}

void
BE_GlobalData::stub_export_include (std::string_view s)
{
  this->stub_export_include_ = s;
}

std::string_view
BE_GlobalData::stub_export_file () const
{
  return this->stub_export_file_;
}

void
BE_GlobalData::stub_export_file (std::string_view s)
{
  this->stub_export_file_ = s;
}

std::string_view
BE_GlobalData::anyop_export_macro () const
{
  return this->anyop_export_macro_;
}

void
BE_GlobalData::anyop_export_macro (std::string_view s)
{
  this->anyop_export_macro_ = s;
}

std::string_view
BE_GlobalData::anyop_export_include () const
{
  return this->anyop_export_include_;
}

void
BE_GlobalData::anyop_export_include (std::string_view s)
{
  this->anyop_export_include_ = s;
}

std::string_view
BE_GlobalData::exec_export_macro () const
{
  return this->exec_export_macro_;
}

void
BE_GlobalData::exec_export_macro (std::string_view s)
{
  this->exec_export_macro_ = s;
}

std::string_view
BE_GlobalData::exec_export_include () const
{
  return this->exec_export_include_;
}

void
BE_GlobalData::exec_export_include (std::string_view s)
{
  this->exec_export_include_ = s;
}

std::string_view
BE_GlobalData::svnt_export_macro () const
{
  return this->svnt_export_macro_;
}

void
BE_GlobalData::svnt_export_macro (std::string_view s)
{
  this->svnt_export_macro_ = s;
}

std::string_view
BE_GlobalData::svnt_export_include () const
{
  return this->svnt_export_include_;
}

void
BE_GlobalData::svnt_export_include (std::string_view s)
{
  this->svnt_export_include_ = s;
}

std::string_view
BE_GlobalData::conn_export_macro () const
{
  return this->conn_export_macro_;
}

void
BE_GlobalData::conn_export_macro (std::string_view s)
{
  this->conn_export_macro_ = s;
}

std::string_view
BE_GlobalData::conn_export_include () const
{
  return this->conn_export_include_;
}

void
BE_GlobalData::conn_export_include (std::string_view s)
{
  this->conn_export_include_ = s;
}

std::string_view
BE_GlobalData::pch_include () const
{
  return this->pch_include_;
}

void
BE_GlobalData::pch_include (std::string_view s)
{
  this->pch_include_ = s;
}

std::string_view
BE_GlobalData::pre_include () const
{
  return this->pre_include_;
}

void
BE_GlobalData::pre_include (std::string_view s)
{
  this->pre_include_ = s;
}

std::string_view
BE_GlobalData::post_include () const
{
  return this->post_include_;
}

void
BE_GlobalData::post_include (std::string_view s)
{
  this->post_include_ = s;
}

std::string_view
BE_GlobalData::include_guard () const
{
  return this->include_guard_;
}

void
BE_GlobalData::include_guard (std::string_view s)
{
  this->include_guard_ = s;
}

std::string_view
BE_GlobalData::safe_include () const
{
  return this->safe_include_;
}

void
BE_GlobalData::safe_include (std::string_view s)
{
  this->safe_include_ = s;
}

std::string_view
BE_GlobalData::unique_include () const
{
  return this->unique_include_;
}

void
BE_GlobalData::unique_include (std::string_view s)
{
  this->unique_include_ = s;
}

std::string_view
BE_GlobalData::stripped_filename () const
{
  return this->stripped_filename_;
}

void
BE_GlobalData::stripped_filename (std::string_view s)
{
  this->stripped_filename_ = s;
}

std::string_view
BE_GlobalData::versioning_begin () const
{
  return this->versioning_begin_;
}

void
BE_GlobalData::versioning_include (std::string_view s)
{
  this->versioning_include_ = s;
}

std::string_view
BE_GlobalData::versioning_include () const
{
  return this->versioning_include_;
}

std::string_view
BE_GlobalData::core_versioning_begin () const
{
  return this->core_versioning_begin_;
}

void
BE_GlobalData::versioning_end (std::string_view s)
{
  this->versioning_end_ = "\n\n";
  this->versioning_end_.append (s);
  this->versioning_end_.append ("\n\n");

  this->core_versioning_begin_ = this->versioning_end_; // Yes, "end".
  this->core_versioning_begin_.append (core_versioned_ns_begin);

  this->anyops_versioning_begin_ =
    this->core_versioning_begin_ + "namespace CORBA {\n";
}

void
BE_GlobalData::versioning_begin (std::string_view s)
{
  this->versioning_begin_ = "\n\n";
  this->versioning_begin_.append (s);
  this->versioning_begin_.append ("\n\n");

  this->core_versioning_end_ = core_versioned_ns_end; // Yes, "begin".
  this->core_versioning_end_.append (this->versioning_begin_);

  this->anyops_versioning_end_ =
    "\n}" + this->core_versioning_end_;
}

std::string_view
BE_GlobalData::versioning_end () const
{
  return this->versioning_end_;
}

std::string_view
BE_GlobalData::core_versioning_end () const
{
  return this->core_versioning_end_;
}

std::string_view
BE_GlobalData::anyops_versioning_begin () const
{
  return this->anyops_versioning_begin_;
}

std::string_view
BE_GlobalData::anyops_versioning_end () const
{
  return this->anyops_versioning_end_;
}

// Set the client_hdr_ending.
void
BE_GlobalData::client_hdr_ending (std::string_view s)
{
  this->client_hdr_ending_ = s;
}

// Get the client_hdr_ending.
std::optional<std::string_view>
BE_GlobalData::client_hdr_ending () const
{
  return optional_string_view (this->client_hdr_ending_);
}

std::string_view
BE_GlobalData::client_hdr_ending_or_empty () const
{
  return this->client_hdr_ending_ ? std::string_view (*this->client_hdr_ending_)
                                  : std::string_view {};
}

void
BE_GlobalData::client_inline_ending  (std::string_view s)
{
  this->client_inline_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::client_inline_ending () const
{
  return optional_string_view (this->client_inline_ending_);
}

// Set the client_stub_ending.
void
BE_GlobalData::client_stub_ending (std::string_view s)
{
  this->client_stub_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::client_stub_ending () const
{
  return optional_string_view (this->client_stub_ending_);
}

void
BE_GlobalData::server_hdr_ending (std::string_view s)
{
  this->server_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::server_hdr_ending () const
{
  return optional_string_view (this->server_hdr_ending_);
}

void
BE_GlobalData::implementation_hdr_ending (std::string_view s)
{
  this->implementation_hdr_ending_ = s;
}

void
BE_GlobalData::implementation_skel_ending (std::string_view s)
{
  this->implementation_skel_ending_ = s;
}

void
BE_GlobalData::impl_class_prefix (std::string_view s)
{
  this->impl_class_prefix_ = s;
}

void
BE_GlobalData::impl_class_suffix (std::string_view s)
{
  this->impl_class_suffix_ = s;
}

std::string_view
BE_GlobalData::impl_class_prefix () const
{
  return this->impl_class_prefix_;
}

std::optional<std::string_view>
BE_GlobalData::implementation_hdr_ending () const
{
  return optional_string_view (this->implementation_hdr_ending_);
}

std::string_view
BE_GlobalData::impl_class_suffix () const
{
  return this->impl_class_suffix_;
}

std::optional<std::string_view>
BE_GlobalData::implementation_skel_ending () const
{
  return optional_string_view (this->implementation_skel_ending_);
}

void
BE_GlobalData::server_template_hdr_ending (std::string_view s)
{
  this->server_template_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::server_template_hdr_ending () const
{
  return optional_string_view (this->server_template_hdr_ending_);
}

void
BE_GlobalData::server_skeleton_ending (std::string_view s)
{
  this->server_skeleton_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::server_skeleton_ending () const
{
  return optional_string_view (this->server_skeleton_ending_);
}

void
BE_GlobalData::server_template_skeleton_ending (std::string_view s)
{
  this->server_template_skeleton_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::server_template_skeleton_ending () const
{
  return optional_string_view (this->server_template_skeleton_ending_);
}

void
BE_GlobalData::anyop_header_ending (std::string_view s)
{
  this->anyop_hdr_ending_ = s;
}

void
BE_GlobalData::use_clonable_in_args (bool clonable)
{
  this->use_clonable_in_args_ = clonable;
}

bool
BE_GlobalData::use_clonable_in_args () const
{
  return this->use_clonable_in_args_;
}

bool
BE_GlobalData::gen_template_export () const
{
  return this->gen_template_export_;
}

void
BE_GlobalData::gen_template_export (bool val)
{
  this->gen_template_export_ = val;
}

bool
BE_GlobalData::gen_ostream_operators () const
{
  return this->gen_ostream_operators_;
}

void
BE_GlobalData::gen_ostream_operators (bool val)
{
  this->gen_ostream_operators_ = val;
}


bool
BE_GlobalData::gen_static_desc_operations () const
{
  return this->gen_static_desc_operations_;
}

void
BE_GlobalData::gen_static_desc_operations (bool val)
{
  this->gen_static_desc_operations_ = val;
}

std::optional<std::string_view>
BE_GlobalData::anyop_header_ending () const
{
  return optional_string_view (this->anyop_hdr_ending_);
}

void
BE_GlobalData::anyop_source_ending (std::string_view s)
{
  this->anyop_src_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::anyop_source_ending () const
{
  return optional_string_view (this->anyop_src_ending_);
}

void
BE_GlobalData::ciao_svnt_header_ending (std::string_view s)
{
  this->ciao_svnt_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_svnt_header_ending () const
{
  return optional_string_view (this->ciao_svnt_hdr_ending_);
}

void
BE_GlobalData::ciao_svnt_source_ending (std::string_view s)
{
  this->ciao_svnt_src_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_svnt_source_ending () const
{
  return optional_string_view (this->ciao_svnt_src_ending_);
}

void
BE_GlobalData::ciao_svnt_header_template_ending (std::string_view s)
{
  this->ciao_svnt_hdr_template_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_svnt_header_template_ending () const
{
  return optional_string_view (this->ciao_svnt_hdr_template_ending_);
}

void
BE_GlobalData::ciao_svnt_source_template_ending (std::string_view s)
{
  this->ciao_svnt_src_template_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_svnt_source_template_ending () const
{
  return optional_string_view (this->ciao_svnt_src_template_ending_);
}

void
BE_GlobalData::ciao_exec_header_ending (std::string_view s)
{
  this->ciao_exec_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_exec_header_ending () const
{
  return optional_string_view (this->ciao_exec_hdr_ending_);
}

void
BE_GlobalData::ciao_exec_source_ending (std::string_view s)
{
  this->ciao_exec_src_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_exec_source_ending () const
{
  return optional_string_view (this->ciao_exec_src_ending_);
}

void
BE_GlobalData::ciao_exec_stub_header_ending (std::string_view s)
{
  this->ciao_exec_stub_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_exec_stub_header_ending () const
{
  return optional_string_view (this->ciao_exec_stub_hdr_ending_);
}

void
BE_GlobalData::ciao_exec_idl_ending (std::string_view s)
{
  this->ciao_exec_idl_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_exec_idl_ending () const
{
  return optional_string_view (this->ciao_exec_idl_ending_);
}

void
BE_GlobalData::ciao_conn_header_ending (std::string_view s)
{
  this->ciao_conn_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_conn_header_ending () const
{
  return optional_string_view (this->ciao_conn_hdr_ending_);
}

void
BE_GlobalData::ciao_conn_source_ending (std::string_view s)
{
  this->ciao_conn_src_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_conn_source_ending () const
{
  return optional_string_view (this->ciao_conn_src_ending_);
}

void
BE_GlobalData::dds_typesupport_hdr_ending (std::string_view s)
{
  this->dds_typesupport_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::dds_typesupport_hdr_ending () const
{
  return optional_string_view (this->dds_typesupport_hdr_ending_);
}

void
BE_GlobalData::ciao_ami_conn_idl_ending (std::string_view s)
{
  this->ciao_ami_conn_idl_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_ami_conn_idl_ending () const
{
  return optional_string_view (this->ciao_ami_conn_idl_ending_);
}

void
BE_GlobalData::ciao_ami_conn_impl_hdr_ending (std::string_view s)
{
  this->ciao_ami_conn_impl_hdr_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_ami_conn_impl_hdr_ending () const
{
  return optional_string_view (this->ciao_ami_conn_impl_hdr_ending_);
}

void
BE_GlobalData::ciao_ami_conn_impl_src_ending (std::string_view s)
{
  this->ciao_ami_conn_impl_src_ending_ = s;
}

std::optional<std::string_view>
BE_GlobalData::ciao_ami_conn_impl_src_ending () const
{
  return optional_string_view (this->ciao_ami_conn_impl_src_ending_);
}

void
BE_GlobalData::ciao_container_type (std::string_view s)
{
  this->ciao_container_type_ = s;
}

std::string_view
BE_GlobalData::ciao_container_type () const
{
  return this->ciao_container_type_;
}

void
BE_GlobalData::output_dir (std::string_view s)
{
  this->output_dir_ = s;
}

std::string_view
BE_GlobalData::output_dir () const
{
  return this->output_dir_ ? std::string_view (*this->output_dir_) : std::string_view ();
}
bool
BE_GlobalData::overwrite_not_exec () const
{
  return this->overwrite_not_exec_;
}

void
BE_GlobalData::overwrite_not_exec (bool val)
{
  this->overwrite_not_exec_ = val;
}

void
BE_GlobalData::skel_output_dir (std::string_view s)
{
  this->skel_output_dir_ = s;
}

std::string_view
BE_GlobalData::skel_output_dir () const
{
  return this->skel_output_dir_ ? std::string_view (*this->skel_output_dir_) : std::string_view ();
}

void
BE_GlobalData::stub_include_dir (std::string_view s)
{
  this->stub_include_dir_ = s;
}

std::string_view
BE_GlobalData::stub_include_dir () const
{
  return this->stub_include_dir_ ? std::string_view (*this->stub_include_dir_) : std::string_view ();
}

void
BE_GlobalData::anyop_output_dir (std::string_view s)
{
  this->anyop_output_dir_ = s;
}

std::string_view
BE_GlobalData::anyop_output_dir () const
{
  return this->anyop_output_dir_ ? std::string_view (*this->anyop_output_dir_) : std::string_view ();
}

void
BE_GlobalData::exec_output_dir (std::string_view s)
{
  this->exec_output_dir_ = s;
}

std::string_view
BE_GlobalData::exec_output_dir () const
{
  return this->exec_output_dir_ ? std::string_view (*this->exec_output_dir_) : std::string_view ();
}
void
BE_GlobalData::any_support (bool val)
{
  this->any_support_ = val;
}

bool
BE_GlobalData::any_support () const
{
  return this->any_support_;
}

void
BE_GlobalData::cdr_support (bool val)
{
  this->cdr_support_ = val;
}

bool
BE_GlobalData::cdr_support () const
{
  return this->cdr_support_;
}

void
BE_GlobalData::tc_support (bool val)
{
  this->tc_support_ = val;
}

bool
BE_GlobalData::tc_support () const
{
  return this->tc_support_;
}

void
BE_GlobalData::obv_opt_accessor (bool val)
{
  this->obv_opt_accessor_ = val;
}

bool
BE_GlobalData::obv_opt_accessor () const
{
  return this->obv_opt_accessor_;
}

void
BE_GlobalData::gen_impl_files (bool val)
{
  this->gen_impl_files_ = val;
}

bool
BE_GlobalData::gen_impl_files () const
{
  return this->gen_impl_files_;
}

void
BE_GlobalData::gen_impl_debug_info (bool val)
{
  this->gen_impl_debug_info_ = val;
}

bool
BE_GlobalData::gen_impl_debug_info () const
{
  return this->gen_impl_debug_info_;
}

void
BE_GlobalData::gen_copy_ctor (bool val)
{
  this->gen_copy_ctor_ = val;
}

bool
BE_GlobalData::gen_copy_ctor () const
{
  return this->gen_copy_ctor_;
}

void
BE_GlobalData::gen_assign_op (bool val)
{
  this->gen_assign_op_ = val;
}

bool
BE_GlobalData::gen_assign_op () const
{
  return this->gen_assign_op_;
}

void
BE_GlobalData::gen_thru_poa_collocation (bool val)
{
  this->gen_thru_poa_collocation_ = val;
}

bool
BE_GlobalData::gen_thru_poa_collocation () const
{
  return this->gen_thru_poa_collocation_;
}

void
BE_GlobalData::gen_direct_collocation (bool val)
{
  this->gen_direct_collocation_ = val;
}

bool
BE_GlobalData::gen_direct_collocation () const
{
  return this->gen_direct_collocation_;
}

void
BE_GlobalData::gen_corba_e (bool val)
{
  this->gen_corba_e_ = val;
}

bool
BE_GlobalData::gen_corba_e () const
{
  return this->gen_corba_e_;
}

void
BE_GlobalData::gen_minimum_corba (bool val)
{
  this->gen_minimum_corba_ = val;
}

bool
BE_GlobalData::gen_minimum_corba () const
{
  return this->gen_minimum_corba_;
}

void
BE_GlobalData::gen_noeventccm (bool val)
{
  this->gen_noeventccm_ = val;
}

bool
BE_GlobalData::gen_noeventccm () const
{
  return this->gen_noeventccm_;
}

void
BE_GlobalData::gen_lwccm (bool val)
{
  this->gen_lwccm_ = val;
}

bool
BE_GlobalData::gen_lwccm () const
{
  return this->gen_lwccm_;
}


void
BE_GlobalData::opt_tc (bool val)
{
  this->opt_tc_ = val;
}

bool
BE_GlobalData::opt_tc () const
{
  return this->opt_tc_;
}

void
BE_GlobalData::ami4ccm_call_back (bool val)
{
  this->ami4ccm_call_back_ = val;
}

bool
BE_GlobalData::ami4ccm_call_back () const
{
  return this->ami4ccm_call_back_;
}

void
BE_GlobalData::ami_call_back (bool val)
{
  this->ami_call_back_ = val;
}

bool
BE_GlobalData::ami_call_back () const
{
  return this->ami_call_back_;
}

void
BE_GlobalData::gen_amh_classes (bool val)
{
  this->gen_amh_classes_ = val;
}

bool
BE_GlobalData::gen_amh_classes () const
{
  return this->gen_amh_classes_;
}

void
BE_GlobalData::gen_tie_classes (bool val)
{
  this->gen_tie_classes_ = val;
}

bool
BE_GlobalData::gen_tie_classes () const
{
  return this->gen_tie_classes_;
}

void
BE_GlobalData::gen_smart_proxies (bool val)
{
  this->gen_smart_proxies_ = val;
}

bool
BE_GlobalData::gen_smart_proxies () const
{
  return this->gen_smart_proxies_;
}

void
BE_GlobalData::gen_inline_constants (bool val)
{
  this->gen_inline_constants_ = val;
}

bool
BE_GlobalData::gen_inline_constants () const
{
  return this->gen_inline_constants_;
}

void
BE_GlobalData::gen_orb_h_include (bool val)
{
  this->gen_orb_h_include_ = val;
}

bool
BE_GlobalData::gen_orb_h_include () const
{
  return this->gen_orb_h_include_;
}

void
BE_GlobalData::gen_empty_anyop_header (bool val)
{
  this->gen_empty_anyop_header_ = val;
}

bool
BE_GlobalData::gen_empty_anyop_header () const
{
  return this->gen_empty_anyop_header_;
}

void
BE_GlobalData::lookup_strategy (LOOKUP_STRATEGY s)
{
  this->lookup_strategy_ = s;
}

BE_GlobalData::LOOKUP_STRATEGY
BE_GlobalData::lookup_strategy () const
{
  return this->lookup_strategy_;
}

void
BE_GlobalData::dds_impl (std::string_view val)
{
  if (val == "ndds")
    {
      this->dds_impl_ = NDDS;
    }
  else if (val == "opensplice")
    {
      this->dds_impl_ = OPENSPLICE;
    }
  else if (val == "opendds")
    {
      this->dds_impl_ = OPENDDS;
    }
  else if (val == "coredx")
    {
      this->dds_impl_ = COREDX;
    }
  else
    {
      std::string const value (val);
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%C: invalid or unknown ")
                  ACE_TEXT ("argument <%C> to -Wb,dds_impl\n"),
                  idl_global->prog_name ().data (),
                  value.c_str ()));
    }
}

BE_GlobalData::DDS_IMPL
BE_GlobalData::dds_impl () const
{
  return this->dds_impl_;
}

void
BE_GlobalData::destroy ()
{
  this->skel_export_macro_.clear ();
  this->skel_export_include_.clear ();
  this->skel_export_file_.clear ();
  this->stub_export_macro_.clear ();
  this->stub_export_include_.clear ();
  this->stub_export_file_.clear ();
  this->anyop_export_macro_.clear ();
  this->anyop_export_include_.clear ();
  this->exec_export_macro_.clear ();
  this->exec_export_include_.clear ();
  this->svnt_export_macro_.clear ();
  this->svnt_export_include_.clear ();
  this->conn_export_macro_.clear ();
  this->conn_export_include_.clear ();
  this->pch_include_.clear ();
  this->pre_include_.clear ();
  this->post_include_.clear ();
  this->include_guard_.clear ();
  this->safe_include_.clear ();
  this->unique_include_.clear ();
  this->stripped_filename_.clear ();

  this->client_hdr_ending_.reset ();
  this->client_stub_ending_.reset ();
  this->client_inline_ending_.reset ();
  this->server_hdr_ending_.reset ();
  this->implementation_hdr_ending_.reset ();
  this->implementation_skel_ending_.reset ();
  this->impl_class_prefix_.clear ();
  this->impl_class_suffix_.clear ();
  this->server_template_hdr_ending_.reset ();
  this->server_skeleton_ending_.reset ();
  this->server_template_skeleton_ending_.reset ();
  this->anyop_hdr_ending_.reset ();
  this->anyop_src_ending_.reset ();
  this->ciao_svnt_hdr_ending_.reset ();
  this->ciao_svnt_src_ending_.reset ();
  this->ciao_svnt_hdr_template_ending_.reset ();
  this->ciao_svnt_src_template_ending_.reset ();
  this->ciao_exec_hdr_ending_.reset ();
  this->ciao_exec_src_ending_.reset ();
  this->ciao_exec_stub_hdr_ending_.reset ();
  this->ciao_exec_idl_ending_.reset ();
  this->ciao_conn_hdr_ending_.reset ();
  this->ciao_conn_src_ending_.reset ();
  this->dds_typesupport_hdr_ending_.reset ();
  this->ciao_ami_conn_idl_ending_.reset ();
  this->ciao_ami_conn_impl_hdr_ending_.reset ();
  this->ciao_ami_conn_impl_src_ending_.reset ();
  this->ciao_container_type_.clear ();

  this->output_dir_.reset ();
  this->stub_include_dir_.reset ();
  this->skel_output_dir_.reset ();
  this->anyop_output_dir_.reset ();
  this->exec_output_dir_.reset ();

  if (nullptr != this->messaging_)
    {
      this->messaging_->destroy ();
      delete this->messaging_;
      this->messaging_ = nullptr;
    }

  if (nullptr != this->messaging_exceptionholder_)
    {
      this->messaging_exceptionholder_->destroy ();
      delete this->messaging_exceptionholder_;
      this->messaging_exceptionholder_ = nullptr;
    }

  if (nullptr != this->messaging_replyhandler_)
    {
      this->messaging_replyhandler_->destroy ();
      delete this->messaging_replyhandler_;
      this->messaging_replyhandler_ = nullptr;
    }

  if (nullptr != tao_cg)
    {
      tao_cg->destroy ();
    }
}

AST_PredefinedType *
BE_GlobalData:: void_type ()
{
  if (nullptr == this->void_type_)
    {
      AST_Decl *d =
        idl_global->root ()->lookup_primitive_type (
          AST_Expression::EV_void);

      this->void_type_ = dynamic_cast<AST_PredefinedType*> (d);
    }

  return this->void_type_;
}

be_interface *
BE_GlobalData::ccmobject ()
{
  if (nullptr == this->ccmobject_)
    {
      Identifier *local_id = nullptr;
      ACE_NEW_RETURN (local_id,
                      Identifier ("CCMObject"),
                      nullptr);
      UTL_ScopedName *local_name = nullptr;
      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (local_id, nullptr),
                      nullptr);

      Identifier *module_id = nullptr;
      ACE_NEW_RETURN (module_id,
                      Identifier ("Components"),
                      nullptr);
      UTL_ScopedName sn (module_id,
                         local_name);

      AST_Decl *d =
        idl_global->scopes ().top_non_null ()->lookup_by_name (&sn,
                                                               true);

      sn.destroy ();

      if (nullptr == d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_global::ccmobject - "
                             "lookup of CCMObject failed\n"),
                            0);
        }

      this->ccmobject_ = dynamic_cast<be_interface*> (d);
    }

  return this->ccmobject_;
}

be_module *
BE_GlobalData::messaging ()
{
  if (nullptr == this->messaging_)
    {
      Identifier *id = nullptr;
      UTL_ScopedName *sn = nullptr;

      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      nullptr);

      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      nullptr),
                      nullptr);

      ACE_NEW_RETURN (this->messaging_,
                      be_module (sn),
                      nullptr);

      this->messaging_->set_name (sn);
    }

  return this->messaging_;
}

be_valuetype *
BE_GlobalData::messaging_exceptionholder ()
{
  if (nullptr == this->messaging_exceptionholder_)
    {
      Identifier *id = nullptr;
      be_module *msg = this->messaging ();
      idl_global->scopes ().push (msg);

      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      nullptr);

      // Create a valuetype "ExceptionHolder"
      // from which we inherit.
      UTL_ScopedName *full_name = nullptr;
      ACE_NEW_RETURN (full_name,
                      UTL_ScopedName (id,
                                      nullptr),
                      nullptr);

      ACE_NEW_RETURN (id,
                      Identifier ("ExceptionHolder"),
                      nullptr);

      UTL_ScopedName *local_name = nullptr;
      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (id,
                                      nullptr),
                      nullptr);

      full_name->nconc (local_name);

      ACE_NEW_RETURN (this->messaging_exceptionholder_,
                      be_valuetype (full_name,
                                    nullptr,
                                    0,
                                    nullptr,
                                    nullptr,
                                    0,
                                    nullptr,
                                    0,
                                    nullptr,
                                    0,
                                    0,
                                    0),
                      nullptr);

      this->messaging_exceptionholder_->set_name (full_name);

      // Notice the valuetype "ExceptionHolder" that it is defined in the
      // "Messaging" module
      this->messaging_exceptionholder_->set_defined_in (msg);
      this->messaging_exceptionholder_->set_prefix_with_typeprefix ("omg.org");

      idl_global->scopes ().pop ();

      // Notice the interface "ReplyHandler" that it is defined in the
      // "Messaging" module.
      this->messaging_exceptionholder_->set_defined_in (msg);
    }

  return this->messaging_exceptionholder_;
}

be_interface *
BE_GlobalData::messaging_replyhandler ()
{
  if (nullptr == this->messaging_replyhandler_)
    {
      be_module *msg = this->messaging ();
      idl_global->scopes ().push (msg);

      Identifier *id = nullptr;
      UTL_ScopedName *local_name = nullptr;

      // Create a virtual module named "Messaging"
      // "and an interface "ReplyHandler"
      // from which we inherit.
      ACE_NEW_RETURN (id,
                      Identifier ("Messaging"),
                      nullptr);

      UTL_ScopedName *full_name = nullptr;
      ACE_NEW_RETURN (full_name,
                      UTL_ScopedName (id,
                                      nullptr),
                      nullptr);

      ACE_NEW_RETURN (id,
                      Identifier ("ReplyHandler"),
                      nullptr);

      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (id,
                                      nullptr),
                      nullptr);

      full_name->nconc (local_name);

      ACE_NEW_RETURN (this->messaging_replyhandler_,
                      be_interface (full_name,
                                    nullptr,  // inherited interfaces
                                    0,  // number of inherited interfaces
                                    nullptr,  // ancestors
                                    0,  // number of ancestors
                                    0,  // not local
                                    0), // not abstract
                      nullptr);

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
BE_GlobalData::gen_anyop_files () const
{
  return this->gen_anyop_files_;
}

void
BE_GlobalData::gen_anyop_files (bool val)
{
  this->gen_anyop_files_ = val;
}

bool
BE_GlobalData::gen_skel_files () const
{
  return this->gen_skel_files_;
}

void
BE_GlobalData::gen_skel_files (bool val)
{
  this->gen_skel_files_ = val;
}

bool
BE_GlobalData::gen_svnt_cpp_files () const
{
  return this->gen_svnt_cpp_files_;
}

void
BE_GlobalData::gen_svnt_cpp_files (bool val)
{
  this->gen_svnt_cpp_files_ = val;
}

bool
BE_GlobalData::gen_svnt_t_files () const
{
  return this->gen_svnt_t_files_;
}

void
BE_GlobalData::gen_svnt_t_files (bool val)
{
  this->gen_svnt_t_files_ = val;
}

bool
BE_GlobalData::gen_client_inline () const
{
  return this->gen_client_inline_;
}

void
BE_GlobalData::gen_client_inline (bool val)
{
  this->gen_client_inline_ = val;
}

bool
BE_GlobalData::gen_client_stub () const
{
  return this->gen_client_stub_;
}

void
BE_GlobalData::gen_client_stub (bool val)
{
  this->gen_client_stub_ = val;
}

bool
BE_GlobalData::gen_client_header () const
{
  return this->gen_client_header_;
}

void
BE_GlobalData::gen_client_header (bool val)
{
  this->gen_client_header_ = val;
}

bool
BE_GlobalData::gen_server_skeleton () const
{
  return this->gen_server_skeleton_;
}

void
BE_GlobalData::gen_server_skeleton (bool val)
{
  this->gen_server_skeleton_ = val;
}

bool
BE_GlobalData::gen_server_header () const
{
  return this->gen_server_header_;
}

void
BE_GlobalData::gen_server_header (bool val)
{
  this->gen_server_header_ = val;
}

bool
BE_GlobalData::gen_local_iface_anyops () const
{
  return this->gen_local_iface_anyops_;
}

void
BE_GlobalData::gen_local_iface_anyops (bool val)
{
  this->gen_local_iface_anyops_ = val;
}

bool
BE_GlobalData::gen_custom_ending () const
{
  return this->gen_custom_ending_;
}

void
BE_GlobalData::gen_custom_ending (bool val)
{
  this->gen_custom_ending_ = val;
}

bool
BE_GlobalData::gen_unique_guards () const
{
  return this->gen_unique_guards_;
}

void
BE_GlobalData::gen_unique_guards (bool val)
{
  this->gen_unique_guards_ = val;
}

bool
BE_GlobalData::gen_ciao_svnt () const
{
  return this->gen_ciao_svnt_;
}

void
BE_GlobalData::gen_ciao_svnt (bool val)
{
  this->gen_ciao_svnt_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_idl () const
{
  return this->gen_ciao_exec_idl_;
}

void
BE_GlobalData::gen_ciao_exec_idl (bool val)
{
  this->gen_ciao_exec_idl_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_impl () const
{
  return this->gen_ciao_exec_impl_;
}

void
BE_GlobalData::gen_ciao_exec_impl (bool val)
{
  this->gen_ciao_exec_impl_ = val;
}

bool
BE_GlobalData::gen_ciao_exec_reactor_impl () const
{
  return this->gen_ciao_exec_reactor_impl_;
}

void
BE_GlobalData::gen_ciao_exec_reactor_impl (bool val)
{
  this->gen_ciao_exec_reactor_impl_ = val;
}

bool
BE_GlobalData::gen_ciao_conn_impl () const
{
  return this->gen_ciao_conn_impl_;
}

void
BE_GlobalData::gen_ciao_conn_impl (bool val)
{
  this->gen_ciao_conn_impl_ = val;
}

bool
BE_GlobalData::gen_dds_typesupport_idl () const
{
  return this->gen_dds_typesupport_idl_;
}

void
BE_GlobalData::gen_dds_typesupport_idl (bool val)
{
  this->gen_dds_typesupport_idl_ = val;
}

bool
BE_GlobalData::gen_ciao_valuefactory_reg () const
{
  return this->gen_ciao_valuefactory_reg_;
}

void
BE_GlobalData::gen_ciao_valuefactory_reg (bool val)
{
  this->gen_ciao_valuefactory_reg_ = val;
}

bool
BE_GlobalData::gen_stub_export_hdr_file () const
{
  return this->gen_stub_export_hdr_file_;
}

void
BE_GlobalData::gen_stub_export_hdr_file (bool val)
{
  this->gen_stub_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_skel_export_hdr_file () const
{
  return this->gen_skel_export_hdr_file_;
}

void
BE_GlobalData::gen_skel_export_hdr_file (bool val)
{
  this->gen_skel_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_svnt_export_hdr_file () const
{
  return this->gen_svnt_export_hdr_file_;
}

void
BE_GlobalData::gen_svnt_export_hdr_file (bool val)
{
  this->gen_svnt_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_exec_export_hdr_file () const
{
  return this->gen_exec_export_hdr_file_;
}

void
BE_GlobalData::gen_exec_export_hdr_file (bool val)
{
  this->gen_exec_export_hdr_file_ = val;
}

bool
BE_GlobalData::gen_conn_export_hdr_file () const
{
  return this->gen_conn_export_hdr_file_;
}

void
BE_GlobalData::gen_conn_export_hdr_file (bool val)
{
  this->gen_conn_export_hdr_file_ = val;
}

bool
BE_GlobalData::alt_mapping () const
{
  return this->alt_mapping_;
}

void
BE_GlobalData::alt_mapping (bool val)
{
  this->alt_mapping_ = val;
}

bool
BE_GlobalData::in_facet_servant () const
{
  return this->in_facet_servant_;
}

void
BE_GlobalData::in_facet_servant (bool val)
{
  this->in_facet_servant_ = val;
}

bool
BE_GlobalData::gen_arg_traits () const
{
  return this->gen_arg_traits_;
}

void
BE_GlobalData::gen_arg_traits (bool val)
{
  this->gen_arg_traits_ = val;
}

bool
BE_GlobalData::gen_anytypecode_adapter () const
{
  return this->gen_anytypecode_adapter_;
}

void
BE_GlobalData::gen_anytypecode_adapter (bool val)
{
  this->gen_anytypecode_adapter_ = val;
}

bool
BE_GlobalData::no_fixed_err () const
{
  return this->no_fixed_err_;
}

void
BE_GlobalData::no_fixed_err (bool val)
{
  this->no_fixed_err_ = val;
}

unsigned long
BE_GlobalData::tab_size () const
{
  return this->tab_size_;
}

void
BE_GlobalData::tab_size (unsigned long val)
{
  this->tab_size_ = val;
}

std::string
BE_GlobalData::spawn_options () const
{
  return std::string (idl_global->idl_flags ());
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
            idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
          }
        break;
        // Operation lookup strategy.
        // <perfect_hash>, <dynamic_hash> or <binary_search>
        // Default is perfect.
      case 'H':
        idl_global->append_idl_flag (av[i + 1]);

        if (av[i + 1] == nullptr || av[i + 1][0] == '-')
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("no selection for -H option\n")));
            idl_global->parse_args_exit (1);
          }
        else if (std::string_view (av[i + 1]) == "dynamic_hash")
          {
            be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
          }
        else if (std::string_view (av[i + 1]) == "perfect_hash")
          {
            be_global->lookup_strategy (BE_GlobalData::TAO_PERFECT_HASH);
          }
        else if (std::string_view (av[i + 1]) == "binary_search")
          {
            be_global->lookup_strategy (BE_GlobalData::TAO_BINARY_SEARCH);
          }
        else if (std::string_view (av[i + 1]) == "linear_search")
          {
            be_global->lookup_strategy (BE_GlobalData::TAO_LINEAR_SEARCH);
          }
        else
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%C: unknown operation lookup <%C>\n"),
                        av[0],
                        av[i + 1]));
            idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
              }
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                av[i]
              ));
            idl_global->parse_args_exit (1);
          }

        break;
      // Path for the perfect hash generator(gperf) program. Default
      // is $ACE_ROOT/bin/ace_gperf.
      case 'g':
        if (av[i][2] == '\0')
          {
            idl_global->append_idl_flag (av[i + 1]);
            std::string tmp (av[i + 1]);
#if defined (ACE_WIN32)
            // WIN32's CreateProcess needs the full executable name
            // when the gperf path is modified, but not for the default
            // path given above. Other platforms don't need the
            // executable name at all.
            tmp += "\\ace_gperf.exe";
#endif
            idl_global->gperf_path (tmp);
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
            idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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

                    idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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

                    idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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

                    idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
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
        else if (av[i][2] == 's' && av[i][3] == 'd')
          {
            // Generate static description operations
            be_global->gen_static_desc_operations (true);

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
                idl_global->parse_args_exit (1);
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
                        idl_global->parse_args_exit (1);
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
                    idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
              }
            break;
          }
        else if (av[i][2] == 't')
          {
            // Optimized typecode generation.
            be_global->opt_tc (true);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
              }
          }
        else if (av[i][2] == 'I')
          {
            size_t const options = std::string_view (av[i]).size () - 3;
            size_t j;
            size_t k = i;
            // Generate implementation files.
            be_global->gen_impl_files (true);

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
                    idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
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
        else if (av[i][2] == 's' && av[i][3] == 'v' && av[i][4] == 'n' && av[i][5] == 't' && av[i][6] == 'c' && '\0' == av[i][7])
          {
            be_global->gen_svnt_cpp_files (false);
          }
        else if (av[i][2] == 's' && av[i][3] == 'v' && av[i][4] == 'n' && av[i][5] == 't' && av[i][6] == 't' && '\0' == av[i][7])
          {
            be_global->gen_svnt_t_files (false);
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
                // No stub
                be_global->gen_client_stub (false);
              }
            else if (av[i][3] == 'h')
              {
                // No stub
                be_global->gen_client_header (false);
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
                idl_global->parse_args_exit (1);
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
                idl_global->parse_args_exit (1);
              }
          }
        else if (av[i][2] == 's')
          {
            if (av[i][3] == 'c')
              {
                // No skeleton inline.
                be_global->gen_server_skeleton (false);
              }
            else if (av[i][3] == 'h')
              {
                // No skeleton inline.
                be_global->gen_server_header (false);
              }
            else
              {
                ACE_ERROR ((
                    LM_ERROR,
                    ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
                    av[i]
                  ));
                idl_global->parse_args_exit (1);
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
            idl_global->parse_args_exit (1);
          }

        break;
      case 'T':
        if (av[i][2] == 'S')
          {
            unsigned long ul = ACE_OS::strtoul (av[i + 1], nullptr, 10);
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
            idl_global->parse_args_exit (1);
          }

        break;
      default:
        ACE_ERROR ((
            LM_ERROR,
            ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
            av[i]
          ));

        idl_global->parse_args_exit (1);
    }
}
