#include "global_extern.h"
#include "be_extern.h"

void
BE_cleanup ()
{
  idl_global->destroy();
}

void
BE_abort ()
{
  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Fatal Error - Aborting\n")));
  BE_cleanup ();
  ACE_OS::exit (1);
}

void
BE_produce()
{
}
