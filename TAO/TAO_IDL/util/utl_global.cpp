/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

#include "idl_defines.h"
#include "idl_global.h"
#include "global_extern.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "utl_string.h"
#include "fe_extern.h"
#include "fe_private.h"
#include "nr_extern.h"
#include "ast_extern.h"

#include "ast_root.h"
#include "ast_generator.h"
#include "ast_valuetype.h"
#include "ast_annotation_decl.h"

#include "ace/OS_NS_unistd.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include <string>
#include <utility>

IDL_GlobalData::IDL_GlobalData ()
{
  // Path for the perfect hash generator(gperf) program.
  // Default is $ACE_ROOT/bin/gperf unless ACE_GPERF is defined.
  // Use ACE_GPERF if $ACE_ROOT hasn't been set or won't be set
  // in the environment.
  // Form the absolute pathname.
  char const * const ace_root = ACE_OS::getenv ("ACE_ROOT");

  if (ace_root == nullptr)
    // This may not cause any problem if -g option is used to specify
    // the correct path for the  gperf program. Let us ignore this
    // error here. It will be caught when we check the existence of
    // the perfect hasher and at that time, we can switch over to some
    // other scheme.
    {
#if defined (ACE_GPERF)
      // The actual gperf program must be included in the definition of
      // ACE_GPERF, not just the directory in which it is located.
      this->gperf_path_ = ACE_GPERF;
#endif
    }
  else
    {
#if defined (ACE_GPERF)
      std::string path (ace_root);
      path.append (ACE_DIRECTORY_SEPARATOR_STR_A "bin" ACE_DIRECTORY_SEPARATOR_STR_A);
      path.append (ACE_GPERF);
      this->gperf_path_ = std::move (path);
#else /* Not ACE_GPERF */
      // Set it to the default value.
      std::string path (ace_root);
      path.append (ACE_DIRECTORY_SEPARATOR_STR_A "bin" ACE_DIRECTORY_SEPARATOR_STR_A "ace_gperf");
      this->gperf_path_ = std::move (path);
#endif /* ACE_GPERF */
    }

#if defined (IDL_ANON_ERROR)
  anon_type_diagnostic (ANON_TYPE_ERROR);
#elif defined (IDL_ANON_WARNING)
  anon_type_diagnostic (ANON_TYPE_WARNING);
#elif defined (IDL_ANON_SILENT)
  anon_type_diagnostic (ANON_TYPE_SILENT);
#endif

  // Reset the flags that are scoped to the current input file.
  this->reset_flag_seen ();
}

// When starting to process the next command line input idl file, reset.
void
IDL_GlobalData::reset_flag_seen ()
{
  abstract_iface_seen_ = false;
  abstractbase_seen_ = false;
  aggregate_seen_ = false;
  any_arg_seen_ = false;
  any_seen_ = false;
  any_seq_seen_ = false;
  array_seen_ = false;
  array_seq_seen_ = false;
  base_object_seen_ = false;
//basic_type_seen_
  bd_string_seen_ = false;
  boolean_seq_seen_ = false;
  char_seq_seen_ = false;
  component_seen_ = false;
  connector_seen_ = false;
  double_seq_seen_ = false;
  enum_seen_ = false;
  exception_seen_ = false;
  fixed_array_decl_seen_ = false;
  fixed_size_decl_seen_ = false;
  float_seq_seen_ = false;
  fwd_iface_seen_ = false;
  fwd_valuetype_seen_ = false;
  iface_seq_seen_ = false;
  interface_seen_ = false;
  local_iface_seen_ = false;
  long_seq_seen_ = false;
  longdouble_seq_seen_ = false;
  longlong_seq_seen_ = false;
  non_local_fwd_iface_seen_ = false;
  non_local_iface_seen_ = false;
  non_local_op_seen_ = false;
  object_arg_seen_ = false;
  octet_seq_seen_ = false;
  operation_seen_ = false;
  pseudo_seq_seen_ = false;
  recursive_type_seen_ = false;
  seq_seen_ = false;
  short_seq_seen_ = false;
  special_basic_decl_seen_ = false;
  map_seen_ = false;
  string_seen_ = false;
  string_member_seen_ = false;
  string_seq_seen_ = false;
  typecode_seen_ = false;
  ub_string_seen_ = false;
  ulong_seq_seen_ = false;
  ulonglong_seq_seen_ = false;
  union_seen_ = false;
  ushort_seq_seen_ = false;
  valuebase_seen_ = false;
  valuefactory_seen_ = false;
  valuetype_seen_ = false;
  var_array_decl_seen_ = false;
  var_size_decl_seen_ = false;
  vt_seq_seen_ = false;
  wchar_seq_seen_ = false;
  wstring_seq_seen_ = false;
  dds_connector_seen_ = false;
  ami_connector_seen_ = false;

  need_skeleton_includes_ = false;
}

// Get or set scopes stack
UTL_ScopeStack &
IDL_GlobalData::scopes ()
{
  return this->pd_scopes;
}

// Get or set root of AST
AST_Root *
IDL_GlobalData::root () const
{
  return this->pd_root;
}

void
IDL_GlobalData::set_root (AST_Root *r)
{
  this->pd_root = r;
}

// Get or set generator object
AST_Generator *
IDL_GlobalData::gen () const
{
  return this->pd_gen;
}

void
IDL_GlobalData::set_gen (AST_Generator *g)
{
  this->pd_gen = g;
}

// Get or set PrimaryKeyBase object
AST_ValueType *
IDL_GlobalData::primary_key_base () const
{
  return this->pd_primary_key_base;
}

void
IDL_GlobalData::primary_key_base (AST_ValueType *v)
{
  this->pd_primary_key_base = v;
}

// Get or set error object
UTL_Error *
IDL_GlobalData::err () const
{
  return this->pd_err;
}

void
IDL_GlobalData::set_err (UTL_Error *e)
{
  this->pd_err = e;
}

// Get or set error count
int
IDL_GlobalData::err_count () const
{
  return this->pd_err_count;
}

void
IDL_GlobalData::set_err_count (int c)
{
  this->pd_err_count = c;
}

// Get or set line number
long
IDL_GlobalData::lineno () const
{
  return this->pd_lineno;
}

void
IDL_GlobalData::set_lineno (long n)
{
  this->pd_lineno = n;
}

// Get or set file name being read now
std::string const&
IDL_GlobalData::filename () const
{
  return this->pd_filename;
}

void
IDL_GlobalData::set_filename (std::string const& filename)
{
  this->pd_filename = filename;
}

// Get or set main file name
std::string const&
IDL_GlobalData::main_filename () const
{
  return this->pd_main_filename;
}

void
IDL_GlobalData::set_main_filename (std::string const& filename)
{
  this->pd_main_filename = filename;
}

// Get or set real file name
std::string const&
IDL_GlobalData::real_filename () const
{
  return this->pd_real_filename;
}

void
IDL_GlobalData::set_real_filename (std::string const& filename)
{
  this->pd_real_filename = filename;
}

// Get or set indicator whether import is on
bool
IDL_GlobalData::imported () const
{
  return this->pd_in_main_file ? false : pd_import;
}

bool
IDL_GlobalData::import () const
{
  return this->pd_import;
}

void
IDL_GlobalData::set_import (bool is_in)
{
  this->pd_import = is_in;
}

// Get or set indicator whether we're reading the main file now
bool
IDL_GlobalData::in_main_file () const
{
  return this->pd_in_main_file;
}

void
IDL_GlobalData::set_in_main_file (bool is_in)
{
  this->pd_in_main_file = is_in;
}

// Get or set stripped file name
std::string const&
IDL_GlobalData::stripped_filename () const
{
  return this->pd_stripped_filename;
}

void
IDL_GlobalData::set_stripped_filename (std::string const& filename)
{
  this->pd_stripped_filename = filename;
}

// Get or set cache value for argv[0]
std::string const&
IDL_GlobalData::prog_name () const
{
  return this->pd_prog_name;
}

void
IDL_GlobalData::set_prog_name (std::string const& name)
{
  this->pd_prog_name = name;
}

void
IDL_GlobalData::clear_prog_name ()
{
  this->pd_prog_name.clear ();
}

// Get or set location to find C preprocessor
std::string const&
IDL_GlobalData::cpp_location () const
{
  return this->pd_cpp_location;
}

void
IDL_GlobalData::set_cpp_location (std::string const& location)
{
  this->pd_cpp_location = location;
}

// Get or set IDL compiler flags
long
IDL_GlobalData::compile_flags () const
{
  return this->pd_compile_flags;
}

void
IDL_GlobalData::set_compile_flags (long cf)
{
  if (cf & IDL_CF_ONLY_USAGE)
    {
      print_help ();
    }
  if (cf & IDL_CF_DUMP_AST)
    {
      syntax_only_ = true;
    }
  this->pd_compile_flags = cf;
}

// Get local escapes. This provides an additional mechanism to pass
// information to a BE.
std::string const&
IDL_GlobalData::local_escapes () const
{
  return this->pd_local_escapes;
}

// Append a local escape.
void
IDL_GlobalData::append_local_escape (std::string const& escape)
{
  this->pd_local_escapes.append (escape);
  this->pd_local_escapes.push_back (' ');
}

void
IDL_GlobalData::set_local_escapes (std::string const& escapes)
{
  this->pd_local_escapes = escapes;
}

// Get or set indent object
UTL_Indenter *
IDL_GlobalData::indent () const
{
  return this->pd_indent;
}

void
IDL_GlobalData::set_indent (UTL_Indenter *i)
{
  this->pd_indent = i;
}

// Have we seen this #include file name before?
long
IDL_GlobalData::seen_include_file_before (std::string const& filename)
{
  for (size_t i = 0; i < this->pd_include_file_names_.size (); ++i)
    {
      if (filename == this->pd_include_file_names_[i])
        {
          return ++this->include_file_seen_count_[i];
        }
    }

  return 0;
}

// Store the name of an #include file.
void
IDL_GlobalData::store_include_file_name (std::string const& filename)
{
  // Check if we need to store it at all or whether we've seen it already.
  if (this->seen_include_file_before (filename))
    {
      return;
    }

  this->pd_include_file_names_.push_back (filename);
  this->include_file_seen_count_.push_back (1);
}

std::vector<std::string> const&
IDL_GlobalData::include_file_names () const
{
  return this->pd_include_file_names_;
}

size_t
IDL_GlobalData::n_include_file_names () const
{
  return this->pd_include_file_names_.size ();
}

// Access methods to deal with other IDL files included in the main
// IDL file.

void
IDL_GlobalData::add_to_included_idl_files (std::string const& file_name)
{
  // Let's avoid duplicates.
  for (std::string const &included_file : this->included_idl_files_)
    {
      if (file_name == included_file)
        {
          return;
        }
    }

  this->included_idl_files_.emplace_back (file_name);
}

std::vector<std::string> &
IDL_GlobalData::included_idl_files ()
{
  return this->included_idl_files_;
}

size_t
IDL_GlobalData::n_included_idl_files ()
{
  return this->included_idl_files_.size ();
}

void
IDL_GlobalData::set_parse_state(ParseState ps)
{
  pd_parse_state = ps;
}

IDL_GlobalData::ParseState
IDL_GlobalData::parse_state ()
{
  return pd_parse_state;
}

// returns the IDL source file being copiled
std::string const& IDL_GlobalData::idl_src_file () const
{
  return this->pd_idl_src_file;
}

// set the source IDL file that is being parsed
void IDL_GlobalData::idl_src_file (std::string const& filename)
{
  this->pd_idl_src_file = filename;
}

void
IDL_GlobalData::temp_dir (std::string const& s)
{
  this->temp_dir_ = s;

  size_t const separator_length = sizeof (ACE_DIRECTORY_SEPARATOR_STR_A) - 1u;
  if (this->temp_dir_.size () <= separator_length
      || this->temp_dir_.compare (this->temp_dir_.size () - separator_length,
                                   separator_length,
                                   ACE_DIRECTORY_SEPARATOR_STR_A) != 0)
    {
      this->temp_dir_.append (ACE_DIRECTORY_SEPARATOR_STR_A);
    }
}

std::string const&
IDL_GlobalData::temp_dir () const
{
  return this->temp_dir_;
}

void
IDL_GlobalData::tao_root (std::string const& s)
{
  this->tao_root_ = s;
}

std::string const&
IDL_GlobalData::tao_root () const
{
  return this->tao_root_;
}

void
IDL_GlobalData::gperf_path (std::string const& s)
{
  this->gperf_path_ = s;
}

std::string const&
IDL_GlobalData::gperf_path () const
{
  return this->gperf_path_;
}

void
IDL_GlobalData::ident_string (std::string const& s)
{
  this->ident_string_ = s;
}

std::string const&
IDL_GlobalData::ident_string () const
{
  return this->ident_string_;
}

void
IDL_GlobalData::case_diff_error (bool val)
{
  this->case_diff_error_ = val;
}

bool
IDL_GlobalData::case_diff_error ()
{
  return this->case_diff_error_;
}

void
IDL_GlobalData::nest_orb (bool val)
{
  this->nest_orb_ = val;
}

bool
IDL_GlobalData::nest_orb ()
{
  return this->nest_orb_;
}

void
IDL_GlobalData::destroy ()
{
  this->pd_filename.clear ();
  this->pd_main_filename.clear ();
  this->pd_real_filename.clear ();
  this->pd_stripped_filename.clear ();
  this->pd_idl_src_file.clear ();

  size_t size = this->pragma_prefixes ().size  ();
  char *trash = nullptr;

  for (size_t i = 0; i < size; ++i)
    {
      this->pragma_prefixes ().pop (trash);
      delete [] trash;
      trash = nullptr;
    }

  this->pd_include_file_names_.clear ();
  this->include_file_seen_count_.clear ();

  this->included_idl_files_.clear ();

  this->recursion_start_.reset ();

  // Reset the member of the CORBA module containing the basic types
  // to point to itself, since all the other CORBA modules (if any)
  // will be destroyed.
  if (this->corba_module_ != nullptr)
    {
      this->corba_module_->reset_last_in_same_parent_scope ();
    }

  if (nullptr != this->pd_root)
    {
      this->pd_root->destroy ();
    }
}

void
IDL_GlobalData::append_idl_flag (std::string const& s)
{
  this->idl_flags_.append (" ");
  this->idl_flags_.append (s);
}

std::string const&
IDL_GlobalData::idl_flags () const
{
  return this->idl_flags_;
}

std::unordered_set<std::string> &
IDL_GlobalData::idl_keywords ()
{
  return this->idl_keywords_;
}

ACE_Unbounded_Stack<char *> &
IDL_GlobalData::pragma_prefixes ()
{
  return this->pragma_prefixes_;
}

void
IDL_GlobalData::update_prefix (char *filename)
{
  // If we are just starting up and processing the temporary filename,
  // there are no prefix issues to deal with yet.
  if (this->pd_main_filename.empty () || this->pd_filename.empty ())
    {
      return;
    }

  std::string const& current_filename = this->pd_filename;
  std::string const next_filename (filename);

  // We have to do this check because some preprocessors (gcc 3.2
  // on RedHat Linux 7.1, for one) output the same filename
  // multiple times for no apparent reason, and we don't want it
  // to clear the prefix.
  if (current_filename == next_filename
      || (current_filename.size () >= 3
          && current_filename.substr (current_filename.size () - 3) == ".cc"))
    {
      return;
    }

  std::string const& main_filename = this->pd_main_filename;

  char *prefix = nullptr;
  int status = this->file_prefixes_.find (filename, prefix);

  if (status == 0)
    {
      this->pd_root->prefix (prefix);
    }
  else
    {
      prefix = ACE::strnew ("");
      (void) this->file_prefixes_.bind (ACE::strnew (filename), prefix);
      this->pd_root->prefix ("");
    }

  // The first branch is executed if we are finishing an
  // included IDL file (but the current filename has not yet
  // been changed). So we check for (1) the current filename is
  // not the same as the main filename (2) the prefix stack size
  // is greater than 1 (to skip the case where we are passed the
  // temporary filename) and (3) we have either seen the filename
  // passed in before as an included file or we are passed the
  // main filename. Otherwise we know we are beginning an included
  // file, so we push a blank prefix on the stack, which may
  // possibly be changed later.
  if (this->seen_include_file_before (filename) != 0
      || next_filename == main_filename
      || next_filename != current_filename)
    {
      if (!this->pd_in_main_file)
        {
          std::string current_filename (this->pd_filename);
          status =
            this->file_prefixes_.find (current_filename.data (),
                                       prefix);

          // This function is called just before we transition to a
          // new file at global scope. If there is a non-null prefix
          // stored in the table under our not-yet-changed filename,
          // pop it.
          if (status == 0 && prefix != nullptr && prefix[0] != '\0')
            {
              char *trash = nullptr;
              this->pragma_prefixes_.pop (trash);
              delete [] trash;
            }
        }
    }
  else
    {
      this->pragma_prefixes_.push (ACE::strnew (""));
    }
}

/**
 Whether we should not mung idl element names that are
 C++ keywords e.g. delete, operator etc. with _cxx_ prefix.
 Should be true when being used by the IFR Service
 */
bool
IDL_GlobalData::preserve_cpp_keywords ()
{
  return preserve_cpp_keywords_;
}

/**
 Set whether we should not mung idl element names that are C++
 keywords e.g. delete, operator etc. with _cxx_ prefix.
 Is unset by the tao_idl compiler.
 */
void
IDL_GlobalData::preserve_cpp_keywords (bool val)
{
  preserve_cpp_keywords_ = val;
}

void
IDL_GlobalData::add_include_path (std::string const& path, bool is_system)
{
  this->include_paths_.push_back ({path, is_system});
}

void
IDL_GlobalData::add_rel_include_path (char const *s)
{
  if (s != nullptr)
    {
      this->rel_include_paths_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::rel_include_paths () const
{
  return this->rel_include_paths_;
}

void
IDL_GlobalData::add_ciao_lem_file_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_lem_file_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_lem_file_names () const
{
  return this->ciao_lem_file_names_;
}

void
IDL_GlobalData::add_ciao_rti_ts_file_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_rti_ts_file_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_rti_ts_file_names () const
{
  return this->ciao_rti_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_spl_ts_file_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_spl_ts_file_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_spl_ts_file_names () const
{
  return this->ciao_spl_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_oci_ts_file_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_oci_ts_file_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_oci_ts_file_names () const
{
  return this->ciao_oci_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_coredx_ts_file_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_coredx_ts_file_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_coredx_ts_file_names () const
{
  return this->ciao_coredx_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_ami_iface_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_ami_iface_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_ami_iface_names () const
{
  return this->ciao_ami_iface_names_;
}

void
IDL_GlobalData::add_ciao_ami_recep_names (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_ami_recep_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_ami_recep_names () const
{
  return this->ciao_ami_recep_names_;
}

void
IDL_GlobalData::add_included_ami_recep_names (char const *s)
{
  if (s != nullptr)
    {
      this->included_ami_recep_names_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::included_ami_recep_names () const
{
  return this->included_ami_recep_names_;
}

bool
IDL_GlobalData::included_ami_receps_done () const
{
  return this->included_ami_receps_done_;
}

void
IDL_GlobalData::included_ami_receps_done (bool val)
{
  this->included_ami_receps_done_ = val;
}

void
IDL_GlobalData::add_ciao_ami_idl_fnames (char const *s)
{
  if (s != nullptr)
    {
      this->ciao_ami_idl_fnames_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::ciao_ami_idl_fnames () const
{
  return this->ciao_ami_idl_fnames_;
}

void
IDL_GlobalData::add_dds4ccm_impl_fnames (char const *s)
{
  if (s != nullptr)
    {
      this->dds4ccm_impl_fnames_.emplace_back (s);
    }
}

std::vector<std::string> const &
IDL_GlobalData::dds4ccm_impl_fnames () const
{
  return this->dds4ccm_impl_fnames_;
}

ACE_Unbounded_Queue<AST_Interface *> &
IDL_GlobalData::mixed_parentage_interfaces ()
{
  return this->mixed_parentage_interfaces_;
}

ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex> &
IDL_GlobalData::file_prefixes ()
{
  return this->file_prefixes_;
}

bool
IDL_GlobalData::pass_orb_idl () const
{
  return this->pass_orb_idl_;
}

void
IDL_GlobalData::pass_orb_idl (bool val)
{
  this->pass_orb_idl_ = val;
}

bool
IDL_GlobalData::using_ifr_backend () const
{
  return this->using_ifr_backend_;
}

void
IDL_GlobalData::using_ifr_backend (bool val)
{
  this->using_ifr_backend_ = val;
}

bool
IDL_GlobalData::ignore_idl3 () const
{
  return this->ignore_idl3_;
}

void
IDL_GlobalData::ignore_idl3 (bool val)
{
  this->ignore_idl3_ = val;
}

bool
IDL_GlobalData::dcps_support_zero_copy_read () const
{
  return this->dcps_support_zero_copy_read_;
}

void
IDL_GlobalData::dcps_support_zero_copy_read (bool val)
{
  this->dcps_support_zero_copy_read_ = val;
}

bool
IDL_GlobalData::dcps_gen_zero_copy_read () const
{
  return this->dcps_gen_zero_copy_read_;
}

void
IDL_GlobalData::dcps_gen_zero_copy_read (bool val)
{
  this->dcps_gen_zero_copy_read_ = val;
}

// Return 0 on success, -1 failure. The <errno> corresponding to the
// error that caused the GPERF execution is also set.
int
IDL_GlobalData::check_gperf ()
{
  // If absolute path is not specified yet, let us call just
  // "gperf". Hopefully PATH is set up correctly to locate the gperf.
  if (this->gperf_path_.empty ())
    {
      // If ACE_GPERF is defined then use that gperf program instead of "gperf."
#if defined (ACE_GPERF)
      this->gperf_path (ACE_GPERF);
#else
      this->gperf_path ("ace_gperf");
#endif /* ACE_GPERF */
    }

  // If we have absolute path for the <gperf> rather than just the
  // executable name <gperf>, make sure the file exists
  // firsts. Otherwise just call <gperf>. Probably PATH is set
  // correctly to take care of this.

  // If ACE_GPERF is defined then use that gperf program instead of "gperf."
#if defined (ACE_GPERF)
  if (this->gperf_path_ != ACE_GPERF)
#else
  if (this->gperf_path_ != "ace_gperf")
#endif /* ACE_GPERF */
    {
      // It is absolute path. Check the existence, permissions and
      // the modes.
      if (ACE_OS::access (this->gperf_path_.c_str (),
                          F_OK | X_OK) == -1)
        {
          // Problem with the file. No point in having the absolute
          // path. Swith to "gperf".
          // If ACE_GPERF is defined then use that gperf program
          //instead of "gperf."
#if defined (ACE_GPERF)
          this->gperf_path (ACE_GPERF);
#else
          this->gperf_path ("ace_gperf");
#endif /* ACE_GPERF */
        }
    }

  // Just call gperf in silent mode. It will come and immly exit.

  // Using ACE_Process.
  ACE_Process process;
  ACE_Process_Options process_options;

  // Set the command line for the gperf program.
  process_options.command_line (ACE_TEXT ("\"%s\" -V"),
                                this->gperf_path_.c_str ());

  // Spawn a process for gperf.
  if (process.spawn (process_options) == -1)
    {
      return -1;
    }

#if defined (ACE_WIN32)
  // No wait or anything in Win32.
  return 0;
#else
  // Wait for gperf to complete.
  ACE_exitcode wait_status = 0;
  if (process.wait (&wait_status) == -1)
    {
      return -1;
    }
  else
    {
      // Wait is successful, we will check the exit code from the
      // spawned process.
      if (WIFEXITED (wait_status))
        {
          // Normal exit.

          // Check the exit value of the spawned process. ACE_Process
          // exits with <errno> as exit code, if it is not able to
          // exec gperf program, so get the exit code now and set that
          // to <errno> again, so that it can be used to print error
          // messages.
          errno = WEXITSTATUS (wait_status);

          if (errno)
            {
              // <exec> has failed.
              return -1;
            }
          else
            {
              // Everything was alright.
              return 0;
            }
        }
      else
        {
          // Not a normal exit. No <errno> might be set.
          return -1;
        }
    }
#endif /* ACE_WIN32 */
}

void
IDL_GlobalData::fini ()
{
  if (nullptr != this->pd_root)
    {
      this->pd_root->fini ();
      delete this->pd_root;
      this->pd_root = nullptr;
    }

  delete this->pd_err;
  this->pd_err = nullptr;
  delete this->pd_gen;
  this->pd_gen = nullptr;
  delete this->pd_indent;
  this->pd_indent = nullptr;
  this->pd_local_escapes.clear ();
  this->tao_root_.clear ();
  this->gperf_path_.clear ();
  this->temp_dir_.clear ();
  this->ident_string_.clear ();
  this->pd_include_file_names_.clear ();
  this->include_file_seen_count_.clear ();

  ACE_Hash_Map_Entry<char *, char *> *entry = nullptr;

  for (ACE_Hash_Map_Iterator<char *, char *, ACE_Null_Mutex> hiter (
           this->file_prefixes_
         );
       !hiter.done ();
       hiter.advance ())
    {
      hiter.next (entry);
      ACE::strdelete (entry->ext_id_);
      ACE::strdelete (entry->int_id_);
    }

  for (std::pair<std::string const,
                 std::unique_ptr<DCPS_Data_Type_Info>> const& dcps_entry :
         this->dcps_type_info_map_)
    {
      dcps_entry.second->name_->destroy ();
    }

  this->dcps_type_info_map_.clear ();
}

ACE_Unbounded_Queue<AST_ValueType *> &
IDL_GlobalData::primary_keys ()
{
  return this->primary_keys_;
}

void
IDL_GlobalData::check_primary_keys ()
{
  AST_ValueType *holder = nullptr;

  while (!this->primary_keys_.is_empty ())
    {
      // Dequeue the element at the head of the queue.
      if (this->primary_keys_.dequeue_head (holder) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) idl_global::check_primary_keys - "
                      "dequeue_head failed\n"));
          throw Bailout ();
        }

      if (!holder->legal_for_primary_key ())
        {
          this->pd_err->illegal_primary_key (holder);
        }
    }
}

std::optional<std::string> const&
IDL_GlobalData::recursion_start () const
{
  return this->recursion_start_;
}

void
IDL_GlobalData::recursion_start (std::string const& val)
{
  /// Strip off any trailing slashes (not needed
  /// for further processing).
  std::string tmp (val);
  while (!tmp.empty () && (tmp.back () == '\\' || tmp.back () == '/'))
    {
      tmp.pop_back ();
    }

  this->recursion_start_ = std::move (tmp);
}

bool
IDL_GlobalData::multi_file_input () const
{
  return this->multi_file_input_;
}

void
IDL_GlobalData::multi_file_input (bool val)
{
  this->multi_file_input_ = val;
}

std::string const&
IDL_GlobalData::big_file_name () const
{
  static std::string const name = "PICML_IDL_file_bag";
  return name;
}

FE_Utils::T_PARAMLIST_INFO const *
IDL_GlobalData::current_params () const
{
  return this->current_params_;
}

void
IDL_GlobalData::current_params (
FE_Utils::T_PARAMLIST_INFO *params)
{
  this->current_params_ = params;
}

UTL_StrList const *
IDL_GlobalData::alias_params () const
{
  return this->alias_params_;
}

void
IDL_GlobalData::alias_params (
UTL_StrList *params)
{
  this->alias_params_ = params;
}

UTL_StrList const *
IDL_GlobalData::for_new_holder () const
{
  return this->for_new_holder_;
}

void
IDL_GlobalData::for_new_holder (
  UTL_StrList *params)
{
  this->for_new_holder_ = params;
}

void
IDL_GlobalData::set_dcps_sequence_type (std::string const& seq_type)
{
  this->dcps_sequence_types_list_.emplace_back (seq_type);
}

bool
IDL_GlobalData::dcps_sequence_type_defined (std::string const& seq_type) const
{
  for (std::string const &type_name : this->dcps_sequence_types_list_)
    {
      if (seq_type == type_name)
        {
          return true;
        }
    }
  return false;
}

void
IDL_GlobalData::add_dcps_data_type (std::string const& id)
{
  // Check if the type already exists.
  std::string const& type_name = id;

  if (this->dcps_type_info_map_.find (type_name) == this->dcps_type_info_map_.end ())
    {
      // No existing entry, add one.

      UTL_ScopedName* t1 =
        FE_Utils::string_to_scoped_name (type_name);

      // Chained with null Identifier required!!
      UTL_ScopedName* target =
        new UTL_ScopedName (new Identifier (""), t1);

      std::unique_ptr<DCPS_Data_Type_Info> newinfo (
        new DCPS_Data_Type_Info ());
      newinfo->name_.reset (target);

      // Add the newly formed entry to the map.
      std::pair<DCPS_Type_Info_Map::iterator, bool> const result =
        this->dcps_type_info_map_.try_emplace (type_name, std::move (newinfo));
      if (!result.second)
        {
          newinfo->name_->destroy ();
          newinfo->name_.reset ();
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to insert type into ")
                      ACE_TEXT ("DCPS type container: %s.\n"),
                      type_name.c_str ()));
          return;
        }
    }
  else
    {
      ACE_ERROR ((LM_WARNING,
                  ACE_TEXT ("Duplicate DCPS type defined: %s.\n"),
                  type_name.c_str ()));
    }
}

bool
IDL_GlobalData::add_dcps_data_key (std::string const& id, std::string const& key)
{
  // Search the map for the type.
  std::string const& type_name = id;
  DCPS_Type_Info_Map::iterator const entry =
    this->dcps_type_info_map_.find (type_name);

  if (entry != this->dcps_type_info_map_.end ())
    {
      // Add the new key field to the type.
      entry->second->key_list_.push_back (key);
      return true;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("missing previous #pragma ")
                  ACE_TEXT ("DCPS_DATA_TYPE\n")));
    }

  return false;
}

IDL_GlobalData::DCPS_Data_Type_Info*
IDL_GlobalData::is_dcps_type (UTL_ScopedName* target)
{
  // Traverse the entire map.
  for (std::pair<std::string const,
                 std::unique_ptr<DCPS_Data_Type_Info>> const& entry :
         this->dcps_type_info_map_)
    {
      // Look for our Identifier.
      if (0 == entry.second->name_->compare (target))
        {
          // Found it!
          return entry.second.get ();
        }
    }

  // No joy.
  return nullptr;
}

AST_Module *
IDL_GlobalData::corba_module () const
{
  return this->corba_module_;
}

void
IDL_GlobalData::corba_module (AST_Module *m)
{
  this->corba_module_ = m;
}

std::vector<IDL_GlobalData::Include_Path_Info> const&
IDL_GlobalData::include_paths () const
{
  return this->include_paths_;
}

void
IDL_GlobalData::anon_type_diagnostic (
  IDL_GlobalData::ANON_TYPE_DIAGNOSTIC val)
{
  anon_type_diagnostic_ = val;
  explicit_anon_type_diagnostic_ = true;
}

bool
IDL_GlobalData::explicit_anon_type_diagnostic () const
{
  return explicit_anon_type_diagnostic_;
}

bool
IDL_GlobalData::anon_error () const
{
  if (idl_version_ >= IDL_VERSION_4 && !explicit_anon_type_diagnostic_) {
    return false;
  }
  return anon_type_diagnostic_ == ANON_TYPE_ERROR;
}

bool
IDL_GlobalData::anon_warning () const
{
  if (idl_version_ >= IDL_VERSION_4 && !explicit_anon_type_diagnostic_) {
    return false;
  }
  return anon_type_diagnostic_ == ANON_TYPE_WARNING;
}

bool
IDL_GlobalData::anon_silent () const
{
  if (idl_version_ >= IDL_VERSION_4 && !explicit_anon_type_diagnostic_) {
    return true;
  }
  return anon_type_diagnostic_ == ANON_TYPE_SILENT;
}

bool
IDL_GlobalData::in_typedef () const
{
  return this->in_typedef_;
}

void
IDL_GlobalData::in_typedef (bool val)
{
  this->in_typedef_ = val;
}

bool
IDL_GlobalData::in_tmpl_mod_no_alias () const
{
  return this->in_tmpl_mod_no_alias_;
}

void
IDL_GlobalData::in_tmpl_mod_no_alias (bool val)
{
  this->in_tmpl_mod_no_alias_ = val;
}

bool
IDL_GlobalData::in_tmpl_mod_alias () const
{
  return this->in_tmpl_mod_alias_;
}

void
IDL_GlobalData::in_tmpl_mod_alias (bool val)
{
  this->in_tmpl_mod_alias_ = val;
}

void
IDL_GlobalData::parse_args_exit (int status)
{
  parse_args_exit_ = true;
  parse_args_exit_status_ = status;
}

void
IDL_GlobalData::print_help ()
{
  print_help_ = true;
  parse_args_exit (0);
}

void
IDL_GlobalData::print_version ()
{
  print_version_ = true;
  parse_args_exit (0);
}

bool
IDL_GlobalData::print_warnings ()
{
  return ! (idl_global->compile_flags () & IDL_CF_NOWARNINGS);
}

/*
 * These are generated in idl.yy.cpp but they are not put in the header file,
 * so to use them we must declare them here.
 */
struct yy_buffer_state;
extern yy_buffer_state *tao_yy_scan_string (char const *);
extern int tao_yylex_destroy ();

namespace
{
  class OldState
  {
  public:
    explicit OldState (bool disable_output = false)
        : old_filename_ (idl_global->filename ()),
          old_lineno_ (idl_global->lineno ()),
          old_idl_src_file_ (idl_global->idl_src_file ()),
          disable_output_ (disable_output),
          default_streambuf_ (ACE_DEFAULT_LOG_STREAM->rdbuf ()),
          flags_ (ACE_LOG_MSG->flags ())
      {
        idl_global->in_eval_ = true;

        idl_global->set_lineno (1);
        idl_global->set_filename ({});

        // Name this pseudo-file "builtin-N"
        static unsigned n = 1;
        std::string builtin_name ("builtin-");
        builtin_name += std::to_string (n++);
        idl_global->idl_src_file (builtin_name);
        idl_global->set_filename (builtin_name);

        if (disable_output_)
          {
            ACE_DEFAULT_LOG_STREAM->rdbuf (nullptr);
            ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
            ACE_LOG_MSG->clr_flags (ACE_LOG_MSG->flags ());
         }
      }

      ~OldState()
      {
        idl_global->set_lineno (old_lineno_);

        idl_global->set_filename (old_filename_);
        idl_global->idl_src_file (old_idl_src_file_);
        idl_global->reset_flag_seen ();

        if (disable_output_)
          {
            ACE_DEFAULT_LOG_STREAM->rdbuf (default_streambuf_);
            ACE_LOG_MSG->set_flags (flags_);
          }

        tao_yylex_destroy ();
        idl_global->in_eval_ = false;
      }

    private:
      std::string const old_filename_;
      long const old_lineno_;
      std::string const old_idl_src_file_;
      bool const disable_output_;
      std::streambuf *const default_streambuf_;
      unsigned long const flags_;
  };
}

void
IDL_GlobalData::eval (char const *string, bool disable_output)
{
  OldState old (disable_output);

  // Set up Flex to read from string
  tao_yy_scan_string (string);

  // emulate DRV_drive()
  FE_yyparse ();
  idl_global->check_primary_keys ();
  AST_check_fwd_decls ();
}

void
IDL_GlobalData::dump_ast ()
{
  idl_global->set_compile_flags (idl_global->compile_flags ()
                                 | IDL_CF_DUMP_AST);
}
