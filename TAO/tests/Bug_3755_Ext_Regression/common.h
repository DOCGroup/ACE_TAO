#include "ace/ace_wchar.h"

using orbs_check_callback = int (*) ();

int do_main (int argc, ACE_TCHAR *argv[], orbs_check_callback check);
