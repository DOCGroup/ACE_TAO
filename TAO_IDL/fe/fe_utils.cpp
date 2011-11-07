// $Id$

#include "fe_utils.h"
#include "fe_private.h"
#include "fe_extern.h"

#include "ast_uses.h"
#include "ast_component.h"
#include "ast_generator.h"
#include "ast_root.h"
#include "ast_structure.h"
#include "ast_valuetype.h"
#include "ast_sequence.h"
#include "ast_template_module.h"

#include "global_extern.h"

#include "utl_scoped_name.h"
#include "utl_strlist.h"
#include "utl_namelist.h"
#include "utl_identifier.h"
#include "utl_string.h"

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

FE_Utils::T_Param_Info::T_Param_Info (void)
  : const_type_ (AST_Expression::EV_none),
    enum_const_type_decl_ (0)
{
}

bool
FE_Utils::duplicate_param_id (T_PARAMLIST_INFO *params)
{
  size_t cur_pos = 0UL;
  size_t size = params->size ();

  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*params);
       !i.done ();
       i.advance (), ++cur_pos)
    {
      FE_Utils::T_Param_Info *this_one = 0;
      FE_Utils::T_Param_Info *that_one = 0;

      i.next (this_one);

      for (size_t j = cur_pos + 1; j < size; ++j)
        {
          params->get (that_one, j);

          if (this_one->name_ == that_one->name_)
            {
              return true;
            }
        }
    }

  return false;
}

void
FE_Utils::T_ARGLIST::destroy (void)
{
  AST_Decl **d = 0;

  for (T_ARGLIST::ITERATOR i (this->begin ());
       !i.done ();
       i.advance ())
    {
      i.next (d);
      /// This is a problem - nodes that aren't NT_const have
      /// already been destroyed. Must find some other way of
      /// ferreting out the NT_const arglist nodes.
/*
      // These were created on the fly and not part of any scope
      // so we manage their lifetime here.
      AST_Decl *tmp = *d;
      if (tmp->node_type () == AST_Decl::NT_const)
        {
          tmp->destroy ();
          delete tmp;
          tmp = 0;
        } */
    }
}

/*
 * Convert a PredefinedType to an ExprType
 */
AST_Expression::ExprType
FE_Utils::PredefinedTypeToExprType (
  AST_PredefinedType::PredefinedType pt)
{
  switch (pt)
  {
    case AST_PredefinedType::PT_long:
      return AST_Expression::EV_long;
    case AST_PredefinedType::PT_ulong:
      return AST_Expression::EV_ulong;
    case AST_PredefinedType::PT_short:
      return AST_Expression::EV_short;
    case AST_PredefinedType::PT_ushort:
      return AST_Expression::EV_ushort;
    case AST_PredefinedType::PT_longlong:
      return AST_Expression::EV_longlong;
    case AST_PredefinedType::PT_ulonglong:
      return AST_Expression::EV_ulonglong;
    case AST_PredefinedType::PT_float:
      return AST_Expression::EV_float;
    case AST_PredefinedType::PT_double:
      return AST_Expression::EV_double;
    case AST_PredefinedType::PT_longdouble:
      return AST_Expression::EV_longdouble;
    case AST_PredefinedType::PT_char:
      return AST_Expression::EV_char;
    case AST_PredefinedType::PT_wchar:
      return AST_Expression::EV_wchar;
    case AST_PredefinedType::PT_octet:
      return AST_Expression::EV_octet;
    case AST_PredefinedType::PT_boolean:
      return AST_Expression::EV_bool;
    case AST_PredefinedType::PT_void:
      return AST_Expression::EV_void;
    default:
      return AST_Expression::EV_enum;
  }
}

AST_PredefinedType::PredefinedType
FE_Utils::ExprTypeToPredefinedType (AST_Expression::ExprType et)
{
  switch (et)
    {
    case AST_Expression::EV_short:
      return AST_PredefinedType::PT_short;
    case AST_Expression::EV_ushort:
      return AST_PredefinedType::PT_ushort;
    case AST_Expression::EV_long:
      return AST_PredefinedType::PT_long;
    case AST_Expression::EV_ulong:
      return AST_PredefinedType::PT_ulong;
    case AST_Expression::EV_longlong:
      return AST_PredefinedType::PT_longlong;
    case AST_Expression::EV_ulonglong:
      return AST_PredefinedType::PT_ulonglong;
    case AST_Expression::EV_float:
      return AST_PredefinedType::PT_float;
    case AST_Expression::EV_double:
      return AST_PredefinedType::PT_double;
    case AST_Expression::EV_longdouble:
      return AST_PredefinedType::PT_longdouble;
    case AST_Expression::EV_char:
      return AST_PredefinedType::PT_char;
    case AST_Expression::EV_wchar:
      return AST_PredefinedType::PT_wchar;
    case AST_Expression::EV_octet:
      return AST_PredefinedType::PT_octet;
    case AST_Expression::EV_bool:
      return AST_PredefinedType::PT_boolean;
    case AST_Expression::EV_any:
      return AST_PredefinedType::PT_any;
    case AST_Expression::EV_object:
      return AST_PredefinedType::PT_object;
    case AST_Expression::EV_void:
      return AST_PredefinedType::PT_void;
    case AST_Expression::EV_enum:
    case AST_Expression::EV_string:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_none:
    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("FE_Utils::ExprTypeToPredefinedType - ")
                  ACE_TEXT ("no pathing PredefinedType enum value\n")));

      /// A deliberately odd return value, so the caller can check
      /// it and take further action if necessary.
      return AST_PredefinedType::PT_pseudo;
    }
}

UTL_ScopedName *
FE_Utils::string_to_scoped_name (const char *s)
{
  UTL_ScopedName *retval = 0;
  ACE_CString str (s);
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  while (! str.empty ())
    {
      // Skip a leading double colon.
      if (str.find (':') == 0)
        {
          str = str.substr (2);
        }

      // Find the next double colon (if any) and get the next
      // name segment.
      ACE_CString::size_type pos = str.find (':');
      ACE_CString lname (str.substr (0, pos));

      // Construct a UTL_ScopedName segment.
      ACE_NEW_RETURN (id,
                      Identifier (lname.c_str ()),
                      0);

      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id, 0),
                      0);

      // Either make it the head of a new list or the tail of
      // an existing one.
      if (retval == 0)
        {
          retval = sn;
        }
      else
        {
          retval->nconc (sn);
        }

      // Update the working string.
      str = str.substr (pos);
    }

  return retval;
}

const char *
FE_Utils::stripped_preproc_include (const char *name)
{
  // Some preprocessors prepend "./" to filenames in the
  // working directory, some others prepend ".\". If either
  // of these are here, we want to strip them.
  if (name[0] == '.')
    {
      if (name[1] == '\\' || name[1] == '/')
        {
          return name + 2;
        }
    }

  return name;
}

void
FE_Utils::create_uses_multiple_stuff (AST_Component *c,
                                      AST_Uses *u,
                                      const char *prefix)
{
  ACE_CString struct_name (prefix);

  if (!struct_name.empty ())
    {
      struct_name += '_';
    }

  struct_name += u->local_name ()->get_string ();
  struct_name += "Connection";
  Identifier struct_id (struct_name.c_str ());
  UTL_ScopedName sn (&struct_id, 0);

  // In case this call comes from the backend. We
  // will pop the scope before returning.
  idl_global->scopes ().push (c);

  AST_Structure *connection =
    idl_global->gen ()->create_structure (&sn, 0, 0);

  struct_id.destroy ();

  /// If the field type is a param holder, we want
  /// to use the lookup to create a fresh one,
  /// since the field will own it and destroy it.
  UTL_ScopedName *fn = u->uses_type ()->name ();
  AST_Decl *d =
    idl_global->root ()->lookup_by_name (fn, true, false);
  AST_Type *ft = AST_Type::narrow_from_decl (d);

  Identifier object_id ("objref");
  UTL_ScopedName object_name (&object_id,
                              0);
  AST_Field *object_field =
    idl_global->gen ()->create_field (ft,
                                      &object_name,
                                      AST_Field::vis_NA);
  (void) DeclAsScope (connection)->fe_add_field (object_field);
  object_id.destroy ();

  Identifier local_id ("Cookie");
  UTL_ScopedName local_name (&local_id,
                             0);
  Identifier module_id ("Components");
  UTL_ScopedName scoped_name (&module_id,
                              &local_name);

  d = c->lookup_by_name (&scoped_name, true);
  local_id.destroy ();
  module_id.destroy ();

  if (d == 0)
    {
      // This would happen if we haven't included Components.idl.
      idl_global->err ()->lookup_error (&scoped_name);
      return;
    }

  AST_ValueType *cookie = AST_ValueType::narrow_from_decl (d);

  Identifier cookie_id ("ck");
  UTL_ScopedName cookie_name (&cookie_id,
                              0);
  AST_Field *cookie_field =
    idl_global->gen ()->create_field (cookie,
                                      &cookie_name,
                                      AST_Field::vis_NA);
  (void) DeclAsScope (connection)->fe_add_field (cookie_field);
  cookie_id.destroy ();

  (void) c->fe_add_structure (connection);

  ACE_CDR::ULong bound = 0;
  AST_Expression *bound_expr =
    idl_global->gen ()->create_expr (bound,
                                     AST_Expression::EV_ulong);
  AST_Sequence *sequence =
    idl_global->gen ()->create_sequence (bound_expr,
                                         connection,
                                         0,
                                         0,
                                         0);

  ACE_CString seq_string (struct_name);
  seq_string += 's';
  Identifier seq_id (seq_string.c_str ());
  UTL_ScopedName seq_name (&seq_id,
                           0);
  AST_Typedef *connections =
    idl_global->gen ()->create_typedef (sequence,
                                        &seq_name,
                                        0,
                                        0);
  seq_id.destroy ();

  (void) c->fe_add_typedef (connections);

  // In case this call comes from the backend.
  idl_global->scopes ().pop ();
}

void
FE_Utils::create_implied_ami_uses_stuff (void)
{
  if (idl_global->included_ami_receps_done ())
    {
      return;
    }

  for (ACE_Unbounded_Queue<char *>::CONST_ITERATOR i (
         idl_global->included_ami_recep_names ());
       ! i.done ();
       i.advance ())
    {
      char **item = 0;
      i.next (item);

      UTL_ScopedName *sn =
        FE_Utils::string_to_scoped_name (*item);

      AST_Decl *d =
        idl_global->root ()->lookup_by_name (sn, true);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (sn);

          sn->destroy ();
          delete sn;
          sn = 0;

          continue;
        }

      sn->destroy ();
      delete sn;
      sn = 0;

      AST_Uses *u = AST_Uses::narrow_from_decl (d);

      if (u == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("idl_global::create_")
                      ACE_TEXT ("implied_ami_uses_stuff - ")
                      ACE_TEXT ("narrow to receptacle ")
                      ACE_TEXT ("failed\n")));

          continue;
        }

      if (!u->is_multiple ())
        {
          continue;
        }

      AST_Component *c =
        AST_Component::narrow_from_scope (u->defined_in ());

      if (c == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("idl_global::create_")
                      ACE_TEXT ("implied_ami_uses_stuff - ")
                      ACE_TEXT ("receptacle not defined")
                      ACE_TEXT ("in a component\n")));

          continue;
        }

      FE_Utils::create_uses_multiple_stuff (c, u, "sendc");
    }

  idl_global->included_ami_receps_done (true);
}

int
FE_Utils::path_cmp (const char *s, const char *t)
{
#if defined (WIN32) || defined (ACE_OPENVMS)
  // Since Windows has case-insensitive filenames, the preprocessor,
  // when searching using a provided relative path, will sometimes
  // capitalize the first letter of the last segment of a path name
  // and make the rest lowercase, regardless of how it was actually
  // spelled when created. This 'feature' was preventing the
  // validation of included IDL files, necessary before generating
  // the corresponding C++ includes.
  return ACE_OS::strcasecmp (s, t);
#else
  return ACE_OS::strcmp (s, t);
#endif /* defined (WIN32) */
}

bool
FE_Utils::hasspace (const char *s)
{
  if (s)
    {
      const size_t length = ACE_OS::strlen (s);

      // Windows can't have a space as the first or last character
      // but a unix filename can. Need to check all characters.
      for (size_t i = 0u; i < length; ++i)
        {
          if (ACE_OS::ace_isspace (s [i]))
            {
              return true;
            }
        }
    }

  return false;
}

ACE_CString
FE_Utils::check_for_seq_of_param (FE_Utils::T_PARAMLIST_INFO *list)
{
  ACE_CString id, retval;
  const char *pattern = "sequence<";
  size_t len = ACE_OS::strlen (pattern);
  size_t index = 0;


  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*list);
       !i.done ();
       i.advance (), ++index)
    {
      FE_Utils::T_Param_Info *param = 0;
      i.next (param);

      if (param->name_.find (pattern) == 0)
        {
          // Get the substring of what's between the brackets.
          // It will have to match a previous param in the list.
          id = param->name_.substr (len,
                                    param->name_.length () - (len + 1));

          if (!FE_Utils::check_one_seq_of_param (list, id, index))
            {
              retval = id;
              break;
            }
        }
    }

  return retval;
}

FILE *
FE_Utils::open_included_file (char const * filename,
                              char const *& directory)
{
  FILE * f = 0;
  ACE_CString const the_file (ACE_CString ('/')
                              + ACE_CString (filename));

  for (IDL_GlobalData::Unbounded_Paths_Queue_Iterator i (
         idl_global->include_paths ());
       !i.done () && f == 0;
       i.advance ())
    {
      IDL_GlobalData::Include_Path_Info *path_info = 0;
      (void) i.next (path_info);

      if (path_info->path_ != 0)
        {
          ACE_CString const complete_filename (ACE_CString (path_info->path_)
                                               + the_file);

          f = ACE_OS::fopen (complete_filename.c_str (), "r");

          if (f != 0)
            directory = path_info->path_;
        }
    }

  return f;
}

bool
FE_Utils::is_include_file_found (ACE_CString & inc_file,
                                 UTL_String * idl_file_name)
{
  char abspath[MAXPATHLEN] = "";
  char *full_path = 0;

  // If the include path has literal "s (because of an include
  // of a Windows path with spaces), we must remove them here.
  const char *tmp_inc_file = inc_file.c_str ();

  if (tmp_inc_file
      && FE_Utils::hasspace (tmp_inc_file)
      && tmp_inc_file[0] == '\"')
    {
      inc_file =
        inc_file.substr (1, inc_file.length () - 2);
    }

  inc_file += ACE_DIRECTORY_SEPARATOR_STR_A;
  inc_file += idl_file_name->get_string ();
  full_path =
    ACE_OS::realpath (inc_file.c_str (), abspath);

  if (full_path != 0)
    {
      FILE *test = ACE_OS::fopen (abspath, "r");

      if (test == 0)
        {
          return false;
        }
      else
        {
          // Overwrite inc_file with abspath since the later
          // is normalized to the native OS representation.
          inc_file = abspath;
          ACE_OS::fclose (test);
          return true;
        }
    }

  return false;
}

/// Validate the included idl files, some files might have been
/// ignored by the preprocessor.
void
FE_Utils::validate_included_idl_files (void)
{
  // Flag to make sure we don't repeat things.
  static bool already_done = false;

  if (already_done)
    {
      return;
    }

  already_done = true;

  // New number of included_idl_files.
  size_t newj = 0;

  size_t n_pre_preproc_includes =
    idl_global->n_included_idl_files ();

  char **pre_preproc_includes =
    idl_global->included_idl_files ();

  size_t n_post_preproc_includes =
    idl_global->n_include_file_names ();

  UTL_String **post_preproc_includes =
    idl_global->include_file_names ();

  char pre_abspath [MAXPATHLEN] = "";
  char post_abspath [MAXPATHLEN] = "";
  IDL_GlobalData::Include_Path_Info *path_info = 0;
  char *post_tmp = 0;
  char *full_path = 0;

  // We are going to assemble a list of the include files found in
  // the top level file in the order that they are found in the pre-
  // processor output here
  // @see bug TAO#711 / Bugzilla #3513 for more
  char** ordered_include_files =
    new char* [n_pre_preproc_includes];

  for (size_t i = 0u; i < n_post_preproc_includes; ++i)
    {
      post_tmp = post_preproc_includes [i]->get_string ();
      full_path = ACE_OS::realpath (post_tmp, post_abspath);

      if (full_path)
        {
          for (size_t j = 0u; j < n_pre_preproc_includes; ++j)
            {
              // Check this name with the name that we got from the
              // preprocessor.

              bool valid_file = false;
              full_path = ACE_OS::realpath (pre_preproc_includes [j],
                                            pre_abspath);
              if (full_path &&
                  FE_Utils::path_cmp (pre_abspath, post_abspath) == 0 &&
                  ACE_OS::access (post_abspath, R_OK) == 0)
                {
                  // This file name is valid.
                  valid_file = true;
                }
              else for (IDL_GlobalData::Unbounded_Paths_Queue_Iterator iter (
                          idl_global->include_paths ());
                        !iter.done ();
                        iter.advance ())
                {
                  iter.next (path_info);
                  ACE_CString pre_partial (path_info->path_);

                  // If the include path has literal "s (because of an include
                  // of a Windows path with spaces), we must remove them here.
                  if (pre_partial.c_str () &&
                      2u < pre_partial.length () &&
                      '"' == pre_partial [0] &&
                      '"' == pre_partial [pre_partial.length () - 1u])
                    {
                      pre_partial =
                        pre_partial.substr (1, pre_partial.length () - 2u);
                    }
                  pre_partial += ACE_DIRECTORY_SEPARATOR_STR_A;
                  pre_partial += pre_preproc_includes [j];
                  full_path =
                    ACE_OS::realpath (pre_partial.c_str (),
                                      pre_abspath);

                  if (full_path &&
                      FE_Utils::path_cmp (pre_abspath, post_abspath) == 0 &&
                      ACE_OS::access (post_abspath, R_OK) == 0)
                    {
                      // This file name is valid.
                      valid_file = true;
                      break;
                    }
                }

              if (valid_file)
                {
                  // File is valid.
                  // Move to the new index position.
                  // ... in the ordered list
                  ordered_include_files [newj] =
                        pre_preproc_includes [j];

                  // Increment the new index.
                  ++newj;

                  for (size_t k = j + 1; k < n_pre_preproc_includes; ++k)
                    {
                      // Shift remaining entries down
                      pre_preproc_includes [k-1] = pre_preproc_includes [k];
                    }

                  // Reduce length and zero the discarded element
                  pre_preproc_includes [--n_pre_preproc_includes] = 0;

                  // Break out to next entry in pre-processor
                  // output
                  break;
                }
            }

          if (n_pre_preproc_includes == 0)
            {
              break;
            }
        }
    }

  // Tidy up not required includes
  for (size_t l = 0u; l < n_pre_preproc_includes; ++l)
    {
      delete [] pre_preproc_includes [l];
      pre_preproc_includes [l] = 0;
    }

  // Copy list back
  for (size_t m = 0u; m < newj; ++m)
    {
      pre_preproc_includes [m] = ordered_include_files [m];
    }

  delete [] ordered_include_files;

  // Now adjust the count on the included_idl_files.
  idl_global->n_included_idl_files (newj);
}

bool
FE_Utils::validate_orb_include (UTL_String * idl_file_name)
{
  char foundpath[MAXPATHLEN] = "";

  {
    // Check in the current folder.
    char abspath[MAXPATHLEN] = "";
    ACE_CString cwd_path = ACE_OS::getcwd (abspath,
                                           sizeof (abspath) / sizeof (char));
    if (FE_Utils::is_include_file_found (cwd_path, idl_file_name))
      {
        ACE_OS::strcpy (foundpath, cwd_path.c_str ());
      }
  }

  for (IDL_GlobalData::Unbounded_Paths_Queue_Iterator iter (
         idl_global->include_paths ());
       !iter.done ();
       iter.advance ())
    {
      IDL_GlobalData::Include_Path_Info *path_info = 0;
      iter.next (path_info);

      ACE_CString partial = path_info->path_;

      // We don't need to check anything if the file is already
      // found and the folder where are currently checking is
      // provided by user.
      if (foundpath[0] != 0 && !path_info->is_system_)
        {
          continue;
        }

      if (FE_Utils::is_include_file_found (partial, idl_file_name))
        {
          if (path_info->is_system_)
            {
              if (foundpath[0] == 0 ||
                  ACE_OS::strcmp (foundpath, partial.c_str ()) == 0)
                {
                  return true;
                }
            }
          else
            {
              // We can fill in foundpath here since we are sure
              // that it was not set before. Check above ensures that.
              ACE_OS::strcpy (foundpath, partial.c_str ());
              continue;
            }
        }
    }

  return false;
}

void
FE_Utils::original_local_name (Identifier *local_name)
{
  const char *lname = local_name->get_string ();

  // Remove _cxx_ if:
  // 1. it occurs and
  // 2. it occurs at the beginning of the string and
  // 3. the rest of the string is a C++ keyword
  if (ACE_OS::strstr (lname, "_cxx_") == lname)
    {
      TAO_IDL_CPP_Keyword_Table cpp_key_tbl;

      unsigned int len =
        static_cast<unsigned int> (ACE_OS::strlen (lname + 5));

      const TAO_IDL_CPP_Keyword_Entry *entry =
        cpp_key_tbl.lookup (lname + 5, len);

      if (entry != 0)
        {
          ACE_CString tmp (lname + 5);
          local_name->replace_string (tmp.c_str ());
        }
    }
}

bool
FE_Utils::can_be_redefined (AST_Decl *prev_decl,
                            AST_Decl *curr_decl)
{
  AST_Decl::NodeType pnt = prev_decl->node_type ();
  AST_Decl::NodeType cnt = curr_decl->node_type ();

  switch (cnt)
  {
    /// For these, any non-zero previous decl
    /// is an error.
    case AST_Decl::NT_attr:
    case AST_Decl::NT_op:
    case AST_Decl::NT_ext_port:
    case AST_Decl::NT_mirror_port:
    case AST_Decl::NT_provides:
    case AST_Decl::NT_publishes:
    case AST_Decl::NT_consumes:
    case AST_Decl::NT_uses:
      return false;
    default:
      break;
  }

  UTL_Scope *prev_scope = prev_decl->defined_in ();
  UTL_Scope *curr_scope = curr_decl->defined_in ();
  AST_Structure *s = 0;
  AST_StructureFwd *s_fwd = 0;
  AST_Interface *i = 0;
  AST_InterfaceFwd *i_fwd = 0;
  AST_Template_Module *ptm = 0;
  AST_Template_Module *ctm = 0;

  bool nt_eq = (pnt == cnt);
  bool s_eq = (prev_scope == curr_scope);

  switch (pnt)
  {
    case AST_Decl::NT_module:
      /// Need to check that both are modules.
      if (cnt != AST_Decl::NT_module)
        {
          return false;
        }

      /// Neither can be a template module.
      ptm = AST_Template_Module::narrow_from_decl (prev_decl);
      ctm = AST_Template_Module::narrow_from_decl (curr_decl);
      return (ptm == 0 && ctm == 0);
    /// For the *_fwd types, if scopes aren't related, it's ok.
    /// If they are related, then we need another fwd or a full decl.
    case AST_Decl::NT_component_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_component));
    case AST_Decl::NT_eventtype_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_eventtype));
    case AST_Decl::NT_interface_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_interface));
    case AST_Decl::NT_struct_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_struct));
    case AST_Decl::NT_union_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_union));
    case AST_Decl::NT_valuetype_fwd:
      return (!s_eq || (nt_eq || cnt == AST_Decl::NT_valuetype));
    /// If scopes aren't related, it's ok. If they are, check
    /// if the previous is a dummy for a fwd decl. Even a redef
    /// in a derived interface type is ok.
    case AST_Decl::NT_struct:
    case AST_Decl::NT_union:
      s = AST_Structure::narrow_from_decl (prev_decl);
      s_fwd = (s == 0 ? 0 : s->fwd_decl ());
      return (!s_eq || s_fwd != 0);
    case AST_Decl::NT_interface:
    case AST_Decl::NT_component:
    case AST_Decl::NT_eventtype:
    case AST_Decl::NT_valuetype:
    case AST_Decl::NT_connector:
      i = AST_Interface::narrow_from_decl (prev_decl);
      i_fwd = (i == 0 ? 0 : i->fwd_decl ());
      return (!s_eq || i_fwd != 0);
    /// For factories, exceptions, constants and types, even a
    /// redef in a derived interface type is ok. Checking for
    /// unequal scopes covers boxed valuetypes as well.
    case AST_Decl::NT_valuebox:
    case AST_Decl::NT_except:
    case AST_Decl::NT_typedef:
    case AST_Decl::NT_const:
    case AST_Decl::NT_factory:
    case AST_Decl::NT_type:
    {
      return !s_eq;
    }
    /// What's left are the case from the previous switch statement.
    /// Same rule applies - no overrides or redefs, ever.
    default:
      return false;
  }
}

void
FE_Utils::tmpl_mod_ref_check (AST_Decl *context,
                              AST_Decl *ref)
{
  if (ref == 0
      || ref->node_type () == AST_Decl::NT_param_holder
      || idl_global->in_tmpl_mod_alias ())
    {
      return;
    }

  bool ok = true;

  if (ref->in_tmpl_mod_not_aliased ())
    {
      if (! context->in_tmpl_mod_not_aliased ())
        {
          ok = false;
        }
      else
        {
          AST_Template_Module *context_tm =
            FE_Utils::get_tm_container (context);
          AST_Template_Module *ref_tm =
            FE_Utils::get_tm_container (ref);

          if (context_tm != ref_tm)
            {
              ok = false;
            }
        }
    }

  if (! ok)
    {
      idl_global->err ()->template_scope_ref_not_aliased (ref);
      throw Bailout ();
    }
}

bool
FE_Utils::check_one_seq_of_param (FE_Utils::T_PARAMLIST_INFO *list,
                                  ACE_CString &param_id,
                                  size_t index)
{
  size_t local_index = 0;

  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*list);
       !i.done ();
       i.advance (), ++local_index)
    {
      if (local_index == index)
        {
          break;
        }

      FE_Utils::T_Param_Info *info = 0;
      i.next (info);

      if (info->name_ == param_id)
        {
          return true;
        }
    }

  return false;
}

AST_Template_Module *
FE_Utils::get_tm_container (AST_Decl *contained)
{
  AST_Decl *d = contained;

  while (d != 0)
    {
      AST_Template_Module *tm =
        AST_Template_Module::narrow_from_decl (d);

      if (tm != 0)
        {
          return tm;
        }

      d = ScopeAsDecl (d->defined_in ());
    }

  return 0;
}
