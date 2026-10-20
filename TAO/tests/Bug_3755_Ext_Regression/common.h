#include "ace/ace_wchar.h"

#define FIRST_ORB_NAME "first_orb"
#define SECOND_ORB_NAME "second_orb"

using orbs_check_callback = int (*) ();

int do_main (int argc, ACE_TCHAR *argv[], orbs_check_callback check);
