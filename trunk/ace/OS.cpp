// $Id$

#define ACE_BUILD_DLL
#include "ace/OS.h"
#include "ace/Sched_Params.h"

// Perhaps we should *always* include ace/OS.i in order to make sure
// we can always link against the OS symbols?
#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/Synch_T.h"
#include "ace/streams.h"

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
# include "ace/Containers.h"
# if defined (ACE_WIN32)
#   include "ace/Thread_Manager.h"
# endif /* ACE_WIN32 */
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */

ACE_RCSID(ace, OS, "$Id$")

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
# include "ace/Object_Manager.h"

# if defined (ACE_HAS_WINCE)
const wchar_t *ACE_OS::day_of_week_name[] = {ACE_TEXT ("Sun"), ACE_TEXT ("Mon"),
                                             ACE_TEXT ("Tue"), ACE_TEXT ("Wed"),
                                             ACE_TEXT ("Thr"), ACE_TEXT ("Fri"),
                                             ACE_TEXT ("Sat")};
const wchar_t *ACE_OS::month_name[] = {ACE_TEXT ("Jan"), ACE_TEXT ("Feb"),
                                       ACE_TEXT ("Mar"), ACE_TEXT ("Apr"),
                                       ACE_TEXT ("May"), ACE_TEXT ("Jun"),
                                       ACE_TEXT ("Jul"), ACE_TEXT ("Aug"),
                                       ACE_TEXT ("Sep"), ACE_TEXT ("Oct"),
                                       ACE_TEXT ("Nov"), ACE_TEXT ("Dec") };

static const ASYS_TCHAR *ACE_OS_CTIME_R_FMTSTR = ACE_TEXT ("%3s %3s %02d %02d:%02d:%02d %04d\n");
# endif /* ACE_HAS_WINCE */

# if defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
int
ACE_OS::netdb_acquire (void)
{
  ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
  return ace_os_monitor_lock->acquire ();
}

int
ACE_OS::netdb_release (void)
{
  ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
  return ace_os_monitor_lock->release ();
}
# endif /* defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS) */
#endif /* defined (ACE_MT_SAFE) */

ACE_EXIT_HOOK ACE_OS::exit_hook_ = 0;

// Static constant representing `zero-time'.
const ACE_Time_Value ACE_Time_Value::zero;

// Static constant representing `maximum-time'.
const ACE_Time_Value ACE_Time_Value::max_time (LONG_MAX,
                                               ACE_ONE_SECOND_IN_USECS - 1);

ACE_ALLOC_HOOK_DEFINE(ACE_Time_Value)

// Initializes the ACE_Time_Value object from a timeval.

#if defined (ACE_WIN32)
//  Initializes the ACE_Time_Value object from a Win32 FILETIME

// Static constant to remove time skew between FILETIME and POSIX
// time.
//
// In the beginning (Jan. 1, 1601), there was no time and no computer.
// And Bill said: "Let there be time," and there was time....
const DWORDLONG ACE_Time_Value::FILETIME_to_timval_skew = 0x19db1ded53e8000i64;

ACE_Time_Value::ACE_Time_Value (const FILETIME &file_time)
{
  // ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (file_time);
}

void ACE_Time_Value::set (const FILETIME &file_time)
{
  //  Initializes the ACE_Time_Value object from a Win32 FILETIME
  ULARGE_INTEGER _100ns = {file_time.dwLowDateTime,
                           file_time.dwHighDateTime};
  _100ns.QuadPart -= ACE_Time_Value::FILETIME_to_timval_skew;

  // Convert 100ns units to seconds;
  this->tv_.tv_sec = long (_100ns.QuadPart / (10000 * 1000));
  // Convert remainder to microseconds;
  this->tv_.tv_usec = long ((_100ns.QuadPart % (10000 * 1000)) / 10);
}

// Returns the value of the object as a Win32 FILETIME.

ACE_Time_Value::operator FILETIME () const
{
  // ACE_TRACE ("ACE_Time_Value::operator FILETIME");
  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (((DWORDLONG) this->tv_.tv_sec * (10000 * 1000) +
                      this->tv_.tv_usec * 10) +
                     ACE_Time_Value::FILETIME_to_timval_skew);
  FILETIME file_time;

# if defined(__BORLANDC__)
#   define LOWPART(x) x.u.LowPart
#   define HIGHPART(x) x.u.HighPart
# else
#   define LOWPART(x) x.LowPart
#   define HIGHPART(x) x.HighPart
# endif /* defined(__BORLANDC__) */

  file_time.dwLowDateTime = LOWPART(_100ns);
  file_time.dwHighDateTime = HIGHPART(_100ns);

  return file_time;
}

#endif /* ACE_WIN32 */

ACE_Cleanup_Info::ACE_Cleanup_Info (void)
  : object_ (0),
    cleanup_hook_ (0),
    param_ (0)
{
}

int
ACE_Cleanup_Info::operator== (const ACE_Cleanup_Info &o) const
{
  return o.object_ == this->object_
    && o.cleanup_hook_ == this->cleanup_hook_
    && o.param_ == this->param_;
}

int
ACE_Cleanup_Info::operator!= (const ACE_Cleanup_Info &o) const
{
  return !(*this == o);
}

void
ACE_Time_Value::dump (void) const
{
  // ACE_TRACE ("ACE_Time_Value::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntv_sec_ = %d"), this->tv_.tv_sec));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntv_usec_ = %d\n"), this->tv_.tv_usec));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Time_Value::normalize (void)
{
  // ACE_TRACE ("ACE_Time_Value::normalize");
  // New code from Hans Rohnert...

  if (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS)
    {
      do
        {
          this->tv_.tv_sec++;
          this->tv_.tv_usec -= ACE_ONE_SECOND_IN_USECS;
        }
      while (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS);
    }
  else if (this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS)
    {
      do
        {
          this->tv_.tv_sec--;
          this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS;
        }
      while (this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS);
    }

  if (this->tv_.tv_sec >= 1 && this->tv_.tv_usec < 0)
    {
      this->tv_.tv_sec--;
      this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS;
    }
  else if (this->tv_.tv_sec < 0 && this->tv_.tv_usec > 0)
    {
      this->tv_.tv_sec++;
      this->tv_.tv_usec -= ACE_ONE_SECOND_IN_USECS;
    }
}

int
ACE_Countdown_Time::start (void)
{
  if (this->max_wait_time_ != 0)
    {
      this->start_time_ = ACE_OS::gettimeofday ();
      this->stopped_ = 0;
    }
  return 0;
}

int
ACE_Countdown_Time::update (void)
{
  return (this->stop () == 0) && this->start ();
}

int
ACE_Countdown_Time::stop (void)
{
  if (this->max_wait_time_ != 0 && this->stopped_ == 0)
    {
      ACE_Time_Value elapsed_time =
        ACE_OS::gettimeofday () - this->start_time_;

      if (*this->max_wait_time_ > elapsed_time)
        *this->max_wait_time_ -= elapsed_time;
      else
        {
          // Used all of timeout.
          *this->max_wait_time_ = ACE_Time_Value::zero;
          // errno = ETIME;
        }
      this->stopped_ = 1;
    }
  return 0;
}

ACE_Countdown_Time::ACE_Countdown_Time (ACE_Time_Value *max_wait_time)
  : max_wait_time_ (max_wait_time),
    stopped_ (0)
{
  this->start ();
}

ACE_Countdown_Time::~ACE_Countdown_Time (void)
{
  this->stop ();
}

#if defined (ACE_HAS_POWERPC_TIMER) && defined (ghs)
void
ACE_OS::readPPCTimeBase (u_long &most, u_long &least)
{
  // ACE_TRACE ("ACE_OS::readPPCTimeBase");

  // This function can't be inline because it depends on the arguments
  // being in particular registers (r3 and r4), in conformance with the
  // EABI standard.  It would be nice if we knew how to put the variable
  // names directly into the assembler instructions . . .
  asm("aclock:");
  asm("mftb  r5,TBU");
  asm("mftb  r6,TBL");
  asm("mftb  r7,TBU");
  asm("cmpw  r5,r7");
  asm("bne   aclock");

  asm("stw r5, 0(r3)");
  asm("stw r6, 0(r4)");
}
#elif defined (ACE_HAS_POWERPC_TIMER) && defined (__GNUG__)
void
ACE_OS::readPPCTimeBase (u_long &most, u_long &least)
{
  // ACE_TRACE ("ACE_OS::readPPCTimeBase");

  // This function can't be inline because it defines a symbol,
  // aclock.  If there are multiple calls to the function in a
  // compilation unit, then that symbol would be multiply defined if
  // the function was inline.
  asm volatile ("aclock:\n"
                "mftbu 5\n"     /* upper time base register */
                "mftb 6\n"      /* lower time base register */
                "mftbu 7\n"     /* upper time base register */
                "cmpw 5,7\n" /* check for rollover of upper */
                "bne aclock\n"
                "stw 5,%0\n"                        /* most */
                "stw 6,%1"                         /* least */
                : "=m" (most), "=m" (least)      /* outputs */
                :                              /* no inputs */
                : "5", "6", "7", "memory"    /* constraints */);
}
#endif /* ACE_HAS_POWERPC_TIMER  &&  (ghs or __GNUG__) */

#if defined (ACE_WIN32) || defined (VXWORKS) || defined (CHORUS) || defined (ACE_PSOS)
// Don't inline on those platforms because this function contains
// string literals, and some compilers, e.g., g++, don't handle those
// efficiently in unused inline functions.
int
ACE_OS::uname (struct utsname *name)
{
  // ACE_TRACE ("ACE_OS::uname");
# if defined (ACE_WIN32)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, ACE_TEXT ("Win32"));

  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx (&vinfo);

  SYSTEM_INFO sinfo;
  ::GetSystemInfo(&sinfo);

  ACE_OS::strcpy (name->sysname, ACE_TEXT ("Win32"));

  if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    // Get information from the two structures
    ACE_OS::sprintf (name->release,
#   if defined (ACE_HAS_WINCE)
                     ACE_TEXT ("Windows CE %d.%d"),
#   else
                     ACE_TEXT ("Windows NT %d.%d"),
#   endif /* ACE_HAS_WINCE */
                     vinfo.dwMajorVersion,
                     vinfo.dwMinorVersion);
    ACE_OS::sprintf (name->version,
                     ACE_TEXT ("Build %d %s"),
                     vinfo.dwBuildNumber,
                     vinfo.szCSDVersion);

    // We have to make sure that the size of (processor + subtype) is
    // not greater than the size of name->machine.  So we give half
    // the space to the processor and half the space to subtype.  The
    // -1 is necessary for because of the space between processor and
    // subtype in the machine name.
    const int bufsize = ((sizeof (name->machine) / sizeof (TCHAR)) / 2) - 1;
    TCHAR processor[bufsize] = ACE_TEXT ("Unknown");
    TCHAR subtype[bufsize] = ACE_TEXT ("Unknown");

    WORD arch;
#if (! defined(__BORLANDC__))
    arch = sinfo.wProcessorArchitecture;
#else
    // Hack alarm:
    // Some Borland compilers ship with an incorrect definition
    // of SYSTEM_INFO that makes it impossible to access the
    // wProcessArchitecture field.
    // This awful hack will work around that, but no guarantees!
    switch(sinfo.dwProcessorType)
    {
      case PROCESSOR_INTEL_386:
      case PROCESSOR_INTEL_486:
      case PROCESSOR_INTEL_PENTIUM:
        arch = PROCESSOR_ARCHITECTURE_INTEL;
        break;

      case PROCESSOR_MIPS_R4000:
        arch = PROCESSOR_ARCHITECTURE_MIPS;
        break;

      case PROCESSOR_ALPHA_21064:
        arch = PROCESSOR_ARCHITECTURE_ALPHA;
        break;

      default:
        arch = PROCESSOR_ARCHITECTURE_UNKNOWN;
    }
#endif /* (! defined(__BORLANDC__)) */

    switch (arch)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
      ACE_OS::strcpy (processor, ACE_TEXT ("Intel"));
      if (sinfo.wProcessorLevel == 3)
        ACE_OS::strcpy (subtype, ACE_TEXT ("80386"));
      else if (sinfo.wProcessorLevel == 4)
        ACE_OS::strcpy (subtype, ACE_TEXT ("80486"));
      else if (sinfo.wProcessorLevel == 5)
        ACE_OS::strcpy (subtype, ACE_TEXT ("Pentium"));
      else if (sinfo.wProcessorLevel == 6)
        ACE_OS::strcpy (subtype, ACE_TEXT ("Pentium Pro"));
      else if (sinfo.wProcessorLevel == 7)  // I'm guessing here
        ACE_OS::strcpy (subtype, ACE_TEXT ("Pentium II"));
      break;
    case PROCESSOR_ARCHITECTURE_MIPS:
      ACE_OS::strcpy (processor, ACE_TEXT ("MIPS"));
      ACE_OS::strcpy (subtype, ACE_TEXT ("R4000"));
      break;
    case PROCESSOR_ARCHITECTURE_ALPHA:
      ACE_OS::strcpy (processor, ACE_TEXT ("Alpha"));
      ACE_OS::sprintf (subtype, ACE_TEXT ("%d"), sinfo.wProcessorLevel);
      break;
    case PROCESSOR_ARCHITECTURE_PPC:
      ACE_OS::strcpy (processor, ACE_TEXT ("PPC"));
      if (sinfo.wProcessorLevel == 1)
        ACE_OS::strcpy (subtype, ACE_TEXT ("601"));
      else if (sinfo.wProcessorLevel == 3)
        ACE_OS::strcpy (subtype, ACE_TEXT ("603"));
      else if (sinfo.wProcessorLevel == 4)
        ACE_OS::strcpy (subtype, ACE_TEXT ("604"));
      else if (sinfo.wProcessorLevel == 6)
        ACE_OS::strcpy (subtype, ACE_TEXT ("603+"));
      else if (sinfo.wProcessorLevel == 9)
        ACE_OS::strcpy (subtype, ACE_TEXT ("804+"));
      else if (sinfo.wProcessorLevel == 20)
        ACE_OS::strcpy (subtype, ACE_TEXT ("620"));
      break;
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
    default:
      // @@ We could provide WinCE specific info here.  But let's
      //    defer that to some later point.
      ACE_OS::strcpy (processor, ACE_TEXT ("Unknown"));
      break;
    }
    ACE_OS::sprintf(name->machine, ACE_TEXT ("%s %s"), processor, subtype);
  }
  else if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
  {
    // Get Windows 95 Information
    ACE_OS::strcpy (name->release, ACE_TEXT ("Windows 95"));
    ACE_OS::sprintf (name->version, ACE_TEXT ("%d"), LOWORD (vinfo.dwBuildNumber));
    if (sinfo.dwProcessorType == PROCESSOR_INTEL_386)
      ACE_OS::strcpy (name->machine, ACE_TEXT ("Intel 80386"));
    else if (sinfo.dwProcessorType == PROCESSOR_INTEL_486)
      ACE_OS::strcpy (name->machine, ACE_TEXT ("Intel 80486"));
    else if (sinfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM)
      ACE_OS::strcpy (name->machine, ACE_TEXT ("Intel Pentium"));
  }
  else
  {
    // We don't know what this is!

    ACE_OS::strcpy (name->release, ACE_TEXT ("???"));
    ACE_OS::strcpy (name->version, ACE_TEXT ("???"));
    ACE_OS::strcpy (name->machine, ACE_TEXT ("???"));
  }

  return ACE_OS::hostname (name->nodename, maxnamelen);
# elif defined (VXWORKS)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, "VxWorks");
  ACE_OS::strcpy (name->release, "???");
  ACE_OS::strcpy (name->version, sysBspRev ());
  ACE_OS::strcpy (name->machine, sysModel ());

  return ACE_OS::hostname (name->nodename, maxnamelen);
# elif defined (CHORUS)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, "CHORUS/ClassiX");
  ACE_OS::strcpy (name->release, "???");
  ACE_OS::strcpy (name->version, "???");
  ACE_OS::strcpy (name->machine, "???");

  return ACE_OS::hostname (name->nodename, maxnamelen);
#elif defined (ACE_PSOS)
  ACE_UNUSED_ARG (name);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}
#endif /* ACE_WIN32 || VXWORKS */


#if defined (VXWORKS)
struct hostent *
ACE_OS::gethostbyname (const char *name)
{
  // ACE_TRACE ("ACE_OS::gethostbyname");

  // not thread safe!
  static hostent ret;
  static int first_addr;
  static char *hostaddr[2];
  static char *aliases[1];

  ACE_OSCALL (::hostGetByName ((char *) name), int, -1, first_addr);
  if (first_addr == -1)
    return 0;

  hostaddr[0] = (char *) &first_addr;
  hostaddr[1] = 0;
  aliases[0] = 0;

  // Might not be official: just echo input arg.
  ret.h_name = (char *) name;
  ret.h_addrtype = AF_INET;
  ret.h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;
  ret.h_addr_list = hostaddr;
  ret.h_aliases = aliases;

  return &ret;
}

struct hostent *
ACE_OS::gethostbyaddr (const char *addr, int length, int type)
{
  // ACE_TRACE ("ACE_OS::gethostbyaddr");

  if (length != 4 || type != AF_INET)
    {
      errno = EINVAL;
      return 0;
    }

  // not thread safe!
  static hostent ret;
  static char name [MAXNAMELEN + 1];
  static char *hostaddr[2];
  static char *aliases[1];

  if (::hostGetByAddr (*(int *) addr, name) != 0)
    {
      // errno will have been set to S_hostLib_UNKNOWN_HOST.
      return 0;
    }

  // Might not be official: just echo input arg.
  hostaddr[0] = (char *) addr;
  hostaddr[1] = 0;
  aliases[0] = 0;

  ret.h_name = name;
  ret.h_addrtype = AF_INET;
  ret.h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;
  ret.h_addr_list = hostaddr;
  ret.h_aliases = aliases;

  return &ret;
}

struct hostent *
ACE_OS::gethostbyaddr_r (const char *addr, int length, int type,
                         hostent *result, ACE_HOSTENT_DATA buffer,
                         int *h_errnop)
{
  // ACE_TRACE ("ACE_OS::gethostbyaddr_r");
  if (length != 4 || type != AF_INET)
    {
      errno = EINVAL;
      return 0;
    }

  if (ACE_OS::netdb_acquire ())
    return 0;
  else
    {
      // buffer layout:
      // buffer[0-3]: h_addr_list[0], the first (and only) addr.
      // buffer[4-7]: h_addr_list[1], the null terminator for the h_addr_list.
      // buffer[8]: the name of the host, null terminated.

      // Call ::hostGetByAddr (), which puts the (one) hostname into
      // buffer.
      if (::hostGetByAddr (*(int *) addr, &buffer[8]) == 0)
        {
          // Store the return values in result.
          result->h_name = &buffer[8];  // null-terminated host name
          result->h_addrtype = AF_INET;
          result->h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN.

          result->h_addr_list = (char **) buffer;
          // Might not be official: just echo input arg.
          result->h_addr_list[0] = (char *) addr;
          // Null-terminate the list of addresses.
          result->h_addr_list[1] = 0;
          // And no aliases, so null-terminate h_aliases.
          result->h_aliases = &result->h_addr_list[1];
        }
      else
        {
          // errno will have been set to S_hostLib_UNKNOWN_HOST.
          result = 0;
        }
    }

  ACE_OS::netdb_release ();
  *h_errnop = errno;
  return result;
}

struct hostent *
ACE_OS::gethostbyname_r (const char *name, hostent *result,
                         ACE_HOSTENT_DATA buffer,
                         int *h_errnop)
{
  // ACE_TRACE ("ACE_OS::gethostbyname_r");

  if (ACE_OS::netdb_acquire ())
    return 0;
  else
    {
      int addr;
      ACE_OSCALL (::hostGetByName ((char *) name), int, -1, addr);

      if (addr == -1)
        {
          // errno will have been set to S_hostLib_UNKNOWN_HOST
          result = 0;
        }
      else
        {
          // Might not be official: just echo input arg.
          result->h_name = (char *) name;
          result->h_addrtype = AF_INET;
          result->h_length = 4;  // VxWorks 5.2/3 doesn't define IP_ADDR_LEN;

          // buffer layout:
          // buffer[0-3]: h_addr_list[0], pointer to the addr.
          // buffer[4-7]: h_addr_list[1], null terminator for the h_addr_list.
          // buffer[8-11]: the first (and only) addr.

          // Store the address list in buffer.
          result->h_addr_list = (char **) buffer;
          // Store the actual address _after_ the address list.
          result->h_addr_list[0] = (char *) &result->h_addr_list[2];
          result->h_addr_list[2] = (char *) addr;
          // Null-terminate the list of addresses.
          result->h_addr_list[1] = 0;
          // And no aliases, so null-terminate h_aliases.
          result->h_aliases = &result->h_addr_list[1];
        }
    }

  ACE_OS::netdb_release ();
  *h_errnop = errno;
  return result;
}
#endif /* VXWORKS */

void
ACE_OS::ace_flock_t::dump (void) const
{
// ACE_TRACE ("ACE_OS::ace_flock_t::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("handle_ = %u"), this->handle_));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nInternal = %d"), this->overlapped_.Internal));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nInternalHigh = %d"), this->overlapped_.InternalHigh));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nOffsetHigh = %d"), this->overlapped_.OffsetHigh));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nhEvent = %d"), this->overlapped_.hEvent));
#elif !defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nl_whence = %d"), this->lock_.l_whence));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nl_start = %d"), this->lock_.l_start));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nl_len = %d"), this->lock_.l_len));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nl_type = %d"), this->lock_.l_type));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_OS::mutex_lock_cleanup (void *mutex)
{
// ACE_TRACE ("ACE_OS::mutex_lock_cleanup");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_PTHREADS)
  ACE_mutex_t *p_lock = (ACE_mutex_t *) mutex;
  ACE_OS::mutex_unlock (p_lock);
# else
  ACE_UNUSED_ARG (mutex);
# endif /* ACE_HAS_PTHREADS */
#else
  ACE_UNUSED_ARG (mutex);
#endif /* ACE_HAS_THREADS */
}

// The following *printf functions aren't inline because
// they use varargs.

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
FILE *
ACE_OS::fopen (const char *filename, const char *mode)
{
  // ACE_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  for (const char *mode_ptr = mode; *mode_ptr != 0; mode_ptr++)
    ACE_OS::fopen_mode_to_open_mode_converter (*mode_ptr, hmode);

  ACE_HANDLE handle = ACE_OS::open (filename, hmode);
  if (handle != ACE_INVALID_HANDLE)
    {
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;
      int fd = _open_osfhandle ((long) handle, hmode);
      if (fd != -1)
        {
          FILE *fp = _fdopen (fd, mode);
          if (fp != NULL)
            return fp;
          _close (fd);
        }
      ACE_OS::close (handle);
    }
  return NULL;
}
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

#if defined (ACE_WIN32)
FILE *
ACE_OS::fopen (const wchar_t *filename, const wchar_t *mode)
{
  // ACE_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  for (const wchar_t *mode_ptr = mode; *mode_ptr != 0; mode_ptr++)
    ACE_OS::fopen_mode_to_open_mode_converter ((char) *mode_ptr, hmode);

  ACE_HANDLE handle = ACE_OS::open (filename, hmode);
#   if defined (ACE_HAS_WINCE)
  return handle;
#   else
  if (handle != ACE_INVALID_HANDLE)
    {
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;
      int fd = _open_osfhandle ((long) handle, hmode);
      if (fd != -1)
        {
          FILE *fp = _wfdopen (fd, mode);
          if (fp != NULL)
            return fp;
          _close (fd);
        }
      ACE_OS::close (handle);
    }
  return NULL;
#   endif /* !ACE_HAS_WINCE */
}
# endif /* ACE_WIN32 */

#if !defined (ACE_HAS_WINCE)
int
ACE_OS::fprintf (FILE *fp, const char *format, ...)
{
  // ACE_TRACE ("ACE_OS::fprintf");
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vfprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;
}

int
ACE_OS::printf (const char *format, ...)
{
  // ACE_TRACE ("ACE_OS::printf");
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vprintf (format, ap), int, -1, result);
  va_end (ap);
  return result;
}

int
ACE_OS::sprintf (char *buf, const char *format, ...)
{
  // ACE_TRACE ("ACE_OS::sprintf");
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vsprintf (buf, format, ap)), int, -1, result);
  va_end (ap);
  return result;
}

char *
ACE_OS::gets (char *str, int n)
{
  // ACE_TRACE ("ACE_OS::gets");
  int c;
  char *s = str;

  if (str == 0 || n < 0) n = 0;
  if (n == 0) str = 0;
  else n--;

  while ((c = getchar ()) != '\n')
    {

#   if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
      if (c == EOF && errno == EINTR && ACE_LOG_MSG->restart ())
        continue;
#   endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */

      if (c == EOF)
        break;

      if (n > 0)
        n--, *s++ = c;
    }
  if (s) *s = '\0';

  return (c == EOF) ? 0 : str;
}

#else
int
fprintf (FILE *fp, char *format, const char *msg)
{
  ACE_DEBUG ((LM_DEBUG, ASYS_WIDE_STRING (format), ASYS_WIDE_STRING (msg)));
  return 0;
}
#endif /* ! ACE_HAS_WINCE */

#if defined (ACE_HAS_UNICODE)
# if defined (ACE_WIN32)

int
ACE_OS::fprintf (FILE *fp, const wchar_t *format, ...)
{
  // ACE_TRACE ("ACE_OS::fprintf");
#   if defined (ACE_HAS_WINCE)
  ACE_NOTSUP_RETURN (-1);
#   else
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vfwprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;
#   endif /* ACE_HAS_WINCE */
}

int
ACE_OS::sprintf (wchar_t *buf, const wchar_t *format, ...)
{
  // ACE_TRACE ("ACE_OS::sprintf");
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vswprintf (buf, format, ap), int, -1, result);
  va_end (ap);
  return result;
}

#   if 0
int
ACE_OS::sprintf (wchar_t *buf, const char *format, ...)
{
  // ACE_TRACE ("ACE_OS::sprintf");
  const wchar_t *wide_format = ACE_WString (format).fast_rep ();
  int result;
  va_list ap;
  va_start (ap, wide_format);
  ACE_OSCALL (::vswprintf (buf, wide_format, ap), int, -1, result);
  va_end (ap);
  return result;
}
#   endif /* 0 */

# endif /* ACE_WIN32 */

# if defined (ACE_LACKS_MKTEMP)
wchar_t *
ACE_OS::mktemp (wchar_t *s)
{
  // ACE_TRACE ("ACE_OS::mktemp");
  if (s == 0)
    // check for null template string failed!
    return 0;
  else
    {
      wchar_t *xxxxxx = ACE_OS::strstr (s, ACE_TEXT ("XXXXXX"));

      if (xxxxxx == 0)
        // the template string doesn't contain "XXXXXX"!
        return s;
      else
        {
          wchar_t unique_letter = L'a';
          struct stat sb;

          // Find an unused filename for this process.  It is assumed
          // that the user will open the file immediately after
          // getting this filename back (so, yes, there is a race
          // condition if multiple threads in a process use the same
          // template).  This appears to match the behavior of the
          // Solaris 2.5 mktemp().
          ACE_OS::sprintf (xxxxxx, ACE_TEXT ("%05d%c"), getpid (), unique_letter);
          while (ACE_OS::stat (s, &sb) >= 0)
            {
              if (++unique_letter <= L'z')
                ACE_OS::sprintf (xxxxxx, ACE_TEXT ("%05d%c"), getpid (), unique_letter);
              else
                {
                  // maximum of 26 unique files per template, per process
                  ACE_OS::sprintf (xxxxxx, ACE_TEXT ("%s"), L"");
                  return s;
                }
            }
        }
      return s;
    }
}
# endif /* ACE_LACKS_MKTEMP */
#endif /* ACE_HAS_UNICODE */

int
ACE_OS::execl (const char * /* path */, const char * /* arg0 */, ...)
{
  // ACE_TRACE ("ACE_OS::execl");
#if defined (ACE_WIN32) || defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  // ACE_OSCALL_RETURN (::execv (path, argv), int, -1);
#endif /* ACE_WIN32 */
}

int
ACE_OS::execle (const char * /* path */, const char * /* arg0 */, ...)
{
  // ACE_TRACE ("ACE_OS::execle");
#if defined (ACE_WIN32) || defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  //  ACE_OSCALL_RETURN (::execve (path, argv, envp), int, -1);
#endif /* ACE_WIN32 */
}

int
ACE_OS::execlp (const char * /* file */, const char * /* arg0 */, ...)
{
  // ACE_TRACE ("ACE_OS::execlp");
#if defined (ACE_WIN32) || defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  //  ACE_OSCALL_RETURN (::execvp (file, argv), int, -1);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_PRIOCNTL)
# include /**/ <sys/rtpriocntl.h>
# include /**/ <sys/tspriocntl.h>
#endif /* ACE_HAS_PRIOCNTL */

int
ACE_OS::scheduling_class (const char *class_name, ACE_id_t &id)
{
#if defined (ACE_HAS_PRIOCNTL)
  // Get the priority class ID.
  pcinfo_t pcinfo;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcinfo, 0, sizeof pcinfo);

  ACE_OS::strcpy (pcinfo.pc_clname, class_name);
  if (ACE_OS::priority_control (P_ALL /* ignored */,
                                P_MYID /* ignored */,
                                PC_GETCID,
                                (char *) &pcinfo) == -1)
    {
      return -1;
    }
  else
    {
      id = pcinfo.pc_cid;
      return 0;
    }
#else  /* ! ACE_HAS_PRIOCNTL */
  ACE_UNUSED_ARG (class_name);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_PRIOCNTL */
}

int
ACE_OS::set_scheduling_params (const ACE_Sched_Params &sched_params,
                               ACE_id_t id)
{
#if defined (ACE_HAS_PRIOCNTL)
  // Set priority class, priority, and quantum of this LWP or process as
  // specified in sched_params.

  // Get the priority class ID.
  ACE_id_t class_id;
  if (ACE_OS::scheduling_class (sched_params.policy() == ACE_SCHED_OTHER  ?
                                  "TS"  :
                                  "RT", class_id) == -1)
    {
      return -1;
    }

  pcparms_t pcparms;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcparms, 0, sizeof pcparms);

  pcparms.pc_cid = class_id;

  if (sched_params.policy () == ACE_SCHED_OTHER  &&
      sched_params.quantum () == ACE_Time_Value::zero)
      // Solaris doesn't support non-zero quantums in time-sharing class:  use
      // real-time class instead.
    {
      tsparms_t tsparms;
      // The following is just to avoid Purify warnings about unitialized
      // memory reads.
      ACE_OS::memset (&tsparms, 0, sizeof tsparms);

      // Don't change ts_uprilim (user priority limit)
      tsparms.ts_uprilim = TS_NOCHANGE;
      tsparms.ts_upri = sched_params.priority ();

      // Package up the TS class ID and parameters for the
      // priority_control () call.
      ACE_OS::memcpy (pcparms.pc_clparms, &tsparms, sizeof tsparms);
    }
  else if (sched_params.policy () == ACE_SCHED_FIFO  ||
           (sched_params.policy () == ACE_SCHED_RR &&
            sched_params.quantum () != ACE_Time_Value::zero))
           // must have non-zero quantum for RR, to make it meaningful
           // A zero quantum with FIFO has special significance:  it actually
           // means infinite time quantum, i.e., run-to-completion.
    {
      rtparms_t rtparms;
      // The following is just to avoid Purify warnings about unitialized
      // memory reads.
      ACE_OS::memset (&rtparms, 0, sizeof rtparms);

      rtparms.rt_pri = sched_params.priority ();

      if (sched_params.quantum () == ACE_Time_Value::zero)
        {
          // rtparms.rt_tqsecs is ignored with RT_TQINF
          rtparms.rt_tqnsecs = RT_TQINF;
        }
      else
        {
          rtparms.rt_tqsecs = (ulong) sched_params.quantum ().sec ();
          rtparms.rt_tqnsecs = sched_params.quantum ().usec () * 1000;
        }

      // Package up the RT class ID and parameters for the
      // priority_control () call.
      ACE_OS::memcpy (pcparms.pc_clparms, &rtparms, sizeof rtparms);
    }
  else
    {
      errno = EINVAL;
      return -1;
    }

  if (ACE_OS::priority_control ((idtype_t) (sched_params.scope () ==
                                              ACE_SCOPE_THREAD  ?
                                            ACE_SCOPE_PROCESS  :
                                            sched_params.scope ()),
                                id,
                                PC_SETPARMS,
                                (char *) &pcparms) < 0)
    {
      return ACE_OS::last_error ();
    }

  return 0;
#else  /* ! ACE_HAS_PRIOCNTL */
  ACE_UNUSED_ARG (sched_params);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_PRIOCNTL */
}

int
ACE_OS::thr_setprio (const ACE_Sched_Priority prio)
{
  // Set the thread priority on the current thread.
  ACE_hthread_t my_thread_id;
  ACE_OS::thr_self (my_thread_id);

  int status = ACE_OS::thr_setprio (my_thread_id, prio);

# if defined (ACE_NEEDS_LWP_PRIO_SET)
  // If the thread is in the RT class, then set the priority on its
  // LWP.  (Instead of doing this if the thread is in the RT class, it
  // should be done for all bound threads.  But, there doesn't appear
  // to be an easy way to determine if the thread is bound.)

  if (status == 0)
    {
      // Find what scheduling class the thread's LWP is in.
      ACE_Sched_Params sched_params (ACE_SCHED_OTHER, 0);
      if (ACE_OS::lwp_getparams (sched_params) == -1)
        {
          return -1;
        }
      else if (sched_params.policy () == ACE_SCHED_FIFO  ||
               sched_params.policy () == ACE_SCHED_RR)
        {
          // This thread's LWP is in the RT class, so we need to set
          // its priority.
          sched_params.priority (prio);
          return ACE_OS::lwp_setparams (sched_params);
        }
      // else this is not an RT thread.  Nothing more needs to be
      // done.
    }
# endif /* ACE_NEEDS_LWP_PRIO_SET */

  return status;
}

int
ACE_OS::sched_params (const ACE_Sched_Params &sched_params,
                      ACE_id_t id)
{
  // ACE_TRACE ("ACE_OS::sched_params");
# if defined (CHORUS)
  ACE_UNUSED_ARG (id);
  int result;
  struct sched_param param;
  ACE_thread_t thr_id = ACE_OS::thr_self ();

  param.sched_priority = sched_params.priority ();

  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (thr_id,
                                                                sched_params.policy (),
                                                                &param),
                                       result),
                     int, -1);
# elif defined (ACE_HAS_STHREADS)
  return ACE_OS::set_scheduling_params (sched_params, id);
# elif defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_SETSCHED)
  ACE_UNUSED_ARG (id);
  if (sched_params.quantum () != ACE_Time_Value::zero)
    {
      // quantums not supported
      errno = EINVAL;
      return -1;
    }

  // Thanks to Thilo Kielmann <kielmann@informatik.uni-siegen.de> for
  // providing this code for 1003.1c PThreads.  Please note that this
  // has only been tested for POSIX 1003.1c threads, and may cause problems
  // with other PThreads flavors!

  struct sched_param param;
  param.sched_priority = sched_params.priority ();

  if (sched_params.scope () == ACE_SCOPE_PROCESS)
    {
      int result = ::sched_setscheduler (0, // this process
                                         sched_params.policy (),
                                         &param) == -1 ? -1 : 0;
#   if defined DIGITAL_UNIX
        return result == 0
          ? // Use priocntl (2) to set the process in the RT class,
            // if using an RT policy.
            ACE_OS::set_scheduling_params (sched_params)
          : result;
#   else  /* ! DIGITAL_UNIX */
        return result;
#   endif /* ! DIGITAL_UNIX */
    }
  else if (sched_params.scope () == ACE_SCOPE_THREAD)
    {
      ACE_thread_t thr_id = ACE_OS::thr_self ();

#   if defined (ACE_HAS_PTHREADS_DRAFT4)
      return (::pthread_setscheduler(thr_id,
                                     sched_params.policy (),
                                     sched_params.priority()) == -1 ? -1 : 0);
#   else
      int result;
      ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (
                                             thr_id,
                                             sched_params.policy (),
                                             &param),
                                           result),
                         int, -1);
#   endif  /* ACE_HAS_PTHREADS_DRAFT4 */

    }
#if defined (sun)
  // We need to be able to set LWP priorities on Suns, even without
  // ACE_HAS_STHREADS, to obtain preemption.
  else if (sched_params.scope () == ACE_SCOPE_LWP)
    return ACE_OS::set_scheduling_params (sched_params, id);
#endif /* sun */
  else // sched_params.scope () == ACE_SCOPE_LWP, which isn't POSIX
    {
      errno = EINVAL;
      return -1;
    }

# elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (id);

  if (sched_params.scope () != ACE_SCOPE_PROCESS  ||
      sched_params.quantum () != ACE_Time_Value::zero)
    {
      // Win32 only allows setting priority class (therefore, policy)
      // at the process level.  I don't know of a way to set the quantum.
      errno = EINVAL;
      return -1;
    }

  // Set the priority class of this process to the REALTIME process class
  // _if_ the policy is ACE_SCHED_FIFO.  Otherwise, set to NORMAL.
  if (! ::SetPriorityClass (
          ::GetCurrentProcess (),
          sched_params.policy () == ACE_SCHED_FIFO
            ?  REALTIME_PRIORITY_CLASS
            :  NORMAL_PRIORITY_CLASS))
    {
      return -1;
    }

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());

# elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (id);

  // There is only one class of priorities on VxWorks, and no
  // time quanta.  So, just set the current thread's priority.

  if (sched_params.policy () != ACE_SCHED_FIFO  ||
      sched_params.scope () != ACE_SCOPE_PROCESS  ||
      sched_params.quantum () != ACE_Time_Value::zero)
    {
      errno = EINVAL;
      return -1;
    }

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());
#else
  ACE_UNUSED_ARG (sched_params);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
#endif /* CHORUS */
}

// = Static initialization.

// This is necessary to deal with POSIX pthreads insanity.  This
// guarantees that we've got a "zero'd" thread id even when
// ACE_thread_t, ACE_hthread_t, and ACE_thread_key_t are implemented
// as structures...  Under no circumstances should these be given
// initial values.
ACE_thread_t ACE_OS::NULL_thread;
ACE_hthread_t ACE_OS::NULL_hthread;
ACE_thread_key_t ACE_OS::NULL_key;

#if defined (CHORUS)
KnCap ACE_OS::actorcaps_[ACE_CHORUS_MAX_ACTORS];
// This is used to map an actor's id into a KnCap for killing and
// waiting actors.
#endif /* CHORUS */

#if defined (ACE_WIN32)

// = Static initialization.

// Keeps track of whether we've initialized the WinSock DLL.
int ACE_OS::socket_initialized_;

#endif /* WIN32 */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)

// Moved class ACE_TSS_Ref declaration to OS.h so it can be visible to
// the single file of template instantiations.

ACE_TSS_Ref::ACE_TSS_Ref (ACE_thread_t id)
  : tid_(id)
{
// ACE_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

ACE_TSS_Ref::ACE_TSS_Ref (void)
{
// ACE_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

// Check for equality.
int
ACE_TSS_Ref::operator== (const ACE_TSS_Ref &info) const
{
// ACE_TRACE ("ACE_TSS_Ref::operator==");

  return this->tid_ == info.tid_;
}

// Check for inequality.
inline
int
ACE_TSS_Ref::operator!= (const ACE_TSS_Ref &tss_ref) const
{
// ACE_TRACE ("ACE_TSS_Ref::operator==");

  return !(*this == tss_ref);
}

// moved class ACE_TSS_Info declaration
// to OS.h so it can be visible to the
// single file of template instantiations

ACE_TSS_Info::ACE_TSS_Info (ACE_thread_key_t key,
                            void (*dest)(void *),
                            void *tss_inst)
  : key_ (key),
    destructor_ (dest),
    tss_obj_ (tss_inst),
    thread_count_ (-1)
{
// ACE_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

ACE_TSS_Info::ACE_TSS_Info (void)
{
// ACE_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

# if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
  static inline int operator== (const ACE_thread_key_t &lhs,
                                const ACE_thread_key_t &rhs)
  {
    return ! ACE_OS::memcmp (&lhs, &rhs, sizeof (ACE_thread_key_t));
  }

  static inline int operator!= (const ACE_thread_key_t &lhs,
                                const ACE_thread_key_t &rhs)
  {
    return ! (lhs == rhs);
  }
# endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

// Check for equality.
int
ACE_TSS_Info::operator== (const ACE_TSS_Info &info) const
{
// ACE_TRACE ("ACE_TSS_Info::operator==");

  return this->key_ == info.key_;
}

// Check for inequality.
int
ACE_TSS_Info::operator!= (const ACE_TSS_Info &info) const
{
// ACE_TRACE ("ACE_TSS_Info::operator==");

  return !(*this == info);
}

void
ACE_TSS_Info::dump (void)
{
//  ACE_TRACE ("ACE_TSS_Info::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("key_ = %u"), this->key_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ndestructor_ = %u"), this->destructor_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntss_obj_ = %u"), this->tss_obj_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// moved class ACE_TSS_Keys declaration
// to OS.h so it can be visible to the
// single file of template instantiations

ACE_TSS_Keys::ACE_TSS_Keys (void)
{
  for (u_int i = 0; i < ACE_WORDS; ++i)
    {
      key_bit_words_[i] = 0;
    }
}

inline
void
ACE_TSS_Keys::find (const u_int key, u_int &word, u_int &bit)
{
  word = key / ACE_BITS_PER_WORD;
  bit = key % ACE_BITS_PER_WORD;
}

int
ACE_TSS_Keys::test_and_set (const ACE_thread_key_t key)
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  if (ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit))
    {
      return 1;
    }
  else
    {
      ACE_SET_BITS (key_bit_words_[word], 1 << bit);
      return 0;
    }
}

int
ACE_TSS_Keys::test_and_clear (const ACE_thread_key_t key)
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  if (ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit))
    {
      ACE_CLR_BITS (key_bit_words_[word], 1 << bit);
      return 0;
    }
  else
    {
      return 1;
    }
}

class ACE_TSS_Cleanup
  // = TITLE
  //     Singleton that knows how to clean up all the thread-specific
  //     resources for Win32.
  //
  // = DESCRIPTION
  //     All this nonsense is required since Win32 doesn't
  //     automatically cleanup thread-specific storage on thread exit,
  //     unlike real operating systems... ;-)
{
public:
  static ACE_TSS_Cleanup *instance (void);

  ~ACE_TSS_Cleanup (void);

  void exit (void *status);
  // Cleanup the thread-specific objects.  Does _NOT_ exit the thread.

  int insert (ACE_thread_key_t key, void (*destructor)(void *), void *inst);
  // Insert a <key, destructor> tuple into the table.

  int remove (ACE_thread_key_t key);
  // Remove a <key, destructor> tuple from the table.

  int detach (void *inst);
  // Detaches a tss_instance from its key.

  void key_used (ACE_thread_key_t key);
  // Mark a key as being used by this thread.

  int free_all_key_left (void);
  // Free all key left in the table before destruct myself.

  static int lockable () { return instance_ != 0; }
  // Indication of whether the ACE_TSS_CLEANUP_LOCK is usable, and
  // therefore whether we are in static constructor/destructor phase
  // or not.

protected:
  void dump (void);

  ACE_TSS_Cleanup (void);
  // Ensure singleton.

private:
  // Array of <ACE_TSS_Info> objects.
  typedef ACE_Array<ACE_TSS_Info> ACE_TSS_TABLE;
  typedef ACE_Array_Iterator<ACE_TSS_Info> ACE_TSS_TABLE_ITERATOR;

  ACE_TSS_TABLE table_;
  // Table of <ACE_TSS_Info>'s.

  ACE_TSS<ACE_TSS_Keys> in_use_;
  // Array, per thread (in TSS), of whether each TSS key is in use.

  // = Static data.
  static ACE_TSS_Cleanup *instance_;
  // Pointer to the singleton instance.
};

// = Static object initialization.

// Pointer to the singleton instance.
ACE_TSS_Cleanup *ACE_TSS_Cleanup::instance_ = 0;

ACE_TSS_Cleanup::~ACE_TSS_Cleanup (void)
{
  // Zero out the instance pointer to support lockable () accessor.
  ACE_TSS_Cleanup::instance_ = 0;
}

void
ACE_TSS_Cleanup::exit (void * /* status */)
{
  // ACE_TRACE ("ACE_TSS_Cleanup::exit");

  ACE_TSS_Info *key_info = 0;
  ACE_TSS_Info info_arr[ACE_DEFAULT_THREAD_KEYS];
  int info_ix = 0;

  // While holding the lock, we only collect the ACE_TSS_Info objects
  // in an array without invoking the according destructors.

  {
    ACE_MT (ACE_Recursive_Thread_Mutex *lock =
      ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
        (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
      ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, *lock));

    // Iterate through all the thread-specific items and free them all
    // up.

    for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
         iter.next (key_info) != 0;
         iter.advance ())
      {
        if (! key_info->key_in_use ()) continue;

        // If the key's ACE_TSS_Info in-use bit for this thread was set,
        // unset it and decrement the key's thread_count_.
        if (! in_use_->test_and_clear (key_info->key_))
          {
            --key_info->thread_count_;
          }

        void *tss_info = 0;

        if (key_info->destructor_
            && ACE_OS::thr_getspecific (key_info->key_, &tss_info) == 0
            && tss_info)
          {
            info_arr[info_ix].key_ = key_info->key_;
            info_arr[info_ix].destructor_ = key_info->destructor_;
            info_arr[info_ix++].tss_obj_ = key_info->tss_obj_;
          }
      }
  }

  // Now we have given up the ACE_TSS_Cleanup::lock_ and we start
  // invoking destructors, in the reverse order of creation.
  for (int i = info_ix - 1; i >= 0; --i)
    {
      void *tss_info = 0;

      ACE_OS::thr_getspecific (info_arr[i].key_, &tss_info);

      if (tss_info != 0)
        {
          // Only call the destructor if the value is non-zero for this
          // thread.
          (*info_arr[i].destructor_)(tss_info);
        }
    }

  // Acquiring ACE_TSS_Cleanup::lock_ to free TLS keys and remove
  // entries from ACE_TSS_Info table.
  {
    ACE_MT (ACE_Recursive_Thread_Mutex *lock =
      ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
        (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
      ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, *lock));

# if 0
    // We shouldn't free the key and remove it from the table here
    // because if we do and some thread ends before other threads
    // even get started (or their TSS object haven't been created yet,)
    // it's entry will be removed from the table and we are in big chaos.
    // For TSS object, these have been done in ACE_TSS_Cleanup::detach.
    // Two other use cases will be user managed TSS'es and system wide
    // TSS, ones are users responsibilities and the others should be
    // persistant system wide.
    for (int i = 0; i < index; i++)
      {
# if defined (ACE_WIN32)
        // Calling thr_keyfree here ensure the key
        // gets removed appropriately.  Notice that
        // a key should be removed before freeing it.
        ACE_OS::thr_keyfree (key_info->key_);
# else
        // don't bother to free the key
        this->remove (key_info->key_);
# endif /* ACE_WIN32 */
      }
# endif /* 0 */
  }
}

int
ACE_TSS_Cleanup::free_all_key_left (void)
  // This is call from ACE_OS::cleanup_tss
  // When this gets called, all threads should
  // have exited except the main thread.
  // No key should be freed from this routine.
  // It there's any, something might be wrong.
{
  ACE_thread_key_t key_arr[ACE_DEFAULT_THREAD_KEYS];
  ACE_TSS_Info *key_info = 0;
  int idx = 0;

  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    key_arr [idx++] = key_info->key_;

  for (int i = 0; i < idx; i++)
    if (key_arr[i] != ACE_OS::NULL_key)
      ACE_OS::thr_keyfree (key_arr[i]);

  return 0;
}

ACE_TSS_Cleanup::ACE_TSS_Cleanup (void)
  : table_ (ACE_DEFAULT_THREAD_KEYS, ACE_TSS_Info (ACE_OS::NULL_key)),
    in_use_ ()
{
// ACE_TRACE ("ACE_TSS_Cleanup::ACE_TSS_Cleanup");
}

ACE_TSS_Cleanup *
ACE_TSS_Cleanup::instance (void)
{
// ACE_TRACE ("ACE_TSS_Cleanup::instance");

  // Create and initialize thread-specific key.
  if (ACE_TSS_Cleanup::instance_ == 0)
    {
      // Insure that we are serialized!
      ACE_MT (ACE_Recursive_Thread_Mutex *lock =
        ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
          (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
        ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *lock, 0));

      // Now, use the Double-Checked Locking pattern to make sure we
      // only create the ACE_TSS_Cleanup instance once.
      if (instance_ == 0)
        {
          ACE_NEW_RETURN (ACE_TSS_Cleanup::instance_, ACE_TSS_Cleanup, 0);
        }
    }

  return ACE_TSS_Cleanup::instance_;
}

int
ACE_TSS_Cleanup::insert (ACE_thread_key_t key,
                         void (*destructor)(void *),
                         void *inst)
{
// ACE_TRACE ("ACE_TSS_Cleanup::insert");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
    ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *lock, -1));

  ACE_KEY_INDEX (key_index, key);
  return this->table_.set (ACE_TSS_Info (key, destructor, inst), key_index);
}

int
ACE_TSS_Cleanup::remove (ACE_thread_key_t key)
{
  // ACE_TRACE ("ACE_TSS_Cleanup::remove");
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
    ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *lock, -1));

  ACE_KEY_INDEX (key_index, key);
  if (key_index <= this->table_.size ())
    {
      // "Remove" the TSS_Info table entry by zeroing out its key_ and
      // destructor_ fields.  Also, keep track of the number threads
      // using the key.
      ACE_TSS_Info &info = this->table_ [key_index];

      // Don't bother to check <in_use_> if the program is shutting
      // down.  Doing so will cause a new ACE_TSS object getting
      // created again.
      if (!ACE_Object_Manager::shutting_down ()
          && !in_use_->test_and_clear (info.key_))
        --info.thread_count_;

      info.key_ = ACE_OS::NULL_key;
      info.destructor_ = 0;
      return 0;
    }
  else
    return -1;
}

int
ACE_TSS_Cleanup::detach (void *inst)
{
  ACE_MT (ACE_Recursive_Thread_Mutex *lock =
    ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
    ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *lock, -1));

  ACE_TSS_Info *key_info = 0;
  int success = 0;
  int ref_cnt = 0;

  // Mark the key as detached in the TSS_Info table.
  // It only works for the first key that "inst" owns.
  // I don't know why.
  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    {
      if (key_info->tss_obj_ == inst)
        {
          key_info->tss_obj_ = 0;
          ref_cnt = key_info->thread_count_;
          success = 1;
          break;
        }
    }

  if (success == 0)
    return -1;
  else if (ref_cnt == 0)
    {
      // Mark the key as no longer being used.
      key_info->key_in_use (0);
# if defined (ACE_WIN32)
      ACE_thread_key_t temp_key = key_info->key_;
# endif /* ACE_WIN32 */
      int retv = this->remove (key_info->key_);

# if defined (ACE_WIN32)
      ::TlsFree (temp_key);
# endif /* ACE_WIN32 */
      return retv;
    }

  return 0;
}

void
ACE_TSS_Cleanup::key_used (ACE_thread_key_t key)
{
  // If the key's ACE_TSS_Info in-use bit for this thread is not set,
  // set it and increment the key's thread_count_.
  if (! in_use_->test_and_set (key))
    {
      ACE_MT (ACE_Recursive_Thread_Mutex *lock =
        ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
          (ACE_Object_Manager::ACE_TSS_CLEANUP_LOCK);
        ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon, *lock));

      // Retrieve the key's ACE_TSS_Info and increment its thread_count_.
      ACE_KEY_INDEX (key_index, key);
      ACE_TSS_Info &key_info = this->table_ [key_index];
      if (key_info.thread_count_ == -1)
        key_info.key_in_use (1);
      else
        ++key_info.thread_count_;
    }
}

void
ACE_TSS_Cleanup::dump (void)
{
  ACE_TSS_Info *key_info = 0;

  // Iterate through all the thread-specific items and dump them all.

  for (ACE_TSS_TABLE_ITERATOR iter (this->table_);
       iter.next (key_info) != 0;
       iter.advance ())
    key_info->dump ();
}

# if defined (ACE_HAS_TSS_EMULATION)
u_int ACE_TSS_Emulation::total_keys_ = 0;

ACE_TSS_Emulation::ACE_TSS_DESTRUCTOR
ACE_TSS_Emulation::tss_destructor_ [ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX] = { 0 };

#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)

int ACE_TSS_Emulation::key_created_ = 0;

ACE_OS_thread_key_t ACE_TSS_Emulation::native_tss_key_;

/* static */
#if defined (ACE_HAS_THR_C_FUNC)
extern "C"
#endif /* ACE_HAS_THR_C_FUNC */
void
ACE_TSS_Emulation_cleanup (void *ptr)
{
   ACE_UNUSED_ARG (ptr);
   // Really this must be used for ACE_TSS_Emulation code to make the TSS
   // cleanup
}

void **
ACE_TSS_Emulation::tss_base (void* ts_storage[])
{
  // TSS Singleton implementation.

  // Create the one native TSS key, if necessary.
  if (key_created_ == 0)
    {
      ACE_Recursive_Thread_Mutex *lock =
        ACE_Managed_Object<ACE_Recursive_Thread_Mutex>::get_preallocated_object
          (ACE_Object_Manager::ACE_TSS_BASE_LOCK);

      ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *lock, 0);

      if (key_created_ == 0)
        {
          ACE_NO_HEAP_CHECK;
          if (ACE_OS::thr_keycreate (&native_tss_key_,
                                     &ACE_TSS_Emulation_cleanup) != 0)
           {
              return 0; // Major problems, this should *never* happen!
           }
          key_created_ = 1;
        }
    }

  void **old_ts_storage = 0;

  // Get the tss_storage from thread-OS specific storage.
  if (ACE_OS::thr_getspecific (native_tss_key_,
                               (void **) &old_ts_storage) == -1)
    return 0; // This should not happen!

  // Check to see if this is the first time in for this thread.
  // This block can also be entered after a fork () in the child process,
  // at least on Pthreads Draft 4 platforms.
  if (old_ts_storage == 0)
    {
      // Use the ts_storage passed as argument, if non-zero.  It is
      // possible that this has been implemented in the stack. At the
      // moment, this is unknown.  The cleanup must not do nothing.
      // If ts_storage is zero, allocate (and eventually leak) the
      // storage array.
      if (ts_storage == 0)
      {
        ACE_NO_HEAP_CHECK;

        ACE_NEW_RETURN (ts_storage, void*[ACE_TSS_THREAD_KEYS_MAX], 0);

        // Zero the entire TSS array.  Do it manually instead of using
        // memset, for optimum speed.  Though, memset may be faster :-)
        void **tss_base_p = ts_storage;
        for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
          {
            *tss_base_p = 0;
          }
      }

     // Store the pointer in thread-specific storage.  It gets deleted
     // via the ACE_TSS_Emulation_cleanup function when the thread
     // terminates.
     if (ACE_OS::thr_setspecific (native_tss_key_,
                                 (void *) ts_storage) != 0)
          return 0; // Major problems, this should *never* happen!
    }

  return old_ts_storage;
}
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */

void *
ACE_TSS_Emulation::tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX])
{
#   if defined (ACE_PSOS)
  u_long tss_base;

  // Use the supplied array for this thread's TSS.
  tss_base = (u_long) ts_storage;
  t_setreg (0, PSOS_TASK_REG_TSS, tss_base);

  // Zero the entire TSS array.
  void **tss_base_p = ts_storage;
  for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
  {
    *tss_base_p = 0;
  }

  return (void *) tss_base;
#   else  /* ! ACE_PSOS */
#     if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
        // On VxWorks, in particular, don't check to see if the field
        // is 0.  It isn't always, specifically, when a program is run
        // directly by the shell (without spawning a new task) after
        // another program has been run.

  if (tss_base (ts_storage) == 0)
    {
#     else  /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */
      tss_base () = ts_storage;
#     endif

      // Zero the entire TSS array.  Do it manually instead of using
      // memset, for optimum speed.  Though, memset may be faster :-)
      void **tss_base_p = tss_base ();
      for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
        {
          *tss_base_p = 0;
        }

      return tss_base ();
#     if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
    }
  else
    {
      return 0;
    }
#     endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
#   endif /* ! ACE_PSOS */
}

# endif /* ACE_HAS_TSS_EMULATION */

# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array<ACE_TSS_Info>;
template class ACE_Array_Base<ACE_TSS_Info>;
template class ACE_Array_Iterator<ACE_TSS_Info>;
template class ACE_Node<ACE_TSS_Ref>;
template class ACE_TSS<ACE_TSS_Keys>;
# elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#   pragma instantiate ACE_Array<ACE_TSS_Info>
#   pragma instantiate ACE_Array_Base<ACE_TSS_Info>
#   pragma instantiate ACE_Array_Iterator<ACE_TSS_Info>
#   pragma instantiate ACE_Node<ACE_TSS_Ref>
#   pragma instantiate ACE_TSS<ACE_TSS_Keys>
# endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* WIN32 || ACE_HAS_TSS_EMULATION */

void
ACE_OS::cleanup_tss (const u_int main_thread)
{
#if defined (ACE_HAS_TSS_EMULATION) || defined (ACE_WIN32)
  // Call TSS destructors for current thread.
  ACE_TSS_Cleanup::instance ()->exit (0);
#endif /* ACE_HAS_TSS_EMULATION || ACE_WIN32 */

  if (main_thread)
    {
#if ! defined (ACE_HAS_TSS_EMULATION)
      // Just close the ACE_Log_Msg for the current (which should be
      // main) thread.  We don't have TSS emulation; if there's native
      // TSS, it should call its destructors when the main thread
      // exits.
      ACE_Log_Msg::close ();
#endif /* ! ACE_HAS_TSS_EMULATION */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
      // Remove all TSS_Info table entries.
      ACE_TSS_Cleanup::instance ()->free_all_key_left ();
      // Finally, free up the ACE_TSS_Cleanup instance.  This method gets
      // called by the ACE_Object_Manager.
      delete ACE_TSS_Cleanup::instance ();
#endif /* WIN32 || ACE_HAS_TSS_EMULATION */
    }
}

void
ACE_Thread_Adapter::inherit_log_msg (void)
{
#if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
  // Inherit the logging features if the parent thread has an
  // <ACE_Log_Msg>.  Note that all of the following operations occur
  // within thread-specific storage.
  ACE_Log_Msg *new_log = ACE_LOG_MSG;

  if (this->ostream_)
    {
      new_log->msg_ostream (this->ostream_);
      new_log->priority_mask (this->priority_mask_);

      if (this->tracing_enabled_)
        new_log->start_tracing ();

      new_log->restart (this->restart_);
      new_log->trace_depth (this->trace_depth_);
    }

# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
  new_log->seh_except_selector (this->seh_except_selector_);
  new_log->seh_except_handler (this->seh_except_handler_);
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

  // @@ Now the TSS Log_Mesg has been created, cache my thread
  // descriptor in.

  if (this->thr_desc_ != 0)
    ACE_LOG_MSG->thr_desc (this->thr_desc_);
  // Block the thread from proceeding until
  // thread manager has thread descriptor ready.

#endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */
}

void *
ACE_Thread_Adapter::invoke (void)
{
  // Inherit the logging features if the parent thread has an
  // ACE_Log_Msg instance in thread-specific storage.
  this->inherit_log_msg ();

#if !defined(ACE_USE_THREAD_MANAGER_ADAPTER)
  // NOTE: this preprocessor directive should match the one in
  // above ACE_Thread_Exit::instance ().  With the Xavier Pthreads
  // package, the exit_hook in TSS causes a seg fault.  So, this
  // works around that by creating exit_hook on the stack.
# if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)
  // Obtain our thread-specific exit hook and make sure that it
  // knows how to clean us up!  Note that we never use this
  // pointer directly (it's stored in thread-specific storage), so
  // it's ok to dereference it here and only store it as a
  // reference.
  if (this->thr_mgr () != 0)
    {
      ACE_Thread_Exit &exit_hook = *ACE_Thread_Exit::instance ();
      // Keep track of the <Thread_Manager> that's associated with this
      // <exit_hook>.
      exit_hook.thr_mgr (this->thr_mgr ());
    }
# else
  // Without TSS, create an <ACE_Thread_Exit> instance.  When this
  // function returns, its destructor will be called because the
  // object goes out of scope.  The drawback with this appraoch is
  // that the destructor _won't_ get called if <thr_exit> is
  // called.  So, threads shouldn't exit that way.  Instead, they
  // should return from <svc>.
  ACE_Thread_Exit exit_hook;
  exit_hook.thr_mgr (this->thr_mgr ());
# endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION */

#endif

  // Extract the arguments.
  ACE_THR_FUNC_INTERNAL func = ACE_reinterpret_cast (ACE_THR_FUNC_INTERNAL,
                                                     this->user_func_);
  void *arg = this->arg_;
#if defined (ACE_WIN32) && defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  ACE_Thread_Descriptor *thr_desc = this->thr_desc_;
#endif /* ACE_WIN32 && ACE_HAS_MFC && (ACE_HAS_MFC != 0) */

  // Delete ourselves since we don't need <this> anymore.  Make sure
  // not to access <this> anywhere below this point.
  delete this;

#if defined (ACE_NEEDS_LWP_PRIO_SET)
  // On Solaris 2.5.x and 2.6, the LWP priority needs to be set in
  // order to get preemption when running in the RT class.  This is
  // the ACE way to do that . . .
  ACE_hthread_t thr_handle;
  ACE_OS::thr_self (thr_handle);
  int prio;

  // thr_getprio () on the current thread should never fail.
  ACE_OS::thr_getprio (thr_handle, prio);

  // ACE_OS::thr_setprio () has the special logic to set the LWP priority,
  // if running in the RT class.
  ACE_OS::thr_setprio (prio);

#endif /* ACE_NEEDS_LWP_PRIO_SET */

  void *status = 0;

  ACE_SEH_TRY
    {
      ACE_SEH_TRY
        {
#if defined (ACE_PSOS)
          // pSOS thread functions do not return a value
          status = 0;
          (*func) (arg);  // Call thread entry point.
#else /* ! ACE_PSOS */
          // Call thread entry point.
          status = (void*) (*func) (arg);
#endif /* ! ACE_PSOS */
        }

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
      ACE_SEH_EXCEPT (ACE_LOG_MSG->seh_except_selector ()(
                          (void *) GetExceptionInformation ()))
        {
          ACE_LOG_MSG->seh_except_handler ()(0);
        }
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
    }

  ACE_SEH_FINALLY
    {
// If we changed this to 1, change the respective if in Task::svc_run to 0
#if 0
      // Call the <Task->close> hook.
      if (func == ACE_reinterpret_cast (ACE_THR_FUNC_INTERNAL,
                                        ACE_Task_Base::svc_run))
        {
          ACE_Task_Base *task_ptr = (ACE_Task_Base *) arg;
          ACE_Thread_Manager *thr_mgr_ptr = task_ptr->thr_mgr ();

          // This calls the Task->close () hook.
          task_ptr->cleanup (task_ptr, 0);

          // This prevents a second invocation of the cleanup code
          // (called later by <ACE_Thread_Manager::exit>.
          thr_mgr_ptr->at_exit (task_ptr, 0, 0);
        }
#endif

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
# if defined (ACE_WIN32) && defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
      int using_afx = -1;
      if (thr_desc)
        using_afx = ACE_BIT_ENABLED (thr_desc->flags (), THR_USE_AFX);
# endif /* ACE_WIN32 && ACE_HAS_MFC && (ACE_HAS_MFC != 0) */
          // Call TSS destructors.
      ACE_OS::cleanup_tss (0 /* not main thread */);

# if defined (ACE_WIN32)
      // Exit the thread.  Allow CWinThread-destructor to be
      // invoked from AfxEndThread.  _endthreadex will be called
      // from AfxEndThread so don't exit the thread now if we are
      // running an MFC thread.
#   if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
      if (using_afx != -1)
        {
          if (using_afx)
            ::AfxEndThread ((DWORD)status);
          else
            ::_endthreadex ((DWORD) status);
        }
      else
        {
          // Not spawned by ACE_Thread_Manager, use the old buggy
          // version.  You should seriously consider using
          // ACE_Thread_Manager to spawn threads.  The following code
          // is know to cause some problem.
          CWinThread *pThread = ::AfxGetThread ();

          if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
            ::_endthreadex ((DWORD) status);
          else
            ::AfxEndThread ((DWORD)status);
        }
#   else
      ::_endthreadex ((DWORD) status);
#   endif /* ACE_HAS_MFC && ACE_HAS_MFS != 0*/
# endif /* ACE_WIN32 */
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */

      return status;
    }

  ACE_NOTREACHED (return status);
}

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
int ACE_SEH_Default_Exception_Selector (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) Win32 structured exception exiting thread\n")));
  return (DWORD) ACE_SEH_DEFAULT_EXCEPTION_HANDLING_ACTION;
}

int ACE_SEH_Default_Exception_Handler (void *)
{
  return 0;
}
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

extern "C" void
ace_cleanup_destroyer (ACE_Cleanup *object, void *param)
{
  object->cleanup (param);
}

// Run the thread entry point for the <ACE_Thread_Adapter>.  This must
// be an extern "C" to make certain compilers happy...

extern "C" void *
ace_thread_adapter (void *args)
{
  // ACE_TRACE ("ace_thread_adapter");

#if defined (ACE_HAS_TSS_EMULATION)
  // As early as we can in the execution of the new thread, allocate
  // its local TS storage.  Allocate it on the stack, to save dynamic
  // allocation/dealloction.
  void *ts_storage[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX];
  ACE_TSS_Emulation::tss_open (ts_storage);
#endif /* ACE_HAS_TSS_EMULATION */

  ACE_Thread_Adapter *thread_args = (ACE_Thread_Adapter *) args;

  // Invoke the user-supplied function with the args.
  void *status = thread_args->invoke ();

  return status;
}

ACE_Thread_Adapter::ACE_Thread_Adapter (ACE_THR_FUNC user_func,
                                        void *arg,
                                        ACE_THR_C_FUNC entry_point,
                                        ACE_Thread_Manager *tm,
                                        ACE_Thread_Descriptor *td
#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                                        , ACE_SEH_EXCEPT_HANDLER selector,
                                        ACE_SEH_EXCEPT_HANDLER handler
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
                                        )
  : user_func_ (user_func),
    arg_ (arg),
    entry_point_ (entry_point),
    thr_mgr_ (tm),
    thr_desc_ (td)
#if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
    ,
    ostream_ (0),
    priority_mask_ (0),
    tracing_enabled_ (0),
    restart_ (1),
    trace_depth_ (0)
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
    , seh_except_selector_ (selector),
    seh_except_handler_ (handler)
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
#endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */
{
// ACE_TRACE ("Ace_Thread_Adapter::Ace_Thread_Adapter");
#if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
  if (ACE_Log_Msg::exists ())
    {
      ACE_Log_Msg *inherit_log_ = ACE_LOG_MSG;
      this->ostream_ = inherit_log_->msg_ostream ();
      this->priority_mask_ = inherit_log_->priority_mask ();
      this->tracing_enabled_ = inherit_log_->tracing_enabled ();
      this->restart_ = inherit_log_->restart ();
      this->trace_depth_ = inherit_log_->trace_depth ();
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
      this->seh_except_selector_ = selector;
      this->seh_except_handler_ = handler;
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
    }
#endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */
}

int
ACE_OS::thr_create (ACE_THR_FUNC func,
                    void *args,
                    long flags,
                    ACE_thread_t *thr_id,
                    ACE_hthread_t *thr_handle,
                    long priority,
                    void *stack,
                    size_t stacksize,
                    ACE_Thread_Adapter *thread_adapter)
{
  // ACE_TRACE ("ACE_OS::thr_create");

  if (ACE_BIT_DISABLED (flags, THR_DETACHED) &&
      ACE_BIT_DISABLED (flags, THR_JOINABLE))
    ACE_SET_BITS (flags, THR_JOINABLE);

# if defined (ACE_NO_THREAD_ADAPTER)
#   define  ACE_THREAD_FUNCTION  func
#   define  ACE_THREAD_ARGUMENT  args
# else /* ! defined (ACE_NO_THREAD_ADAPTER) */
#   if defined (ACE_PSOS)
#     define  ACE_THREAD_FUNCTION (PSOS_TASK_ENTRY_POINT) thread_args->entry_point ()
#   else
#     define  ACE_THREAD_FUNCTION  thread_args->entry_point ()
#   endif /* defined (ACE_PSOS) */
#   define  ACE_THREAD_ARGUMENT  thread_args
# endif /* ! defined (ACE_NO_THREAD_ADAPTER) */

  ACE_Thread_Adapter *thread_args;
  if (thread_adapter == 0)
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
    ACE_NEW_RETURN (thread_args,
                    ACE_Thread_Adapter (func, args,
                                        (ACE_THR_C_FUNC) ace_thread_adapter,
                                        0,
                                        0,
                                        ACE_LOG_MSG->seh_except_selector(),
                                        ACE_LOG_MSG->seh_except_handler()),
                    -1);
# else
    ACE_NEW_RETURN (thread_args,
                    ACE_Thread_Adapter (func, args,
                                        (ACE_THR_C_FUNC) ace_thread_adapter),
                    -1);

# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  else
    thread_args = thread_adapter;

# if defined (ACE_HAS_THREADS)

#   if !defined (VXWORKS)
  // On VxWorks, the OS will provide a task name if the user doesn't.
  // So, we don't need to create a tmp_thr.  If the caller of this
  // member function is the Thread_Manager, than thr_id will be non-zero
  // anyways.
  ACE_thread_t tmp_thr;

  if (thr_id == 0)
    thr_id = &tmp_thr;
#   endif /* ! VXWORKS */

  ACE_hthread_t tmp_handle;
  if (thr_handle == 0)
    thr_handle = &tmp_handle;

#   if defined (ACE_HAS_PTHREADS)

  int result;
  pthread_attr_t attr;
#     if defined (ACE_HAS_PTHREADS_DRAFT4)
  if (::pthread_attr_create (&attr) != 0)
#     else /* ACE_HAS_PTHREADS_DRAFT4 */
  if (::pthread_attr_init (&attr) != 0)
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */
      return -1;

  // *** Set Stack Size
#     if defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
  if (stacksize < ACE_NEEDS_HUGE_THREAD_STACKSIZE)
    stacksize = ACE_NEEDS_HUGE_THREAD_STACKSIZE;
#     endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

#     if defined (CHORUS)
  // If it is a super actor, we can't set stacksize.  But for the time
  // being we are all non-super actors.  Should be fixed to take care
  // of super actors!!!
  if (stacksize == 0)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
  else if (stacksize < ACE_CHORUS_DEFAULT_MIN_STACK_SIZE)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
#     endif /*CHORUS */

  if (stacksize != 0)
    {
      size_t size = stacksize;

#     if defined (PTHREAD_STACK_MIN)
      if (size < ACE_static_cast (size_t, PTHREAD_STACK_MIN))
        size = PTHREAD_STACK_MIN;
#     endif /* PTHREAD_STACK_MIN */

#     if !defined (ACE_LACKS_THREAD_STACK_SIZE)      // JCEJ 12/17/96
#       if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
      if (::pthread_attr_setstacksize (&attr, size) != 0)
#       else
      if (ACE_ADAPT_RETVAL(pthread_attr_setstacksize (&attr, size), result) == -1)
#       endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */
        {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
          ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
          return -1;
        }
#     else
      ACE_UNUSED_ARG (size);
#     endif /* !ACE_LACKS_THREAD_STACK_SIZE */
    }

  // *** Set Stack Address
#     if !defined (ACE_LACKS_THREAD_STACK_ADDR)
  if (stack != 0)
    {
      if (::pthread_attr_setstackaddr (&attr, stack) != 0)
        {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
          ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
          return -1;
        }
    }
#     else
  ACE_UNUSED_ARG (stack);
#     endif /* !ACE_LACKS_THREAD_STACK_ADDR */

  // *** Deal with various attributes
  if (flags != 0)
    {
      // *** Set Detach state
#     if !defined (ACE_LACKS_SETDETACH)
      if (ACE_BIT_ENABLED (flags, THR_DETACHED)
          || ACE_BIT_ENABLED (flags, THR_JOINABLE))
        {
          int dstate = PTHREAD_CREATE_JOINABLE;

          if (ACE_BIT_ENABLED (flags, THR_DETACHED))
            dstate = PTHREAD_CREATE_DETACHED;

#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          if (::pthread_attr_setdetach_np (&attr, dstate) != 0)
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
#         if defined (ACE_HAS_PTHREADS_DRAFT6)
          if (::pthread_attr_setdetachstate (&attr, &dstate) != 0)
#         else
          if (ACE_ADAPT_RETVAL(::pthread_attr_setdetachstate (&attr, dstate),
                               result) != 0)
#         endif /* ACE_HAS_PTHREADS_DRAFT6 */
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
            {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }

      // Note: if ACE_LACKS_SETDETACH and THR_DETACHED is enabled, we
      // call ::pthread_detach () below.  If THR_DETACHED is not
      // enabled, we call ::pthread_detach () in the Thread_Manager,
      // after joining with the thread.
#     endif /* ACE_LACKS_SETDETACH */

      // *** Set Policy
#     if !defined (ACE_LACKS_SETSCHED)
      // If we wish to set the priority explicitly, we have to enable
      // explicit scheduling, and a policy, too.
      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          ACE_SET_BITS (flags, THR_EXPLICIT_SCHED);
          if (ACE_BIT_DISABLED (flags, THR_SCHED_FIFO)
              && ACE_BIT_DISABLED (flags, THR_SCHED_RR)
              && ACE_BIT_DISABLED (flags, THR_SCHED_DEFAULT))
            ACE_SET_BITS (flags, THR_SCHED_DEFAULT);
        }

      if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO)
          || ACE_BIT_ENABLED (flags, THR_SCHED_RR)
          || ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
        {
          int spolicy;

#       if defined (ACE_HAS_ONLY_SCHED_OTHER)
            // Solaris, thru version 2.6, only supports SCHED_OTHER.
            spolicy = SCHED_OTHER;
#       else
          // Make sure to enable explicit scheduling, in case we didn't
          // enable it above (for non-default priority).
          ACE_SET_BITS (flags, THR_EXPLICIT_SCHED);

          if (ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
            spolicy = SCHED_OTHER;
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            spolicy = SCHED_FIFO;
#         if defined (SCHED_IO)
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            spolicy = SCHED_IO;
#         else
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            {
              errno = ENOSYS;
              return -1;
            }
#         endif /* SCHED_IO */
          else
            spolicy = SCHED_RR;

#         if defined (ACE_HAS_FSU_PTHREADS)
          int ret;
          switch (spolicy)
            {
            case SCHED_FIFO:
            case SCHED_RR:
              ret = 0;
              break;
            default:
              ret = 22;
              break;
            }
          if (ret != 0)
            {
              ::pthread_attr_destroy (&attr);
              return -1;
            }
#         endif    /*  ACE_HAS_FSU_PTHREADS */

#       endif /* ACE_HAS_ONLY_SCHED_OTHER */

#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          result = ::pthread_attr_setsched (&attr, spolicy);
#       elif defined (ACE_HAS_PTHREADS_DRAFT6)
          result = ::pthread_attr_setschedpolicy (&attr, spolicy);
#       else  /* draft 7 or std */
          ACE_ADAPT_RETVAL(::pthread_attr_setschedpolicy (&attr, spolicy),
                           result);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
          if (result != 0)
              {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
                ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
                ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
                return -1;
              }
        }

      // *** Set Priority (use reasonable default priorities)
#       if defined(ACE_HAS_PTHREADS_STD)
      // If we wish to explicitly set a scheduling policy, we also
      // have to specify a priority.  We choose a "middle" priority as
      // default.  Maybe this is also necessary on other POSIX'ish
      // implementations?
      if ((ACE_BIT_ENABLED (flags, THR_SCHED_FIFO)
           || ACE_BIT_ENABLED (flags, THR_SCHED_RR)
           || ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
          && priority == ACE_DEFAULT_THREAD_PRIORITY)
        {
          if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            priority = ACE_THR_PRI_FIFO_DEF;
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_RR))
            priority = ACE_THR_PRI_RR_DEF;
          else // THR_SCHED_DEFAULT
            priority = ACE_THR_PRI_OTHER_DEF;
        }
#       endif /* ACE_HAS_PTHREADS_STD */
      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          struct sched_param sparam;
          ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);

#       if defined (ACE_HAS_IRIX62_THREADS)
          sparam.sched_priority = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#       elif defined (PTHREAD_MAX_PRIORITY) && !defined(ACE_HAS_PTHREADS_STD)
          /* For MIT pthreads... */
          sparam.prio = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#       elif defined(ACE_HAS_PTHREADS_STD) && !defined (ACE_HAS_STHREADS)
          // The following code forces priority into range.
          if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_FIFO_MAX, ACE_MAX (ACE_THR_PRI_FIFO_MIN, priority));
          else if (ACE_BIT_ENABLED(flags, THR_SCHED_RR))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_RR_MAX, ACE_MAX (ACE_THR_PRI_RR_MIN, priority));
          else // Default policy, whether set or not
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_OTHER_MAX, ACE_MAX (ACE_THR_PRI_OTHER_MIN, priority));
#       elif defined (PRIORITY_MAX)
          sparam.sched_priority = ACE_MIN (priority, PRIORITY_MAX);
#       else
          sparam.sched_priority = priority;
#       endif /* ACE_HAS_IRIX62_THREADS */

#       if defined (ACE_HAS_FSU_PTHREADS)
         if (sparam.sched_priority >= PTHREAD_MIN_PRIORITY
             && sparam.sched_priority <= PTHREAD_MAX_PRIORITY)
           attr.prio = sparam.sched_priority;
         else
           {
             pthread_attr_destroy (&attr);
             errno = EINVAL;
             return -1;
           }
#       else
         {
#         if defined (sun)
           // Solaris POSIX only allows priorities > 0 to
           // ::pthread_attr_setschedparam.  If a priority of 0 was
           // requested, set the thread priority after creating it, below.
           if (priority > 0)
#         endif /* sun */
             {
#         if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
               result = ::pthread_attr_setprio (&attr,
                                                sparam.sched_priority);
#         else /* this is draft 7 or std */
               ACE_ADAPT_RETVAL(::pthread_attr_setschedparam (&attr, &sparam),
                                result);
#         endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */
               if (result != 0)
                 {
#         if defined (ACE_HAS_PTHREADS_DRAFT4)
                   ::pthread_attr_delete (&attr);
#         else /* ACE_HAS_PTHREADS_DRAFT4 */
                   ::pthread_attr_destroy (&attr);
#         endif /* ACE_HAS_PTHREADS_DRAFT4 */
                   return -1;
                 }
             }
         }
#       endif    /* ACE_HAS_FSU_PTHREADS */
        }

      // *** Set scheduling explicit or inherited
      if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED)
          || ACE_BIT_ENABLED (flags, THR_EXPLICIT_SCHED))
        {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
          int sched = PTHREAD_DEFAULT_SCHED;
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
          int sched = PTHREAD_EXPLICIT_SCHED;
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
          if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED))
            sched = PTHREAD_INHERIT_SCHED;
          if (::pthread_attr_setinheritsched (&attr, sched) != 0)
            {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }
#     else /* ACE_LACKS_SETSCHED */
      ACE_UNUSED_ARG (priority);
#     endif /* ACE_LACKS_SETSCHED */

      // *** Set Scope
#     if !defined (ACE_LACKS_THREAD_PROCESS_SCOPING)
      if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM)
          || ACE_BIT_ENABLED (flags, THR_SCOPE_PROCESS))
        {
          int scope = PTHREAD_SCOPE_PROCESS;
          if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM))
            scope = PTHREAD_SCOPE_SYSTEM;

          if (::pthread_attr_setscope (&attr, scope) != 0)
            {
#       if defined (ACE_HAS_PTHREADS_DRAFT4)
              ::pthread_attr_delete (&attr);
#       else /* ACE_HAS_PTHREADS_DRAFT4 */
              ::pthread_attr_destroy (&attr);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
              return -1;
            }
        }
#     endif /* !ACE_LACKS_THREAD_PROCESS_SCOPING */

      if (ACE_BIT_ENABLED (flags, THR_NEW_LWP))
        {
          // Increment the number of LWPs by one to emulate the
          // Solaris semantics.
          int lwps = ACE_OS::thr_getconcurrency ();
          if (lwps == -1)
            {
              if (errno == ENOTSUP)
                {
                  // Suppress the ENOTSUP because it's harmless.
                  errno = 0;
                }
              else
                {
                  // This should never happen on Solaris:
                  // ::thr_getconcurrency () should always succeed.
                  return -1;
                }
            }
          else
            {
              if (ACE_OS::thr_setconcurrency (lwps + 1) == -1)
                {
                  if (errno == ENOTSUP)
                    {
                      // Unlikely:  ::thr_getconcurrency () is supported but
                      // ::thr_setconcurrency () is not?
                    }
                  else
                    {
                      return -1;
                    }
                }
            }
        }
    }

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
  ACE_OSCALL (::pthread_create (thr_id, attr,
                                thread_args->entry_point (),
                                thread_args),
              int, -1, result);

#       if defined (ACE_LACKS_SETDETACH)
  if (ACE_BIT_ENABLED (flags, THR_DETACHED))
    {
#         if defined (HPUX_10)
    // HP-UX DCE threads' pthread_detach will smash thr_id if it's
    // just given as an argument.  This will cause ACE_Thread_Manager
    // (if it's doing this create) to lose track of the new thread
    // since the ID will be passed back equal to 0.  So give
    // pthread_detach a junker to scribble on.
      ACE_thread_t  junker;
      cma_handle_assign(thr_id, &junker);
      ::pthread_detach (&junker);
#         else
      ::pthread_detach (thr_id);
#         endif /* HPUX_10 */
    }
#       endif /* ACE_LACKS_SETDETACH */

  ::pthread_attr_delete (&attr);

#     elif defined (ACE_HAS_PTHREADS_DRAFT6)
  ACE_OSCALL (::pthread_create (thr_id, &attr,
                                thread_args->entry_point (),
                                thread_args),
              int, -1, result);
  ::pthread_attr_destroy (&attr);

#     else /* this is draft 7 or std */
  ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (thr_id,
                                                  &attr,
                                                  thread_args->entry_point (),
                                                  thread_args),
                                result),
              int, -1, result);
  ::pthread_attr_destroy (&attr);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */

  // This is a Solaris or POSIX implementation of pthreads,
  // where we assume that ACE_thread_t and ACE_hthread_t are the same.
  // If this *isn't* correct on some platform, please let us know.
  if (result != -1)
    *thr_handle = *thr_id;

#     if defined (sun)
        // If the priority is 0, then we might have to set it now
        // because we couldn't set it with
        // ::pthread_attr_setschedparam, as noted above.  This doesn't
        // provide strictly correct behavior, because the thread was
        // created (above) with the priority of its parent.  (That
        // applies regardless of the inherit_sched attribute: if it
        // was PTHREAD_INHERIT_SCHED, then it certainly inherited its
        // parent's priority.  If it was PTHREAD_EXPLICIT_SCHED, then
        // "attr" was initialized by the Solaris ::pthread_attr_init
        // () to contain NULL for the priority, which indicated to
        // Solaris ::pthread_create () to inherit the parent
        // priority.)
        if (priority == 0)
          {
            // Check the priority of this thread, which is the parent
            // of the newly created thread.  If it is 0, then the
            // newly created thread will have inherited the priority
            // of 0, so there's no need to explicitly set it.
            struct sched_param sparam;
            int policy = 0;
            ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_getschedparam (thr_self (),
                                                                   &policy,
                                                                   &sparam),
                                          result), int,
                        -1, result);

            if (sparam.sched_priority != 0)
              {
                ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);
                // The memset to 0 sets the priority to 0, so we don't need
                // to explicitly set sparam.sched_priority.

                // The only policy supported by by Solaris, thru version 2.6,
                // is SCHED_OTHER, so that's hard-coded below.
                ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (
                                                        *thr_id,
                                                        SCHED_OTHER,
                                                        &sparam),
                                                     result),
                                   int, -1);
              }
          }

#       if defined (ACE_NEEDS_LWP_PRIO_SET)
#         if 0
            // It would be useful if we could make this work.  But, it
            // requires a mechanism for determining the ID of an LWP
            // to which another thread is bound.  Is there a way to do
            // that?  Instead, just rely on the code in
            // ACE_Thread_Adapter::invoke () to set the LWP priority.

            // If the thread is bound, then set the priority on its LWP.
            if (ACE_BIT_ENABLED (flags, THR_BOUND))
              {
                ACE_Sched_Params sched_params (ACE_BIT_ENABLED (flags,
                  THR_SCHED_FIFO) || ACE_BIT_ENABLED (flags,
                  THR_SCHED_RR)  ?  ACE_SCHED_FIFO  :ACE_SCHED_OTHER,
                                               priority);
                result = ACE_OS::lwp_setparams (sched_params,
                                      /* ? How do we find the ID of the LWP
                                           to which *thr_id is bound? */);
              }
#         endif /* 0 */
#       endif /* ACE_NEEDS_LWP_PRIO_SET */

#     endif /* sun */
  return result;
#   elif defined (ACE_HAS_STHREADS)
  int result;
  int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

  if (priority != ACE_DEFAULT_THREAD_PRIORITY)
    // If we need to set the priority, then we need to start the
    // thread in a suspended mode.
    ACE_SET_BITS (flags, THR_SUSPENDED);

  ACE_OSCALL (ACE_ADAPT_RETVAL (::thr_create (stack, stacksize,
                                              thread_args->entry_point (),
                                              thread_args,
                                              flags, thr_id), result),
              int, -1, result);

  if (result != -1)
    {
      // With Solaris threads, ACE_thread_t and ACE_hthread_t are the same.
      *thr_handle = *thr_id;

      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          // Set the priority of the new thread and then let it
          // continue, but only if the user didn't start it suspended
          // in the first place!
          if ((result = ACE_OS::thr_setprio (*thr_id, priority)) != 0)
            {
              errno = result;
              return -1;
            }

          if (start_suspended == 0)
            {
              if ((result = ACE_OS::thr_continue (*thr_id)) != 0)
                {
                  errno = result;
                  return -1;
                }
            }
        }
    }
  return result;
#   elif defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (stack);
#     if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  if (ACE_BIT_ENABLED (flags, THR_USE_AFX))
    {
      CWinThread *cwin_thread =
        ::AfxBeginThread ((AFX_THREADPROC) &thread_args->entry_point (),
                          thread_args, priority, 0,
                          flags | THR_SUSPENDED);
      // Have to duplicate the handle because
      // CWinThread::~CWinThread() closes the original handle.
#       if !defined (ACE_HAS_WINCE)
      (void) ::DuplicateHandle (::GetCurrentProcess (),
                                cwin_thread->m_hThread,
                                ::GetCurrentProcess (),
                                thr_handle,
                                0,
                                TRUE,
                                DUPLICATE_SAME_ACCESS);
#       endif /* ! ACE_HAS_WINCE */
      *thr_id = cwin_thread->m_nThreadID;

      if (ACE_BIT_ENABLED (flags, THR_SUSPENDED) == 0)
        cwin_thread->ResumeThread ();
      // cwin_thread will be deleted in AfxThreadExit()
      // Warning: If AfxThreadExit() is called from within the
      // thread, ACE_TSS_Cleanup->exit() never gets called !
    }
  else
#     endif /* ACE_HAS_MFC */
    {
      int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        // If we need to set the priority, then we need to start the
        // thread in a suspended mode.
        ACE_SET_BITS (flags, THR_SUSPENDED);

      *thr_handle = (void *) ::_beginthreadex
        (0,
         stacksize,
         (unsigned (__stdcall *) (void *)) thread_args->entry_point (),
         thread_args,
         flags,
         (unsigned int *) thr_id);

      if (priority != ACE_DEFAULT_THREAD_PRIORITY && *thr_handle != 0)
        {
          // Set the priority of the new thread and then let it
          // continue, but only if the user didn't start it suspended
          // in the first place!
          ACE_OS::thr_setprio (*thr_handle, priority);

          if (start_suspended == 0)
            ACE_OS::thr_continue (*thr_handle);
        }
    }
#     if 0
  *thr_handle = ::CreateThread
    (0,
     stacksize,
     LPTHREAD_START_ROUTINE (thread_args->entry_point ()),
     thread_args,
     flags,
     thr_id);
#     endif /* 0 */

  // Close down the handle if no one wants to use it.
  if (thr_handle == &tmp_handle)
    ::CloseHandle (tmp_handle);

  if (*thr_handle != 0)
    return 0;
  else
    ACE_FAIL_RETURN (-1);
  /* NOTREACHED */

#   elif defined (ACE_PSOS)

  // stack is created in the task's memory region 0
  ACE_UNUSED_ARG (stack);

  // task creation and start flags are fixed
  ACE_UNUSED_ARG (flags);

  // lowest priority is reserved for the IDLE pSOS+ system daemon,
  // highest are reserved for high priority pSOS+ system daemons
  if (priority < PSOS_TASK_MIN_PRIORITY)
  {
    priority = PSOS_TASK_MIN_PRIORITY;
  }
  else if (priority > PSOS_TASK_MAX_PRIORITY)
  {
    priority = PSOS_TASK_MAX_PRIORITY;
  }

  ACE_hthread_t tid;
  *thr_handle = 0;

  // create the thread
  if (t_create ((char *) thr_id, // task name
                priority,        // (possibly adjusted) task priority
                stacksize,       // passed stack size is used for supervisor stack
                0,               // no user stack: tasks run strictly in supervisor mode
                T_LOCAL,         // local to the pSOS+ node (does not support pSOS+m)
                &tid)            // receives task id
      != 0)
  {
    return -1;
  }

  // pSOS tasks are passed an array of 4 u_longs
  u_long targs[4];
  targs[0] = (u_long) ACE_THREAD_ARGUMENT;
  targs[1] = 0;
  targs[2] = 0;
  targs[3] = 0;

  // start the thread
  if (t_start (tid,
               T_PREEMPT |            // Task can be preempted
               T_NOTSLICE |           // Task is not timesliced with other tasks at same priority
               T_NOASR |              // Task level signals disabled
               T_SUPV |               // Task runs strictly in supervisor mode
               T_ISR,                 // Hardware interrupts are enabled
               ACE_THREAD_FUNCTION,   // Task entry point
               targs)                 // Task argument(s)
      != 0)
  {
    return -1;
  }

  // store the task id in the handle and return success
  *thr_handle = tid;
  return 0;

#   elif defined (VXWORKS)
  // The hard-coded values below are what ::sp () would use.  (::sp ()
  // hardcodes priority to 100, flags to VX_FP_TASK, and stacksize to
  // 20,000.)  stacksize should be an even integer.  If a stack is not
  // specified, ::taskSpawn () is used so that we can set the
  // priority, flags, and stacksize.  If a stack is specified,
  // ::taskInit ()/::taskActivate() are used.

  // If called with thr_create() defaults, use same default values as ::sp ():
  if (priority == ACE_DEFAULT_THREAD_PRIORITY) priority = 100;
  // Assumes that there is a floating point coprocessor.  As noted
  // above, ::sp () hardcodes this, so we should be safe with it.
  if (flags == 0) flags = VX_FP_TASK;
  if (stacksize == 0) stacksize = 20000;

  const u_int thr_id_provided =
    thr_id  &&  *thr_id  &&  (*thr_id)[0] != ACE_THR_ID_ALLOCATED;

  ACE_hthread_t tid;
#     if 0 /* Don't support setting of stack, because it doesn't seem to work. */
  if (stack == 0)
    {
#     else
  ACE_UNUSED_ARG (stack);
#     endif /* 0 */
      // The call below to ::taskSpawn () causes VxWorks to assign a
      // unique task name of the form: "t" + an integer, because the
      // first argument is 0.
      tid = ::taskSpawn (thr_id_provided  ?  *thr_id  :  0,
                         priority,
                         (int) flags,
                         (int) stacksize,
                         thread_args->entry_point (),
                         (int) thread_args,
                         0, 0, 0, 0, 0, 0, 0, 0, 0);
#     if 0 /* Don't support setting of stack, because it doesn't seem to work. */
    }
  else
    {
      // If a task name (thr_id) was not supplied, then the task will
      // not have a unique name.  That's VxWorks' behavior.

      // Carve out a TCB at the beginning of the stack space.  The TCB
      // occupies 400 bytes with VxWorks 5.3.1/I386.
      WIND_TCB *tcb = (WIND_TCB *) stack;

      // The TID is defined to be the address of the TCB.
      int status = ::taskInit (tcb,
                               thr_id_provided  ?  *thr_id  :  0,
                               priority,
                               (int) flags,
                               (char *) stack + sizeof (WIND_TCB),
                               (int) (stacksize - sizeof (WIND_TCB)),
                               thread_args->entry_point (),
                               (int) thread_args,
                               0, 0, 0, 0, 0, 0, 0, 0, 0);

      if (status == OK)
        {
          // The task was successfully initialized, now activate it.
          status = ::taskActivate ((ACE_hthread_t) tcb);
        }

      tid = status == OK  ?  (ACE_hthread_t) tcb  :  ERROR;
    }
#     endif /* 0 */

  if (tid == ERROR)
    return -1;
  else
    {
      if (! thr_id_provided  &&  thr_id)
        {
          if (*thr_id  &&  (*thr_id)[0] == ACE_THR_ID_ALLOCATED)
            // *thr_id was allocated by the Thread_Manager.  ::taskTcb
            // (int tid) returns the address of the WIND_TCB (task
            // control block).  According to the ::taskSpawn()
            // documentation, the name of the new task is stored at
            // pStackBase, but is that of the current task?  If so, it
            // might be a bit quicker than this extraction of the tcb
            // . . .
            ACE_OS::strncpy (*thr_id + 1, ::taskTcb (tid)->name, 10);
          else
            // *thr_id was not allocated by the Thread_Manager.
            // Pass back the task name in the location pointed to
            // by thr_id.
            *thr_id = ::taskTcb (tid)->name;
        }
      // else if the thr_id was provided, there's no need to overwrite
      // it with the same value (string).  If thr_id is 0, then we can't
      // pass the task name back.

      if (thr_handle)
        *thr_handle = tid;

      return 0;
    }

#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (func);
  ACE_UNUSED_ARG (args);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (thr_id);
  ACE_UNUSED_ARG (thr_handle);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (stack);
  ACE_UNUSED_ARG (stacksize);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

void
ACE_OS::thr_exit (void *status)
{
// ACE_TRACE ("ACE_OS::thr_exit");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_PTHREADS)
    ::pthread_exit (status);
#   elif defined (ACE_HAS_STHREADS)
    ::thr_exit (status);
#   elif defined (ACE_HAS_WTHREADS)
    // Can't call it here because on NT, the thread is exited
    // directly by ACE_Thread_Adapter::invoke ().
    //   ACE_TSS_Cleanup::instance ()->exit (status);

#     if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
    int using_afx = -1;
    ACE_Thread_Descriptor *td = ACE_Log_Msg::instance ()->thr_desc ();
    if (td)
      using_afx = ACE_BIT_ENABLED (td->flags (), THR_USE_AFX);
#     endif /* ACE_HAS_MFC && (ACE_HAS_MFC != 0) */

    // Call TSS destructors.
    ACE_OS::cleanup_tss (0 /* not main thread */);

    // Exit the thread.
    // Allow CWinThread-destructor to be invoked from AfxEndThread.
    // _endthreadex will be called from AfxEndThread so don't exit the
    // thread now if we are running an MFC thread.
#     if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
    if (using_afx != -1)
      {
        if (using_afx)
          ::AfxEndThread ((DWORD)status);
        else
          ::_endthreadex ((DWORD) status);
      }
    else
      {
        // Not spawned by ACE_Thread_Manager, use the old buggy
        // version.  You should seriously consider using
        // ACE_Thread_Manager to spawn threads.  The following code is
        // know to cause some problem.
        CWinThread *pThread = ::AfxGetThread ();
        if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
          ::_endthreadex ((DWORD) status);
        else
          ::AfxEndThread ((DWORD)status);
      }
#     else
    ::_endthreadex ((DWORD) status);
#     endif /* ACE_HAS_MFC && ACE_HAS_MFS != 0*/

#   elif defined (VXWORKS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    *((int *) status) = ::taskDelete (tid);
#   elif defined (ACE_PSOS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    *((u_long *) status) = ::t_delete (tid);
#   endif /* ACE_HAS_PTHREADS */
# else
  ACE_UNUSED_ARG (status);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::lwp_getparams (ACE_Sched_Params &sched_params)
{
# if defined (ACE_HAS_STHREADS) || defined (sun)
  // Get the class TS and RT class IDs.
  ACE_id_t rt_id;
  ACE_id_t ts_id;
  if (ACE_OS::scheduling_class ("RT", rt_id) == -1
      || ACE_OS::scheduling_class ("TS", ts_id) == -1)
    return -1;

  // Get this LWP's scheduling parameters.
  pcparms_t pcparms;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcparms, 0, sizeof pcparms);
  pcparms.pc_cid = PC_CLNULL;

  if (ACE_OS::priority_control (P_LWPID,
                                P_MYID,
                                PC_GETPARMS,
                                (char *) &pcparms) == -1)
    return -1;
  else if (pcparms.pc_cid == rt_id)
    {
      // RT class.
      rtparms_t rtparms;
      ACE_OS::memcpy (&rtparms, pcparms.pc_clparms, sizeof rtparms);

      sched_params.policy (ACE_SCHED_FIFO);
      sched_params.priority (rtparms.rt_pri);
      sched_params.scope (ACE_SCOPE_THREAD);
      ACE_Time_Value quantum (rtparms.rt_tqsecs,
                              rtparms.rt_tqnsecs == RT_TQINF
                              ? 0 : rtparms.rt_tqnsecs * 1000);
      sched_params.quantum (quantum);
      return 0;
    }
  else if (pcparms.pc_cid == ts_id)
    {
      /* TS class */
      tsparms_t tsparms;
      ACE_OS::memcpy (&tsparms, pcparms.pc_clparms, sizeof tsparms);

      sched_params.policy (ACE_SCHED_RR);
      sched_params.priority (tsparms.ts_upri);
      sched_params.scope (ACE_SCOPE_THREAD);
      return 0;
    }
  else
    return -1;

# else  /* ! ACE_HAS_STHREADS && ! sun */
  ACE_UNUSED_ARG (sched_params);
  ACE_NOTSUP_RETURN (-1);
# endif /* ! ACE_HAS_STHREADS && ! sun */
}

int
ACE_OS::lwp_setparams (const ACE_Sched_Params &sched_params)
{
# if defined (ACE_HAS_STHREADS) || defined (sun)
  ACE_Sched_Params lwp_params (sched_params);
  lwp_params.scope (ACE_SCOPE_LWP);
  return ACE_OS::sched_params (lwp_params);
# else  /* ! ACE_HAS_STHREADS && ! sun */
  ACE_UNUSED_ARG (sched_params);
  ACE_NOTSUP_RETURN (-1);
# endif /* ! ACE_HAS_STHREADS && ! sun */
}

# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
int
ACE_OS::thr_setspecific (ACE_OS_thread_key_t key, void *data)
{
// ACE_TRACE ("ACE_OS::thr_setspecific");
#   if defined (ACE_HAS_THREADS)
#     if defined (ACE_HAS_PTHREADS)
#       if defined (ACE_HAS_FSU_PTHREADS)
      // Call pthread_init() here to initialize threads package.  FSU
      // threads need an initialization before the first thread constructor.
      // This seems to be the one; however, a segmentation fault may
      // indicate that another pthread_init() is necessary, perhaps in
      // Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
      // more than once, so another call to pthread_init will not adversely
      // affect existing threads.
      pthread_init ();
#       endif /*  ACE_HAS_FSU_PTHREADS */
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data), ace_result_), int, -1);
#     elif defined (ACE_HAS_STHREADS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
#     elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
#     endif /* ACE_HAS_STHREADS */
#   else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (data);
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

int
ACE_OS::thr_setspecific (ACE_thread_key_t key, void *data)
{
// ACE_TRACE ("ACE_OS::thr_setspecific");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_TSS_EMULATION)
    ACE_KEY_INDEX (key_index, key);
    if (key_index >= ACE_TSS_Emulation::total_keys ())
      {
        errno = EINVAL;
        data = 0;
        return -1;
      }
    else
      {
        ACE_TSS_Emulation::ts_object (key) = data;
        ACE_TSS_Cleanup::instance ()->key_used (key);

        return 0;
      }
#   elif defined (ACE_HAS_PTHREADS)
#     if defined (ACE_HAS_FSU_PTHREADS)
      // Call pthread_init() here to initialize threads package.  FSU
      // threads need an initialization before the first thread constructor.
      // This seems to be the one; however, a segmentation fault may
      // indicate that another pthread_init() is necessary, perhaps in
      // Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
      // more than once, so another call to pthread_init will not adversely
      // affect existing threads.
      pthread_init ();
#     endif /*  ACE_HAS_FSU_PTHREADS */

#     if defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_setspecific (key, data), int, -1);
#     else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data),
                                         ace_result_),
                       int, -1);
#     endif /* ACE_HAS_PTHREADS_DRAFT4, 6 */

#   elif defined (ACE_HAS_STHREADS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
#   elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (data);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_keyfree (ACE_thread_key_t key)
{
// ACE_TRACE ("ACE_OS::thr_keyfree");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_TSS_EMULATION)
    return ACE_TSS_Cleanup::instance ()->remove (key);
#   elif defined (ACE_HAS_PTHREADS_DRAFT4) || defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
#   elif defined (ACE_HAS_PTHREADS)
    return ::pthread_key_delete (key);
#   elif defined (ACE_HAS_THR_KEYDELETE)
    return ::thr_keydelete (key);
#   elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
#   elif defined (ACE_HAS_WTHREADS)
    // Extract out the thread-specific table instance and free up
    // the key and destructor.
    ACE_TSS_Cleanup::instance ()->remove (key);
    ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::TlsFree (key), ace_result_), int, -1);
#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
int
ACE_OS::thr_keycreate (ACE_OS_thread_key_t *key,
#   if defined (ACE_HAS_THR_C_DEST)
                       ACE_THR_C_DEST dest,
#   else
                       ACE_THR_DEST dest,
#   endif /* ACE_HAS_THR_C_DEST */
                       void *inst)
{
// ACE_TRACE ("ACE_OS::thr_keycreate");
#   if defined (ACE_HAS_THREADS)
#     if defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);


#       if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#       elif defined (ACE_HAS_PTHREADS_DRAFT4)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#       elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#       else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#       endif /* ACE_HAS_STDARG_THR_DEST */

#     elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
#     elif defined (ACE_HAS_WTHREADS)
    *key = ::TlsAlloc ();

    if (*key != ACE_SYSCALL_FAILED)
      {
        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      ACE_FAIL_RETURN (-1);
      /* NOTREACHED */
#     endif /* ACE_HAS_STHREADS */
#   else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (dest);
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

int
ACE_OS::thr_keycreate (ACE_thread_key_t *key,
# if defined (ACE_HAS_THR_C_DEST)
                       ACE_THR_C_DEST dest,
# else
                       ACE_THR_DEST dest,
# endif /* ACE_HAS_THR_C_DEST */
                       void *inst)
{
// ACE_TRACE ("ACE_OS::thr_keycreate");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_TSS_EMULATION)
    if (ACE_TSS_Emulation::next_key (*key) == 0)
      {
        ACE_TSS_Emulation::tss_destructor (*key, dest);

        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      {
        errno = EAGAIN;
        return -1;
      }
#   elif defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);

#     if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#     elif defined (ACE_HAS_PTHREADS_DRAFT4)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#     elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#     else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#     endif /* ACE_HAS_STDARG_THR_DEST */

#   elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
#   elif defined (ACE_HAS_WTHREADS)
    *key = ::TlsAlloc ();

    if (*key != ACE_SYSCALL_FAILED)
      {
        // Extract out the thread-specific table instance and stash away
        // the key and destructor so that we can free it up later on...
        return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
      }
    else
      ACE_FAIL_RETURN (-1);
      /* NOTREACHED */
#   endif /* ACE_HAS_STHREADS */
# else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (dest);
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_key_used (ACE_thread_key_t key)
{
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  ACE_TSS_Cleanup::instance ()->key_used (key);
  return 0;
# else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */
}

int
ACE_OS::thr_key_detach (void *inst)
{
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  if (ACE_TSS_Cleanup::lockable ())
    return ACE_TSS_Cleanup::instance()->detach (inst);
  else
    // We're in static constructor/destructor phase.  Don't
    // try to use the ACE_TSS_Cleanup instance because its lock
    // might not have been constructed yet, or might have been
    // destroyed already.  Just leak the key . . .
    return -1;
# else
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */
}

int
ACE_OS::argv_to_string (ASYS_TCHAR **argv,
                        ASYS_TCHAR *&buf,
                        int substitute_env_args)
{
  if (argv == 0 || argv[0] == 0)
    return 0;

  int buf_len = 0;

  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
      ASYS_TCHAR *temp;

      // Account for environment variables.
      if (substitute_env_args
          && (argv[i][0] == '$'
              && (temp = ACE_OS::getenv (&argv[i][1])) != 0))
        buf_len += ACE_OS::strlen (temp);
      else
        buf_len += ACE_OS::strlen (argv[i]);

      // Add one for the extra space between each string.
      buf_len++;
    }

  // Step through all argv params and copy each one into buf; separate
  // each param with white space.

  ACE_NEW_RETURN (buf,
                  ASYS_TCHAR[buf_len + 1],
                  0);

  // Initial null charater to make it a null string.
  buf[0] = '\0';
  ASYS_TCHAR *end = buf;
  int j;

  for (j = 0; argv[j] != 0; j++)
    {
      ASYS_TCHAR *temp;

      // Account for environment variables.
      if (substitute_env_args
      && (argv[j][0] == '$'
              && (temp = ACE_OS::getenv (&argv[j][1])) != 0))
        end = ACE::strecpy (end, temp);
      else
        end = ACE::strecpy (end, argv[j]);

      // Replace the null char that strecpy put there with white
      // space.
      end[-1] = ' ';
    }

  // Null terminate the string.
  *end = '\0';
  // The number of arguments.
  return j;
}

int
ACE_OS::string_to_argv (ASYS_TCHAR *buf,
                        size_t &argc,
                        ASYS_TCHAR **&argv,
                        int substitute_env_args)
{
  // Reset the number of arguments
  argc = 0;

  if (buf == 0)
    return -1;

  ASYS_TCHAR *cp = buf;

  // First pass: count arguments.

  // '#' is the start-comment token..
  while (*cp != '\0' && *cp != '#')
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*cp))
        cp++;

      // Increment count and move to next whitespace..
      if (*cp != '\0')
        argc++;

      // Grok quotes....
      if (*cp == '\'' || *cp == '"')
        {
          ASYS_TCHAR quote = *cp;

          // Scan past the string..
          for (cp++; *cp != '\0' && *cp != quote; cp++)
            continue;

          // '\0' implies unmatched quote..
          if (*cp == '\0')
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("unmatched %c detected\n"),
                          quote));
              argc--;
              break;
            }
          else
            cp++;
        }
      else // Skip over non-whitespace....
        while (*cp != '\0' && !ACE_OS::ace_isspace (*cp))
          cp++;
    }

  // Second pass: copy arguments.
  ASYS_TCHAR arg[ACE_DEFAULT_ARGV_BUFSIZ];
  ASYS_TCHAR *argp = arg;

  // Make sure that the buffer we're copying into is always large
  // enough.
  if (cp - buf >= ACE_DEFAULT_ARGV_BUFSIZ)
    ACE_NEW_RETURN (argp,
                    ASYS_TCHAR[cp - buf + 1],
                    -1);

  // Make a new argv vector of argc + 1 elements.
  ACE_NEW_RETURN (argv,
                  ASYS_TCHAR *[argc + 1],
                  -1);

  ASYS_TCHAR *ptr = buf;

  for (size_t i = 0; i < argc; i++)
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*ptr))
        ptr++;

      // Copy next argument and move to next whitespace..
      if (*ptr == '\'' || *ptr == '"')
        {
          ASYS_TCHAR quote = *ptr++;

          for (cp = argp;
               *ptr != '\0' && *ptr != quote;
               ptr++, cp++)
            {
              // @@ We can probably remove this since we ensure it's
              // big enough earlier!
              ACE_ASSERT (unsigned (cp - argp) < ACE_DEFAULT_ARGV_BUFSIZ);
              *cp = *ptr;
            }

          *cp = '\0';
          if (*ptr == quote)
            ptr++;
        }
      else
        {
          for (cp = arg;
               *ptr && !ACE_OS::ace_isspace (*ptr);
               ptr++, cp++)
            {
              // @@ We can probably remove this since we ensure it's
              // big enough earlier!
              ACE_ASSERT (u_int (cp - argp) < ACE_DEFAULT_ARGV_BUFSIZ);
              *cp = *ptr;
            }

          *cp = '\0';
        }

      // Check for environment variable substitution here.
#if !defined (ACE_HAS_WINCE)
      if (substitute_env_args)
        ACE_ALLOCATOR_RETURN (argv[i],
                              ACE_OS::strenvdup (arg),
                              -1);
      else
#endif /* ACE_HAS_WINCE */
        ACE_ALLOCATOR_RETURN (argv[i],
                              ACE_OS::strdup (arg),
                              -1);
    }

  if (argp != arg)
    delete [] argp;

  argv[argc] = 0;
  return 0;
}

// Create a contiguous command-line argument buffer with each arg
// separated by spaces.

pid_t
ACE_OS::fork_exec (ASYS_TCHAR *argv[])
{
# if defined (ACE_WIN32)
  ASYS_TCHAR *buf;

  if (ACE_OS::argv_to_string (argv, buf) != -1)
    {
      PROCESS_INFORMATION process_info;
#   if !defined (ACE_HAS_WINCE)
      STARTUPINFO startup_info;
      ACE_OS::memset ((void *) &startup_info,
                      0,
                      sizeof startup_info);
      startup_info.cb = sizeof startup_info;

      if (::CreateProcess (0,
                           (LPTSTR) ACE_WIDE_STRING (buf),
                           0, // No process attributes.
                           0,  // No thread attributes.
                           TRUE, // Allow handle inheritance.
                           0, // Don't create a new console window.
                           0, // No environment.
                           0, // No current directory.
                           &startup_info,
                           &process_info))
#   else
      if (::CreateProcess (0,
                           (LPTSTR) buf,
                           0, // No process attributes.
                           0,  // No thread attributes.
                           FALSE, // Can's inherit handles on CE
                           0, // Don't create a new console window.
                           0, // No environment.
                           0, // No current directory.
                           0, // Can't use startup info on CE
                           &process_info))
#   endif /* ! ACE_HAS_WINCE */
        {
          // Free resources allocated in kernel.
          ACE_OS::close (process_info.hThread);
          ACE_OS::close (process_info.hProcess);
          // Return new process id.
          delete [] buf;
          return process_info.dwProcessId;
        }
    }

  // CreateProcess failed.
  return -1;
# elif defined (CHORUS)
  return ACE_OS::execv (argv[0], argv);
# else
      pid_t result = ACE_OS::fork ();

      switch (result)
        {
        case -1:
          // Error.
          return -1;
        case 0:
          // Child process.
          if (ACE_OS::execv (argv[0], argv) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "%p Exec failed\n"));

              // If the execv fails, this child needs to exit.
              ACE_OS::exit (errno);
            }
        default:
          // Server process.  The fork succeeded.
          return result;
        }
# endif /* ACE_WIN32 */
}

# if defined (ACE_LACKS_WRITEV)

// "Fake" writev for operating systems without it.  Note that this is
// thread-safe.

extern "C" int
writev (ACE_HANDLE handle, ACE_WRITEV_TYPE iov[], int n)
{
  // ACE_TRACE ("::writev");

  size_t length = 0;
  int i;

  // Determine the total length of all the buffers in <iov>.
  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;

#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf, char[length], -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  char *ptr = buf;

  for (i = 0; i < n; i++)
    {
      ACE_OS::memcpy (ptr, iov[i].iov_base, iov[i].iov_len);
      ptr += iov[i].iov_len;
    }

  ssize_t result = ACE::write_n (handle, buf, length);
#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return result;
}
# endif /* ACE_LACKS_WRITEV */

# if defined (ACE_LACKS_READV)

// "Fake" readv for operating systems without it.  Note that this is
// thread-safe.

extern "C" int
readv (ACE_HANDLE handle,
       ACE_READV_TYPE *iov,
       int n)
{
// ACE_TRACE ("readv");

  ssize_t length = 0;
  int i;

  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;
#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf, char[length], -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  length = ACE::read_n (handle, buf, length);

  if (length != -1)
    {
      char *ptr = buf;
      int copyn = length;

      for (i = 0;
           i < n && copyn > 0;
           i++)
        {
          ACE_OS::memcpy (iov[i].iov_base, ptr,
                          // iov_len is int on some platforms, size_t on others
                          copyn > (int) iov[i].iov_len
                            ? (size_t) iov[i].iov_len
                            : (size_t) copyn);
          ptr += iov[i].iov_len;
          copyn -= iov[i].iov_len;
        }
    }

#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return length;
}
# endif /* ACE_LACKS_READV */

# if defined (ACE_NEEDS_FTRUNCATE)
extern "C" int
ftruncate (ACE_HANDLE handle, long len)
{
  struct flock fl;
  fl.l_whence = 0;
  fl.l_len = 0;
  fl.l_start = len;
  fl.l_type = F_WRLCK;

  return ::fcntl (handle, F_FREESP, &fl);
}
# endif /* ACE_NEEDS_FTRUNCATE */

# if defined (ACE_LACKS_MKTEMP) && !defined (ACE_HAS_MOSTLY_UNICODE_APIS)
char *
ACE_OS::mktemp (char *s)
{
  // ACE_TRACE ("ACE_OS::mktemp");
  if (s == 0)
    // check for null template string failed!
    return 0;
  else
    {
      char *xxxxxx = ACE_OS::strstr (s, "XXXXXX");

      if (xxxxxx == 0)
        // the template string doesn't contain "XXXXXX"!
        return s;
      else
        {
          char unique_letter = 'a';
          struct stat sb;

          // Find an unused filename for this process.  It is assumed
          // that the user will open the file immediately after
          // getting this filename back (so, yes, there is a race
          // condition if multiple threads in a process use the same
          // template).  This appears to match the behavior of the
          // Solaris 2.5 mktemp().
          ACE_OS::sprintf (xxxxxx, "%05d%c", getpid (), unique_letter);
          while (ACE_OS::stat (s, &sb) >= 0)
            {
              if (++unique_letter <= 'z')
                ACE_OS::sprintf (xxxxxx, "%05d%c", getpid (), unique_letter);
              else
                {
                  // maximum of 26 unique files per template, per process
                  ACE_OS::sprintf (xxxxxx, "%s", "");
                  return s;
                }
            }
        }
      return s;
    }
}
# endif /* ACE_LACKS_MKTEMP && !ACE_HAS_MOSTLY_UNICODE_APIS */

int
ACE_OS::socket_init (int version_high, int version_low)
{
# if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ == 0)
    {
      // cout << "WSAStartup" << endl;
      WORD version_requested = MAKEWORD (version_high, version_low);
      WSADATA wsa_data;
      int error = WSAStartup (version_requested, &wsa_data);

      if (error != 0)
#   if defined (ACE_HAS_WINCE)
        {
          wchar_t fmt[] = ACE_TEXT ("%s failed, WSAGetLastError returned %d");
          wchar_t buf[80];  // @@ Eliminate magic number.
          ACE_OS::sprintf (buf, fmt, ACE_TEXT ("WSAStartup"), error);
          ::MessageBox (NULL, buf, ACE_TEXT ("WSAStartup failed!"), MB_OK);
        }
#   else
        cerr << "WSAStartup failed, WSAGetLastError returned " << error << endl;
#   endif /* ACE_HAS_WINCE */

      ACE_OS::socket_initialized_ = 1;
    }
# else
  ACE_UNUSED_ARG (version_high);
  ACE_UNUSED_ARG (version_low);
# endif /* ACE_WIN32 */
  return 0;
}

int
ACE_OS::socket_fini (void)
{
# if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ != 0)
    {
      // cout << "WSACleanup" << endl;
      if (WSACleanup () != 0)
        {
          int error = ::WSAGetLastError ();
#   if defined (ACE_HAS_WINCE)
          wchar_t fmt[] = ACE_TEXT ("%s failed, WSAGetLastError returned %d");
          wchar_t buf[80];  // @@ Eliminate magic number.
          ACE_OS::sprintf (buf, fmt, ACE_TEXT ("WSACleanup"), error);
          ::MessageBox (NULL, buf , ACE_TEXT ("WSACleanup failed!"), MB_OK);
#   else
          cerr << "WSACleanup failed, WSAGetLastError returned " << error << endl;
#   endif /* ACE_HAS_WINCE */
        }
      ACE_OS::socket_initialized_ = 0;
    }
# endif /* ACE_WIN32 */
  return 0;
}

# if defined (ACE_LACKS_SYS_NERR)
int sys_nerr = ERRMAX + 1;
# endif /* ACE_LACKS_SYS_NERR */

# if defined (VXWORKS)
#   include /**/ <usrLib.h>   /* for ::sp() */

// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.
//
// usage: -> spa main, "arg1", "arg2"
//
// All arguments must be quoted, even numbers.
int
spa (FUNCPTR entry, ...)
{
  static const unsigned int MAX_ARGS = 10;
  static char *argv[MAX_ARGS];
  va_list pvar;
  unsigned int argc;

  // Hardcode a program name because the real one isn't available
  // through the VxWorks shell.
  argv[0] = "ace_main";

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.  This function could be used to increase that
  // limit, but then it couldn't depend on the trailing 0.  So, the
  // number of arguments would have to be passed.
  va_start (pvar, entry);

  for (argc = 1; argc <= MAX_ARGS; ++argc)
    {
      argv[argc] = va_arg (pvar, char *);

      if (argv[argc] == 0)
        break;
    }

  if (argc > MAX_ARGS  &&  argv[argc-1] != 0)
    {
      // try to read another arg, and warn user if the limit was exceeded
      if (va_arg (pvar, char *) != 0)
        ACE_OS::fprintf (stderr, "spa(): number of arguments limited to %d\n",
                         MAX_ARGS);
    }
  else
    {
      // fill unused argv slots with 0 to get rid of leftovers
      // from previous invocations
      for (unsigned int i = argc; i <= MAX_ARGS; ++i)
        argv[i] = 0;
    }

  // The hard-coded options are what ::sp () uses, except for the
  // larger stack size (instead of ::sp ()'s 20000).
  const int ret = ::taskSpawn (argv[0],    // task name
                               100,        // task priority
                               VX_FP_TASK, // task options
                               1000000,    // stack size
                               entry,      // entry point
                               argc,       // first argument to main ()
                               (int) argv, // second argument to main ()
                               0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::taskSpawn () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}
# endif /* VXWORKS */

# if !defined (ACE_HAS_SIGINFO_T)
siginfo_t::siginfo_t (ACE_HANDLE handle)
  : si_handle_ (handle),
    si_handles_ (&handle)
{
}

siginfo_t::siginfo_t (ACE_HANDLE *handles)
  : si_handle_ (handles[0]),
    si_handles_ (handles)
{
}
# endif /* ACE_HAS_SIGINFO_T */

pid_t
ACE_OS::fork (const char *program_name)
{
  // ACE_TRACE ("ACE_OS::fork");
# if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (program_name);
  ACE_NOTSUP_RETURN (pid_t (-1));
# else
  pid_t pid =
# if defined (ACE_HAS_STHREADS)
    ::fork1 ();
#else
    ::fork ();
#endif /* ACE_HAS_STHREADS */

  if (pid == 0)
    ACE_LOG_MSG->sync (program_name);

  return pid;
# endif /* ACE_WIN32 */
}

void
ACE_Cleanup::cleanup (void *)
{
  delete this;
}

// This is necessary to work around nasty problems with MVS C++.

extern "C" void
ace_mutex_lock_cleanup_adapter (void *args)
{
  ACE_OS::mutex_lock_cleanup (args);
}

ACE_Thread_ID::ACE_Thread_ID (ACE_thread_t thread_id,
                              ACE_hthread_t thread_handle)
  : thread_id_ (thread_id),
    thread_handle_ (thread_handle)
{
}

ACE_Thread_ID::ACE_Thread_ID (const ACE_Thread_ID &id)
  : thread_id_ (id.thread_id_),
    thread_handle_ (id.thread_handle_)
{
}

ACE_thread_t
ACE_Thread_ID::id (void)
{
  return this->thread_id_;
}

void
ACE_Thread_ID::id (ACE_thread_t thread_id)
{
  this->thread_id_ = thread_id;
}

ACE_hthread_t
ACE_Thread_ID::handle (void)
{
  return this->thread_handle_;
}

void
ACE_Thread_ID::handle (ACE_hthread_t thread_handle)
{
  this->thread_handle_ = thread_handle;
}

int
ACE_Thread_ID::operator== (const ACE_Thread_ID &rhs) const
{
  return ACE_OS::thr_cmp (this->thread_handle_, rhs.thread_handle_) == 0
    && ACE_OS::thr_equal (this->thread_id_, rhs.thread_id_) == 0;
}

int
ACE_Thread_ID::operator!= (const ACE_Thread_ID &rhs) const
{
  return !(*this == rhs);
}

int
ACE_OS::inet_aton (const char *host_name, struct in_addr *addr)
{
  ACE_UINT32 ip_addr = ACE_OS::inet_addr (host_name);

  if (ip_addr == (ACE_UINT32) htonl ((ACE_UINT32) ~0)
      // Broadcast addresses are weird...
      && ACE_OS::strcmp (host_name, "255.255.255.255") != 0)
    return 0;
  else if (addr == 0)
    return 0;
  else
    {
#if !defined(_UNICOS)
      ACE_OS::memcpy ((void *) addr, (void *) &ip_addr, sizeof ip_addr);
#else /* ! _UNICOS */
      // on UNICOS, perform assignment to bitfield, since doing the above
      // actually puts the address outside of the 32-bit bitfield
      addr->s_addr = ip_addr;
#endif /* ! _UNICOS */
      return 1;
    }
}

ssize_t
ACE_OS::pread (ACE_HANDLE handle,
               void *buf,
               size_t nbytes,
               off_t offset)
{
# if defined (ACE_HAS_P_READ_WRITE)
#   if defined (ACE_WIN32)

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              NULL,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  DWORD altered_position = ::SetFilePointer (handle,
                                             offset,
                                             NULL,
                                             FILE_BEGIN);
  if (altered_position == 0xFFFFFFFF)
    return -1;

  DWORD bytes_read;

#     if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)

  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  BOOL result = ::ReadFile (handle,
                            buf,
                            nbytes,
                            &bytes_read,
                            &overlapped);

  if (result == FALSE)
    {
      if (::GetLastError () != ERROR_IO_PENDING)
        return -1;

      else
        {
          result = ::GetOverlappedResult (handle,
                                          &overlapped,
                                          &bytes_read,
                                          TRUE);
          if (result == FALSE)
            return -1;
        }
    }

#     else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  BOOL result = ::ReadFile (handle,
                            buf,
                            nbytes,
                            &bytes_read,
                            NULL);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        NULL,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_read;

#   else /* ACE_WIN32 */

  return ::pread (handle, buf, nbytes, offset);

#   endif /* ACE_WIN32 */

# else /* ACE_HAS_P_READ_WRITE */

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_SET);

  if (original_position == -1)
    return -1;

  // Go to the correct position
  off_t altered_position = ACE_OS::lseek (handle,
                                          offset,
                                          SEEK_SET);

  if (altered_position == -1)
    return -1;

  ssize_t bytes_read = ACE_OS::read (handle,
                                     buf,
                                     nbytes);

  if (bytes_read == -1)
    return -1;

  if (ACE_OS::lseek (handle,
                     original_position,
                     SEEK_SET) == -1)
    return -1;

  return bytes_read;

# endif /* ACE_HAD_P_READ_WRITE */
}

ssize_t
ACE_OS::pwrite (ACE_HANDLE handle,
                const void *buf,
                size_t nbytes,
                off_t offset)
{
# if defined (ACE_HAS_P_READ_WRITE)
#   if defined (ACE_WIN32)

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              NULL,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  DWORD altered_position = ::SetFilePointer (handle,
                                             offset,
                                             NULL,
                                             FILE_BEGIN);
  if (altered_position == 0xFFFFFFFF)
    return -1;

  DWORD bytes_written;

#     if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)

  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  BOOL result = ::WriteFile (handle,
                             buf,
                             nbytes,
                             &bytes_written,
                             &overlapped);

  if (result == FALSE)
    {
      if (::GetLastError () != ERROR_IO_PENDING)
        return -1;

      else
        {
          result = ::GetOverlappedResult (handle,
                                          &overlapped,
                                          &bytes_written,
                                          TRUE);
          if (result == FALSE)
            return -1;
        }
    }

#     else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  BOOL result = ::WriteFile (handle,
                             buf,
                             nbytes,
                             &bytes_written,
                             NULL);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        NULL,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_written;

#   else /* ACE_WIN32 */

  return ::pwrite (handle, buf, nbytes, offset);
#   endif /* ACE_WIN32 */
# else /* ACE_HAS_P_READ_WRITE */

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_SET);
  if (original_position == -1)
    return -1;

  // Go to the correct position
  off_t altered_position = ACE_OS::lseek (handle,
                                          offset,
                                          SEEK_SET);
  if (altered_position == -1)
    return -1;

  ssize_t bytes_written = ACE_OS::write (handle,
                                         buf,
                                         nbytes);
  if (bytes_written == -1)
    return -1;

  if (ACE_OS::lseek (handle,
                     original_position,
                     SEEK_SET) == -1)
    return -1;

  return bytes_written;
# endif /* ACE_HAD_P_READ_WRITE */
}

ACE_HANDLE
ACE_OS::open (const char *filename,
              int mode,
              int perms,
              LPSECURITY_ATTRIBUTES sa)
{
  // ACE_TRACE ("ACE_OS::open");
#if defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (filename);
  ACE_UNUSED_ARG (mode);
  ACE_UNUSED_ARG (perms);
  ACE_UNUSED_ARG (sa);
  return 0;
#elif defined (ACE_WIN32)
  ACE_UNUSED_ARG (perms);

  DWORD access = GENERIC_READ;
  if (ACE_BIT_ENABLED (mode, O_WRONLY))
    access = GENERIC_WRITE;
  else if (ACE_BIT_ENABLED (mode, O_RDWR))
    access = GENERIC_READ | GENERIC_WRITE;

  DWORD creation = OPEN_EXISTING;

  if ((mode & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL))
    creation = CREATE_NEW;
  else if ((mode & (_O_CREAT | _O_TRUNC)) == (_O_CREAT | _O_TRUNC))
    creation = CREATE_ALWAYS;
  else if (ACE_BIT_ENABLED (mode, _O_CREAT))
    creation = OPEN_ALWAYS;
  else if (ACE_BIT_ENABLED (mode, _O_TRUNC))
    creation = TRUNCATE_EXISTING;

  DWORD flags = 0;

  if (ACE_BIT_ENABLED (mode, _O_TEMPORARY))
    flags |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;

  if (ACE_BIT_ENABLED (mode, FILE_FLAG_WRITE_THROUGH))
    flags |= FILE_FLAG_WRITE_THROUGH;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_OVERLAPPED))
    flags |= FILE_FLAG_OVERLAPPED;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_NO_BUFFERING))
    flags |= FILE_FLAG_NO_BUFFERING;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_RANDOM_ACCESS))
    flags |= FILE_FLAG_RANDOM_ACCESS;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_SEQUENTIAL_SCAN))
    flags |= FILE_FLAG_SEQUENTIAL_SCAN;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_DELETE_ON_CLOSE))
    flags |= FILE_FLAG_DELETE_ON_CLOSE;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_BACKUP_SEMANTICS))
    flags |= FILE_FLAG_BACKUP_SEMANTICS;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_POSIX_SEMANTICS))
    flags |= FILE_FLAG_POSIX_SEMANTICS;

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock = 0;)

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      ACE_MT
        (
          ace_os_monitor_lock =
            ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
          ace_os_monitor_lock->acquire ();
         )
    }

  ACE_HANDLE h = ::CreateFileA (filename, access,
#if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
                                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
#else
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
#endif /* ACE_HAS_WINNT4 && ACE_HAS_WINNT4 != 0 */
                                ACE_OS::default_win32_security_attributes (sa),
                                creation,
                                flags,
                                0);

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      if (h != ACE_INVALID_HANDLE)
        {
          ::SetFilePointer (h, 0, 0, FILE_END);
        }

      ACE_MT (ace_os_monitor_lock->release ();)
    }

  if (h == ACE_INVALID_HANDLE)
    ACE_FAIL_RETURN (h);
  else
    return h;
#elif defined (ACE_PSOS)
  ACE_UNUSED_ARG (mode);
  ACE_UNUSED_ARG (perms);
  ACE_UNUSED_ARG (sa);
# if defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (filename);
  return 0;
# else
  unsigned long result, handle;
  result = ::open_f (&handle, ACE_const_cast(char *, filename), 0);
  if (result != 0)
    {
      errno = result;
      return ACE_static_cast (ACE_HANDLE, -1);
    }
  return ACE_static_cast (ACE_HANDLE, handle);
# endif /* defined (ACE_PSOS_LACKS_PHILE) */
#else
  ACE_UNUSED_ARG (sa);
  ACE_OSCALL_RETURN (::open (filename, mode, perms), ACE_HANDLE, -1);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_UNICODE) && defined (ACE_WIN32)

ACE_HANDLE
ACE_OS::open (const wchar_t *filename,
              int mode,
              int perms,
              LPSECURITY_ATTRIBUTES sa)
{
  ACE_UNUSED_ARG (perms);
  // ACE_TRACE ("ACE_OS::open");
  // Warning: This function ignores _O_APPEND
  DWORD access = GENERIC_READ;
  if (ACE_BIT_ENABLED (mode, O_WRONLY))
    access = GENERIC_WRITE;
  else if (ACE_BIT_ENABLED (mode, O_RDWR))
    access = GENERIC_READ | GENERIC_WRITE;

  DWORD creation = OPEN_EXISTING;

  if ((mode & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL))
    creation = CREATE_NEW;
  else if ((mode & (_O_CREAT | _O_TRUNC)) == (_O_CREAT | _O_TRUNC))
    creation = CREATE_ALWAYS;
  else if (ACE_BIT_ENABLED (mode, _O_CREAT))
    creation = OPEN_ALWAYS;
  else if (ACE_BIT_ENABLED (mode, _O_TRUNC))
    creation = TRUNCATE_EXISTING;

  DWORD flags = 0;

  if (ACE_BIT_ENABLED (mode, _O_TEMPORARY))
    flags |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;

  if (ACE_BIT_ENABLED (mode, FILE_FLAG_WRITE_THROUGH))
    flags |= FILE_FLAG_WRITE_THROUGH;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_OVERLAPPED))
    flags |= FILE_FLAG_OVERLAPPED;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_NO_BUFFERING))
    flags |= FILE_FLAG_NO_BUFFERING;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_RANDOM_ACCESS))
    flags |= FILE_FLAG_RANDOM_ACCESS;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_SEQUENTIAL_SCAN))
    flags |= FILE_FLAG_SEQUENTIAL_SCAN;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_DELETE_ON_CLOSE))
    flags |= FILE_FLAG_DELETE_ON_CLOSE;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_BACKUP_SEMANTICS))
    flags |= FILE_FLAG_BACKUP_SEMANTICS;
  if (ACE_BIT_ENABLED (mode, FILE_FLAG_POSIX_SEMANTICS))
    flags |= FILE_FLAG_POSIX_SEMANTICS;

  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock = 0;)

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      ACE_MT
        (
          ace_os_monitor_lock =
            ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
          ace_os_monitor_lock->acquire ();
         )
    }

  ACE_HANDLE h = ::CreateFileW (filename, access,
#if !defined (ACE_HAS_WINCE)
                                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
#else
                                // @@ CE doesn't support FILE_SHARE_DELETE???
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
#endif /* !ACE_HAS_WINCE */
                                ACE_OS::default_win32_security_attributes (sa),
                                creation,
                                flags,
                                0);

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      if (h != ACE_INVALID_HANDLE)
        {
          ::SetFilePointer (h, 0, 0, FILE_END);
        }

      ACE_MT (ace_os_monitor_lock->release ();)
    }

  if (h == ACE_INVALID_HANDLE)
    ACE_FAIL_RETURN (h);
  else
    return h;
}

#endif /* ACE_HAS_UNICODE && ACE_WIN32 */

# if defined (ACE_LACKS_DIFFTIME)
double
ACE_OS::difftime (time_t t1, time_t t0)
{
  /* return t1 - t0 in seconds */
  struct tm tms[2], *ptms[2], temp;
  double seconds;
  double days;
  int swap = 0;

  /* extract the tm structure from time_t */
  ptms[1] = gmtime_r (&t1, &tms[1]);
  if (ptms[1] == 0) return 0.0;

  ptms[0] = gmtime_r (&t0, &tms[0]);
  if (ptms[0] == 0) return 0.0;

    /* make sure t1 is > t0 */
  if (tms[1].tm_year < tms[0].tm_year)
    swap = 1;
  else if (tms[1].tm_year == tms[0].tm_year)
    {
      if (tms[1].tm_yday < tms[0].tm_yday)
        swap = 1;
      else if (tms[1].tm_yday == tms[0].tm_yday)
        {
          if (tms[1].tm_hour < tms[0].tm_hour)
            swap = 1;
          else if (tms[1].tm_hour == tms[0].tm_hour)
            {
              if (tms[1].tm_min < tms[0].tm_min)
                swap = 1;
              else if (tms[1].tm_min == tms[0].tm_min)
                {
                  if (tms[1].tm_sec < tms[0].tm_sec)
                    swap = 1;
                }
            }
        }
    }

  if (swap)
    temp = tms[0], tms[0] = tms[1], tms[1] = temp;

  seconds = 0.0;
  if (tms[1].tm_year > tms[0].tm_year)
    {
      // Accumulate the time until t[0] catches up to t[1]'s year.
      seconds = 60 - tms[0].tm_sec;
      tms[0].tm_sec = 0;
      tms[0].tm_min += 1;
      seconds += 60 * (60 - tms[0].tm_min);
      tms[0].tm_min = 0;
      tms[0].tm_hour += 1;
      seconds += 60*60 * (24 - tms[0].tm_hour);
      tms[0].tm_hour = 0;
      tms[0].tm_yday += 1;

#   define ISLEAPYEAR(y) ((y)&3u?0:(y)%25u?1:(y)/25u&12?0:1)

      if (ISLEAPYEAR(tms[0].tm_year))
        seconds += 60*60*24 * (366 - tms[0].tm_yday);
      else
        seconds += 60*60*24 * (365 - tms[0].tm_yday);

      tms[0].tm_yday = 0;
      tms[0].tm_year += 1;

      while (tms[1].tm_year > tms[0].tm_year)
        {
          if (ISLEAPYEAR(tms[0].tm_year))
            seconds += 60*60*24 * 366;
          else
            seconds += 60*60*24 * 365;

          tms[0].tm_year += 1;
        }

#   undef ISLEAPYEAR

    }
  else
    {
      // Normalize
      if (tms[1].tm_sec < tms[0].tm_sec)
        {
          if (tms[1].tm_min == 0)
            {
              if (tms[1].tm_hour == 0)
                {
                  tms[1].tm_yday -= 1;
                  tms[1].tm_hour += 24;
                }
              tms[1].tm_hour -= 1;
              tms[1].tm_min += 60;
            }
          tms[1].tm_min -= 1;
          tms[1].tm_sec += 60;
        }
      tms[1].tm_sec -= tms[0].tm_sec;

      if (tms[1].tm_min < tms[0].tm_min)
        {
          if (tms[1].tm_hour == 0)
            {
              tms[1].tm_yday -= 1;
              tms[1].tm_hour += 24;
            }
          tms[1].tm_hour -= 1;
          tms[1].tm_min += 60;
        }
      tms[1].tm_min -= tms[0].tm_min;

      if (tms[1].tm_hour < tms[0].tm_hour)
        {
          tms[1].tm_yday -= 1;
          tms[1].tm_hour += 24;
        }
      tms[1].tm_hour -= tms[0].tm_hour;

      tms[1].tm_yday -= tms[0].tm_yday;
    }

  // accumulate the seconds
  seconds += tms[1].tm_sec;
  seconds += 60 * tms[1].tm_min;
  seconds += 60*60 * tms[1].tm_hour;
  seconds += 60*60*24 * tms[1].tm_yday;

  return seconds;
}
# endif /* ACE_LACKS_DIFFTIME */

# if defined (ACE_HAS_WINCE)
wchar_t *
ACE_OS::ctime (const time_t *t)
{
  wchar_t buf[26];              // 26 is a "magic number" ;)
  return ACE_OS::ctime_r (t, buf, 26);
}

wchar_t *
ACE_OS::ctime_r (const time_t *clock,
                 wchar_t *buf,
                 int buflen)
{
  // buflen must be at least 26 wchar_t long.
  if (buflen < 26)              // Again, 26 is a magic number.
    return 0;
  // This is really stupid, converting FILETIME to timeval back and
  // forth.  It assumes FILETIME and DWORDLONG are the same structure
  // internally.
  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (DWORDLONG) *clock * 10000 * 1000
                     + ACE_Time_Value::FILETIME_to_timval_skew;
  FILETIME file_time;
  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

#   if 1
  FILETIME localtime;
  SYSTEMTIME systime;
  FileTimeToLocalFileTime (&file_time, &localtime);
  FileTimeToSystemTime (&localtime, &systime);
#   else
  SYSTEMTIME systime;
  FileTimeToSystemTime ((FILETIME *) &file_time, &systime);
#   endif /* 0 */
  ACE_OS::sprintf (buf, ACE_OS_CTIME_R_FMTSTR,
                   ACE_OS::day_of_week_name[systime.wDayOfWeek],
                   ACE_OS::month_name[systime.wMonth - 1],
                   systime.wDay,
                   systime.wHour,
                   systime.wMinute,
                   systime.wSecond,
                   systime.wYear);
  return buf;
}
# endif /* ACE_HAS_WINCE */

# if !defined (ACE_HAS_WINCE)
time_t
ACE_OS::mktime (struct tm *t)
{
  // ACE_TRACE ("ACE_OS::asctime");
#   if defined (ACE_HAS_MT_SAFE_MKTIME) || !defined (ACE_HAS_THREADS)
  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#   else
  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, (time_t) -1));

  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#   endif /* ACE_HAS_MT_SAFE_MKTIME */
}
# endif /* !ACE_HAS_WINCE */

# if !defined (ACE_HAS_THREADS) || defined (ACE_LACKS_RWLOCK_T)
int
ACE_OS::rwlock_init (ACE_rwlock_t *rw,
                     int type,
                     LPCTSTR name,
                     void *arg)
{
  // ACE_TRACE ("ACE_OS::rwlock_init");
#   if defined (ACE_HAS_THREADS) && defined (ACE_LACKS_RWLOCK_T)
  // NT, POSIX, and VxWorks don't support this natively.
  ACE_UNUSED_ARG (name);
  int result = -1;

  // Since we cannot use the user specified name for all three
  // objects, we will create three completely new names.
  TCHAR name1[ACE_UNIQUE_NAME_LEN];
  TCHAR name2[ACE_UNIQUE_NAME_LEN];
  TCHAR name3[ACE_UNIQUE_NAME_LEN];
  TCHAR name4[ACE_UNIQUE_NAME_LEN];

  ACE::unique_name ((const void *) &rw->lock_,
                    name1,
                    ACE_UNIQUE_NAME_LEN);
  ACE::unique_name ((const void *) &rw->waiting_readers_,
                    name2,
                    ACE_UNIQUE_NAME_LEN);
  ACE::unique_name ((const void *) &rw->waiting_writers_,
                    name3,
                    ACE_UNIQUE_NAME_LEN);
  ACE::unique_name ((const void *) &rw->waiting_important_writer_,
                    name4,
                    ACE_UNIQUE_NAME_LEN);

  if (ACE_OS::mutex_init (&rw->lock_, type, name1, arg) == 0
      && ACE_OS::cond_init (&rw->waiting_readers_, type, name2, arg) == 0
      && ACE_OS::cond_init (&rw->waiting_writers_, type, name3, arg) == 0
      && ACE_OS::cond_init (&rw->waiting_important_writer_, type, name4, arg) == 0)
  {
      // Success!
      rw->ref_count_ = 0;
      rw->num_waiting_writers_ = 0;
      rw->num_waiting_readers_ = 0;
      rw->important_writer_ = 0;

      result = 0;
    }

  if (result == -1)
    {
      int error = errno;
      ACE_OS::mutex_destroy (&rw->lock_);
      ACE_OS::cond_destroy (&rw->waiting_readers_);
      ACE_OS::cond_destroy (&rw->waiting_writers_);
      ACE_OS::cond_destroy (&rw->waiting_important_writer_);
      errno = error;
    }
  return result;
#   else
  ACE_UNUSED_ARG (rw);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ! ACE_HAS_THREADS || ACE_LACKS_RWLOCK_T */

#if defined (ACE_LACKS_COND_T)
// NOTE: The ACE_OS::cond_* functions for some non-Unix platforms are
// defined here either because they're too big to be inlined, or
// to avoid use before definition if they were inline.

int
ACE_OS::cond_destroy (ACE_cond_t *cv)
{
  // ACE_TRACE ("ACE_OS::cond_destroy");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_WTHREADS)
  ACE_OS::event_destroy (&cv->waiters_done_);
#   elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_OS::sema_destroy (&cv->waiters_done_);
#   endif /* VXWORKS */
  ACE_OS::thread_mutex_destroy (&cv->waiters_lock_);
  return ACE_OS::sema_destroy (&cv->sema_);
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_init (ACE_cond_t *cv, int type, LPCTSTR name, void *arg)
{
// ACE_TRACE ("ACE_OS::cond_init");
# if defined (ACE_HAS_THREADS)
  cv->waiters_ = 0;
  cv->was_broadcast_ = 0;

  int result = 0;
  if (ACE_OS::sema_init (&cv->sema_, 0, type, name, arg) == -1)
    result = -1;
  else if (ACE_OS::thread_mutex_init (&cv->waiters_lock_) == -1)
    result = -1;
#   if defined (VXWORKS) || defined (ACE_PSOS)
  else if (ACE_OS::sema_init (&cv->waiters_done_, 0, type) == -1)
#   else
  else if (ACE_OS::event_init (&cv->waiters_done_) == -1)
#   endif /* VXWORKS */
    result = -1;
  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_signal (ACE_cond_t *cv)
{
// ACE_TRACE ("ACE_OS::cond_signal");
# if defined (ACE_HAS_THREADS)
  // If there aren't any waiters, then this is a no-op.  Note that
  // this function *must* be called with the <external_mutex> held
  // since other wise there is a race condition that can lead to the
  // lost wakeup bug...  This is needed to ensure that the <waiters_>
  // value is not in an inconsistent internal state while being
  // updated by another thread.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  int have_waiters = cv->waiters_ > 0;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (have_waiters != 0)
    return ACE_OS::sema_post (&cv->sema_);
  else
    return 0; // No-op
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_broadcast (ACE_cond_t *cv)
{
// ACE_TRACE ("ACE_OS::cond_broadcast");
# if defined (ACE_HAS_THREADS)
  // The <external_mutex> must be locked before this call is made.

  // This is needed to ensure that <waiters_> and <was_broadcast_> are
  // consistent relative to each other.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  int have_waiters = 0;

  if (cv->waiters_ > 0)
    {
      // We are broadcasting, even if there is just one waiter...
      // Record the fact that we are broadcasting.  This helps the
      // cond_wait() method know how to optimize itself.  Be sure to
      // set this with the <waiters_lock_> held.
      cv->was_broadcast_ = 1;
      have_waiters = 1;
    }
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);
  int result = 0;
  if (have_waiters)
    {
      // Wake up all the waiters.
      if (ACE_OS::sema_post (&cv->sema_, cv->waiters_) == -1)
        result = -1;
      // Wait for all the awakened threads to acquire their part of
      // the counting semaphore.
#   if defined (VXWORKS) || defined (ACE_PSOS)
      else if (ACE_OS::sema_wait (&cv->waiters_done_) == -1)
#   else
      else if (ACE_OS::event_wait (&cv->waiters_done_) == -1)
#   endif /* VXWORKS */
        result = -1;
      // This is okay, even without the <waiters_lock_> held because
      // no other waiter threads can wake up to access it.
      cv->was_broadcast_ = 0;
    }
  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_wait (ACE_cond_t *cv,
                   ACE_mutex_t *external_mutex)
{
  // ACE_TRACE ("ACE_OS::cond_wait");
# if defined (ACE_HAS_THREADS)
  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;

#   if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    // This call will automatically release the mutex and wait on the semaphore.
    ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (external_mutex->proc_mutex_,
                                                            cv->sema_, INFINITE, FALSE),
                                     result),
                   int, -1, result);
  else
#   endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
    {
      // We keep the lock held just long enough to increment the count of
      // waiters by one.  Note that we can't keep it held across the call
      // to ACE_OS::sema_wait() since that will deadlock other calls to
      // ACE_OS::cond_signal().
      if (ACE_OS::mutex_unlock (external_mutex) != 0)
        return -1;

      // Wait to be awakened by a ACE_OS::cond_signal() or
      // ACE_OS::cond_broadcast().
      result = ACE_OS::sema_wait (&cv->sema_);
    }

  // Reacquire lock to avoid race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  // We're ready to return, so there's one less waiter.
  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  // Release the lock so that other collaborating threads can make
  // progress.
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result == -1)
    // Bad things happened, so let's just return below.
    /* NOOP */;
#   if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  else if (external_mutex->type_ == USYNC_PROCESS)
    {
      if (last_waiter)

        // This call atomically signals the <waiters_done_> event and
        // waits until it can acquire the mutex.  This is important to
        // prevent unfairness.
        ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (cv->waiters_done_,
                                                                external_mutex->proc_mutex_,
                                                                INFINITE, FALSE),
                                         result),
                       int, -1, result);
      else
        // We must always regain the <external_mutex>, even when
        // errors occur because that's the guarantee that we give to
        // our callers.
        ACE_OS::mutex_lock (external_mutex);

      return result;
      /* NOTREACHED */
    }
#   endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
  // If we're the last waiter thread during this particular broadcast
  // then let all the other threads proceed.
  else if (last_waiter)
#   if defined (VXWORKS) || defined (ACE_PSOS)
    ACE_OS::sema_post (&cv->waiters_done_);
#   else
    ACE_OS::event_signal (&cv->waiters_done_);
#   endif /* VXWORKS */

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::mutex_lock (external_mutex);

  return result;
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (external_mutex);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_timedwait (ACE_cond_t *cv,
                        ACE_mutex_t *external_mutex,
                        ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE_OS::cond_timedwait");
# if defined (ACE_HAS_THREADS)
  // Handle the easy case first.
  if (timeout == 0)
    return ACE_OS::cond_wait (cv, external_mutex);
#   if defined (ACE_HAS_WTHREADS) || defined (VXWORKS) || defined (ACE_PSOS)

  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  int error = 0;
  int msec_timeout;

  if (timeout->sec () == 0 && timeout->usec () == 0)
    msec_timeout = 0; // Do a "poll."
  else
    {
      // Note that we must convert between absolute time (which is
      // passed as a parameter) and relative time (which is what
      // WaitForSingleObjects() expects).
      ACE_Time_Value relative_time (*timeout - ACE_OS::gettimeofday ());

      // Watchout for situations where a context switch has caused the
      // current time to be > the timeout.
      if (relative_time < ACE_Time_Value::zero)
        msec_timeout = 0;
      else
        msec_timeout = relative_time.msec ();
    }

#     if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    // This call will automatically release the mutex and wait on the
    // semaphore.
    result = ::SignalObjectAndWait (external_mutex->proc_mutex_,
                                    cv->sema_,
                                    msec_timeout,
                                    FALSE);
  else
#     endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
    {
      // We keep the lock held just long enough to increment the count
      // of waiters by one.  Note that we can't keep it held across
      // the call to WaitForSingleObject since that will deadlock
      // other calls to ACE_OS::cond_signal().
      if (ACE_OS::mutex_unlock (external_mutex) != 0)
        return -1;

      // Wait to be awakened by a ACE_OS::signal() or
      // ACE_OS::broadcast().
#     if defined (ACE_WIN32)
#       if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
      result = ::WaitForSingleObject (cv->sema_, msec_timeout);
#       else /* ACE_USES_WINCE_SEMA_SIMULATION */
      // Can't use Win32 API on our simulated semaphores.
      result = ACE_OS::sema_wait (&cv->sema_,
                                  ACE_Time_Value (0, msec_timeout * 1000));
#       endif /* ACE_USES_WINCE_SEMA_SIMULATION */
#     elif defined (ACE_PSOS)
      // Inline the call to ACE_OS::sema_wait () because it takes an
      // ACE_Time_Value argument.  Avoid the cost of that conversion . . .
      u_long ticks = (KC_TICKS2SEC * msec_timeout) / ACE_ONE_SECOND_IN_MSECS;
      result = ::sm_p (cv->sema_.sema_, SM_WAIT, ticks);
#     elif defined (VXWORKS)
      // Inline the call to ACE_OS::sema_wait () because it takes an
      // ACE_Time_Value argument.  Avoid the cost of that conversion . . .
      int ticks_per_sec = ::sysClkRateGet ();
      int ticks = msec_timeout * ticks_per_sec / ACE_ONE_SECOND_IN_MSECS;
      result = ::semTake (cv->sema_.sema_, ticks);
#     endif /* ACE_WIN32 || VXWORKS */
    }

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

#     if defined (ACE_WIN32)
  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
      result = -1;
    }
#     elif defined (ACE_PSOS)
  if (result != 0)
    {
      switch (result)
        {
        case ERR_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = errno;
          break;
        }
      result = -1;
    }
#     elif defined (VXWORKS)
  if (result == ERROR)
    {
      switch (errno)
        {
        case S_objLib_OBJ_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = errno;
          break;
        }
      result = -1;
    }
#     endif /* ACE_WIN32 || VXWORKS */
#     if defined (ACE_HAS_SIGNAL_OBJECT_AND_WAIT)
  if (external_mutex->type_ == USYNC_PROCESS)
    {
      if (last_waiter)
        // This call atomically signals the <waiters_done_> event and
        // waits until it can acquire the mutex.  This is important to
        // prevent unfairness.
        ACE_WIN32CALL (ACE_ADAPT_RETVAL (::SignalObjectAndWait (cv->waiters_done_,
                                                                external_mutex->proc_mutex_,
                                                                INFINITE, FALSE),
                                         result),
                       int, -1, result);
      else
        // We must always regain the <external_Mutex>, even when
        // errors occur because that's the guarantee that we give to
        // our callers.
        ACE_OS::mutex_lock (external_mutex);

      return result;
      /* NOTREACHED */
    }
#     endif /* ACE_HAS_SIGNAL_OBJECT_AND_WAIT */
  else if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
#     if defined (ACE_WIN32)
    ACE_OS::event_signal (&cv->waiters_done_);
#     else
    ACE_OS::sema_post (&cv->waiters_done_);
#     endif /* ACE_WIN32 */

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::mutex_lock (external_mutex);

  errno = error;
  return result;
#   endif /* ACE_HAS_WTHREADS || ACE_HAS_VXWORKS || ACE_PSOS */
# else
  ACE_UNUSED_ARG (cv);
  ACE_UNUSED_ARG (external_mutex);
  ACE_UNUSED_ARG (timeout);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_THREADS */
}

# if defined (ACE_HAS_WTHREADS)
int
ACE_OS::cond_timedwait (ACE_cond_t *cv,
                        ACE_thread_mutex_t *external_mutex,
                        ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE_OS::cond_timedwait");
#   if defined (ACE_HAS_THREADS)
  // Handle the easy case first.
  if (timeout == 0)
    return ACE_OS::cond_wait (cv, external_mutex);

  // Prevent race conditions on the <waiters_> count.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  int error = 0;
  int msec_timeout;

  if (timeout->sec () == 0 && timeout->usec () == 0)
    msec_timeout = 0; // Do a "poll."
  else
    {
      // Note that we must convert between absolute time (which is
      // passed as a parameter) and relative time (which is what
      // WaitForSingleObjects() expects).
      ACE_Time_Value relative_time (*timeout - ACE_OS::gettimeofday ());

      // Watchout for situations where a context switch has caused the
      // current time to be > the timeout.
      if (relative_time < ACE_Time_Value::zero)
        msec_timeout = 0;
      else
        msec_timeout = relative_time.msec ();
    }

  // We keep the lock held just long enough to increment the count of
  // waiters by one.  Note that we can't keep it held across the call
  // to WaitForSingleObject since that will deadlock other calls to
  // ACE_OS::cond_signal().
  if (ACE_OS::thread_mutex_unlock (external_mutex) != 0)
    return -1;

  // Wait to be awakened by a ACE_OS::signal() or ACE_OS::broadcast().
#     if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
  result = ::WaitForSingleObject (cv->sema_, msec_timeout);
#     else
  // Can't use Win32 API on simulated semaphores.
  result = ACE_OS::sema_wait (&cv->sema_,
                              ACE_Time_Value (0, msec_timeout * 1000));
#     endif /* ACE_USES_WINCE_SEMA_SIMULATION */

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
      result = -1;
    }

  if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
    ACE_OS::event_signal (&cv->waiters_done_);

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::thread_mutex_lock (external_mutex);
  errno = error;
  return result;
#   else
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}

int
ACE_OS::cond_wait (ACE_cond_t *cv,
                   ACE_thread_mutex_t *external_mutex)
{
  // ACE_TRACE ("ACE_OS::cond_wait");
#   if defined (ACE_HAS_THREADS)
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);
  cv->waiters_++;
  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  int result = 0;
  int error = 0;

  // We keep the lock held just long enough to increment the count of
  // waiters by one.  Note that we can't keep it held across the call
  // to ACE_OS::sema_wait() since that will deadlock other calls to
  // ACE_OS::cond_signal().
  if (ACE_OS::thread_mutex_unlock (external_mutex) != 0)
    return -1;

  // Wait to be awakened by a ACE_OS::cond_signal() or
  // ACE_OS::cond_broadcast().
#     if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
  result = ::WaitForSingleObject (cv->sema_, INFINITE);
#     else
  // Can't use Win32 API on simulated semaphores.
  result = ACE_OS::sema_wait (&cv->sema_);
#     endif /* ACE_USES_WINCE_SEMA_SIMULATION */

  // Reacquire lock to avoid race conditions.
  ACE_OS::thread_mutex_lock (&cv->waiters_lock_);

  cv->waiters_--;

  int last_waiter = cv->was_broadcast_ && cv->waiters_ == 0;

  ACE_OS::thread_mutex_unlock (&cv->waiters_lock_);

  if (result != WAIT_OBJECT_0)
    {
      switch (result)
        {
        case WAIT_TIMEOUT:
          error = ETIME;
          break;
        default:
          error = ::GetLastError ();
          break;
        }
    }
  else if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
    ACE_OS::event_signal (&cv->waiters_done_);

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::thread_mutex_lock (external_mutex);

  // Reset errno in case mutex_lock() also fails...
  errno = error;
  return result;
#   else
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_THREADS */
}
# endif /* ACE_HAS_WTHREADS */
#endif /* ACE_LACKS_COND_T */

void
ACE_OS::exit (int status)
{
  // ACE_TRACE ("ACE_OS::exit");

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_HAS_WINCE) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
  // Shut down the ACE_Object_Manager, if it had registered its exit_hook.
  // With ACE_HAS_NONSTATIC_OBJECT_MANAGER, the ACE_Object_Manager is
  // instantiated on the main's stack.  ::exit () doesn't destroy it.
  if (exit_hook_)
    (*exit_hook_) ();
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

#if !defined (ACE_HAS_WINCE)
# if defined (ACE_WIN32)
  ::ExitProcess ((UINT) status);
# elif defined (ACE_PSOSIM)
  ::u_exit (status);
# else
  ::exit (status);
# endif /* ACE_WIN32 */
#else
  // @@ This is not exactly the same as ExitProcess.  But this is the
  // closest one I can get.
  ::TerminateProcess (::GetCurrentProcess (), status);
#endif /* ACE_HAS_WINCE */
}

# if defined (ACE_PSOS)

// bit masks and shifts for prying info out of the pSOS time encoding
const u_long ACE_PSOS_Time_t::year_mask     = 0x0000FFFFul;
const u_long ACE_PSOS_Time_t::month_mask    = 0x000000FFul;
const u_long ACE_PSOS_Time_t::day_mask      = 0x000000FFul;
const u_long ACE_PSOS_Time_t::hour_mask     = 0x0000FFFFul;
const u_long ACE_PSOS_Time_t::minute_mask   = 0x000000FFul;
const u_long ACE_PSOS_Time_t::second_mask   = 0x000000FFul;
const int ACE_PSOS_Time_t::year_shift   = 16;
const int ACE_PSOS_Time_t::month_shift  = 8;
const int ACE_PSOS_Time_t::hour_shift   = 16;
const int ACE_PSOS_Time_t::minute_shift = 8;
const int ACE_PSOS_Time_t::year_origin = 1900;
const int ACE_PSOS_Time_t::month_origin = 1;

// maximum number of clock ticks supported
const u_long ACE_PSOS_Time_t::max_ticks = ~0UL;

ACE_PSOS_Time_t::ACE_PSOS_Time_t (void)
  : date_ (0),
    time_ (0),
    ticks_ (0)
{
}

// default ctor: date, time, and ticks all zeroed

ACE_PSOS_Time_t::ACE_PSOS_Time_t (const timespec_t& t)
{
  struct tm* tm_struct = ACE_OS::gmtime (&(t.tv_sec));

  // Encode date values from tm struct into pSOS date bit array.
  date_ = (ACE_PSOS_Time_t::year_mask &
           ACE_static_cast (u_long,
                            tm_struct->tm_year + ACE_PSOS_Time_t::year_origin)) <<
    ACE_PSOS_Time_t::year_shift;
  date_ |= (ACE_PSOS_Time_t::month_mask &
            ACE_static_cast (u_long,
                             tm_struct->tm_mon + ACE_PSOS_Time_t::month_origin)) <<
    ACE_PSOS_Time_t::month_shift;
  date_ |= ACE_PSOS_Time_t::day_mask &
    ACE_static_cast (u_long, tm_struct->tm_mday);
  // Encode time values from tm struct into pSOS time bit array.
  time_ = (ACE_PSOS_Time_t::hour_mask  &
            ACE_static_cast (u_long, tm_struct->tm_hour)) <<
    ACE_PSOS_Time_t::hour_shift;
  time_ |= (ACE_PSOS_Time_t::minute_mask &
            ACE_static_cast (u_long, tm_struct->tm_min)) <<
    ACE_PSOS_Time_t::minute_shift;
  time_ |= ACE_PSOS_Time_t::second_mask &
    ACE_static_cast (u_int, tm_struct->tm_sec);

  // encode nanoseconds as system clock ticks
  ticks_ = ACE_static_cast (u_long,
                            ((ACE_static_cast (double, t.tv_nsec) *
                              ACE_static_cast (double, KC_TICKS2SEC)) /
                             ACE_static_cast (double, 1000000000)));

}

// ctor from a timespec_t

ACE_PSOS_Time_t::operator timespec_t (void)
{
  struct tm tm_struct;

  // Decode date and time bit arrays and fill in fields of tm_struct.

  tm_struct.tm_year =
    ACE_static_cast (int, (ACE_PSOS_Time_t::year_mask &
                           (date_ >> ACE_PSOS_Time_t::year_shift))) -
    ACE_PSOS_Time_t::year_origin;
  tm_struct.tm_mon =
    ACE_static_cast (int, (ACE_PSOS_Time_t::month_mask &
                           (date_ >> ACE_PSOS_Time_t::month_shift))) -
    ACE_PSOS_Time_t::month_origin;
  tm_struct.tm_mday =
    ACE_static_cast (int, (ACE_PSOS_Time_t::day_mask & date_));
  tm_struct.tm_hour =
    ACE_static_cast (int, (ACE_PSOS_Time_t::hour_mask &
                           (time_ >> ACE_PSOS_Time_t::hour_shift)));
  tm_struct.tm_min =
    ACE_static_cast (int, (ACE_PSOS_Time_t::minute_mask &
                           (time_ >> ACE_PSOS_Time_t::minute_shift)));
  tm_struct.tm_sec =
    ACE_static_cast (int, (ACE_PSOS_Time_t::second_mask & time_));

  // Indicate values we don't know as negative numbers.
  tm_struct.tm_wday  = -1;
  tm_struct.tm_yday  = -1;
  tm_struct.tm_isdst = -1;

  timespec_t t;

  // Convert calendar time to time struct.
  t.tv_sec = ACE_OS::mktime (&tm_struct);

  // Encode nanoseconds as system clock ticks.
  t.tv_nsec = ACE_static_cast (long,
                               ((ACE_static_cast (double, ticks_) *
                                 ACE_static_cast (double, 1000000000)) /
                                ACE_static_cast (double, KC_TICKS2SEC)));
  return t;
}

// type cast operator (to a timespec_t)

u_long
ACE_PSOS_Time_t::get_system_time (ACE_PSOS_Time_t& t)
{
  u_long ret_val = 0;

#   if defined (ACE_PSOSIM) // system time is broken in simulator.
  timeval tv;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&tv, 0), int, -1, result);
  if (result == -1)
    return 1;

  ACE_Time_Value atv (tv);
  timespec ts = atv;
  ACE_PSOS_Time_t pt (ts);
  t.date_ = pt.date_;
  t.time_ = pt.time_;
  t.ticks_ = pt.ticks_;
#   else
  ret_val = tm_get (&(t.date_), &(t.time_), &(t.ticks_));
#   endif  /* ACE_PSOSIM */
  return ret_val;
}

// Static member function to get current system time.

u_long
ACE_PSOS_Time_t::set_system_time (const ACE_PSOS_Time_t& t)
{
  return tm_set (t.date_, t.time_, t.ticks_);
}

// Static member function to set current system time.

#   if defined (ACE_PSOSIM)

u_long
ACE_PSOS_Time_t::init_simulator_time (void)
{
  // This is a hack using a direct UNIX system call, because the
  // appropriate ACE_OS method ultimately uses the pSOS tm_get
  // function, which would fail because the simulator's system time is
  // uninitialized (chicken and egg).
  timeval t;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&t, 0),
              int,
              -1,
              result);

  if (result == -1)
    return 1;
  else
    {
      ACE_Time_Value tv (t);
      timespec ts = tv;
      ACE_PSOS_Time_t pt (ts);
      u_long ret_val =
        ACE_PSOS_Time_t::set_system_time (pt);
      return ret_val;

    }
}

// Static member function to initialize system time, using UNIX calls.

#   endif /* ACE_PSOSIM */
# endif /* ACE_PSOS */

# if defined (__DGUX) && defined (ACE_HAS_THREADS) && defined (_POSIX4A_DRAFT10_SOURCE)
extern "C" int __d6_sigwait (sigset_t *set);

extern "C" int __d10_sigwait (const sigset_t *set, int *sig)
{
  sigset_t unconst_set = *set;
  int caught_sig = __d6_sigwait (&unconst_set);

  if (caught == -1)
    return -1;

  *sig = caught_sig;
  return 0;
}
# endif /* __DGUX && PTHREADS && _POSIX4A_DRAFT10_SOURCE */

# if defined (CHORUS)
extern "C"
void
ace_sysconf_dump (void)
{
  ACE_Time_Value time = ACE_OS::gettimeofday ();

  if (time == -1)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("Cannot get time\n")));
  else
    time.dump ();

  ACE_DEBUG ((LM_DEBUG,
              "ARG_MAX \t= \t%d\t"
              "DELAYTIMER_MAX \t= \t%d\n"
              "_MQ_OPEN_MAX \t= \t%d\t"
              "_MQ_PRIO_MAX \t= \t%d\n"
              "_MQ_DFL_MSGSIZE\t= \t%d\t"
              "_MQ_DFL_MAXMSGNB\t= \t%d\n"
              "_MQ_PATHMAX \t= \t%d\n"
              "NGROUPS_MAX \t= \t%d\t"
              "OPEN_MAX \t= \t%d\n"
              "PAGESIZE \t= \t%d\n"
              "PTHREAD_DESTRUCTOR_ITERATIONS \t= \t%d\n"
              "PTHREAD_KEYS_MAX \t= \t%d\n"
              "PTHREAD_STACK_MIN \t= \t%d\n"
              "PTHREAD_THREADS_MAX \t= \t%d\n"
              "SEM_VALUE_MAX \t= \t%d\n"
              "SEM_PATHMAX \t= \t%d\n"
              "TIMER_MAX \t= \t%d\n"
              "TZNAME_MAX \t= \t%d\n"
              "_POSIX_MESSAGE_PASSING \t= \t%d\n"
              "_POSIX_SEMAPHORES \t= \t%d\n"
              "_POSIX_SHARED_MEMORY_OBJECTS \t= \t%d\n"
              "_POSIX_THREADS \t= \t%d\n"
              "_POSIX_THREAD_ATTR_STACKADDR \t= \t%d\n"
              "_POSIX_THREAD_ATTR_STACKSIZE \t= \t%d\n"
              "_POSIX_THREAD_PRIORITY_SCHEDULING= \t%d\n"
              "_POSIX_THREAD_PRIO_INHERIT \t= \t%d\n"
              "_POSIX_THREAD_PRIO_PROTECT \t= \t%d\n"
              "_POSIX_THREAD_PROCESS_SHARED \t= \t%d\n"
              "_POSIX_THREAD_SAFE_FUNCTIONS \t= \t%d\n"
              "_POSIX_TIMERS \t= \t%d\n"
              "_POSIX_VERSION \t= \t%d\n",
              ACE_OS::sysconf (_SC_ARG_MAX),
              ACE_OS::sysconf (_SC_DELAYTIMER_MAX),
              ACE_OS::sysconf (_SC_MQ_OPEN_MAX),
              ACE_OS::sysconf (_SC_MQ_PRIO_MAX),
              ACE_OS::sysconf (_SC_MQ_DFL_MSGSIZE),
              ACE_OS::sysconf (_SC_MQ_DFL_MAXMSGNB),
              ACE_OS::sysconf (_SC_MQ_PATHMAX),
              ACE_OS::sysconf (_SC_NGROUPS_MAX),
              ACE_OS::sysconf (_SC_OPEN_MAX),
              ACE_OS::sysconf (_SC_PAGESIZE),
              ACE_OS::sysconf (_SC_PTHREAD_DESTRUCTOR_ITERATIONS),
              ACE_OS::sysconf (_SC_PTHREAD_KEYS_MAX),
              ACE_OS::sysconf (_SC_PTHREAD_STACK_MIN),
              ACE_OS::sysconf (_SC_PTHREAD_THREADS_MAX),
              ACE_OS::sysconf (_SC_SEM_VALUE_MAX),
              ACE_OS::sysconf (_SC_SHM_PATHMAX),
              ACE_OS::sysconf (_SC_TIMER_MAX),
              ACE_OS::sysconf (_SC_TZNAME_MAX),
              ACE_OS::sysconf (_SC_MESSAGE_PASSING),
              ACE_OS::sysconf (_SC_SEMAPHORES),
              ACE_OS::sysconf (_SC_SHARED_MEMORY_OBJECTS),
              ACE_OS::sysconf (_SC_THREADS),
              ACE_OS::sysconf (_SC_THREAD_ATTR_STACKADDR),
              ACE_OS::sysconf (_SC_THREAD_ATTR_STACKSIZE),
              ACE_OS::sysconf (_SC_THREAD_PRIORITY_SCHEDULING),
              ACE_OS::sysconf (_SC_THREAD_PRIO_INHERIT),
              ACE_OS::sysconf (_SC_THREAD_PRIO_PROTECT),
              ACE_OS::sysconf (_SC_THREAD_PROCESS_SHARED),
              ACE_OS::sysconf (_SC_THREAD_SAFE_FUNCTIONS),
              ACE_OS::sysconf (_SC_TIMERS),
              ACE_OS::sysconf (_SC_VERSION)));
}
# endif /* CHORUS */

ACE_OS_WString::ACE_OS_WString (const ACE_USHORT16 *s)
  : rep_ (0)
{
  size_t len = ACE_OS::strlen (s);
  ACE_NEW (this->rep_,
           char[len]);

  for (size_t i = 0; i < len; i++)
    {
      ACE_USHORT16 *t = ACE_const_cast (ACE_USHORT16 *, s);
      this->rep_[i] = ACE_static_cast (char, *(t + i));
    }

  this->rep_[len] = '\0';
  return;
}

ACE_OS_CString::ACE_OS_CString (const char *s)
  : rep_ (0)
{
  size_t len = ACE_OS::strlen (s);
  ACE_NEW (this->rep_,
           ACE_USHORT16[len]);

  for (size_t i = 0; i < len; i++)
    this->rep_[i] = s[i];

  this->rep_[len] = '\0';
  return;
}

# if defined (ACE_HAS_WINCE)
ACE_CE_Bridge *ACE_CE_Bridge::default_text_bridge_ = 0;

ACE_CE_Bridge::ACE_CE_Bridge (void)
  : text_output_ (0),
    notification_ (0),
    idc_ (0)
{
}

ACE_CE_Bridge::ACE_CE_Bridge (CWnd *w, int n, int i)
  : text_output_ (w),
    notification_ (n),
    idc_ (i)
{
}

void
ACE_CE_Bridge::set_window (CWnd *w, int n, int i)
{
  this->text_output_ = w;
  this->notification_ = n;
  this->idc_ = i;
}

ACE_CE_Bridge::~ACE_CE_Bridge (void)
{
  // This method needs to be defined because there seems to be a bug
  // in CE's compiler.
}

void
ACE_CE_Bridge::set_self_default (void)
{
  ACE_CE_Bridge::default_text_bridge_ = this;
}

int
ACE_CE_Bridge::notification (void)
{
  return this->notification_;
}

int
ACE_CE_Bridge::idc (void)
{
  return this->idc_;
}

CWnd *
ACE_CE_Bridge::window (void)
{
  return this->text_output_;
}

ACE_CE_Bridge *
ACE_CE_Bridge::get_default_winbridge (void)
{
  return ACE_CE_Bridge::default_text_bridge_;
}

int
ACE_CE_Bridge::write_msg (LPCTSTR str)
{
  return this->write_msg (new CString (str));
}

int
ACE_CE_Bridge::write_msg (CString *s)
{
  // Don't ask!
  return this->text_output_->PostMessage (WM_COMMAND,
                                          MAKEWORD (this->idc_,
                                                    this->notification_),
                                          (long)((void *) s));
}

//          **** Warning ****
// You should not use the following function under CE at all.  This
// function is used to make Svc_Conf_l.cpp compile under WinCE.  It
// might not do what it is expected to do under regular environments.
//          **** Warning ****

void
exit (int status)
{
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_HAS_WINCE) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
  // Shut down the ACE_Object_Manager, if it had registered its exit_hook.
  // With ACE_HAS_NONSTATIC_OBJECT_MANAGER, the ACE_Object_Manager is
  // instantiated on the main's stack.  ::exit () doesn't destroy it.
  if (exit_hook_)
    (*exit_hook_) ();
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

  ACE_OS::exit (status);
}

# endif /* ACE_HAS_WINCE */
