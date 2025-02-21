#include "be_global.h"
#include "be_util.h"
#include "ast_generator.h"
#include "global_extern.h"
#include "idl_defines.h"
#include "utl_err.h"
#include "utl_string.h"

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/ARGV.h"

BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData ()
{
}

void
BE_GlobalData::destroy ()
{
}

ACE_CString
BE_GlobalData::spawn_options ()
{
  return idl_global->idl_flags ();
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  ACE_ERROR ((LM_ERROR,
    ACE_TEXT ("IDL: I don't understand the '%C' option\n"),
    av[i]
  ));
  idl_global->parse_args_exit (1);
}
