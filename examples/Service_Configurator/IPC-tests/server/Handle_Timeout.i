/* -*- C++ -*- */
// $Id$

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"

ACE_INLINE
Handle_Timeout::Handle_Timeout (void): count (0)
{
}

ACE_INLINE int
Handle_Timeout::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s", "# tests timeout facility\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_Timeout::init (int argc, char *argv[])
{
  ACE_Time_Value delta (10);
  ACE_Time_Value interval (1);
  ACE_Get_Opt    get_opt (argc, argv, "a:d:i:", 0);
  int        arg = 0;

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'd':
         delta.sec (ACE_OS::atoi (get_opt.opt_arg ()));
         break;
       case 'i':
         interval.sec (ACE_OS::atoi (get_opt.opt_arg ()));
         break;
       case 'a':
         arg = atoi (get_opt.opt_arg ());
         break;
       default:
         break;
       }

  if (ACE_Reactor::instance ()->schedule_timer (this,
                                                ACE_reinterpret_cast (void *, 
                                                                      arg),
                                                      delta,
                                                      interval) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_Timeout::fini (void)
{
  return 0;
}

ACE_INLINE ACE_HANDLE
Handle_Timeout::get_handle (void) const
{
  return ACE_INVALID_HANDLE;
}

ACE_INLINE int
Handle_Timeout::handle_timeout (const ACE_Time_Value &tv,
                                const void *arg)
{
  if (this->count++ >= 10)
    return -1; // Automatically cancel periodic timer...

  // Cast arg to a long, first, because a pointer is the same
  // size as a long on all current ACE platforms.
  ACE_DEBUG ((LM_INFO,
              "time for this(%u) expired at (%d, %d) with arg = %d\n",
             this, tv.sec (), tv.usec (), (int) (long) arg));
  return 0;
}
