// $Id$

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

#include "idl_global.h"
#include "global_extern.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "utl_string.h"
#include "fe_extern.h"
#include "fe_private.h"
#include "nr_extern.h"

#include "ast_root.h"
#include "ast_generator.h"
#include "ast_valuetype.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"

// Define an increment for the size of the array used to store names of
// included files.
#undef INCREMENT
#define INCREMENT 64

static long *pSeenOnce= 0;

#if defined (ACE_OPENVMS)
#include <unixlib.h>
char* IDL_GlobalData::translateName(const char* name, char *name_buf)
{
  char* transName = (ACE_OS::strpbrk (name, ":[") == 0
                      ? (char*)name
                      : ::decc$translate_vms (name));
  if (transName)
    {
      ACE_OS::strcpy (name_buf, transName);
      transName = name_buf;
    }

  return (transName == 0 || ((int)transName) == -1 ) ? 0 : transName;
}
#endif

IDL_GlobalData::IDL_GlobalData (void)
  : pd_root (0),
    pd_gen (0),
    pd_primary_key_base (0),
    pd_err (0),
    pd_err_count (0),
    pd_lineno (0),
    pd_filename (0),
    pd_main_filename (0),
    pd_real_filename (0),
    pd_stripped_filename (0),
    pd_import (false),
    pd_in_main_file (false),
    pd_prog_name (0),
    pd_cpp_location (0),
    pd_compile_flags (0),
    pd_local_escapes (0),
    pd_indent (0),
    pd_include_file_names (0),
    pd_n_include_file_names (0),
    pd_n_alloced_file_names (0),
    included_idl_files_ (0),
    n_included_idl_files_ (0),
    n_allocated_idl_files_ (0),
    pd_parse_state (PS_NoState),
    pd_idl_src_file (0),
    tao_root_ (0),
    gperf_path_ (0),
    temp_dir_ (0),
    ident_string_ (0),
    case_diff_error_ (true),
    nest_orb_ (false),
    idl_flags_ (""),
    preserve_cpp_keywords_ (true),
    pass_orb_idl_ (false),
    using_ifr_backend_ (false),
    ignore_idl3_ (false),
    dcps_support_zero_copy_read_ (false),
    dcps_gen_zero_copy_read_ (false),
    recursion_start_ (0),
    multi_file_input_ (false),
    big_file_name_ ("PICML_IDL_file_bag"),
    current_params_ (0),
    alias_params_ (0),
    included_ami_receps_done_ (false),
    corba_module_ (0),
    anon_type_diagnostic_ (ANON_TYPE_ERROR),
    in_typedef_ (false)
{
  // Path for the perfect hash generator(gperf) program.
  // Default is $ACE_ROOT/bin/gperf unless ACE_GPERF is defined.
  // Use ACE_GPERF if $ACE_ROOT hasn't been set or won't be set
  // in the environment.
  // Form the absolute pathname.
  char* ace_root = ACE_OS::getenv ("ACE_ROOT");

  if (ace_root == 0)
    // This may not cause any problem if -g option is used to specify
    // the correct path for the  gperf program. Let us ignore this
    // error here. It will be caught when we check the existence of
    // the perfect hasher and at that time, we can switch over to some
    // other scheme.
    {
#if defined (ACE_GPERF)
      // The actual gperf program must be included in the definition of
      // ACE_GPERF, not just the directory in which it is located.
      const char ace_gperf[] = ACE_GPERF;
      ACE_NEW (this->gperf_path_,
               char [ACE_OS::strlen (ace_gperf) + 1]);
      ACE_OS::sprintf (this->gperf_path_,
                       "%s",
                       ace_gperf);
#else
      this->gperf_path_ = 0;
#endif
    }
  else
    {
#if defined (ACE_GPERF)
      const char ace_gperf[] = ACE_GPERF;
      ACE_NEW (this->gperf_path_,
               char [ACE_OS::strlen (ace_root)
                     + ACE_OS::strlen ("/bin/")
                     + ACE_OS::strlen (ace_gperf)
                     + 1]);
      ACE_OS::sprintf (this->gperf_path_,
                       "%s" ACE_DIRECTORY_SEPARATOR_STR_A "bin" ACE_DIRECTORY_SEPARATOR_STR_A "%s",
                       ace_root,
                       ace_gperf);
#else /* Not ACE_GPERF */
      // Set it to the default value.
      ACE_NEW (this->gperf_path_,
               char [ACE_OS::strlen (ace_root)
                     + ACE_OS::strlen ("/bin/ace_gperf")
                     + 1]);
      ACE_OS::sprintf (this->gperf_path_,
                       "%s" ACE_DIRECTORY_SEPARATOR_STR_A "bin" ACE_DIRECTORY_SEPARATOR_STR_A "ace_gperf",
                       ace_root);
#endif /* ACE_GPERF */
    }

#if defined (IDL_ANON_ERROR)
  this->anon_type_diagnostic_ = ANON_TYPE_ERROR;
#elif defined (IDL_ANON_WARNING)
  this->anon_type_diagnostic_ = ANON_TYPE_WARNING;
#elif defined (IDL_ANON_SILENT)
  this->anon_type_diagnostic_ = ANON_TYPE_SILENT;
#endif

  // ambiguous_type_seen_ and basic_type_seen_ are not reset between
  // command line idl files, so do those here and then reset the rest.
  this->ambiguous_type_seen_ = false;
  this->basic_type_seen_ = false;
  this->reset_flag_seen ();
}

IDL_GlobalData::~IDL_GlobalData (void)
{
}

// When starting to process the next command line input idl file, reset.
void
IDL_GlobalData::reset_flag_seen (void)
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
  basic_arg_seen_ = false;
//basic_type_seen_
  bd_string_arg_seen_ = false;
  boolean_seq_seen_ = false;
  char_seq_seen_ = false;
  component_seen_ = false;
  connector_seen_ = false;
  double_seq_seen_ = false;
  enum_seen_ = false;
  exception_seen_ = false;
  fixed_array_arg_seen_ = false;
  fixed_size_arg_seen_ = false;
  float_seq_seen_ = false;
  fwd_iface_seen_ = false;
  fwd_valuetype_seen_ = false;
  iface_seq_seen_ = false;
  interface_seen_ = false;
  local_iface_seen_ = false;
  long_seq_seen_ = false;
  longdouble_seq_seen_ = false;
  longlong_seq_seen_ = false;
  non_local_iface_seen_ = false;
  non_local_op_seen_ = false;
  object_arg_seen_ = false;
  octet_seq_seen_ = false;
  operation_seen_ = false;
  pseudo_seq_seen_ = false;
  recursive_type_seen_ = false;
  seq_seen_ = false;
  short_seq_seen_ = false;
  special_basic_arg_seen_ = false;
  string_seen_ = false;
  string_member_seen_ = false;
  string_seq_seen_ = false;
  typecode_seen_ = false;
  ub_string_arg_seen_ = false;
  ulong_seq_seen_ = false;
  ulonglong_seq_seen_ = false;
  union_seen_ = false;
  ushort_seq_seen_ = false;
  valuebase_seen_ = false;
  valuefactory_seen_ = false;
  valuetype_seen_ = false;
  var_array_arg_seen_ = false;
  var_size_arg_seen_ = false;
  vt_seq_seen_ = false;
  wchar_seq_seen_ = false;
  wstring_seq_seen_ = false;
  dds_connector_seen_ = false;
  ami_connector_seen_ = false;

  need_skeleton_includes_ = false;
}

// Get or set scopes stack
UTL_ScopeStack &
IDL_GlobalData::scopes (void)
{
  return this->pd_scopes;
}

// Get or set root of AST
AST_Root *
IDL_GlobalData::root (void)
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
IDL_GlobalData::gen (void)
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
IDL_GlobalData::primary_key_base (void)
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
IDL_GlobalData::err (void)
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
IDL_GlobalData::err_count (void)
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
IDL_GlobalData::lineno (void)
{
  return this->pd_lineno;
}

void
IDL_GlobalData::set_lineno (long n)
{
  this->pd_lineno = n;
}

// Get or set file name being read now
UTL_String *
IDL_GlobalData::filename (void)
{
  return this->pd_filename;
}

void
IDL_GlobalData::set_filename (UTL_String *s)
{
  if (this->pd_filename != 0)
    {
      this->pd_filename->destroy ();
      delete this->pd_filename;
      this->pd_filename = 0;
    }

  this->pd_filename = s;
}

// Get or set main file name
UTL_String *
IDL_GlobalData::main_filename (void)
{
  return this->pd_main_filename;
}

void
IDL_GlobalData::set_main_filename (UTL_String *n)
{
  if (this->pd_main_filename != 0)
    {
      this->pd_main_filename->destroy ();
      delete this->pd_main_filename;
      this->pd_main_filename = 0;
    }
  this->pd_main_filename = n;
}

// Get or set real file name
UTL_String *
IDL_GlobalData::real_filename (void)
{
  return this->pd_real_filename;
}

void
IDL_GlobalData::set_real_filename (UTL_String *n)
{
  if (this->pd_real_filename != 0)
    {
      this->pd_real_filename->destroy ();
      delete this->pd_real_filename;
      this->pd_real_filename = 0;
    }

  this->pd_real_filename = n;
}

// Get or set indicator whether import is on
bool
IDL_GlobalData::imported (void)
{
  return this->pd_in_main_file ? false : pd_import;
}

bool
IDL_GlobalData::import (void)
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
IDL_GlobalData::in_main_file (void)
{
  return this->pd_in_main_file;
}

void
IDL_GlobalData::set_in_main_file (bool is_in)
{
  this->pd_in_main_file = is_in;
}

// Get or set stripped file name
UTL_String *
IDL_GlobalData::stripped_filename (void)
{
  return this->pd_stripped_filename;
}

void
IDL_GlobalData::set_stripped_filename (UTL_String *nm)
{
  if (this->pd_stripped_filename != 0)
    {
      delete this->pd_stripped_filename;
    }

  this->pd_stripped_filename = nm;
}

// Get or set cache value for argv[0]
const char *
IDL_GlobalData::prog_name (void)
{
  return this->pd_prog_name;
}

void
IDL_GlobalData::set_prog_name (const char *pn)
{
  this->pd_prog_name = pn;
}

// Get or set location to find C preprocessor
const char *
IDL_GlobalData::cpp_location (void)
{
  return this->pd_cpp_location;
}

void
IDL_GlobalData::set_cpp_location (const char *l)
{
  this->pd_cpp_location = l;
}

// Get or set IDL compiler flags
long
IDL_GlobalData::compile_flags (void)
{
  return this->pd_compile_flags;
}

void
IDL_GlobalData::set_compile_flags (long cf)
{
  this->pd_compile_flags = cf;
}

// Get or set local escapes string. This provides an additional
// mechanism to pass information to a BE.
char *
IDL_GlobalData::local_escapes (void)
{
  return this->pd_local_escapes;
}

void
IDL_GlobalData::set_local_escapes (const char *e)
{
  if (this->pd_local_escapes != 0)
    {
      delete [] this->pd_local_escapes;
    }

  this->pd_local_escapes = ACE::strnew (e);
}

// Get or set indent object
UTL_Indenter *
IDL_GlobalData::indent (void)
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
IDL_GlobalData::seen_include_file_before (char *n)
{
  unsigned long i;
  char *incl = 0;
  char *tmp = n;

  for (i = 0; i < this->pd_n_include_file_names; ++i)
    {
      incl = this->pd_include_file_names[i]->get_string ();

      if (ACE_OS::strcmp (tmp, incl) == 0)
        {
          return ++pSeenOnce[i];
        }
    }

  return 0;
}

// Store the name of an #include file.
void
IDL_GlobalData::store_include_file_name (UTL_String *n)
{
  // Check if we need to store it at all or whether we've seen it already.
  if (this->seen_include_file_before (n->get_string ()))
    {
      n->destroy ();
      delete n; // Don't keep filenames we don't store!
      return;
    }

  // OK, need to store. Make sure there's space for one more string
  if (this->pd_n_include_file_names == this->pd_n_alloced_file_names)
    {
      // Allocating more space.
      if (this->pd_n_alloced_file_names == 0)
        {
          this->pd_n_alloced_file_names = INCREMENT;
          ACE_NEW (this->pd_include_file_names,
                   UTL_String *[this->pd_n_alloced_file_names]);
          ACE_NEW (pSeenOnce,
                   long [this->pd_n_alloced_file_names]);
        }
      else
        {
          UTL_String    **o_include_file_names=   this->pd_include_file_names;
          unsigned long   o_n_alloced_file_names= this->pd_n_alloced_file_names;
          long           *o_pSeenOnce=            pSeenOnce;

          this->pd_n_alloced_file_names += INCREMENT;
          ACE_NEW (this->pd_include_file_names,
                   UTL_String *[this->pd_n_alloced_file_names]);
          ACE_NEW (pSeenOnce,
                   long [this->pd_n_alloced_file_names]);

          for (unsigned long i = 0; i < o_n_alloced_file_names; ++i)
            {
              this->pd_include_file_names[i] = o_include_file_names[i];
              pSeenOnce[i]= o_pSeenOnce[i];
            }

          delete [] o_include_file_names;
          delete [] o_pSeenOnce;
        }
    }

  // Store it.
  pSeenOnce[this->pd_n_include_file_names] = 1;
  this->pd_include_file_names[this->pd_n_include_file_names++] = n;
}

void
IDL_GlobalData::set_include_file_names (UTL_String **ns)
{
  this->pd_include_file_names = ns;
}

UTL_String **
IDL_GlobalData::include_file_names (void)
{
  return this->pd_include_file_names;
}

void
IDL_GlobalData::set_n_include_file_names (unsigned long n)
{
  pd_n_include_file_names = n;
}

unsigned long
IDL_GlobalData::n_include_file_names (void)
{
  return pd_n_include_file_names;
}

// Access methods to deal with other IDL files included in the main
// IDL file.

void
IDL_GlobalData::add_to_included_idl_files (const char *file_name)
{
  // Let's avoid duplicates.
  for (size_t index = 0; index < this->n_included_idl_files_; ++index)
    {
      if (!ACE_OS::strcmp (file_name, this->included_idl_files_[index]))
        {
          return;
        }
    }

  // Is there enough space there to store one more file.
  if (this->n_included_idl_files_ == this->n_allocated_idl_files_)
    {
      // Allocating more space.
      if (this->n_allocated_idl_files_ == 0)
        {
          // First time creation.
          this->n_allocated_idl_files_ = INCREMENT;
          ACE_NEW (this->included_idl_files_,
                   char *[this->n_allocated_idl_files_]);
        }
      else
        {
          // Adding more storage.
          char** old_included_idl_files =
            this->included_idl_files_;
          size_t n_old_allocated_idl_files =
            this->n_allocated_idl_files_;
          this->n_allocated_idl_files_ += INCREMENT;
          ACE_NEW (this->included_idl_files_,
                   char *[this->n_allocated_idl_files_]);

          for (size_t i = 0; i < n_old_allocated_idl_files; ++i)
            {
              this->included_idl_files_ [i] =
                old_included_idl_files [i];
            }

          delete [] old_included_idl_files;
        }
    }

  // Store it.
  this->included_idl_files_ [this->n_included_idl_files_++] =
    ACE::strnew (file_name);
}

char**
IDL_GlobalData::included_idl_files (void)
{
  return this->included_idl_files_;
}

size_t
IDL_GlobalData::n_included_idl_files (void)
{
  return this->n_included_idl_files_;
}

// Set the number of included_idl_files. Use this carefully. This
// method is used when we validate all the #included idl files,
// against the ones that we get after preprocessing.
void
IDL_GlobalData::n_included_idl_files (size_t n)
{
  this->n_included_idl_files_ = n;
}

void
IDL_GlobalData::set_parse_state(ParseState ps)
{
  pd_parse_state = ps;
}

IDL_GlobalData::ParseState
IDL_GlobalData::parse_state (void)
{
  return pd_parse_state;
}

// returns the IDL source file being copiled
UTL_String* IDL_GlobalData::idl_src_file (void)
{
  return this->pd_idl_src_file;
}

// set the source IDL file that is being parsed
void IDL_GlobalData::idl_src_file (UTL_String *s)
{
  this->pd_idl_src_file = s;
}

void
IDL_GlobalData::temp_dir (const char *s)
{
  // Delete the old pointer.
  delete [] this->temp_dir_;
  this->temp_dir_ = 0; // In case the ACE_NEW fails below.

  const size_t lengthSep = sizeof (ACE_DIRECTORY_SEPARATOR_STR_A) - 1u;
  const size_t lengthPath = ACE_OS::strlen (s);

  // Allocate memory, 1 for the end of string.
  ACE_NEW (this->temp_dir_, char [lengthPath + lengthSep + 1u]);

  // Copy the strings.
  if (lengthSep < lengthPath &&
      0 == ACE_OS::strcmp (s + lengthPath - lengthSep, ACE_DIRECTORY_SEPARATOR_STR_A))
    {
      // Already has a directory seporator on end of temp root, don't add another.
      ACE_OS::strcpy (this->temp_dir_, s);
    }
  else
    {
      // Need to add a directory seporator to temp root.
      ACE_OS::sprintf (this->temp_dir_,
                       "%s%s",
                       s,
                       ACE_DIRECTORY_SEPARATOR_STR_A);
    }
}

const char *
IDL_GlobalData::temp_dir (void) const
{
  return this->temp_dir_;
}

void
IDL_GlobalData::tao_root (const char *s)
{
  delete [] this->tao_root_;
  this->tao_root_ = ACE::strnew (s);
}

const char *
IDL_GlobalData::tao_root (void) const
{
  return this->tao_root_;
}

void
IDL_GlobalData::gperf_path (const char *s)
{
  delete [] this->gperf_path_;
  this->gperf_path_ = ACE::strnew (s);
}

const char *
IDL_GlobalData::gperf_path (void) const
{
  return this->gperf_path_;
}

void
IDL_GlobalData::ident_string (const char *s)
{
  delete [] this->ident_string_;
  this->ident_string_ = ACE::strnew (s);
}

const char *
IDL_GlobalData::ident_string (void) const
{
  return this->ident_string_;
}

void
IDL_GlobalData::case_diff_error (bool val)
{
  this->case_diff_error_ = val;
}

bool
IDL_GlobalData::case_diff_error (void)
{
  return this->case_diff_error_;
}

void
IDL_GlobalData::nest_orb (bool val)
{
  this->nest_orb_ = val;
}

bool
IDL_GlobalData::nest_orb (void)
{
  return this->nest_orb_;
}

void
IDL_GlobalData::destroy (void)
{
  if (this->pd_filename != 0)
    {
      this->pd_filename->destroy ();
      delete this->pd_filename;
      this->pd_filename = 0;
    }

  if (this->pd_main_filename != 0)
    {
      this->pd_main_filename->destroy ();
      delete this->pd_main_filename;
      this->pd_main_filename = 0;
    }

  if (this->pd_real_filename != 0)
    {
      this->pd_real_filename->destroy ();
      delete this->pd_real_filename;
      this->pd_real_filename = 0;
    }

  if (this->pd_stripped_filename != 0)
    {
      this->pd_stripped_filename->destroy ();
      delete this->pd_stripped_filename;
      this->pd_stripped_filename = 0;
    }

  if (this->pd_idl_src_file != 0)
    {
      this->pd_idl_src_file->destroy ();
      delete this->pd_idl_src_file;
      this->pd_idl_src_file = 0;
    }

  size_t size = this->pragma_prefixes ().size  ();
  char *trash = 0;

  for (size_t i = 0; i < size; ++i)
    {
      this->pragma_prefixes ().pop (trash);
      delete [] trash;
      trash = 0;
    }

  // Clean up each included file name - the array allocation itself
  // gets cleaned up in fini().
  for (unsigned long j = 0; j < this->pd_n_include_file_names; ++j)
    {
      this->pd_include_file_names[j]->destroy ();
      delete this->pd_include_file_names[j];
      this->pd_include_file_names[j] = 0;
    }

  this->pd_n_include_file_names = 0;

  for (size_t k = 0; k < n_included_idl_files_; ++k)
    {
      ACE::strdelete (this->included_idl_files_[k]);
      this->included_idl_files_[k] = 0;
    }

  this->n_included_idl_files_ = 0;
  this->n_allocated_idl_files_ = 0;
  delete [] this->included_idl_files_;
  this->included_idl_files_ = 0;

  ACE::strdelete (this->recursion_start_);
  this->recursion_start_ = 0;

  if (0 != this->pd_root)
    {
      this->pd_root->destroy ();
    }
}

void
IDL_GlobalData::append_idl_flag (const char *s)
{
  this->idl_flags_ += " " + ACE_CString (s);
}

const char *
IDL_GlobalData::idl_flags (void) const
{
  return this->idl_flags_.c_str ();
}

ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> &
IDL_GlobalData::idl_keywords (void)
{
  return this->idl_keywords_;
}

ACE_Unbounded_Stack<char *> &
IDL_GlobalData::pragma_prefixes (void)
{
  return this->pragma_prefixes_;
}

void
IDL_GlobalData::update_prefix (char *filename)
{
  // If we are just starting up and processing the temporary filename,
  // there are no prefix issues to deal with yet.
  if (this->pd_main_filename == 0 || this->pd_filename == 0)
    {
      return;
    }

  char *fstring = this->pd_filename->get_string ();
  char *tail = fstring + ACE_OS::strlen (fstring) - 3;

  // We have to do this check because some preprocessors (gcc 3.2
  // on RedHat Linux 7.1, for one) output the same filename
  // multiple times for no apparent reason, and we don't want it
  // to clear the prefix.
  if (ACE_OS::strcmp (fstring, filename) == 0
      || ACE_OS::strcmp (tail, ".cc") == 0)
    {
      return;
    }

  ACE_CString tmp ("", 0, false);
  char *main_filename = this->pd_main_filename->get_string ();

  char *prefix = 0;
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
      || ACE_OS::strcmp (filename, main_filename) == 0
      || ACE_OS::strcmp (filename, this->pd_filename->get_string ()) != 0)
    {
      if (!this->pd_in_main_file)
        {
          status =
            this->file_prefixes_.find (this->pd_filename->get_string (),
                                       prefix);

          // This function is called just before we transition to a
          // new file at global scope. If there is a non-null prefix
          // stored in the table under our not-yet-changed filename,
          // pop it.
          if (status == 0 && ACE_OS::strcmp (prefix, "") != 0)
            {
              char *trash = 0;
              this->pragma_prefixes_.pop (trash);
              delete [] trash;
            }
        }
    }
  else
    {
      this->pragma_prefixes_.push (tmp.rep ());
    }
}

/**
 Whether we should not mung idl element names that are
 C++ keywords e.g. delete, operator etc. with _cxx_ prefix.
 Should be true when being used by the IFR Service
 */
bool
IDL_GlobalData::preserve_cpp_keywords (void)
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
IDL_GlobalData::add_include_path (const char *s, bool is_system)
{
  Include_Path_Info info = { ACE::strnew (s), is_system };
  this->include_paths_.enqueue_tail (info);
}

void
IDL_GlobalData::add_rel_include_path (const char *s)
{
  this->rel_include_paths_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> const &
IDL_GlobalData::rel_include_paths (void) const
{
  return this->rel_include_paths_;
}

void
IDL_GlobalData::add_ciao_lem_file_names (const char *s)
{
  this->ciao_lem_file_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_lem_file_names (void)
{
  return this->ciao_lem_file_names_;
}

void
IDL_GlobalData::add_ciao_rti_ts_file_names (const char *s)
{
  this->ciao_rti_ts_file_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_rti_ts_file_names (void)
{
  return this->ciao_rti_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_spl_ts_file_names (const char *s)
{
  this->ciao_spl_ts_file_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_spl_ts_file_names (void)
{
  return this->ciao_spl_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_oci_ts_file_names (const char *s)
{
  this->ciao_oci_ts_file_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_oci_ts_file_names (void)
{
  return this->ciao_oci_ts_file_names_;
}

void
IDL_GlobalData::add_ciao_ami_iface_names (const char *s)
{
  this->ciao_ami_iface_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_ami_iface_names (void)
{
  return this->ciao_ami_iface_names_;
}

void
IDL_GlobalData::add_ciao_ami_recep_names (const char *s)
{
  this->ciao_ami_recep_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_ami_recep_names (void)
{
  return this->ciao_ami_recep_names_;
}

void
IDL_GlobalData::add_included_ami_recep_names (const char *s)
{
  this->included_ami_recep_names_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::included_ami_recep_names (void)
{
  return this->included_ami_recep_names_;
}

bool
IDL_GlobalData::included_ami_receps_done (void) const
{
  return this->included_ami_receps_done_;
}

void
IDL_GlobalData::included_ami_receps_done (bool val)
{
  this->included_ami_receps_done_ = val;
}

void
IDL_GlobalData::add_ciao_ami_idl_fnames (const char *s)
{
  this->ciao_ami_idl_fnames_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::ciao_ami_idl_fnames (void)
{
  return this->ciao_ami_idl_fnames_;
}

void
IDL_GlobalData::add_dds4ccm_impl_fnames (const char *s)
{
  this->dds4ccm_impl_fnames_.enqueue_tail (ACE::strnew (s));
}

ACE_Unbounded_Queue<char *> &
IDL_GlobalData::dds4ccm_impl_fnames (void)
{
  return this->dds4ccm_impl_fnames_;
}

ACE_Unbounded_Queue<AST_Interface *> &
IDL_GlobalData::mixed_parentage_interfaces (void)
{
  return this->mixed_parentage_interfaces_;
}

ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex> &
IDL_GlobalData::file_prefixes (void)
{
  return this->file_prefixes_;
}

bool
IDL_GlobalData::pass_orb_idl (void) const
{
  return this->pass_orb_idl_;
}

void
IDL_GlobalData::pass_orb_idl (bool val)
{
  this->pass_orb_idl_ = val;
}

bool
IDL_GlobalData::using_ifr_backend (void) const
{
  return this->using_ifr_backend_;
}

void
IDL_GlobalData::using_ifr_backend (bool val)
{
  this->using_ifr_backend_ = val;
}

bool
IDL_GlobalData::ignore_idl3 (void) const
{
  return this->ignore_idl3_;
}

void
IDL_GlobalData::ignore_idl3 (bool val)
{
  this->ignore_idl3_ = val;
}

bool
IDL_GlobalData::dcps_support_zero_copy_read (void) const
{
  return this->dcps_support_zero_copy_read_;
}

void
IDL_GlobalData::dcps_support_zero_copy_read (bool val)
{
  this->dcps_support_zero_copy_read_ = val;
}

bool
IDL_GlobalData::dcps_gen_zero_copy_read (void) const
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
IDL_GlobalData::check_gperf (void)
{
  // If absolute path is not specified yet, let us call just
  // "gperf". Hopefully PATH is set up correctly to locate the gperf.
  if (this->gperf_path_ == 0)
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
  if (ACE_OS::strcmp (this->gperf_path_, ACE_GPERF) != 0)
#else
  if (ACE_OS::strcmp (this->gperf_path_, "ace_gperf") != 0)
#endif /* ACE_GPERF */
    {
      // It is absolute path. Check the existence, permissions and
      // the modes.
      if (ACE_OS::access (this->gperf_path_,
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
                                this->gperf_path_);

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
IDL_GlobalData::fini (void)
{
  if (0 != this->pd_root)
    {
      this->pd_root->fini ();
      delete this->pd_root;
      this->pd_root = 0;
    }

  delete this->pd_err;
  this->pd_err = 0;
  delete this->pd_gen;
  this->pd_gen = 0;
  delete this->pd_indent;
  this->pd_indent = 0;
  delete [] this->pd_local_escapes;
  this->pd_local_escapes = 0;
  delete [] this->tao_root_;
  this->tao_root_ = 0;
  delete [] this->gperf_path_;
  this->gperf_path_ = 0;
  delete [] this->temp_dir_;
  this->temp_dir_ = 0;
  delete [] this->ident_string_;
  this->ident_string_ = 0;
  delete [] this->pd_include_file_names;
  this->pd_include_file_names = 0;

  Include_Path_Info *path_info = 0;

  for (Unbounded_Paths_Queue_Iterator qiter (this->include_paths_);
       qiter.done () == 0;
       qiter.advance ())
    {
      qiter.next (path_info);
      ACE::strdelete (path_info->path_);
    }

  char **path_tmp  = 0;

  for (ACE_Unbounded_Queue_Iterator<char *>riter (
            this->rel_include_paths_
          );
       riter.done () == 0;
       riter.advance ())
    {
      riter.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter1 (
         this->ciao_lem_file_names_);
       iter1.done () == 0;
       iter1.advance ())
    {
      iter1.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter2 (
         this->ciao_rti_ts_file_names_);
       iter2.done () == 0;
       iter2.advance ())
    {
      iter2.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter3 (
         this->ciao_spl_ts_file_names_);
       iter3.done () == 0;
       iter3.advance ())
    {
      iter3.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter4 (
         this->ciao_oci_ts_file_names_);
       iter4.done () == 0;
       iter4.advance ())
    {
      iter4.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter5 (
         this->ciao_ami_iface_names_);
       iter5.done () == 0;
       iter5.advance ())
    {
      iter5.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter6 (
         this->ciao_ami_recep_names_);
       iter6.done () == 0;
       iter6.advance ())
    {
      iter6.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter7 (
         this->included_ami_recep_names_);
       iter7.done () == 0;
       iter7.advance ())
    {
      iter7.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter8 (
         this->ciao_ami_idl_fnames_);
       iter8.done () == 0;
       iter8.advance ())
    {
      iter8.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  for (ACE_Unbounded_Queue_Iterator<char *>iter9 (
         this->dds4ccm_impl_fnames_);
       iter9.done () == 0;
       iter9.advance ())
    {
      iter9.next (path_tmp);
      ACE::strdelete (*path_tmp);
    }

  ACE_Hash_Map_Entry<char *, char *> *entry = 0;

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

  DCPS_Type_Info_Map::ENTRY *dcps_entry = 0;

  for (DCPS_Type_Info_Map::ITERATOR dcps_iter (
         this->dcps_type_info_map_);
       !dcps_iter.done ();
       dcps_iter.advance ())
    {
      dcps_iter.next (dcps_entry);

      dcps_entry->int_id_->name_->destroy ();
      delete dcps_entry->int_id_->name_;
      dcps_entry->int_id_->name_ = 0;

      delete dcps_entry->int_id_;
      dcps_entry->int_id_ = 0;

      delete [] dcps_entry->ext_id_;
      dcps_entry->ext_id_ = 0;
    }
}

ACE_Unbounded_Queue<AST_ValueType *> &
IDL_GlobalData::primary_keys (void)
{
  return this->primary_keys_;
}

void
IDL_GlobalData::check_primary_keys (void)
{
  AST_ValueType *holder = 0;

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

const char *
IDL_GlobalData::recursion_start (void) const
{
  return this->recursion_start_;
}

void
IDL_GlobalData::recursion_start (const char *val)
{
  ACE::strdelete (this->recursion_start_);

  /// Strip off any trailing slashes (not needed
  /// for further processing).
  ACE_CString tmp (val);
  ACE_CString::size_type len = tmp.length ();
  ACE_TCHAR c = tmp[len - 1];

  while (c == '\\' || c == '/')
    {
      tmp = tmp.substr (0, --len);
      c = tmp[len - 1];
    }

  this->recursion_start_ = ACE::strnew (tmp.c_str ());
}

bool
IDL_GlobalData::multi_file_input (void) const
{
  return this->multi_file_input_;
}

void
IDL_GlobalData::multi_file_input (bool val)
{
  this->multi_file_input_ = val;
}

const char *
IDL_GlobalData::big_file_name (void) const
{
  return this->big_file_name_;
}

FE_Utils::T_PARAMLIST_INFO const *
IDL_GlobalData::current_params (void) const
{
  return this->current_params_;
}

void
IDL_GlobalData::current_params (
FE_Utils::T_PARAMLIST_INFO *params)
{
  this->current_params_ = params;
}

FE_Utils::T_PARAMLIST_INFO const *
IDL_GlobalData::alias_params (void) const
{
  return this->alias_params_;
}

void
IDL_GlobalData::alias_params (
FE_Utils::T_PARAMLIST_INFO *params)
{
  this->alias_params_ = params;
}

void
IDL_GlobalData::add_dcps_data_type (const char* id)
{
  // Check if the type already exists.
  DCPS_Data_Type_Info* newinfo ;

  if (this->dcps_type_info_map_.find (id, newinfo) != 0)
    {
      // No existing entry, add one.

      char* foo_type;
      ACE_NEW (foo_type, char [ACE_OS::strlen (id) + 1]);
      ACE_OS::strcpy (foo_type, id);

      UTL_ScopedName* t1 =
        FE_Utils::string_to_scoped_name (foo_type);

      // Chained with null Identifier required!!
      UTL_ScopedName* target =
        new UTL_ScopedName (new Identifier (""), t1);

      newinfo = new DCPS_Data_Type_Info ();
      newinfo->name_ = target;

      // Add the newly formed entry to the map.
      if (this->dcps_type_info_map_.bind (foo_type, newinfo) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to insert type into ")
                      ACE_TEXT ("DCPS type container: %s.\n"),
                      id));
          return;
        }
    }
  else
    {
      ACE_ERROR ((LM_WARNING,
                  ACE_TEXT ("Duplicate DCPS type defined: %s.\n"),
                  id));
    }
}

bool
IDL_GlobalData::add_dcps_data_key (const char* id, const char* key)
{
  // Search the map for the type.
  DCPS_Data_Type_Info* newinfo = 0;

  if (this->dcps_type_info_map_.find (id, newinfo) == 0)
    {
       // Add the new key field to the type.
       newinfo->key_list_.enqueue_tail (ACE_TEXT_CHAR_TO_TCHAR (key));
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
  DCPS_Type_Info_Map::ENTRY* entry ;

  for (DCPS_Type_Info_Map::ITERATOR current (
         this->dcps_type_info_map_);
       current.next (entry);
       current.advance ())
    {
      // Look for our Identifier.
      if (0 == entry->int_id_->name_->compare (target))
        {
          // Found it!
          return entry->int_id_;
        }
    }

  // No joy.
  return 0;
}

AST_Module *
IDL_GlobalData::corba_module (void) const
{
  return this->corba_module_;
}

void
IDL_GlobalData::corba_module (AST_Module *m)
{
  this->corba_module_ = m;
}

IDL_GlobalData::Unbounded_Paths_Queue &
IDL_GlobalData::include_paths (void)
{
  return this->include_paths_;
}

void
IDL_GlobalData::anon_type_diagnostic (
  IDL_GlobalData::ANON_TYPE_DIAGNOSTIC val)
{
  this->anon_type_diagnostic_ = val;
}

bool
IDL_GlobalData::anon_error (void) const
{
  return (this->anon_type_diagnostic_ == ANON_TYPE_ERROR);
}

bool
IDL_GlobalData::anon_warning (void) const
{
  return (this->anon_type_diagnostic_ == ANON_TYPE_WARNING);
}

bool
IDL_GlobalData::anon_silent (void) const
{
  return (this->anon_type_diagnostic_ == ANON_TYPE_SILENT);
}

bool
IDL_GlobalData::in_typedef (void) const
{
  return this->in_typedef_;
}

void
IDL_GlobalData::in_typedef (bool val)
{
  this->in_typedef_ = val;
}
