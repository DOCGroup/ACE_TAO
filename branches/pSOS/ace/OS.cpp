// $Id$

#define ACE_BUILD_DLL
#include "ace/OS.h"
#include "ace/SString.h"
#include "ace/Sched_Params.h"

#if defined (ACE_WIN32)
#include "ace/ARGV.h"
#endif /* ACE_WIN32 */

// Perhaps we should *always* include ace/OS.i in order to make sure
// we can always link against the OS symbols?
#if !defined (ACE_HAS_INLINED_OSCALLS)
#include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/Task.h"
#include "ace/Synch_T.h"
#include "ace/Containers.h"

#include "ace/streams.h"

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#include "ace/Object_Manager.h"

#if defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
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
#endif /* defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS) */
#endif /* defined (ACE_MT_SAFE) */

// Static constant representing `zero-time'.
const ACE_Time_Value ACE_Time_Value::zero;

ACE_ALLOC_HOOK_DEFINE(ACE_Time_Value)

// Initializes the ACE_Time_Value object from a timeval.

#if defined(ACE_WIN32)
//  Initializes the ACE_Time_Value object from a Win32 FILETIME

ACE_Time_Value::ACE_Time_Value (const FILETIME &file_time)
{
  // ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (file_time);
}

void ACE_Time_Value::set (const FILETIME &file_time)
{
  //  Initializes the ACE_Time_Value object from a Win32 FILETIME
  ACE_QWORD _100ns = ACE_MAKE_QWORD (file_time.dwLowDateTime,
                                     file_time.dwHighDateTime);
  // Convert 100ns units to seconds;
  this->tv_.tv_sec = long (_100ns / (10000 * 1000));
  // Convert remainder to microseconds;
  this->tv_.tv_usec = long ((_100ns - (this->tv_.tv_sec * (10000 * 1000))) / 10);
}

// Returns the value of the object as a Win32 FILETIME.

ACE_Time_Value::operator FILETIME () const
{
  // ACE_TRACE ("ACE_Time_Value::operator FILETIME");
  ACE_QWORD _100ns = ((ACE_QWORD) this->tv_.tv_sec * (1000 * 1000) + this->tv_.tv_usec) * 10;
  FILETIME file_time;
  file_time.dwLowDateTime = ACE_LOW_DWORD (_100ns);
  file_time.dwHighDateTime = ACE_HIGH_DWORD (_100ns);
  return file_time;
}

#endif

ACE_Cleanup_Info::ACE_Cleanup_Info (void)
  : object_ (0),
    cleanup_hook_ (0),
    param_ (0)
{
}

int
ACE_Cleanup_Info::operator== (const struct ACE_Cleanup_Info &o) const
{
  return o.object_ == this->object_
    && o.cleanup_hook_ == this->cleanup_hook_
    && o.param_ == this->param_;
}

int
ACE_Cleanup_Info::operator!= (const struct ACE_Cleanup_Info &o) const
{
  return !(*this == o);
}

void
ACE_Time_Value::dump (void) const
{
  // ACE_TRACE ("ACE_Time_Value::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\ntv_sec_ = %d", this->tv_.tv_sec));
  ACE_DEBUG ((LM_DEBUG, "\ntv_usec_ = %d\n", this->tv_.tv_usec));
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
  this->start_time_ = ACE_OS::gettimeofday ();
  this->stopped_ = 0;
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
          errno = ETIME;
        }
      this->stopped_ = 1;
    }
  return 0;
}

ACE_Countdown_Time::ACE_Countdown_Time (ACE_Time_Value *max_wait_time)
  : max_wait_time_ (max_wait_time),
    stopped_ (0)
{
  if (max_wait_time != 0)
    this->start ();
}

ACE_Countdown_Time::~ACE_Countdown_Time (void)
{
  this->stop ();
}

#if defined (ACE_HAS_POWERPC) && defined (ghs)
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
#endif /* ACE_HAS_POWERPC && ghs */

#if defined (ACE_WIN32) || defined (VXWORKS) || defined (CHORUS)
// Don't inline on those platforms because this function contains
// string literals, and some compilers, e.g., g++, don't handle those
// efficiently in unused inline functions.
int
ACE_OS::uname (struct utsname *name)
{
  // ACE_TRACE ("ACE_OS::uname");
#if defined (ACE_WIN32)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, "Win32");

  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx (&vinfo);

  SYSTEM_INFO sinfo;
  ::GetSystemInfo(&sinfo);

  ACE_OS::strcpy (name->sysname, "Win32");

  if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    // Get information from the two structures
    ACE_OS::sprintf (name->release,
                    "Windows NT %d.%d",
                    vinfo.dwMajorVersion,
                    vinfo.dwMinorVersion);
    ACE_OS::sprintf (name->version,
                     "Build %d %s",
                     vinfo.dwBuildNumber,
                     vinfo.szCSDVersion);

    char processor[10] = "Unknown";
    char subtype[10] = "Unknown";

#if defined (__BORLANDC__)
    // Some changes should be made in winbase.h...
    switch (sinfo.s.wProcessorArchitecture)
#else
      switch (sinfo.wProcessorArchitecture)
#endif /* __BORLAND__ */
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
      ACE_OS::strcpy (processor, "Intel");
      if (sinfo.wProcessorLevel == 3)
        ACE_OS::strcpy (subtype, "80386");
      else if (sinfo.wProcessorLevel == 4)
        ACE_OS::strcpy (subtype, "80486");
      else if (sinfo.wProcessorLevel == 5)
        ACE_OS::strcpy (subtype, "Pentium");
      else if (sinfo.wProcessorLevel == 6)
        ACE_OS::strcpy (subtype, "Pentium Pro");
      else if (sinfo.wProcessorLevel == 7)  // I'm guessing here
        ACE_OS::strcpy (subtype, "Pentium II");
      break;
    case PROCESSOR_ARCHITECTURE_MIPS:
      ACE_OS::strcpy (processor, "MIPS");
      ACE_OS::strcpy (subtype, "R4000");
      break;
    case PROCESSOR_ARCHITECTURE_ALPHA:
      ACE_OS::strcpy (processor, "Alpha");
      ACE_OS::sprintf (subtype, "%d", sinfo.wProcessorLevel);
      break;
    case PROCESSOR_ARCHITECTURE_PPC:
      ACE_OS::strcpy (processor, "PPC");
      if (sinfo.wProcessorLevel == 1)
        ACE_OS::strcpy (subtype, "601");
      else if (sinfo.wProcessorLevel == 3)
        ACE_OS::strcpy (subtype, "603");
      else if (sinfo.wProcessorLevel == 4)
        ACE_OS::strcpy (subtype, "604");
      else if (sinfo.wProcessorLevel == 6)
        ACE_OS::strcpy (subtype, "603+");
      else if (sinfo.wProcessorLevel == 9)
        ACE_OS::strcpy (subtype, "804+");
      else if (sinfo.wProcessorLevel == 20)
        ACE_OS::strcpy (subtype, "620");
      break;
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
      ACE_OS::strcpy (processor, "Unknown");
      break;
    }
    ACE_OS::sprintf(name->machine, "%s %s", processor, subtype);
  }
  else if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
  {
    // Get Windows 95 Information
    ACE_OS::strcpy (name->release, "Windows 95");
    ACE_OS::sprintf (name->version, "%d", LOWORD (vinfo.dwBuildNumber));
    if (sinfo.dwProcessorType == PROCESSOR_INTEL_386)
      ACE_OS::strcpy (name->machine, "Intel 80386");
    else if (sinfo.dwProcessorType == PROCESSOR_INTEL_486)
      ACE_OS::strcpy (name->machine, "Intel 80486");
    else if (sinfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM)
      ACE_OS::strcpy (name->machine, "Intel Pentium");
  }
  else
  {
    // We don't know what this is!

    ACE_OS::strcpy (name->release, "???");
    ACE_OS::strcpy (name->version, "???");
    ACE_OS::strcpy (name->machine, "???");
  }

  return ACE_OS::hostname (name->nodename, maxnamelen);
#elif defined (VXWORKS)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, "VxWorks");
  ACE_OS::strcpy (name->release, "???");
  ACE_OS::strcpy (name->version, sysBspRev ());
  ACE_OS::strcpy (name->machine, sysModel ());

  return ACE_OS::hostname (name->nodename, maxnamelen);
#elif defined (CHORUS)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname, "CHORUS/ClassiX");
  ACE_OS::strcpy (name->release, "???");
  ACE_OS::strcpy (name->version, "???");
  ACE_OS::strcpy (name->machine, "???");

  return ACE_OS::hostname (name->nodename, maxnamelen);
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
  ACE_DEBUG ((LM_DEBUG, "handle_ = %u", this->handle_));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, "\nInternal = %d", this->overlapped_.Internal));
  ACE_DEBUG ((LM_DEBUG, "\nInternalHigh = %d", this->overlapped_.InternalHigh));
  ACE_DEBUG ((LM_DEBUG, "\nOffsetHigh = %d", this->overlapped_.OffsetHigh));
  ACE_DEBUG ((LM_DEBUG, "\nhEvent = %d", this->overlapped_.hEvent));
#elif !defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, "\nl_whence = %d", this->lock_.l_whence));
  ACE_DEBUG ((LM_DEBUG, "\nl_start = %d", this->lock_.l_start));
  ACE_DEBUG ((LM_DEBUG, "\nl_len = %d", this->lock_.l_len));
  ACE_DEBUG ((LM_DEBUG, "\nl_type = %d", this->lock_.l_type));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_OS::mutex_lock_cleanup (void *mutex)
{
// ACE_TRACE ("ACE_OS::mutex_lock_cleanup");
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
  ACE_mutex_t *p_lock = (ACE_mutex_t *) mutex;
  ACE_OS::mutex_unlock (p_lock);
#else
  ACE_UNUSED_ARG (mutex);
#endif /* ACE_HAS_DCETHREADS */
#else
  ACE_UNUSED_ARG (mutex);
#endif /* ACE_HAS_THREADS */
}

// The following *printf functions aren't inline because
// they use varargs.
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

#if defined (ACE_HAS_UNICODE)
#if defined (ACE_WIN32)

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

#if 0
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
#endif /* 0 */

#endif /* ACE_WIN32 */
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

#if defined (ACE_HAS_STHREADS)
#include /**/ <sys/rtpriocntl.h>
#include /**/ <sys/tspriocntl.h>
#endif /* ACE_HAS_STHREADS */

int
ACE_OS::thr_setprio (const ACE_Sched_Priority prio)
{
  // Set the thread priority on the current thread.
  ACE_hthread_t my_thread_id;
  ACE_OS::thr_self (my_thread_id);
  return ACE_OS::thr_setprio (my_thread_id, prio);
}

int
ACE_OS::sched_params (const ACE_Sched_Params &sched_params)
{
  // ACE_TRACE ("ACE_OS::sched_params");
#if defined (CHORUS)
  int result;
  struct sched_param param;
  ACE_thread_t thr_id = ACE_OS::thr_self ();

  param.sched_priority = sched_params.priority ();

  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (thr_id,
                                                                sched_params.policy (),
                                                                &param),
                                       result),
                     int, -1);
#elif defined (ACE_HAS_STHREADS)
  // Set priority class, priority, and quantum of this LWP or process as
  // specified in sched_params.

  // Get the priority class ID and attributes.
  pcinfo_t pcinfo;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcinfo, 0, sizeof pcinfo);

  ACE_OS::strcpy (pcinfo.pc_clname,
                  sched_params.policy() == ACE_SCHED_OTHER  ?  "TS"  :  "RT");

  if (::priocntl (P_ALL /* ignored */,
                  P_MYID /* ignored */,
                  PC_GETCID,
                  (char *) &pcinfo) == -1)
    {
      return -1;
    }

  // OK, now we've got the class ID in pcinfo.pc_cid.  In addition,
  // the maximum configured real-time priority is in ((rtinfo_t *)
  // pcinfo.pc_clinfo)->rt_maxpri.

  pcparms_t pcparms;
  // The following is just to avoid Purify warnings about unitialized
  // memory reads.
  ACE_OS::memset (&pcparms, 0, sizeof pcparms);

  pcparms.pc_cid = pcinfo.pc_cid;

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

      // Package up the TS class ID and parameters for the ::priocntl ()
      // call.
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

      // Package up the RT class ID and parameters for the ::priocntl ()
      // call.
      ACE_OS::memcpy (pcparms.pc_clparms, &rtparms, sizeof rtparms);
    }
  else
    {
      errno = EINVAL;
      return -1;
    }

  if (::priocntl ((idtype_t) (sched_params.scope () == ACE_SCOPE_THREAD
                                ? ACE_SCOPE_PROCESS
                                : sched_params.scope ()), P_MYID, PC_SETPARMS,
                  (char *) &pcparms) < 0)
    {
      return ACE_OS::last_error ();
    }

  return 0;

#elif (defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)) && !defined (ACE_LACKS_SETSCHED)
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

  int result;
  struct sched_param param;

  param.sched_priority = sched_params.priority ();

  if (sched_params.scope () == ACE_SCOPE_PROCESS)
    {
      ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::sched_setscheduler (
                                             0,    // this process
                                             sched_params.policy (),
                                             &param),
                                           result),
                         int, -1);
    }
  else if (sched_params.scope () == ACE_SCOPE_THREAD)
    {
      ACE_thread_t thr_id = ACE_OS::thr_self ();

#   if defined (ACE_HAS_DCE_DRAFT4_THREADS)
      return (::pthread_setscheduler(thr_id,
                                     sched_params.policy (),
                                     sched_params.priority()) == -1 ? -1 : 0);
#   else
      ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (
                                             thr_id,
                                             sched_params.policy (),
                                             &param),
                                           result),
                         int, -1);
#   endif  /* ACE_HAS_DCE_DRAFT4_THREADS */

    }
  else // sched_params.scope () == ACE_SCOPE_LWP, which isn't POSIX
    {
      errno = EINVAL;
      return -1;
    }

#elif defined (ACE_WIN32)

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

#elif defined (VXWORKS)
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
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STHREADS */
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

#if defined (ACE_WIN32)

// = Static initialization.

// Keeps track of whether we've initialized the WinSock DLL.
int ACE_OS::socket_initialized_;

#endif /* WIN32 */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)

#include "ace/Array.h"

class ACE_TSS_Ref
  // = TITLE
  //     "Reference count" for thread-specific storage keys.
  //
  // = DESCRIPTION
  //     Since the <ACE_Unbounded_Stack> doesn't allow duplicates, the
  //     "reference count" is the identify of the thread_id.
{
public:
  ACE_TSS_Ref (ACE_thread_t id);
  // Constructor

  ACE_TSS_Ref (void);
  // Default constructor

  int operator== (const ACE_TSS_Ref &);
  // Check for equality.

  int operator!= (const ACE_TSS_Ref &);
  // Check for inequality.

// private:

  ACE_thread_t tid_;
  // ID of thread using a specific key.
};

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
ACE_TSS_Ref::operator== (const ACE_TSS_Ref &info)
{
// ACE_TRACE ("ACE_TSS_Ref::operator==");

  return this->tid_ == info.tid_;
}

// Check for inequality.
inline
int
ACE_TSS_Ref::operator!= (const ACE_TSS_Ref &tss_ref)
{
// ACE_TRACE ("ACE_TSS_Ref::operator==");

  return !(*this == tss_ref);
}

class ACE_TSS_Info
  // = TITLE
  //     Thread Specific Key management.
  //
  // = DESCRIPTION
  //     This class maps a key to a "destructor."
{
public:
  ACE_TSS_Info (ACE_thread_key_t key,
                void (*dest)(void *) = 0,
                void *tss_inst = 0);
  // Constructor

  ACE_TSS_Info (void);
  // Default constructor

  int key_in_use (void) const { return thread_count_ != -1; }
  // Returns 1 if the key is in use, 0 if not.

  void key_in_use (int flag) { thread_count_ = flag == 0  ?  -1  :  1; }
  // Mark the key as being in use if the flag is non-zero, or
  // not in use if the flag is 0.

  int operator== (const ACE_TSS_Info &);
  // Check for equality.

  int operator!= (const ACE_TSS_Info &);
  // Check for inequality.

  void dump (void);
  // Dump the state.

private:
  ACE_thread_key_t key_;
  // Key to the thread-specific storage item.

  void (*destructor_)(void *);
  // "Destructor" that gets called when the item is finally released.

  void *tss_obj_;
  // Pointer to ACE_TSS<xxx> instance that has/will allocate the key.

  int thread_count_;
  // Count of threads that are using this key.  Contains -1 when the
  // key is not in use.

  friend class ACE_TSS_Cleanup;
};

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

#if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
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
#endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

// Check for equality.
int
ACE_TSS_Info::operator== (const ACE_TSS_Info &info)
{
// ACE_TRACE ("ACE_TSS_Info::operator==");

  return this->key_ == info.key_;
}

// Check for inequality.
inline
int
ACE_TSS_Info::operator!= (const ACE_TSS_Info &info)
{
// ACE_TRACE ("ACE_TSS_Info::operator==");

  return !(*this == info);
}

void
ACE_TSS_Info::dump (void)
{
//  ACE_TRACE ("ACE_TSS_Info::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "key_ = %u", this->key_));
  ACE_DEBUG ((LM_DEBUG, "\ndestructor_ = %u", this->destructor_));
  ACE_DEBUG ((LM_DEBUG, "\ntss_obj_ = %u", this->tss_obj_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

class ACE_TSS_Keys
  // = TITLE
  //     Collection of in-use flags for a thread's TSS keys.
  //     For internal use only by ACE_TSS_Cleanup; it is public because
  //     some compilers can't use nested classes for template instantiation
  //     parameters.
  //
  // = DESCRIPTION
  //     Wrapper around array of whether each key is in use.  A simple
  //     typedef doesn't work with Sun C++ 4.2.
{
 public:
  ACE_TSS_Keys (void);
  // Default constructor, to initialize all bits to zero (unused).

  int test_and_set (const ACE_thread_key_t key);
  // Mark the specified key as being in use, if it was not already so marked.
  // Returns 1 if the had already been marked, 0 if not.

  int test_and_clear (const ACE_thread_key_t key);
  // Mark the specified key as not being in use, if it was not already so
  // cleared.  Returns 1 if the had already been cleared, 0 if not.

private:
  static void find (const u_int key, u_int &word, u_int &bit);
  // For a given key, find the word and bit number that represent it.

  enum
    {
#if ACE_SIZEOF_LONG == 8
      ACE_BITS_PER_WORD = 64,
#elif ACE_SIZEOF_LONG == 4
      ACE_BITS_PER_WORD = 32,
#else
#error ACE_TSS_Keys only supports 32 or 64 bit longs.
#endif /* ACE_HAS_64BIT_LONGS */
      ACE_WORDS = (ACE_DEFAULT_THREAD_KEYS - 1) / ACE_BITS_PER_WORD + 1
    };

  u_long key_bit_words_[ACE_WORDS];
  // Bit flag collection.  A bit value of 1 indicates that the key is in
  // use by this thread.
};

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

#if 0
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
#if defined (ACE_WIN32)
        ::TlsFree (key_arr[i]);
#else
        // don't bother to free the key
#endif /* ACE_WIN32 */
        this->table_.remove (ACE_TSS_Info (key_arr[i]));
      }
#endif /* 0 */
  }
}

int
ACE_TSS_Cleanup::free_all_key_left (void)
  // This is call from ACE_OS::cleanup_tss
  // When this gets called, all threads should
  // have exited except the main thread.
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
      // destructor_ fields.
      ACE_TSS_Info &info = this->table_ [key_index];
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

#if defined (ACE_WIN32)
      ::TlsFree (key_info->key_);
#else
      // don't bother to free the key
#endif /* ACE_WIN32 */
      return this->remove (key_info->key_);
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

void *
ACE_TSS_Emulation::tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX])
{
#if ! defined (VXWORKS)
  // On VxWorks, don't check to see if the field is 0.  It isn't always,
  // specifically, when a program is run directly by the shell (without
  // spawning a new task) after another program has been run.

  if (tss_base () == 0)
    {
#endif /* VXWORKS */
      // Use the supplied array for this thread's TSS.
      tss_base () = ts_storage;

      // Zero the entire TSS array.  Do it manually instead of using
      // memset, for optimum speed.
      void **tss_base_p = tss_base ();
      for (u_int i = 0; i < ACE_TSS_THREAD_KEYS_MAX; ++i, ++tss_base_p)
        {
          *tss_base_p = 0;
        }

      return tss_base ();
#if ! defined (VXWORKS)
    }
  else
    {
      return 0;
    }
#endif /* VXWORKS */
}

#if !defined (VXWORKS)
// FOR TESTING ONLY!
void **
ACE_TSS_Emulation::tss_collection_ [ACE_TSS_Emulation::ACE_TSS_THREADS_MAX] = { 0 };
#endif /* VXWORKS */
#endif /* ACE_HAS_TSS_EMULATION */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array<ACE_TSS_Info>;
template class ACE_Array_Iterator<ACE_TSS_Info>;
template class ACE_Node<ACE_TSS_Ref>;
template class ACE_TSS<ACE_TSS_Keys>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array<ACE_TSS_Info>
#pragma instantiate ACE_Array_Iterator<ACE_TSS_Info>
#pragma instantiate ACE_Node<ACE_TSS_Ref>
#pragma instantiate ACE_TSS<ACE_TSS_Keys>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

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
#endif /* ! ACE_HAS_TSS_EMULATION || ACE_WIN32 */

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

  // @@ Now the TSS Log_Meg has been created, cache
  // my thread descriptor in.

  if (this->thr_desc_ != 0)
    ACE_LOG_MSG->thr_desc (this->thr_desc_, this->thr_mgr_);
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

  // Extract the arguments.
  ACE_THR_FUNC func = this->user_func_;
  void *arg = this->arg_;

  // Delete ourselves since we don't need <this> anymore.
  delete (void *) this;

  void *status = 0;

#if 0
  status = (void*) (*func) (arg);  // Call thread entry point.
#else
  ACE_SEH_TRY {
    status = (void*) (*func) (arg);  // Call thread entry point.
  }
  ACE_SEH_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
    ACE_DEBUG ((LM_DEBUG, "(%t) Win32 structured exception exiting thread\n"));
    // Here's where we might want to provide a hook to report this...
    // As it stands now, we just catch all Win32 structured exceptions
    // so that we can make sure to clean up correctly when the thread
    // exits.
  }
#endif /* 0 */

  // Call the Task->close () hook.
  if (func == (ACE_THR_FUNC) ACE_Task_Base::svc_run)
    {
      ACE_Task_Base *task_ptr = (ACE_Task_Base *) arg;
      ACE_Thread_Manager *thr_mgr_ptr = task_ptr->thr_mgr ();

      // This calls the Task->close () hook.
      task_ptr->cleanup (task_ptr, 0);

      // This prevents a second invocation of the cleanup code (called
      // later by ACE_Thread_Manager::exit()).
      thr_mgr_ptr->at_exit (task_ptr, 0, 0);
    }

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  // Call TSS destructors.
  ACE_OS::cleanup_tss (0 /* not main thread */);

# if defined (ACE_WIN32) && defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  // Exit the thread.
  // Allow CWinThread-destructor to be invoked from AfxEndThread.
  // _endthreadex will be called from AfxEndThread so don't exit the
  // thread now if we are running an MFC thread.
  CWinThread *pThread = ::AfxGetThread ();
  if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
    ::_endthreadex ((DWORD) status);
  else
    ::AfxEndThread ((DWORD)status);
# endif /* ACE_WIN32 && ACE_HAS_MFC && ACE_HAS_MFS != 0*/

#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */

  return status;
}

ACE_Cleanup::~ACE_Cleanup ()
{
}

void
ACE_Cleanup::cleanup (void *)
{
  delete this;
}

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
                                        ACE_Thread_Descriptor *td)
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
#endif /* ACE_THREADS_DONT_INHERIT_LOG_MSG */
{
// ACE_TRACE ("Ace_Thread_Adapter::Ace_Thread_Adapter");
#if !defined (ACE_THREADS_DONT_INHERIT_LOG_MSG)
  if ( ACE_Log_Msg::exists() )
    {
      ACE_Log_Msg *inherit_log_ = ACE_LOG_MSG;
      this->ostream_ = inherit_log_->msg_ostream ();
      this->priority_mask_ = inherit_log_->priority_mask ();
      this->tracing_enabled_ = inherit_log_->tracing_enabled ();
      this->restart_ = inherit_log_->restart ();
      this->trace_depth_ = inherit_log_->trace_depth ();
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

#if defined (ACE_NO_THREAD_ADAPTER)
#define  ACE_THREAD_FUNCTION  func
#define  ACE_THREAD_ARGUMENT  args
#else
#define  ACE_THREAD_FUNCTION  thread_args->entry_point ()
#define  ACE_THREAD_ARGUMENT  thread_args

  ACE_Thread_Adapter *thread_args;
  if (thread_adapter == 0)
    ACE_NEW_RETURN (thread_args, ACE_Thread_Adapter (func, args, (ACE_THR_C_FUNC) ace_thread_adapter), -1);
  else
    thread_args = thread_adapter;

#endif /* ACE_NO_THREAD_ADAPTER */

#if defined (ACE_HAS_THREADS)
  ACE_thread_t tmp_thr;
  ACE_hthread_t tmp_handle;

  if (thr_id == 0)
    thr_id = &tmp_thr;

  if (thr_handle == 0)
    thr_handle = &tmp_handle;

#  if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)

  int result;
  pthread_attr_t attr;
#    if defined (ACE_HAS_DCETHREADS)
  if (::pthread_attr_create (&attr) != 0)
#    else /* ACE_HAS_DCETHREADS */
  if (::pthread_attr_init (&attr) != 0)
#    endif /* ACE_HAS_DCETHREADS */
      return -1;
#    if !defined (ACE_LACKS_SETSCHED)
  // The PRIORITY stuff used to be here...-cjc
#    endif /* ACE_LACKS_SETSCHED */


  // *** Set Stack Size
#    if defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
  if (stacksize < ACE_NEEDS_HUGE_THREAD_STACKSIZE)
    stacksize = ACE_NEEDS_HUGE_THREAD_STACKSIZE;
#    endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

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

#    if defined (PTHREAD_STACK_MIN)
      if (size < PTHREAD_STACK_MIN)
        size = PTHREAD_STACK_MIN;
#    endif /* PTHREAD_STACK_MIN */

#    if !defined (ACE_LACKS_THREAD_STACK_SIZE)      // JCEJ 12/17/96
      if (::pthread_attr_setstacksize (&attr, size) != 0)
        {
#      if defined (ACE_HAS_DCETHREADS)
          ::pthread_attr_delete (&attr);
#      else /* ACE_HAS_DCETHREADS */
          ::pthread_attr_destroy (&attr);
#      endif /* ACE_HAS_DCETHREADS */
          return -1;
        }
#    else
      ACE_UNUSED_ARG (size);
#    endif /* !ACE_LACKS_THREAD_STACK_SIZE */
    }

  // *** Set Stack Address
#    if !defined (ACE_LACKS_THREAD_STACK_ADDR)
  if (stack != 0)
    {
      if (::pthread_attr_setstackaddr (&attr, stack) != 0)
        {
#      if defined (ACE_HAS_DCETHREADS)
          ::pthread_attr_delete (&attr);
#      else /* ACE_HAS_DCETHREADS */
          ::pthread_attr_destroy (&attr);
#      endif /* ACE_HAS_DCETHREADS */
          return -1;
        }
    }
#    else
  ACE_UNUSED_ARG (stack);
#    endif /* !ACE_LACKS_THREAD_STACK_ADDR */

  // *** Deal with various attributes
  if (flags != 0)
    {
      // *** Set Detach state
#    if !defined (ACE_LACKS_SETDETACH)
      if (ACE_BIT_ENABLED (flags, THR_DETACHED)
          || ACE_BIT_ENABLED (flags, THR_JOINABLE))
        {
          int dstate = PTHREAD_CREATE_JOINABLE;

          if (ACE_BIT_ENABLED (flags, THR_DETACHED))
            dstate = PTHREAD_CREATE_DETACHED;

#      if defined (ACE_HAS_DCETHREADS)
          if (::pthread_attr_setdetach_np (&attr, dstate) != 0)
#      else /* ACE_HAS_DCETHREADS */
#        if defined (ACE_HAS_PTHREAD_DSTATE_PTR)
          if (::pthread_attr_setdetachstate (&attr, &dstate) != 0)
#        else
          if (::pthread_attr_setdetachstate (&attr, dstate) != 0)
#        endif /* ACE_HAS_PTHREAD_DSTATE_PTR */
#      endif /* ACE_HAS_DCETHREADS */
                {
#      if defined (ACE_HAS_DCETHREADS)
            ::pthread_attr_delete (&attr);
#      else /* ACE_HAS_DCETHREADS */
            ::pthread_attr_destroy (&attr);
#      endif /* ACE_HAS_DCETHREADS */
            return -1;
          }
        }
#    endif /* ACE_LACKS_SETDETACH */

      // *** Set Policy
#    if !defined (ACE_LACKS_SETSCHED)
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

#      if defined (ACE_HAS_ONLY_SCHED_OTHER)
            // Solaris, thru version 2.5.1, only supports SCHED_OTHER.
            spolicy = SCHED_OTHER;
#      else
          if (ACE_BIT_ENABLED (flags, THR_SCHED_DEFAULT))
            spolicy = SCHED_OTHER;
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            spolicy = SCHED_FIFO;
#if defined (SCHED_IO)
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            spolicy = SCHED_IO;
#else
          else if (ACE_BIT_ENABLED (flags, THR_SCHED_IO))
            {
              errno = ENOSYS;
              return -1;
            }
#endif /* SCHED_IO */
          else
            spolicy = SCHED_RR;
#      endif /* ACE_HAS_ONLY_SCHED_OTHER */

#      if !defined (ACE_HAS_FSU_PTHREADS)
#        if defined (ACE_HAS_DCETHREADS)
          result = ::pthread_attr_setsched (&attr, spolicy);
#        else /* ACE_HAS_DCETHREADS */
          result = ::pthread_attr_setschedpolicy (&attr, spolicy);
#        endif /* ACE_HAS_DCETHREADS */
          if (result != 0)
              {
                // Preserve the errno value.
                errno = result;
#        if defined (ACE_HAS_DCETHREADS)
                ::pthread_attr_delete (&attr);
#        else /* ACE_HAS_DCETHREADS */
                ::pthread_attr_destroy (&attr);
#        endif /* ACE_HAS_DCETHREADS */
                return -1;
              }
#      else
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
#        if defined (ACE_HAS_DCETHREADS)
              ::pthread_attr_delete (&attr);
#        else /* ACE_HAS_DCETHREADS */
              ::pthread_attr_destroy (&attr);
#        endif /* ACE_HAS_DCETHREADS */
              return -1;
            }
#      endif    /*  ACE_HAS_FSU_PTHREADS */
        }

      // *** Set Priority (use reasonable default priorities)
#      if defined(ACE_HAS_PTHREADS_1003_DOT_1C)
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
#      endif //ACE_HAS_PTHREADS_1003_DOT_1C
      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        {
          struct sched_param sparam;
          ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);

#      if defined (ACE_HAS_DCETHREADS) && !defined (ACE_HAS_DCE_DRAFT4_THREADS)
          sparam.sched_priority = ACE_MIN (priority, PRIORITY_MAX);
#      elif defined(ACE_HAS_IRIX62_THREADS)
          sparam.sched_priority = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#      elif defined (PTHREAD_MAX_PRIORITY) && !defined(ACE_HAS_PTHREADS_1003_DOT_1C)
          /* For MIT pthreads... */
          sparam.prio = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#      elif defined(ACE_HAS_PTHREADS_1003_DOT_1C)
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
#      else
          sparam.sched_priority = priority;
#      endif

#      if defined (ACE_HAS_FSU_PTHREADS)
         if (sparam.sched_priority >= PTHREAD_MIN_PRIORITY
             && sparam.sched_priority <= PTHREAD_MAX_PRIORITY)
           attr.prio = sparam.sched_priority;
         else
           {
             pthread_attr_destroy (&attr);
             errno = EINVAL;
             return -1;
           }
#      else
         {
#        if defined (ACE_HAS_STHREADS)
           // Solaris POSIX only allows priorities > 0 to
           // ::pthread_attr_setschedparam.  If a priority of 0 was
           // requested, set the thread priority after creating it, below.
           if (priority > 0)
#        endif /* STHREADS */
             {
#        if defined (ACE_HAS_DCETHREADS)
               result = ::pthread_attr_setprio (&attr,
                                                sparam.sched_priority);
#        else /* ACE_HAS_DCETHREADS */
               result = ::pthread_attr_setschedparam (&attr, &sparam);
#        endif /* ACE_HAS_DCETHREADS */
               if (result != 0)
                 {
#        if defined (ACE_HAS_DCETHREADS)
                   ::pthread_attr_delete (&attr);
#        else /* ACE_HAS_DCETHREADS */
                   ::pthread_attr_destroy (&attr);
#        endif /* ACE_HAS_DCETHREADS */
                   errno = result;
                   return -1;
                 }
             }
         }
#      endif    /* ACE_HAS_FSU_PTHREADS */
        }

      // *** Set scheduling explicit or inherited
      if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED)
          || ACE_BIT_ENABLED (flags, THR_EXPLICIT_SCHED))
        {
#      if defined (ACE_HAS_DCETHREADS)
          int sched = PTHREAD_DEFAULT_SCHED;
#      else /* ACE_HAS_DCETHREADS */
          int sched = PTHREAD_EXPLICIT_SCHED;
#      endif /* ACE_HAS_DCETHREADS */
          if (ACE_BIT_ENABLED (flags, THR_INHERIT_SCHED))
            sched = PTHREAD_INHERIT_SCHED;
          if (::pthread_attr_setinheritsched (&attr, sched) != 0)
            {
#      if defined (ACE_HAS_DCETHREADS)
              ::pthread_attr_delete (&attr);
#      else /* ACE_HAS_DCETHREADS */
              ::pthread_attr_destroy (&attr);
#      endif /* ACE_HAS_DCETHREADS */
              return -1;
            }
        }
#    endif /* ACE_LACKS_SETSCHED */


      // *** Set Scope
#    if !defined (ACE_LACKS_THREAD_PROCESS_SCOPING)
      if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM)
          || ACE_BIT_ENABLED (flags, THR_SCOPE_PROCESS))
        {
          int scope = PTHREAD_SCOPE_PROCESS;
          if (ACE_BIT_ENABLED (flags, THR_SCOPE_SYSTEM))
            scope = PTHREAD_SCOPE_SYSTEM;

          if (::pthread_attr_setscope (&attr, scope) != 0)
            {
#      if defined (ACE_HAS_DCETHREADS)
              ::pthread_attr_delete (&attr);
#      else /* ACE_HAS_DCETHREADS */
              ::pthread_attr_destroy (&attr);
#      endif /* ACE_HAS_DCETHREADS */
              return -1;
            }
        }
#    endif /* !ACE_LACKS_THREAD_PROCESS_SCOPING */

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

#    if defined (ACE_HAS_DCETHREADS)
#       if defined (ACE_HAS_DCE_DRAFT4_THREADS)
  ACE_OSCALL (::pthread_create (thr_id, attr,
                                ACE_THREAD_FUNCTION,
                                ACE_THREAD_ARGUMENT),
              int, -1, result);
#       else
  ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (thr_id, attr,
                                                  ACE_THREAD_FUNCTION,
                                                  ACE_THREAD_ARGUMENT),
                                result),
              int, -1, result);
#       endif /* ACE_HAS_DCE_DRAFT4_THREADS */
  ::pthread_attr_delete (&attr);
#    else /* !ACE_HAS_DCETHREADS */
  ACE_OSCALL (ACE_ADAPT_RETVAL (::pthread_create (thr_id,
                                                  &attr,
                                                  ACE_THREAD_FUNCTION,
                                                  ACE_THREAD_ARGUMENT),
                                result),
              int, -1, result);
  ::pthread_attr_destroy (&attr);
#    endif /* ACE_HAS_DCETHREADS */

  // This is a Solaris, POSIX, or DCE implementation of pthreads,
  // where we assume that ACE_thread_t and ACE_hthread_t are the same.
  // If this *isn't* correct on some platform, please let us know.
  if (result != -1)
    *thr_handle = *thr_id;

#    if defined (ACE_HAS_STHREADS)
  // If the priority is 0, then we might have to set it now because we
  // couldn't set it with ::pthread_attr_setschedparam, as noted
  // above.  This doesn't provide strictly correct behavior, because
  // the thread was created (above) with the priority of its parent.
  // (That applies regardless of the inherit_sched attribute: if it
  // was PTHREAD_INHERIT_SCHED, then it certainly inherited its
  // parent's priority.  If it was PTHREAD_EXPLICIT_SCHED, then "attr"
  // was initialized by the Solaris ::pthread_attr_init () to contain
  // NULL for the priority, which indicated to Solaris
  // ::pthread_create () to inherit the parent priority.)
  if (priority == 0)
    {
      // Check the priority of this thread, which is the parent of the
      // newly created thread.  If it is 0, then the newly created
      // thread will have inherited the priority of 0, so there's no
      // need to explicitly set it.
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

          // The only policy currently (version 2.5.1) supported by by Solaris
          // is SCHED_OTHER, so that's hard-coded below.
          ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (
                                                 *thr_id,
                                                 SCHED_OTHER,
                                                 &sparam),
                                               result),
                             int, -1);
        }
    }
#    endif /* ACE_HAS_STHREADS */
  return result;
#  elif defined (ACE_HAS_STHREADS)
  int result;
  int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

  if (priority != ACE_DEFAULT_THREAD_PRIORITY)
    // If we need to set the priority, then we need to start the
    // thread in a suspended mode.
    ACE_SET_BITS (flags, THR_SUSPENDED);

  ACE_OSCALL (ACE_ADAPT_RETVAL (::thr_create (stack, stacksize,
                                              ACE_THREAD_FUNCTION,
                                              ACE_THREAD_ARGUMENT,
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
#  elif defined (ACE_HAS_WTHREADS)
  ACE_UNUSED_ARG (stack);
#    if defined (ACE_HAS_MFC) && (ACE_HAS_MFC != 0)
  if (ACE_BIT_ENABLED (flags, THR_USE_AFX))
    {
      CWinThread *cwin_thread =
        ::AfxBeginThread ((AFX_THREADPROC) &ACE_THREAD_FUNCTION,
                          ACE_THREAD_ARGUMENT, priority, 0,
                          flags | THR_SUSPENDED);
      // Have to duplicate the handle because
      // CWinThread::~CWinThread() closes the original handle.
      (void) ::DuplicateHandle (::GetCurrentProcess (),
                                cwin_thread->m_hThread,
                                ::GetCurrentProcess (),
                                thr_handle,
                                0,
                                TRUE,
                                DUPLICATE_SAME_ACCESS);

      *thr_id = cwin_thread->m_nThreadID;

      if (ACE_BIT_ENABLED (flags, THR_SUSPENDED) == 0)
        cwin_thread->ResumeThread ();
      // cwin_thread will be deleted in AfxThreadExit()
      // Warning: If AfxThreadExit() is called from within the
      // thread, ACE_TSS_Cleanup->exit() never gets called !
    }
  else
#    endif /* ACE_HAS_MFC */
    {
      int start_suspended = ACE_BIT_ENABLED (flags, THR_SUSPENDED);

      if (priority != ACE_DEFAULT_THREAD_PRIORITY)
        // If we need to set the priority, then we need to start the
        // thread in a suspended mode.
        ACE_SET_BITS (flags, THR_SUSPENDED);

      *thr_handle = (void *) ::_beginthreadex
        (0,
         stacksize,
         (unsigned (__stdcall *) (void *)) ACE_THREAD_FUNCTION,
         ACE_THREAD_ARGUMENT,
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
#    if 0
  *thr_handle = ::CreateThread
    (0,
     stacksize,
     LPTHREAD_START_ROUTINE (ACE_THREAD_FUNCTION),
     ACE_THREAD_ARGUMENT,
     flags,
     thr_id);
#    endif /* 0 */

  // Close down the handle if no one wants to use it.
  if (thr_handle == &tmp_handle)
    ::CloseHandle (tmp_handle);

  if (*thr_handle != 0)
    return 0;
  else
    ACE_FAIL_RETURN (-1);
  /* NOTREACHED */
#  elif defined (VXWORKS)
  // The call below to ::taskSpawn () causes VxWorks to assign a
  // unique task name of the form: "t" + an integer, because the
  // first argument is 0.

  // args must be an array of _exactly_ 10 ints.

  // The stack arg is ignored:  if there's a need for it, we'd have to
  // use ::taskInit ()/::taskActivate () instead of ::taskSpawn ().
  ACE_UNUSED_ARG (stack);

  // The hard-coded arguments are what ::sp () would use.  ::taskInit ()
  // is used instead of ::sp () so that we can set the priority, flags,
  // and stacksize.  (::sp () also hardcodes priority to 100, flags
  // to VX_FP_TASK, and stacksize to 20,000.)  stacksize should be
  // an even integer.

  // If called with thr_create() defaults, use same default values as ::sp ():
  if (priority == ACE_DEFAULT_THREAD_PRIORITY) priority = 100;
  if (flags == 0) flags = VX_FP_TASK; // Assumes that there is a
  // floating point coprocessor.
  // As noted above, ::sp () hardcodes
  // this, so we should be safe with it.

  if (stacksize == 0) stacksize = 20000;

  ACE_hthread_t tid = ::taskSpawn (0, priority,
                                   (int) flags,
                                   (int) stacksize,
                                   ACE_THREAD_FUNCTION,
                                   (int) ACE_THREAD_ARGUMENT,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0);
  if (tid == ERROR)
    return -1;
  else
    {
      // ::taskTcb (int tid) returns the address of the WIND_TCB
      // (task control block).  According to the ::taskSpawn()
      // documentation, the name of the new task is stored at
      // pStackBase, but is that of the current task?  If so, it
      // might be a bit quicker than this extraction of the tcb . . .
      *thr_id = ::taskTcb (tid)->name;
      *thr_handle = tid;
      return 0;
    }

#  endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (func);
  ACE_UNUSED_ARG (args);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (thr_id);
  ACE_UNUSED_ARG (thr_handle);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (stack);
  ACE_UNUSED_ARG (stacksize);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

void
ACE_OS::thr_exit (void *status)
{
// ACE_TRACE ("ACE_OS::thr_exit");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
    ::pthread_exit (status);
# elif defined (ACE_HAS_STHREADS)
    ::thr_exit (status);
# elif defined (ACE_HAS_WTHREADS)
    // Can't call it here because on NT, the thread is exited
    // directly by ACE_Thread_Adapter::invoke ().
    //   ACE_TSS_Cleanup::instance ()->exit (status);
# elif defined (VXWORKS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);

    *((int *) status) = ::taskDelete (tid);
# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (status);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_setspecific (ACE_thread_key_t key, void *data)
{
// ACE_TRACE ("ACE_OS::thr_setspecific");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
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
# elif defined (ACE_HAS_DCETHREADS) || defined (ACE_HAS_PTHREADS)
#   if defined (ACE_HAS_FSU_PTHREADS)
      // Call pthread_init() here to initialize threads package.  FSU
      // threads need an initialization before the first thread constructor.
      // This seems to be the one; however, a segmentation fault may
      // indicate that another pthread_init() is necessary, perhaps in
      // Synch.cpp or Synch_T.cpp.  FSU threads will not reinit if called
      // more than once, so another call to pthread_init will not adversely
      // affect existing threads.
      pthread_init ();
#   endif /*  ACE_HAS_FSU_PTHREADS */
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setspecific (key, data), ace_result_), int, -1);
# elif defined (ACE_HAS_STHREADS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_setspecific (key, data), ace_result_), int, -1);
# elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (data);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_keyfree (ACE_thread_key_t key)
{
// ACE_TRACE ("ACE_OS::thr_keyfree");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
    return ACE_TSS_Cleanup::instance ()->remove (key);
# elif defined (ACE_LACKS_KEYDELETE)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_PTHREADS) && !defined (ACE_HAS_FSU_PTHREADS)
    return ::pthread_key_delete (key);
# elif defined (ACE_HAS_DCETHREADS)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_THR_KEYDELETE)
    return ::thr_keydelete (key);
# elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_WTHREADS)
    // Extract out the thread-specific table instance and free up
    // the key and destructor.
    ACE_TSS_Cleanup::instance ()->remove (key);
    ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::TlsFree (key), ace_result_), int, -1);
# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_keycreate (ACE_thread_key_t *key,
#if defined (ACE_HAS_THR_C_DEST)
                       ACE_THR_C_DEST dest,
#else
                       ACE_THR_DEST dest,
#endif /* ACE_HAS_THR_C_DEST */
                       void *inst)
{
// ACE_TRACE ("ACE_OS::thr_keycreate");
#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_TSS_EMULATION)
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
# elif defined (ACE_HAS_DCETHREADS)
    ACE_UNUSED_ARG (inst);
#   if defined (ACE_HAS_STDARG_THR_DEST)
      ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#   else
      ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#   endif /* ACE_HAS_STDARG_THR_DEST */
# elif defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
# elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
# elif defined (ACE_HAS_WTHREADS)
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
# endif /* ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (key);
  ACE_UNUSED_ARG (dest);
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}

int
ACE_OS::thr_key_used (ACE_thread_key_t key)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  ACE_TSS_Cleanup::instance ()->key_used (key);
  return 0;
#else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */
}

int
ACE_OS::thr_key_detach (void *inst)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  if (ACE_TSS_Cleanup::lockable ())
    return ACE_TSS_Cleanup::instance()->detach (inst);
  else
    // We're in static constructor/destructor phase.  Don't
    // try to use the ACE_TSS_Cleanup instance because its lock
    // might not have been constructed yet, or might have been
    // destroyed already.  Just leak the key . . .
    return -1;
#else
  ACE_UNUSED_ARG (inst);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION */
}

// Create a contiguous command-line argument buffer with each arg
// separated by spaces.

pid_t
ACE_OS::fork_exec (char *argv[])
{
#if defined (ACE_WIN32)
  ACE_ARGV argv_buf (argv);

  if (argv_buf.buf () != 0)
    {
      PROCESS_INFORMATION process_info;
      STARTUPINFO startup_info;
      ACE_OS::memset ((void *) &startup_info, 0, sizeof startup_info);
      startup_info.cb = sizeof startup_info;

      if (::CreateProcess (0,
                           (LPTSTR) ACE_WIDE_STRING (argv_buf.buf ()),
                           0, // No process attributes.
                           0,  // No thread attributes.
                           TRUE, // Allow handle inheritance.
                           0, /* CREATE_NEW_CONSOLE */ // Don't create a new console window.
                           0, // No environment.
                           0, // No current directory.
                           &startup_info,
                           &process_info))
        {
          // Free resources allocated in kernel.
          ACE_OS::close (process_info.hThread);
          ACE_OS::close (process_info.hProcess);
          // Return new process id.
          return process_info.dwProcessId;
        }
    }

  // CreateProcess failed.
  return -1;
#elif defined (CHORUS)
  return -1;                    // do it later!!!
#else
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
              ACE_ERROR ((LM_ERROR, "%p Exec failed\n"));

              // If the execv fails, this child needs to exit.
              ACE_OS::exit (errno);
            }
        default:
          // Server process.  The fork succeeded.
          return result;
        }
#endif /* ACE_WIN32 */
    }

#if defined (ACE_NEEDS_WRITEV)

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
    if (iov[i].iov_len < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;

#if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#else
  ACE_NEW_RETURN (buf, char[length], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  char *ptr = buf;

  for (i = 0; i < n; i++)
    {
      ACE_OS::memcpy (ptr, iov[i].iov_base, iov[i].iov_len);
      ptr += iov[i].iov_len;
    }

  ssize_t result = ACE::send_n (handle, buf, length);
#if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#endif /* !defined (ACE_HAS_ALLOCA) */
  return result;
}
#endif /* ACE_NEEDS_WRITEV */

#if defined (ACE_NEEDS_READV)

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
    if (iov[i].iov_len < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;
#if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#else
  ACE_NEW_RETURN (buf, char[length], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  length = ACE::recv_n (handle, buf, length);

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

#if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#endif /* !defined (ACE_HAS_ALLOCA) */
  return length;
}
#endif /* ACE_NEEDS_READV */

#if defined (ACE_NEEDS_FTRUNCATE)
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
#endif /* ACE_NEEDS_FTRUNCATE */

char *
ACE_OS::mktemp (char *s)
{
  // ACE_TRACE ("ACE_OS::mktemp");
#if defined (ACE_LACKS_MKTEMP)
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
          while (::stat (s, &sb) >= 0)
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

#else
  return ::mktemp (s);
#endif /* ACE_LACKS_MKTEMP */
}

int
ACE_OS::socket_init (int version_high, int version_low)
{
#if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ == 0)
    {
      // cout << "WSAStartup" << endl;
      WORD version_requested = MAKEWORD (version_high, version_low);
      WSADATA wsa_data;
      int error = ::WSAStartup (version_requested, &wsa_data);

      if (error != 0)
        cerr << "WSAStartup failed, WSAGetLastError returned " << error << endl;

      ACE_OS::socket_initialized_ = 1;
    }
#else
  ACE_UNUSED_ARG (version_high);
  ACE_UNUSED_ARG (version_low);
#endif /* ACE_WIN32 */
  return 0;
}

int
ACE_OS::socket_fini (void)
{
#if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ != 0)
    {
      // cout << "WSACleanup" << endl;
      if (::WSACleanup () != 0)
        {
          int error = ::WSAGetLastError ();
          cerr << "WSACleanup failed, WSAGetLastError returned " << error << endl;
        }
      ACE_OS::socket_initialized_ = 0;
    }
#endif /* ACE_WIN32 */
  return 0;
}

#if defined (ACE_LACKS_SYS_NERR)
int sys_nerr = ERRMAX + 1;
#endif /* ACE_LACKS_SYS_NERR */

#if defined (VXWORKS)
#include /**/ <usrLib.h>   /* for ::sp() */

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
  argv[0] = "spa ():t";

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
        ::fprintf (stderr, "spa(): number of arguments limited to %d\n",
                   MAX_ARGS);
    }
  else
    {
      // fill unused argv slots with 0 to get rid of leftovers
      // from previous invocations
      for (unsigned int i = argc; i <= MAX_ARGS; ++i)
        argv[i] = 0;
    }

  int ret = ::sp (entry, argc, (int) argv, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::sp () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}
#endif /* VXWORKS */

#if !defined (ACE_HAS_SIGINFO_T)
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
#endif /* ACE_HAS_SIGINFO_T */

pid_t
ACE_OS::fork (const char *program_name)
{
  // ACE_TRACE ("ACE_OS::fork");
#if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (program_name);
  ACE_NOTSUP_RETURN (pid_t (-1));
#else
  pid_t pid = ::fork ();

  if (pid == 0)
    ACE_LOG_MSG->sync (program_name);

  return pid;
#endif /* ACE_WIN32 */
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
ACE_Thread_ID::operator== (const ACE_Thread_ID &rhs)
{
  return ACE_OS::thr_cmp (this->thread_handle_, rhs.thread_handle_) == 0
    && ACE_OS::thr_equal (this->thread_id_, rhs.thread_id_) == 0;
}

int
ACE_Thread_ID::operator!= (const ACE_Thread_ID &rhs)
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
      ACE_OS::memcpy ((void *) addr, (void *) &ip_addr, sizeof ip_addr);
      return 1;
    }
}

ssize_t
ACE_OS::pread (ACE_HANDLE handle,
               void *buf,
               size_t nbytes,
               off_t offset)
{
#if defined (ACE_HAS_P_READ_WRITE)
#if defined (ACE_WIN32)
  // This will work irrespective of whether the <handle> is in
  // OVERLAPPED mode or not.
  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  DWORD bytes_written; // This is set to 0 byte WriteFile.

  if (::ReadFile (handle, buf, nbytes, &bytes_written, &overlapped))
    return (ssize_t) bytes_written;
  else if (::GetLastError () == ERROR_IO_PENDING)
    if (::GetOverlappedResult (handle, &overlapped, &bytes_written, TRUE) == TRUE)
      return (ssize_t) bytes_written;

  return -1;
#else
  return ::pread (handle, buf, nbytes, offset);
#endif /* ACE_WIN32 */
#else
  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  if (ACE_OS::lseek (handle, offset, SEEK_SET) == -1)
    return -1;
  else
    return ACE_OS::read (handle, buf, nbytes);
#endif /* ACE_HAD_P_READ_WRITE */
}

ssize_t
ACE_OS::pwrite (ACE_HANDLE handle,
                const void *buf,
                size_t nbytes,
                off_t offset)
{
#if defined (ACE_HAS_P_READ_WRITE)
#if defined (ACE_WIN32)
  // This will work irrespective of whether the <handle> is in
  // OVERLAPPED mode or not.
  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  DWORD bytes_written; // This is set to 0 byte WriteFile.

  if (::WriteFile (handle, buf, nbytes, &bytes_written, &overlapped))
    return (ssize_t) bytes_written;
  else if (::GetLastError () == ERROR_IO_PENDING)
    if (::GetOverlappedResult (handle, &overlapped, &bytes_written, TRUE) == TRUE)
      return (ssize_t) bytes_written;

  return -1;
#else
  return ::pwrite (handle, buf, nbytes, offset);
#endif /* ACE_WIN32 */
#else
  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, -1));

  if (ACE_OS::lseek (handle, offset, SEEK_SET) == -1)
    return -1;
  else
    return ACE_OS::write (handle, buf, nbytes);
#endif /* ACE_HAD_P_READ_WRITE */
}

time_t
ACE_OS::mktime (struct tm *t)
{
  // ACE_TRACE ("ACE_OS::asctime");
#if defined (ACE_HAS_MT_SAFE_MKTIME) || !defined (ACE_HAS_THREADS)
  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#else
  ACE_MT (ACE_Thread_Mutex *ace_os_monitor_lock =
    ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
      (ACE_Object_Manager::ACE_OS_MONITOR_LOCK);
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *ace_os_monitor_lock, (time_t) -1));

  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#endif /* ACE_HAS_MT_SAFE_MKTIME */
}

#if !defined (ACE_HAS_THREADS) || !defined (ACE_HAS_STHREADS)
// The ACE_HAS_THREADS and ACE_HAS_STHREADS case is in OS.i
int
ACE_OS::rwlock_init (ACE_rwlock_t *rw,
                     int type,
                     LPCTSTR name,
                     void *arg)
{
  // ACE_TRACE ("ACE_OS::rwlock_init");
  type = type;
  name = name;
#if defined (ACE_HAS_THREADS)
#if !defined (ACE_HAS_STHREADS)
  /* NT, POSIX, and VxWorks don't support this natively. */
  ACE_UNUSED_ARG (name);
  int result = -1;

  // Since we cannot use the user specified name for all three
  // objects, we will create three complete new names
  TCHAR name1[ACE_UNIQUE_NAME_LEN];
  TCHAR name2[ACE_UNIQUE_NAME_LEN];
  TCHAR name3[ACE_UNIQUE_NAME_LEN];

  ACE::unique_name ((const void *) &rw->lock_, name1,
                    ACE_UNIQUE_NAME_LEN);
  ACE::unique_name ((const void *) &rw->waiting_readers_, name2,
                    ACE_UNIQUE_NAME_LEN);
  ACE::unique_name ((const void *) &rw->waiting_writers_, name3,
                    ACE_UNIQUE_NAME_LEN);

  if (ACE_OS::mutex_init (&rw->lock_, type, name1, arg) == 0
      && ACE_OS::cond_init (&rw->waiting_readers_, type, name2, arg) == 0
      && ACE_OS::cond_init (&rw->waiting_writers_, type, name3, arg) == 0)
    {
      // Success!
      rw->ref_count_ = 0;
      rw->num_waiting_writers_ = 0;
      rw->num_waiting_readers_ = 0;

      result = 0;
    }

  if (result == -1)
    {
      int error = errno;
      ACE_OS::mutex_destroy (&rw->lock_);
      ACE_OS::cond_destroy (&rw->waiting_readers_);
      ACE_OS::cond_destroy (&rw->waiting_writers_);
      errno = error;
    }
  return result;
#endif /* ! ACE_HAS_STHREADS */
#else
  ACE_UNUSED_ARG (rw);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (arg);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_THREADS */
}
#endif /* ! ACE_HAS_THREADS || ! ACE_HAS_STHREADS */

#if defined (ACE_PSOS)

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


ACE_PSOS_Time_t::ACE_PSOS_Time_t ()
  : date_ (0), time_ (0), ticks_ (0)
{
}
  // default ctor: date, time, and ticks all zeroed

ACE_PSOS_Time_t::ACE_PSOS_Time_t (const timespec_t& t)
{
  struct tm* tm_struct = ACE_OS::gmtime (&(t.tv_sec));

  // encode date values from tm struct into pSOS date bit array

  date_  = (ACE_PSOS_Time_t::year_mask & 
            ACE_static_cast (u_long, 
                             tm_struct->tm_year + ACE_PSOS_Time_t::year_origin)) <<
           ACE_PSOS_Time_t::year_shift;

  date_ |= (ACE_PSOS_Time_t::month_mask & 
            ACE_static_cast (u_long, 
                             tm_struct->tm_mon + ACE_PSOS_Time_t::month_origin)) <<
           ACE_PSOS_Time_t::month_shift;

  date_ |= ACE_PSOS_Time_t::day_mask & 
           ACE_static_cast (u_long, tm_struct->tm_mday);


  // encode time values from tm struct into pSOS time bit array

  time_  = (ACE_PSOS_Time_t::hour_mask  & 
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


ACE_PSOS_Time_t::operator timespec_t ()
{
  struct tm tm_struct;
  
  // decode date and time bit arrays and fill in fields of tm_struct

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

  // indicate values we don't know as negative numbers
  tm_struct.tm_wday  = -1;
  tm_struct.tm_yday  = -1;
  tm_struct.tm_isdst = -1;

  timespec_t t;

  // convert calendar time to time struct
  t.tv_sec = ACE_OS::mktime (&tm_struct);

  // encode nanoseconds as system clock ticks
  t.tv_nsec = ACE_static_cast (long, 
                               ((ACE_static_cast (double, ticks_) * 
                                 ACE_static_cast (double, 1000000000)) /
                                ACE_static_cast (double, KC_TICKS2SEC)));

  return t;
}
  // type cast operator (to a timespec_t)

ACE_INLINE u_long 
ACE_PSOS_Time_t::get_system_time (ACE_PSOS_Time_t& t)
{
  u_long ret_val = 0;

#if defined (ACE_PSOSIM) /* system time is broken in simulator */

  timeval tv;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&tv, 0), int, -1, result);
  if (result == -1)
  {
    return 1;
  }

  ACE_Time_Value atv (tv);
  timespec ts = atv;
  ACE_PSOS_Time_t pt (ts);
  t.date_ = pt.date_;
  t.time_ = pt.time_;
  t.ticks_ = pt.ticks_;

#else

  ret_val = tm_get (&(t.date_), &(t.time_), &(t.ticks_));

#endif  /* ACE_PSOSIM */

  return ret_val;
}
  // static member function to get current system time

ACE_INLINE u_long 
ACE_PSOS_Time_t::set_system_time (const ACE_PSOS_Time_t& t)
{
  u_long ret_val = tm_set (t.date_, t.time_, t.ticks_);
  return ret_val;
}
  // static member function to set current system time

#if defined (ACE_PSOSIM)

ACE_INLINE u_long
ACE_PSOS_Time_t::init_simulator_time ()
{
  // This is a hack using a direct UNIX system call, because the appropriate
  // ACE_OS method ultimately uses the pSOS tm_get function, which would fail
  // because the simulator's system time is uninitialized (chicken and egg).
  timeval t;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&t, 0), int, -1, result);
  if (result == -1)
  {
    return 1;
  }
  else 
  {
    ACE_Time_Value tv (t);
    timespec ts = tv;
    ACE_PSOS_Time_t pt (ts);
    u_long ret_val = ACE_PSOS_Time_t::set_system_time (pt);
    return ret_val;

  }
}
  // static member function to initialize system time, using UNIX calls

#endif /* ACE_PSOSIM */


#endif  /* ACE_PSOS */

#if defined (CHORUS)
extern "C"
void
ace_sysconf_dump (void)
{
  ACE_Time_Value time = ACE_OS::gettimeofday ();
  if (time == -1)
    ACE_DEBUG ((LM_DEBUG, "Cannot get time\n"));
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
#endif
