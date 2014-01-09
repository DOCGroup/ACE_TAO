
//=============================================================================
/**
 *  @file    be_global.cpp
 *
 *  $Id$
 *
 *  Stores global data specific to the compiler back end.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#include "be_global.h"
#include "ast_generator.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "ace/OS_NS_stdio.h"

CIAO_IDL3_TO_XMI_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : ir_filter_ (false),
    filename_ (0),
    output_dir_ (0),
    dtd_ ("XMI.dtd"),
    local_only_(true)
{
}

BE_GlobalData::~BE_GlobalData (void)
{
}

bool
BE_GlobalData::ir_filter (void) const
{
  return this->ir_filter_;
}

void
BE_GlobalData::ir_filter (bool flag)
{
  this->ir_filter_ = flag;
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

const char*
BE_GlobalData::dtd (void) const
{
  return this->dtd_.c_str ();
}

void
BE_GlobalData::dtd (const char* s)
{
  this->dtd_ = s;
}

bool
BE_GlobalData::local_only (void) const
{
  return this->local_only_;
}

void
BE_GlobalData::local_only (bool flag)
{
  this->local_only_ = flag;
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      // Directory where the generated file is to
      // be kept. Default is the current directory from which
      // <tao_idl3_to_idl2> is called.
    case 'f':
      be_global->ir_filter (true);
      break;

    case 'o':
      switch (av[i][2])
        {
        case 'd':
          if (av[i][3] == '\0')
            {
              be_global->output_dir (av [i + 1]);
              i++;
            }
          else
            {
              be_global->output_dir (av[i] + 3);
            }
          break;

        case 'f':
          if (av[i][3] == '\0')
            {
              be_global->filename (av [i + 1]);
              i++;
            }
          else
            {
              be_global->filename (av[i] + 3);
            }
          break;
        default:
          idl_global->set_compile_flags (idl_global->compile_flags ()
                                         | IDL_CF_ONLY_USAGE);
        }
      break;

    case 'x':
      if (av[i][3] == '\0')
        {
          be_global->dtd (av [i + 1]);
          ++i;
        }
      else
        {
          be_global->dtd (av[i] + 3);
        }
      break;

    case 'n':
      be_global->local_only (false);
      break;

    default:
      ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                  av[i]));

      idl_global->set_compile_flags (idl_global->compile_flags ()
                                     | IDL_CF_ONLY_USAGE);
      break;
    }
}

void
BE_GlobalData::destroy (void)
{
}
