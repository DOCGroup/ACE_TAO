// $Id$

// ============================================================================
//
//
// = LIBRARY
//    TAO_IDL3_TO_IDL2L_BE_DLL
//
// = FILENAME
//    be_global.cpp
//
// = DESCRIPTION
//    Stores global data specific to the compiler back end.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "be_global.h"
#include "be_sunsoft.h"
#include "ast_generator.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "utl_string.h"
#include "idl3_to_idl2_visitor.h"

#include "ace/OS_NS_stdio.h"

TAO_IDL3_TO_IDL2_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : gen_copyright_ (true),
    filename_ (0),
    output_dir_ (0),
    encapsulate_idl2_ (false)
{
}

BE_GlobalData::~BE_GlobalData (void)
{
}

const char *
BE_GlobalData::filename (void) const
{
  return this->filename_;
}

void
BE_GlobalData::filename (char *fname)
{
  this->filename_ = fname;
}

const char*
BE_GlobalData::output_dir (void) const
{
  return this->output_dir_;
}

void
BE_GlobalData::output_dir (const char* s)
{
  delete [] this->output_dir_;
  this->output_dir_ = ACE::strnew (s);
}

bool
BE_GlobalData::encapsulate_idl2 (void) const
{
  return this->encapsulate_idl2_;
}

ACE_CString &
BE_GlobalData::excluded_filenames (void)
{
  return this->excluded_filenames_;
}

bool
BE_GlobalData::gen_copyright (void) const
{
  return this->gen_copyright_;
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      // Directory where the generated file is to
      // be kept. Default is the current directory from which
      // <tao_idl3_to_idl2> is called.
      case 'o':
        if (av[i][2] == '\0')
          {
            this->output_dir (av [i + 1]);
            i++;
          }
        else
          {
            this->output_dir (av[i] + 2);
          }
        break;
      case 'x':
        if (this->excluded_filenames_ != "")
          {
            this->excluded_filenames_ += " ";
          }

        if (av[i][2] == '\0')
          {
            this->excluded_filenames_ += av [i + 1];
            i++;
          }
        else
          {
            this->excluded_filenames_ += av[i] + 2;
          }
        break;
      case 'e':
        this->encapsulate_idl2_ = true;
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

// Prepare an argument for a BE.
void
BE_GlobalData::prep_be_arg (char *)
{
}

void
BE_GlobalData::arg_post_proc (void)
{
}

void
BE_GlobalData::usage (void) const
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <dir>\t\tOutput directory for the generated file.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -x <filename>\t\tIncluded IDL file that wasn't processed")
      ACE_TEXT (" by this tool (regenerate include directive unchanged)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -e\t\t\tGenerate just an include of original IDL file")
      ACE_TEXT (" if no IDL3 declarations are found\n")
    ));
}

AST_Generator *
BE_GlobalData::generator_init (void)
{
  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  AST_Generator,
                  0);
  return gen;
}

int
BE_GlobalData::outfile_init (TAO_OutStream *& os,
                             const char *file_prefix,
                             const char *file_suffix,
                             const char *guard_prefix,
                             const char *guard_suffix)
{
  ACE_NEW_RETURN (os,
                  TAO_SunSoft_OutStream,
                  -1);

  ACE_CString fn (idl_global->stripped_filename ()->get_string ());
  fn = fn.substr (0, fn.rfind ('.'));
  fn += file_suffix;

  const char *path = be_global->output_dir ();
  ACE_CString target_name;

  if (path != 0)
    {
      target_name = path;
      target_name += "/";
    }

  target_name += file_prefix;
  target_name += fn;

  if (os->open (target_name.c_str ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failed to open file %s for writing.\n",
                         target_name.c_str ()),
                        -1);
    }

  *os << be_nl;

  os->gen_ifndef_string (fn.c_str (), guard_prefix, guard_suffix);

  return 0;
}

void
BE_GlobalData::destroy (void)
{
}
