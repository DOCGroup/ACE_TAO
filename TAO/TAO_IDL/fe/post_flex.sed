s/ NULL/ 0/g
s/	/    /g
s/  *$//g
s/YY_BREAK break;/YY_BREAK ACE_NOTREACHED (break;)/g
s@#include <errno.h>@#include "ace/OS_NS_errno.h"@
s@#include <stdio.h>@#include "ace/OS_NS_stdio.h"@
s@#include <unistd.h>@#include "ace/os_include/os_ctype.h"@
s@c = getc@c = ACE_OS::getc@
s@199901L@199901L || defined (__HP_aCC)@
/#include <[seu]/d
