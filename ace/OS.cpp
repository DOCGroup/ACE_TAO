// $Id$


#include "ace/OS.h"
#include "ace/Sched_Params.h"
#include "ace/OS_Thread_Adapter.h"

#if !defined (ACE_HAS_WINCE)
#include "ace/OS_QoS.h"
#endif  // ACE_HAS_WINCE

// Perhaps we should *always* include ace/OS.i in order to make sure
// we can always link against the OS symbols?
#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS.i"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#if defined(INTEGRITY) && defined(ACE_HAS_SHM_OPEN)
char* shm_area_name = "ACE_Area";
char* shm_area_password = "******";
#endif

ACE_RCSID(ace, OS, "$Id$")

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
# if defined (ACE_HAS_WINCE)
const wchar_t *ACE_OS::day_of_week_name[] = {ACE_LIB_TEXT ("Sun"), ACE_LIB_TEXT ("Mon"),
                                             ACE_LIB_TEXT ("Tue"), ACE_LIB_TEXT ("Wed"),
                                             ACE_LIB_TEXT ("Thu"), ACE_LIB_TEXT ("Fri"),
                                             ACE_LIB_TEXT ("Sat")};
const wchar_t *ACE_OS::month_name[] = {ACE_LIB_TEXT ("Jan"), ACE_LIB_TEXT ("Feb"),
                                       ACE_LIB_TEXT ("Mar"), ACE_LIB_TEXT ("Apr"),
                                       ACE_LIB_TEXT ("May"), ACE_LIB_TEXT ("Jun"),
                                       ACE_LIB_TEXT ("Jul"), ACE_LIB_TEXT ("Aug"),
                                       ACE_LIB_TEXT ("Sep"), ACE_LIB_TEXT ("Oct"),
                                       ACE_LIB_TEXT ("Nov"), ACE_LIB_TEXT ("Dec") };

static const ACE_TCHAR *ACE_OS_CTIME_R_FMTSTR = ACE_LIB_TEXT ("%3s %3s %02d %02d:%02d:%02d %04d\n");
# endif /* ACE_HAS_WINCE */

# if defined (ACE_WIN32)
OSVERSIONINFO ACE_OS::win32_versioninfo_;
// Cached win32 version information.

HINSTANCE ACE_OS::win32_resource_module_;

#  if defined (ACE_OS_HAS_DLL) && (ACE_OS_HAS_DLL == 1) && !defined (ACE_HAS_WINCE)
// This function is called by the OS when the ACE DLL is loaded. We
// use it to determine the default module containing ACE's resources.
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH)
    {
#if defined (ACE_DISABLES_THREAD_LIBRARY_CALLS) && (ACE_DISABLES_THREAD_LIBRARY_CALLS == 1)
      ::DisableThreadLibraryCalls (instance);
#endif /* ACE_DISABLES_THREAD_LIBRARY_CALLS */
      ACE_OS::set_win32_resource_module(instance);
    }
  return TRUE;
}
#  endif /* ACE_OS_HAS_DLL && ACE_OS_HAS_DLL == 1 */
# endif /* ACE_WIN32 */

/**
 * @class ACE_OS_Thread_Mutex_Guard
 *
 * This data structure is meant to be used within an ACE_OS
 * function.  It performs automatic aquisition and release of
 * an ACE_thread_mutex_t.
 *
 * For internal use only by ACE_OS.
 */
class ACE_OS_Thread_Mutex_Guard
{
public:
  /// Implicitly and automatically acquire the lock.
  ACE_OS_Thread_Mutex_Guard (ACE_thread_mutex_t &m);

  /// Implicitly release the lock.
  ~ACE_OS_Thread_Mutex_Guard (void);

  /// Explicitly acquire the lock.
  int acquire (void);

  /// Explicitly release the lock.
  int release (void);

protected:
  /// Reference to the mutex.
  ACE_thread_mutex_t &lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

  // = Prevent assignment and initialization.
  ACE_OS_Thread_Mutex_Guard &operator= (const ACE_OS_Thread_Mutex_Guard &);
  ACE_OS_Thread_Mutex_Guard (const ACE_OS_Thread_Mutex_Guard &);
};

#if defined (ACE_IS_SPLITTING)
# define ACE_SPECIAL_INLINE
#else
# define ACE_SPECIAL_INLINE inline
#endif

ACE_SPECIAL_INLINE
int
ACE_OS_Thread_Mutex_Guard::acquire (void)
{
  return owner_ = ACE_OS::thread_mutex_lock (&lock_);
}

ACE_SPECIAL_INLINE
int
ACE_OS_Thread_Mutex_Guard::release (void)
{
  if (owner_ == -1)
    return 0;
  else
    {
      owner_ = -1;
      return ACE_OS::thread_mutex_unlock (&lock_);
    }
}

ACE_SPECIAL_INLINE
ACE_OS_Thread_Mutex_Guard::ACE_OS_Thread_Mutex_Guard (ACE_thread_mutex_t &m)
   : lock_ (m)
{
  acquire ();
}

ACE_OS_Thread_Mutex_Guard::~ACE_OS_Thread_Mutex_Guard ()
{
  release ();
}

/**
 * @class ACE_OS_Recursive_Thread_Mutex_Guard
 *
 * @brief For internal use only by ACE_OS.
 *
 * This data structure is meant to be used within an ACE_OS
 * function.  It performs automatic aquisition and release of
 * an ACE_recursive_thread_mutex_t.
 */
class ACE_OS_Recursive_Thread_Mutex_Guard
{
public:
  /// Implicitly and automatically acquire the lock.
  ACE_OS_Recursive_Thread_Mutex_Guard (ACE_recursive_thread_mutex_t &m);

  /// Implicitly release the lock.
  ~ACE_OS_Recursive_Thread_Mutex_Guard (void);

  /// Explicitly acquire the lock.
  int acquire (void);

  /// Explicitly release the lock.
  int release (void);

protected:
  /// Reference to the mutex.
  ACE_recursive_thread_mutex_t &lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

  // = Prevent assignment and initialization.
  ACE_OS_Recursive_Thread_Mutex_Guard &operator= (
    const ACE_OS_Recursive_Thread_Mutex_Guard &);
  ACE_OS_Recursive_Thread_Mutex_Guard (
    const ACE_OS_Recursive_Thread_Mutex_Guard &);
};

ACE_SPECIAL_INLINE
int
ACE_OS_Recursive_Thread_Mutex_Guard::acquire (void)
{
  return owner_ = ACE_OS::recursive_mutex_lock (&lock_);
}

ACE_SPECIAL_INLINE
int
ACE_OS_Recursive_Thread_Mutex_Guard::release (void)
{
  if (owner_ == -1)
    return 0;
  else
    {
      owner_ = -1;
      return ACE_OS::recursive_mutex_unlock (&lock_);
    }
}

ACE_SPECIAL_INLINE
ACE_OS_Recursive_Thread_Mutex_Guard::ACE_OS_Recursive_Thread_Mutex_Guard (
  ACE_recursive_thread_mutex_t &m)
   : lock_ (m),
     owner_ (-1)
{
  acquire ();
}

ACE_OS_Recursive_Thread_Mutex_Guard::~ACE_OS_Recursive_Thread_Mutex_Guard ()
{
  release ();
}

#define ACE_OS_GUARD \
  ACE_OS_Thread_Mutex_Guard ace_os_guard__ (*(ACE_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);

#define ACE_TSS_CLEANUP_GUARD \
  ACE_OS_Recursive_Thread_Mutex_Guard ace_tss_cleanup_guard__ (*(ACE_recursive_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_TSS_CLEANUP_LOCK]);

#define ACE_TSS_BASE_GUARD \
  ACE_OS_Recursive_Thread_Mutex_Guard ace_tss_base_guard__ (*(ACE_recursive_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_TSS_BASE_LOCK]);


# if defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
int
ACE_OS::netdb_acquire (void)
{
  return ACE_OS::thread_mutex_lock ((ACE_thread_mutex_t *)
    ACE_OS_Object_Manager::preallocated_object[
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);
}

int
ACE_OS::netdb_release (void)
{
  return ACE_OS::thread_mutex_unlock ((ACE_thread_mutex_t *)
    ACE_OS_Object_Manager::preallocated_object[
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);
}
# endif /* defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS) */
#else  /* ! ACE_MT_SAFE */
# define ACE_OS_GUARD
# define ACE_TSS_CLEANUP_GUARD
# define ACE_TSS_BASE_GUARD
#endif /* ! ACE_MT_SAFE */

ACE_EXIT_HOOK ACE_OS::exit_hook_ = 0;

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

/**
 * @class ACE_Cleanup_Info_Node
 *
 * @brief For maintaining a list of ACE_Cleanup_Info items.
 *
 * For internal use by ACE_Object_Manager.
 */
class ACE_Cleanup_Info_Node
{
public:
  ACE_Cleanup_Info_Node (void);
  ACE_Cleanup_Info_Node (const ACE_Cleanup_Info &new_info,
                         ACE_Cleanup_Info_Node *next);
  ~ACE_Cleanup_Info_Node (void);
  ACE_Cleanup_Info_Node *insert (const ACE_Cleanup_Info &);
private:
  ACE_Cleanup_Info cleanup_info_;
  ACE_Cleanup_Info_Node *next_;

  friend class ACE_OS_Exit_Info;
};

ACE_Cleanup_Info_Node::ACE_Cleanup_Info_Node (void)
  : cleanup_info_ (),
    next_ (0)
{
}

ACE_Cleanup_Info_Node::ACE_Cleanup_Info_Node (const ACE_Cleanup_Info &new_info,
                                              ACE_Cleanup_Info_Node *next)
  : cleanup_info_ (new_info),
    next_ (next)
{
}

ACE_Cleanup_Info_Node::~ACE_Cleanup_Info_Node (void)
{
  delete next_;
}

ACE_Cleanup_Info_Node *
ACE_Cleanup_Info_Node::insert (const ACE_Cleanup_Info &new_info)
{
  ACE_Cleanup_Info_Node *new_node;

  ACE_NEW_RETURN (new_node,
                  ACE_Cleanup_Info_Node (new_info, this),
                  0);

  return new_node;
}

ACE_OS_Exit_Info::ACE_OS_Exit_Info (void)
{
  ACE_NEW (registered_objects_, ACE_Cleanup_Info_Node);
}

ACE_OS_Exit_Info::~ACE_OS_Exit_Info (void)
{
  delete registered_objects_;
  registered_objects_ = 0;
}

int
ACE_OS_Exit_Info::at_exit_i (void *object,
                             ACE_CLEANUP_FUNC cleanup_hook,
                             void *param)
{
  ACE_Cleanup_Info new_info;
  new_info.object_ = object;
  new_info.cleanup_hook_ = cleanup_hook;
  new_info.param_ = param;

  // Return -1 and sets errno if unable to allocate storage.  Enqueue
  // at the head and dequeue from the head to get LIFO ordering.

  ACE_Cleanup_Info_Node *new_node;

  if ((new_node = registered_objects_->insert (new_info)) == 0)
    return -1;
  else
    {
      registered_objects_ = new_node;
      return 0;
    }
}

int
ACE_OS_Exit_Info::find (void *object)
{
  // Check for already in queue, and return 1 if so.
  for (ACE_Cleanup_Info_Node *iter = registered_objects_;
       iter  &&  iter->next_ != 0;
       iter = iter->next_)
    {
      if (iter->cleanup_info_.object_ == object)
        {
          // The object has already been registered.
          return 1;
        }
    }

  return 0;
}

void
ACE_OS_Exit_Info::call_hooks ()
{
  // Call all registered cleanup hooks, in reverse order of
  // registration.
  for (ACE_Cleanup_Info_Node *iter = registered_objects_;
       iter  &&  iter->next_ != 0;
       iter = iter->next_)
    {
      ACE_Cleanup_Info &info = iter->cleanup_info_;
      if (info.cleanup_hook_ == ACE_reinterpret_cast (ACE_CLEANUP_FUNC,
                                                      ace_cleanup_destroyer))
        // The object is an ACE_Cleanup.
        ace_cleanup_destroyer (ACE_reinterpret_cast (ACE_Cleanup *,
                                                     info.object_),
                               info.param_);
      else if (info.object_ == &ace_exit_hook_marker)
        // The hook is an ACE_EXIT_HOOK.
        (* ACE_reinterpret_cast (ACE_EXIT_HOOK, info.cleanup_hook_)) ();
      else
        (*info.cleanup_hook_) (info.object_, info.param_);
    }
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
  ACE_OS_TRACE ("ACE_OS::readPPCTimeBase");

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
  ACE_OS_TRACE ("ACE_OS::readPPCTimeBase");

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
ACE_OS::uname (ACE_utsname *name)
{
  ACE_OS_TRACE ("ACE_OS::uname");
# if defined (ACE_WIN32)
  size_t maxnamelen = sizeof name->nodename;
  ACE_OS::strcpy (name->sysname,
                  ACE_LIB_TEXT ("Win32"));

  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx (&vinfo);

  SYSTEM_INFO sinfo;
#   if defined (ACE_HAS_PHARLAP)
  // PharLap doesn't do GetSystemInfo.  What's really wanted is the
  // CPU architecture, so we can get that with EtsGetSystemInfo. Fill
  // in what's wanted in the SYSTEM_INFO structure, and carry on. Note
  // that the CPU type values in EK_KERNELINFO have the same values
  // are the ones defined for SYSTEM_INFO.
  EK_KERNELINFO ets_kern;
  EK_SYSTEMINFO ets_sys;
  EtsGetSystemInfo (&ets_kern, &ets_sys);
  sinfo.wProcessorLevel = ACE_static_cast (WORD, ets_kern.CpuType);
  sinfo.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_INTEL;
  sinfo.dwProcessorType = ets_kern.CpuType * 100 + 86;
#   else
  ::GetSystemInfo(&sinfo);

  ACE_OS::strcpy (name->sysname, ACE_LIB_TEXT ("Win32"));
#   endif /* ACE_HAS_PHARLAP */

  const ACE_TCHAR* unknown = ACE_LIB_TEXT ("???");

  if (
      vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT
#   if defined (VER_PLATFORM_WIN32_CE)
      || vinfo.dwPlatformId == VER_PLATFORM_WIN32_CE
#   endif
      )
    {
      // Get information from the two structures
      const ACE_TCHAR *os;
      if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
        os = ACE_LIB_TEXT ("Windows NT %d.%d");
      else
        os = ACE_LIB_TEXT ("Windows CE %d.%d");
      ACE_OS::sprintf (name->release,
                       os,
                       (int) vinfo.dwMajorVersion,
                       (int) vinfo.dwMinorVersion);
      ACE_OS::sprintf (name->version,
                       ACE_LIB_TEXT ("Build %d %s"),
                       (int) vinfo.dwBuildNumber,
                       vinfo.szCSDVersion);

      // We have to make sure that the size of (processor + subtype)
      // is not greater than the size of name->machine.  So we give
      // half the space to the processor and half the space to
      // subtype.  The -1 is necessary for because of the space
      // between processor and subtype in the machine name.
      const int bufsize = ((sizeof (name->machine) / sizeof (ACE_TCHAR)) / 2) - 1;
      ACE_TCHAR processor[bufsize] = ACE_LIB_TEXT ("Unknown");
      ACE_TCHAR subtype[bufsize] = ACE_LIB_TEXT ("Unknown");

#   if defined (ghs)
    WORD arch = sinfo.u.s.wProcessorArchitecture;
#   else
    WORD arch = sinfo.wProcessorArchitecture;
#   endif

      switch (arch)
        {
        case PROCESSOR_ARCHITECTURE_INTEL:
          ACE_OS::strcpy (processor, ACE_LIB_TEXT ("Intel"));
          if (sinfo.wProcessorLevel == 3)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("80386"));
          else if (sinfo.wProcessorLevel == 4)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("80486"));
          else if (sinfo.wProcessorLevel == 5)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("Pentium"));
          else if (sinfo.wProcessorLevel == 6)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("Pentium Pro"));
          else if (sinfo.wProcessorLevel == 7)  // I'm guessing here
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("Pentium II"));
          break;
        case PROCESSOR_ARCHITECTURE_MIPS:
          ACE_OS::strcpy (processor, ACE_LIB_TEXT ("MIPS"));
          ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("R4000"));
          break;
        case PROCESSOR_ARCHITECTURE_ALPHA:
          ACE_OS::strcpy (processor, ACE_LIB_TEXT ("Alpha"));
          ACE_OS::sprintf (subtype, ACE_LIB_TEXT ("%d"), sinfo.wProcessorLevel);
          break;
        case PROCESSOR_ARCHITECTURE_PPC:
          ACE_OS::strcpy (processor, ACE_LIB_TEXT ("PPC"));
          if (sinfo.wProcessorLevel == 1)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("601"));
          else if (sinfo.wProcessorLevel == 3)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("603"));
          else if (sinfo.wProcessorLevel == 4)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("604"));
          else if (sinfo.wProcessorLevel == 6)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("603+"));
          else if (sinfo.wProcessorLevel == 9)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("804+"));
          else if (sinfo.wProcessorLevel == 20)
            ACE_OS::strcpy (subtype, ACE_LIB_TEXT ("620"));
          break;
#     if defined PROCESSOR_ARCHITECTURE_IA64
        case PROCESSOR_ARCHITECTURE_IA64:
          ACE_OS_String::strcpy (processor, ACE_LIB_TEXT ("Itanium"));
          ACE_OS::sprintf (subtype, ACE_LIB_TEXT ("%d"),
                           sinfo.wProcessorLevel);
          break;
#     endif
#     if defined PROCESSOR_ARCHITECTURE_ARM
        case PROCESSOR_ARCHITECTURE_ARM:
          ACE_OS_String::strcpy (processor, ACE_LIB_TEXT ("ARM"));
          ACE_OS::sprintf (subtype, ACE_LIB_TEXT ("%d"),
                           sinfo.wProcessorLevel);
          break;
#     endif
        case PROCESSOR_ARCHITECTURE_UNKNOWN:
        default:
          // @@ We could provide WinCE specific info here.  But let's
          //    defer that to some later point.
          ACE_OS::strcpy (processor, ACE_LIB_TEXT ("Unknown"));
          break;
        }
      ACE_OS::sprintf (name->machine,
                       ACE_LIB_TEXT ("%s %s"),
                       processor, subtype);
    }
  else if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
      if (vinfo.dwMajorVersion == 4 && vinfo.dwMinorVersion == 0)
        {
          ACE_OS::strcpy (name->release, ACE_LIB_TEXT ("Windows 95"));
          if (vinfo.szCSDVersion[1] == 'C')
            ACE_OS::strcat (name->release, ACE_LIB_TEXT (" OSR2"));
        }
      else if (vinfo.dwMajorVersion == 4 && vinfo.dwMinorVersion == 10)
        {
          ACE_OS::strcpy (name->release, ACE_LIB_TEXT ("Windows 98"));
          if (vinfo.szCSDVersion[1] == 'A')
            ACE_OS::strcat (name->release, ACE_LIB_TEXT (" SE"));
        }
      else if (vinfo.dwMajorVersion == 4 && vinfo.dwMinorVersion == 90)
        {
          ACE_OS::strcpy (name->release, ACE_LIB_TEXT ("Windows Me"));
        }
      else
        {
          ACE_OS::strcpy (name->release, unknown);
        }

      ACE_OS::sprintf (name->version, ACE_LIB_TEXT ("%d"),
                       LOWORD (vinfo.dwBuildNumber));
      if (sinfo.dwProcessorType == PROCESSOR_INTEL_386)
        ACE_OS::strcpy (name->machine, ACE_LIB_TEXT ("Intel 80386"));
      else if (sinfo.dwProcessorType == PROCESSOR_INTEL_486)
        ACE_OS::strcpy (name->machine, ACE_LIB_TEXT ("Intel 80486"));
      else if (sinfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM)
        ACE_OS::strcpy (name->machine, ACE_LIB_TEXT ("Intel Pentium"));
      else
        ACE_OS::strcpy (name->machine, unknown);
    }
  else
    {
      // We don't know what this is!

      ACE_OS::strcpy (name->release, unknown);
      ACE_OS::strcpy (name->version, unknown);
      ACE_OS::strcpy (name->machine, unknown);
    }

# if defined (ACE_LACKS_HOSTNAME)
  return 0;
# else /* ACE_LACKS_HOSTNAME */
  return ACE_OS::hostname (name->nodename, maxnamelen);
# endif /* ACE_LACKS_HOSTNAME */

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
  const unsigned long buflen(64);
  char buf[buflen];
  unsigned long len;
  sys_info(PSOS_VERSION,(void *)buf,buflen,&len);
  ACE_OS::strcpy (name->sysname, "pSOS");
  ACE_OS::strcpy (name->release, "???");
  ACE_OS::strcpy (name->version, buf);
  ACE_OS::strcpy (name->machine, "PPC 405");  // a bit of a hack

#endif /* ACE_WIN32 */
}
#endif /* ACE_WIN32 || VXWORKS */


#if defined (VXWORKS)
struct hostent *
ACE_OS::gethostbyname (const char *name)
{
  ACE_OS_TRACE ("ACE_OS::gethostbyname");

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
  ACE_OS_TRACE ("ACE_OS::gethostbyaddr");

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
  ACE_OS_TRACE ("ACE_OS::gethostbyaddr_r");
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
  ACE_OS_TRACE ("ACE_OS::gethostbyname_r");

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

// Leave this in the global scope to allow
// users to adjust the delay value.
int ACE_THR_JOIN_DELAY = 5;

int
ACE_OS::thr_join (ACE_hthread_t thr_handle,
                  ACE_THR_FUNC_RETURN *status)
{
  // We can't get the status of the thread
  if (status != 0)
    {
      *status = 0;
    }

  // This method can not support joining all threads
  if (ACE_OS::thr_cmp (thr_handle, ACE_OS::NULL_hthread))
    {
      ACE_NOTSUP_RETURN (-1);
    }

  int retval = ESRCH;
  ACE_hthread_t current;
  ACE_OS::thr_self (current);

  // Make sure we are not joining ourself
  if (ACE_OS::thr_cmp (thr_handle, current))
    {
      retval = EDEADLK;
    }
  else
    {
      // Whether the task exists or not
      // we will return a successful value
      retval = 0;

      // Verify that the task id still exists
      while (taskIdVerify (thr_handle) == OK)
        {
          // Wait a bit to see if the task is still active.
          ACE_OS::sleep (ACE_THR_JOIN_DELAY);
        }
    }

  // Adapt the return value into errno and return value.
  // The ACE_ADAPT_RETVAL macro doesn't exactly do what
  // we need to do here, so we do it manually.
  if (retval != 0)
    {
      errno = retval;
      retval = -1;
    }

  return retval;
}

int
ACE_OS::thr_join (ACE_thread_t waiter_id,
                  ACE_thread_t *thr_id,
                  ACE_THR_FUNC_RETURN *status)
{
  thr_id = 0;
  return ACE_OS::thr_join (taskNameToId (waiter_id), status);
}
#endif /* VXWORKS */

void
ACE_OS::ace_flock_t::dump (void) const
{
  ACE_OS_TRACE ("ACE_OS::ace_flock_t::dump");

#if 0
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("handle_ = %u"), this->handle_));
#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nInternal = %d"), this->overlapped_.Internal));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nInternalHigh = %d"), this->overlapped_.InternalHigh));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nOffsetHigh = %d"), this->overlapped_.OffsetHigh));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nhEvent = %d"), this->overlapped_.hEvent));
#elif !defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_whence = %d"), this->lock_.l_whence));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_start = %d"), this->lock_.l_start));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_len = %d"), this->lock_.l_len));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\nl_type = %d"), this->lock_.l_type));
#endif /* ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* 0 */
}

void
ACE_OS::mutex_lock_cleanup (void *mutex)
{
  ACE_OS_TRACE ("ACE_OS::mutex_lock_cleanup");
#if defined (ACE_HAS_THREADS)
#  if defined (ACE_HAS_PTHREADS)
  ACE_mutex_t *p_lock = (ACE_mutex_t *) mutex;
  ACE_OS::mutex_unlock (p_lock);
#  else
  ACE_UNUSED_ARG (mutex);
#  endif /* ACE_HAS_PTHREADS */
# else
  ACE_UNUSED_ARG (mutex);
# endif /* ACE_HAS_THREADS */
}

#if defined (ACE_USES_WCHAR)
void ACE_OS::checkUnicodeFormat (FILE* fp)
{
    if (fp != 0)
    {
        // Due to the ACE_TCHAR definition, all default input files, such as
        // svc.conf, have to be in Unicode format (small endian) on WinCE
        // because ACE has all 'char' converted into ACE_TCHAR.
        // However, for TAO, ASCII files, such as IOR file, can still be read and
        // be written without any error since given buffers are all in 'char' type
        // instead of ACE_TCHAR.  Therefore, it is user's reponsibility to select
        // correct buffer type.

        // At this point, check if the file is Unicode or not.
        WORD first_two_bytes;
        int numRead = ACE_OS::fread(&first_two_bytes, sizeof(WORD), 1, fp);

        if (numRead == 1)
        {
            if ((first_two_bytes != 0xFFFE) &&  // not a small endian Unicode file
                (first_two_bytes != 0xFEFF))    // not a big endian Unicode file
            {
                ACE_OS::fseek(fp, 0, FILE_BEGIN);  // set file pointer back to the beginning
            }
        }
        // if it is a Unicode file, file pointer will be right next to the first two-bytes
    }
}
#endif  // ACE_USES_WCHAR

#if defined (ACE_WIN32)
FILE *
ACE_OS::fopen (const ACE_TCHAR *filename,
               const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  int hmode = _O_TEXT;

  for (const ACE_TCHAR *mode_ptr = mode; *mode_ptr != 0; mode_ptr++)
    ACE_OS::fopen_mode_to_open_mode_converter (*mode_ptr, hmode);

  ACE_HANDLE handle = ACE_OS::open (filename, hmode);
  if (handle != ACE_INVALID_HANDLE)
    {
# if defined (ACE_HAS_WINCE)
      FILE *fp = ::_wfdopen (handle, mode);
      if (fp != 0)
      {
        checkUnicodeFormat(fp);
        return fp;
      }
# else
      hmode &= _O_TEXT | _O_RDONLY | _O_APPEND;
#   if defined (ACE_WIN64)
      int fd = _open_osfhandle (intptr_t (handle), hmode);
#   else
      int fd = _open_osfhandle (long (handle), hmode);
#   endif /* ACE_WIN64 */
      if (fd != -1)
        {
#   if defined (__BORLANDC__) && !defined (ACE_USES_WCHAR)
          FILE *fp = ::_fdopen (fd, ACE_const_cast (char *, mode));
#   elif defined (__BORLANDC__) && defined (ACE_USES_WCHAR)
          FILE *fp = ::_wfdopen (fd, ACE_const_cast (wchar_t *, mode));
#   elif defined (ACE_USES_WCHAR)
          FILE *fp = ::_wfdopen (fd, mode);
#   else
          FILE *fp = ::fdopen (fd, mode);
#   endif /* defined(__BORLANDC__) && !defined (ACE_USES_WCHAR)) */
          if (fp != 0)
          {
#   if defined (ACE_USES_WCHAR)
            checkUnicodeFormat(fp);
#   endif  // ACE_USES_WCHAR
            return fp;
          }
          _close (fd);
        }
# endif  // ACE_HAS_WINCE

      ACE_OS::close (handle);
    }
  return 0;
}
#endif /* ACE_WIN32 */

// The following *printf functions aren't inline because
// they use varargs.

int
ACE_OS::fprintf (FILE *fp, const char *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::fprintf");
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vfprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::fprintf (FILE *fp, const wchar_t *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::fprintf");

# if !defined (ACE_HAS_VFWPRINTF)
  ACE_UNUSED_ARG (fp);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);

# else
  int result = 0;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vfwprintf (fp, format, ap), int, -1, result);
  va_end (ap);
  return result;

# endif /* ACE_HAS_VFWPRINTF */
}
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::printf (const char *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::printf");
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
  // ACE_OS_TRACE ("ACE_OS::sprintf");

  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vsprintf (buf, format, ap)), int, -1, result);
  va_end (ap);
  return result;
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::sprintf (wchar_t *buf, const wchar_t *format, ...)
{
  ACE_OS_TRACE ("ACE_OS::sprintf");

# if defined (_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)

  // The XPG4/UNIX98/C99 signature of the wide-char sprintf has a
  // maxlen argument. Since this method doesn't supply one, pass in
  // the max possible length. If this isn't ok, use ACE_OS::snprintf().
  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vswprintf (buf, ULONG_MAX, format, ap), int, -1, result);
  va_end (ap);
  return result;

# elif defined (ACE_WIN32)
  // Windows has vswprintf, but the signature is from the older ISO C
  // standard. Also see ACE_OS::snprintf() for more info on this.

  int result;
  va_list ap;
  va_start (ap, format);
  ACE_OSCALL (::vswprintf (buf, format, ap), int, -1, result);
  va_end (ap);
  return result;

# else

  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);

# endif /* ACE_HAS_VSWPRINTF */
}
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::snprintf (char *buf, size_t maxlen, const char *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::snprintf");
#if defined (ACE_HAS_SNPRINTF)
  int result;
  va_list ap;
  va_start (ap, format);
#  if defined (ACE_WIN32)
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::_vsnprintf (buf, maxlen, format, ap)),
              int, -1, result);
  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buf[maxlen-1] = '\0';
#  else
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vsnprintf (buf, maxlen, format, ap)),
              int, -1, result);
#  endif /* ACE_WIN32 */
  va_end (ap);
  // In out-of-range conditions, C99 defines vsnprintf to return the number
  // of characters that would have been written if enough space was available.
  // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
  // -1. This method follows the C99 standard, but needs to guess at the
  // value; uses maxlen + 1.
  if (result == -1)
    result = ACE_static_cast (int, (maxlen + 1));
  return result;

#else
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SNPRINTF */
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::snprintf (wchar_t *buf, size_t maxlen, const wchar_t *format, ...)
{
  // ACE_OS_TRACE ("ACE_OS::snprintf");
#if (defined (_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) || defined (ACE_WIN32)
  int result;
  va_list ap;
  va_start (ap, format);
#  if defined (ACE_WIN32)
  // Microsoft's vswprintf() doesn't have the maxlen argument that
  // XPG4/UNIX98 define. They do, however, recommend use of _vsnwprintf()
  // as a substitute, which does have the same signature as the UNIX98 one.
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::_vsnwprintf (buf, maxlen, format, ap)),
              int, -1, result);
  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buf[maxlen-1] = '\0';
#  else
  ACE_OSCALL (ACE_SPRINTF_ADAPTER (::vswprintf (buf, maxlen, format, ap)),
              int, -1, result);
#  endif /* ACE_WIN32 */
  va_end (ap);
  // In out-of-range conditions, C99 defines vsnprintf to return the number
  // of characters that would have been written if enough space was available.
  // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
  // -1. This method follows the C99 standard, but needs to guess at the
  // value; uses maxlen + 1.
  if (result == -1)
    result = ACE_static_cast (int, (maxlen + 1));
  return result;

#else
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SNPRINTF */
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS::gets (char *str, int n)
{
  ACE_OS_TRACE ("ACE_OS::gets");
  int c;
  char *s = str;

  if (str == 0 || n < 0) n = 0;
  if (n == 0) str = 0;
  else n--;

  while ((c = getchar ()) != '\n')
    {

      if (c == EOF && errno == EINTR)
        {
#   if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
          continue;
#   else
          break;
#   endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */
        }

      if (c == EOF)
        break;

      if (n > 0)
        n--, *s++ = c;
    }
  if (s) *s = '\0';

  return (c == EOF) ? 0 : str;
}

int
ACE_OS::execl (const char * /* path */, const char * /* arg0 */, ...)
{
  ACE_OS_TRACE ("ACE_OS::execl");
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
  ACE_OS_TRACE ("ACE_OS::execle");
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
  ACE_OS_TRACE ("ACE_OS::execlp");
#if defined (ACE_WIN32) || defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  //  ACE_OSCALL_RETURN (::execvp (file, argv), int, -1);
#endif /* ACE_WIN32 */
}

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
      // SunOS doesn't support non-zero quantums in time-sharing class:  use
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

  if (ACE_OS::priority_control ((idtype_t) (sched_params.scope () == ACE_SCOPE_THREAD
                                            ? ACE_SCOPE_PROCESS
                                            : sched_params.scope ()),
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
  ACE_OS_TRACE ("ACE_OS::sched_params");
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
  // has only been tested for POSIX 1003.1c threads, and may cause
  // problems with other PThreads flavors!

  struct sched_param param;
  param.sched_priority = sched_params.priority ();

  if (sched_params.scope () == ACE_SCOPE_PROCESS)
    {
      int result = ::sched_setscheduler (0, // this process
                                         sched_params.policy (),
                                         &param) == -1 ? -1 : 0;
#   if defined (DIGITAL_UNIX)
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
      return (::pthread_setscheduler (thr_id,
                                      sched_params.policy (),
                                      sched_params.priority()) == -1 ? -1 : 0);
#   else
      int result;
      ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (thr_id,
                                                                    sched_params.policy (),
                                                                    &param),
                                           result),
                         int, -1);
#   endif  /* ACE_HAS_PTHREADS_DRAFT4 */
    }
#   if defined (sun)
  // We need to be able to set LWP priorities on Suns, even without
  // ACE_HAS_STHREADS, to obtain preemption.
  else if (sched_params.scope () == ACE_SCOPE_LWP)
    return ACE_OS::set_scheduling_params (sched_params, id);
#   endif /* sun */
  else // sched_params.scope () == ACE_SCOPE_LWP, which isn't POSIX
    {
      errno = EINVAL;
      return -1;
    }

# elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  // PharLap ETS can act on the current thread - it can set the
  // quantum also, unlike Win32. All this only works on the RT
  // version.
#   if defined (ACE_HAS_PHARLAP_RT)
  if (id != ACE_SELF)
    ACE_NOTSUP_RETURN (-1);

  if (sched_params.quantum() != ACE_Time_Value::zero)
    EtsSetTimeSlice (sched_params.quantum().msec());

#   else
  ACE_UNUSED_ARG (id);

  if (sched_params.scope () != ACE_SCOPE_PROCESS
      || sched_params.quantum () != ACE_Time_Value::zero)
    {
      // Win32 only allows setting priority class (therefore, policy)
      // at the process level.  I don't know of a way to set the
      // quantum.
      errno = EINVAL;
      return -1;
    }

  // Set the priority class of this process to the REALTIME process class
  // _if_ the policy is ACE_SCHED_FIFO.  Otherwise, set to NORMAL.
  if (!::SetPriorityClass (::GetCurrentProcess (),
                           (sched_params.policy () == ACE_SCHED_FIFO ||
			   sched_params.policy () == ACE_SCHED_RR)
                           ? REALTIME_PRIORITY_CLASS
                           : NORMAL_PRIORITY_CLASS))
    {
      ACE_OS::set_errno_to_last_error ();
      return -1;
    }
#   endif /* ACE_HAS_PHARLAP_RT */

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());

# elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (id);

  // There is only one class of priorities on VxWorks, and no time
  // quanta.  So, just set the current thread's priority.

  if (sched_params.policy () != ACE_SCHED_FIFO
      || sched_params.scope () != ACE_SCOPE_PROCESS
      || sched_params.quantum () != ACE_Time_Value::zero)
    {
      errno = EINVAL;
      return -1;
    }

  // Set the thread priority on the current thread.
  return ACE_OS::thr_setprio (sched_params.priority ());
# else
  ACE_UNUSED_ARG (sched_params);
  ACE_UNUSED_ARG (id);
  ACE_NOTSUP_RETURN (-1);
# endif /* CHORUS */
}

// = Static initialization.

// This is necessary to deal with POSIX pthreads insanity.  This
// guarantees that we've got a "zero'd" thread id even when
// ACE_thread_t, ACE_hthread_t, and ACE_thread_key_t are implemented
// as structures...  Under no circumstances should these be given
// initial values.
// Note: these three objects require static construction.
ACE_thread_t ACE_OS::NULL_thread;
ACE_hthread_t ACE_OS::NULL_hthread;
#if defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  ACE_thread_key_t ACE_OS::NULL_key = ACE_static_cast (ACE_thread_key_t, -1);
#else  /* ! ACE_HAS_TSS_EMULATION */
  ACE_thread_key_t ACE_OS::NULL_key;
#endif /* ! ACE_HAS_TSS_EMULATION */

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

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))

// Moved class ACE_TSS_Ref declaration to OS.h so it can be visible to
// the single file of template instantiations.

ACE_TSS_Ref::ACE_TSS_Ref (ACE_thread_t id)
  : tid_(id)
{
  ACE_OS_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

ACE_TSS_Ref::ACE_TSS_Ref (void)
{
  ACE_OS_TRACE ("ACE_TSS_Ref::ACE_TSS_Ref");
}

// Check for equality.
int
ACE_TSS_Ref::operator== (const ACE_TSS_Ref &info) const
{
  ACE_OS_TRACE ("ACE_TSS_Ref::operator==");

  return this->tid_ == info.tid_;
}

// Check for inequality.
ACE_SPECIAL_INLINE
int
ACE_TSS_Ref::operator != (const ACE_TSS_Ref &tss_ref) const
{
  ACE_OS_TRACE ("ACE_TSS_Ref::operator !=");

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
  ACE_OS_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
}

ACE_TSS_Info::ACE_TSS_Info (void)
  : key_ (ACE_OS::NULL_key),
    destructor_ (0),
    tss_obj_ (0),
    thread_count_ (-1)
{
  ACE_OS_TRACE ("ACE_TSS_Info::ACE_TSS_Info");
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
  ACE_OS_TRACE ("ACE_TSS_Info::operator==");

  return this->key_ == info.key_;
}

// Check for inequality.
int
ACE_TSS_Info::operator != (const ACE_TSS_Info &info) const
{
  ACE_OS_TRACE ("ACE_TSS_Info::operator !=");

  return !(*this == info);
}

void
ACE_TSS_Info::dump (void)
{
  //  ACE_OS_TRACE ("ACE_TSS_Info::dump");

#if 0
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("key_ = %u\n"), this->key_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("destructor_ = %u\n"), this->destructor_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("tss_obj_ = %u\n"), this->tss_obj_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* 0 */
}

// Moved class ACE_TSS_Keys declaration to OS.h so it can be visible
// to the single file of template instantiations.

ACE_TSS_Keys::ACE_TSS_Keys (void)
{
  for (u_int i = 0; i < ACE_WORDS; ++i)
    {
      key_bit_words_[i] = 0;
    }
}

ACE_SPECIAL_INLINE
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

int
ACE_TSS_Keys::is_set (const ACE_thread_key_t key) const
{
  ACE_KEY_INDEX (key_index, key);
  u_int word, bit;
  find (key_index, word, bit);

  return ACE_BIT_ENABLED (key_bit_words_[word], 1 << bit);
}


/**
 * @class ACE_TSS_Cleanup
 *
 * @brief Singleton that knows how to clean up all the thread-specific
 * resources for Win32.
 *
 * All this nonsense is required since Win32 doesn't
 * automatically cleanup thread-specific storage on thread exit,
 * unlike real operating systems... ;-)
 */
class ACE_TSS_Cleanup
{
public:
  static ACE_TSS_Cleanup *instance (void);

  ~ACE_TSS_Cleanup (void);

  /// Cleanup the thread-specific objects.  Does _NOT_ exit the thread.
  void exit (void *status);

  /// Insert a <key, destructor> tuple into the table.
  int insert (ACE_thread_key_t key, void (*destructor)(void *), void *inst);

  /// Remove a <key, destructor> tuple from the table.
  int remove (ACE_thread_key_t key);

  /// Detaches a tss_instance from its key.
  int detach (void *inst);

  /// Mark a key as being used by this thread.
  void key_used (ACE_thread_key_t key);

  /// Free all keys left in the table before destruction.
  int free_all_keys_left (void);

  /// Indication of whether the ACE_TSS_CLEANUP_LOCK is usable, and
  /// therefore whether we are in static constructor/destructor phase
  /// or not.
  static int lockable () { return instance_ != 0; }

protected:
  void dump (void);

  /// Ensure singleton.
  ACE_TSS_Cleanup (void);

private:
  // Array of <ACE_TSS_Info> objects.
  typedef ACE_TSS_Info ACE_TSS_TABLE[ACE_DEFAULT_THREAD_KEYS];
  typedef ACE_TSS_Info *ACE_TSS_TABLE_ITERATOR;

  /// Table of <ACE_TSS_Info>'s.
  ACE_TSS_TABLE table_;

  /// Key for the thread-specific array of whether each TSS key is in use.
  ACE_thread_key_t in_use_;

  /// Accessor for this threads ACE_TSS_Keys instance.
  ACE_TSS_Keys *tss_keys ();

#if defined (ACE_HAS_TSS_EMULATION)
  /// Key that is used by in_use_.  We save this key so that we know
  /// not to call its destructor in free_all_keys_left ().
  ACE_thread_key_t in_use_key_;
#endif /* ACE_HAS_TSS_EMULATION */

  // = Static data.
  /// Pointer to the singleton instance.
  static ACE_TSS_Cleanup *instance_;
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
  ACE_OS_TRACE ("ACE_TSS_Cleanup::exit");

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  ACE_TSS_Info info_arr[ACE_DEFAULT_THREAD_KEYS];
  int info_ix = 0;

  // While holding the lock, we only collect the ACE_TSS_Info objects
  // in an array without invoking the according destructors.
  {
    ACE_TSS_CLEANUP_GUARD

    // Iterate through all the thread-specific items and free them all
    // up.

    for (unsigned int i = 0;
         i < ACE_DEFAULT_THREAD_KEYS;
         ++key_info, ++i)
      {
        if (key_info->key_ == ACE_OS::NULL_key  ||
            ! key_info->key_in_use ()) continue;

        // If the key's ACE_TSS_Info in-use bit for this thread was set,
        // unset it and decrement the key's thread_count_.
        if (! tss_keys ()->test_and_clear (key_info->key_))
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

  // Acquire the ACE_TSS_CLEANUP_LOCK, then free TLS keys and remove
  // entries from ACE_TSS_Info table.
  {
    ACE_TSS_CLEANUP_GUARD

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
# if defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
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
ACE_TSS_Cleanup::free_all_keys_left (void)
  // This is called from ACE_OS::cleanup_tss ().  When this gets
  // called, all threads should have exited except the main thread.
  // No key should be freed from this routine.  It there's any,
  // something might be wrong.
{
  ACE_thread_key_t key_arr[ACE_DEFAULT_THREAD_KEYS];
  ACE_TSS_TABLE_ITERATOR key_info = table_;
  unsigned int idx = 0;
  unsigned int i;

  for (i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
#if defined (ACE_HAS_TSS_EMULATION)
    if (key_info->key_ != in_use_key_)
#endif /* ACE_HAS_TSS_EMULATION */
      // Don't call ACE_OS::thr_keyfree () on ACE_TSS_Cleanup's own
      // key.  See the comments in ACE_OS::thr_key_detach ():  the key
      // doesn't get detached, so it will be in the table here.
      // However, there's no resource associated with it, so we don't
      // need to keyfree it.  The dynamic memory associated with it
      // was already deleted by ACE_TSS_Cleanup::exit (), so we don't
      // want to access it again.
      key_arr [idx++] = key_info->key_;

  for (i = 0; i < idx; i++)
    if (key_arr[i] != ACE_OS::NULL_key)
#if defined (ACE_HAS_TSS_EMULATION)
      ACE_OS::thr_keyfree (key_arr[i]);
#elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
      // Don't call ACE_OS::thr_keyfree here.  It will try to use
      // <in_use_> which has already been cleaned up here.
      ::tsd_delete (key_arr[i]);
#else /* ACE_WIN32 */
      // Don't call ACE_OS::thr_keyfree here.  It will try to use
      // <in_use_> which has already been cleaned up here.
      TlsFree (key_arr[i]);
#endif /* ACE_HAS_TSS_EMULATION */

  return 0;
}

extern "C" void
ACE_TSS_Cleanup_keys_destroyer (void *tss_keys)
{
  delete ACE_reinterpret_cast (ACE_TSS_Keys *, tss_keys);
}

ACE_TSS_Cleanup::ACE_TSS_Cleanup (void)
  : in_use_ (ACE_OS::NULL_key)
#if defined (ACE_HAS_TSS_EMULATION)
    // ACE_TSS_Emulation::total_keys () provides the value of the next
    // key to be created.
  , in_use_key_ (ACE_TSS_Emulation::total_keys ())
#endif /* ACE_HAS_TSS_EMULATION */
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::ACE_TSS_Cleanup");
}

ACE_TSS_Cleanup *
ACE_TSS_Cleanup::instance (void)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::instance");

  // Create and initialize thread-specific key.
  if (ACE_TSS_Cleanup::instance_ == 0)
    {
      // Insure that we are serialized!
      ACE_TSS_CLEANUP_GUARD

      // Now, use the Double-Checked Locking pattern to make sure we
      // only create the ACE_TSS_Cleanup instance once.
      if (ACE_TSS_Cleanup::instance_ == 0)
        ACE_NEW_RETURN (ACE_TSS_Cleanup::instance_,
                        ACE_TSS_Cleanup,
                        0);
    }

  return ACE_TSS_Cleanup::instance_;
}

int
ACE_TSS_Cleanup::insert (ACE_thread_key_t key,
                         void (*destructor)(void *),
                         void *inst)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::insert");
  ACE_TSS_CLEANUP_GUARD

  ACE_KEY_INDEX (key_index, key);
  if (key_index < ACE_DEFAULT_THREAD_KEYS)
    {
      table_[key_index] = ACE_TSS_Info (key, destructor, inst);
      return 0;
    }
  else
    {
      return -1;
    }
}

int
ACE_TSS_Cleanup::remove (ACE_thread_key_t key)
{
  ACE_OS_TRACE ("ACE_TSS_Cleanup::remove");
  ACE_TSS_CLEANUP_GUARD

  ACE_KEY_INDEX (key_index, key);
  if (key_index < ACE_DEFAULT_THREAD_KEYS)
    {
      // "Remove" the TSS_Info table entry by zeroing out its key_ and
      // destructor_ fields.  Also, keep track of the number threads
      // using the key.
      ACE_TSS_Info &info = this->table_ [key_index];

      // Don't bother to test/clear the in "use bit" if the program is
      // shutting down.  Doing so will cause a new ACE_TSS object to be
      // created again.
      if (!ACE_OS_Object_Manager::shutting_down ())
        tss_keys ()->test_and_clear (info.key_);
      info.key_in_use (0);
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
  ACE_TSS_CLEANUP_GUARD

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  int success = 0;
  int ref_cnt = 0;

  // Mark the key as detached in the TSS_Info table.
  // It only works for the first key that "inst" owns.
  // I don't know why.
  for (unsigned int i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
    {
      if (key_info->tss_obj_ == inst)
        {
          key_info->tss_obj_ = 0;
          ref_cnt = --key_info->thread_count_;
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
# if defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
      ACE_thread_key_t temp_key = key_info->key_;
# endif /* ACE_WIN32 */
      int retv = this->remove (key_info->key_);

# if defined (ACE_WIN32)
      ::TlsFree (temp_key);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
      ::tsd_delete (temp_key);
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
  if (! tss_keys ()->test_and_set (key))
    {
      ACE_TSS_CLEANUP_GUARD

      // Retrieve the key's ACE_TSS_Info and increment its thread_count_.
      ACE_KEY_INDEX (key_index, key);
      ACE_TSS_Info &key_info = this->table_ [key_index];
      if (!key_info.key_in_use ())
        key_info.key_in_use (1);
      else
        ++key_info.thread_count_;
    }
}

void
ACE_TSS_Cleanup::dump (void)
{
  // Iterate through all the thread-specific items and dump them all.

  ACE_TSS_TABLE_ITERATOR key_info = table_;
  for (unsigned int i = 0;
       i < ACE_DEFAULT_THREAD_KEYS;
       ++key_info, ++i)
    key_info->dump ();
}

ACE_TSS_Keys *
ACE_TSS_Cleanup::tss_keys ()
{
  if (in_use_ == ACE_OS::NULL_key)
    {
      ACE_TSS_CLEANUP_GUARD
      // Double-check;
      if (in_use_ == ACE_OS::NULL_key)
        {
          // Initialize in_use_ with a new key.
          if (ACE_OS::thr_keycreate (&in_use_,
                                     &ACE_TSS_Cleanup_keys_destroyer))
            return 0; // Major problems, this should *never* happen!
        }
    }

  ACE_TSS_Keys *ts_keys = 0;
  if (ACE_OS::thr_getspecific (in_use_,
        ACE_reinterpret_cast (void **, &ts_keys)) == -1)
    return 0; // This should not happen!

  if (ts_keys == 0)
    {
      ACE_NEW_RETURN (ts_keys,
                      ACE_TSS_Keys,
                      0);
      // Store the dynamically allocated pointer in thread-specific
      // storage.
      if (ACE_OS::thr_setspecific (in_use_,
            ACE_reinterpret_cast (void *,
                                  ts_keys)) == -1)
        {
          delete ts_keys;
          return 0; // Major problems, this should *never* happen!
        }
    }

  return ts_keys;
}

# if defined (ACE_HAS_TSS_EMULATION)
u_int ACE_TSS_Emulation::total_keys_ = 0;

ACE_TSS_Keys ACE_TSS_Emulation::tss_keys_used_;

ACE_TSS_Emulation::ACE_TSS_DESTRUCTOR
ACE_TSS_Emulation::tss_destructor_[ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX]
 = { 0 };

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)

int ACE_TSS_Emulation::key_created_ = 0;

ACE_OS_thread_key_t ACE_TSS_Emulation::native_tss_key_;

/* static */
#     if defined (ACE_HAS_THR_C_FUNC)
extern "C"
void
ACE_TSS_Emulation_cleanup (void *ptr)
{
   ACE_UNUSED_ARG (ptr);
   // Really this must be used for ACE_TSS_Emulation code to make the TSS
   // cleanup
}
#else
void
ACE_TSS_Emulation_cleanup (void *ptr)
{
   ACE_UNUSED_ARG (ptr);
   // Really this must be used for ACE_TSS_Emulation code to make the TSS
   // cleanup
}
#     endif /* ACE_HAS_THR_C_FUNC */

void **
ACE_TSS_Emulation::tss_base (void* ts_storage[], u_int *ts_created)
{
  // TSS Singleton implementation.

  // Create the one native TSS key, if necessary.
  if (key_created_ == 0)
    {
      // Double-checked lock . . .
      ACE_TSS_BASE_GUARD

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
      if (ts_created)
        *ts_created = 1u;

      // Use the ts_storage passed as argument, if non-zero.  It is
      // possible that this has been implemented in the stack. At the
      // moment, this is unknown.  The cleanup must not do nothing.
      // If ts_storage is zero, allocate (and eventually leak) the
      // storage array.
      if (ts_storage == 0)
        {
          ACE_NO_HEAP_CHECK;

          ACE_NEW_RETURN (ts_storage,
                          void*[ACE_TSS_THREAD_KEYS_MAX],
                          0);

          // Zero the entire TSS array.  Do it manually instead of
          // using memset, for optimum speed.  Though, memset may be
          // faster :-)
          void **tss_base_p = ts_storage;

          for (u_int i = 0;
               i < ACE_TSS_THREAD_KEYS_MAX;
               ++i)
            *tss_base_p++ = 0;
        }

       // Store the pointer in thread-specific storage.  It gets
       // deleted via the ACE_TSS_Emulation_cleanup function when the
       // thread terminates.
       if (ACE_OS::thr_setspecific (native_tss_key_,
                                    (void *) ts_storage) != 0)
          return 0; // Major problems, this should *never* happen!
    }
  else
    if (ts_created)
      ts_created = 0;

  return ts_storage  ?  ts_storage  :  old_ts_storage;
}
#   endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */

u_int
ACE_TSS_Emulation::total_keys ()
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  return total_keys_;
}

int
ACE_TSS_Emulation::next_key (ACE_thread_key_t &key)
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  if (total_keys_ < ACE_TSS_THREAD_KEYS_MAX)
    {
       u_int counter = 0;
       // Loop through all possible keys and check whether a key is free
       for ( ;counter < ACE_TSS_THREAD_KEYS_MAX; counter++)
         {
            ACE_thread_key_t localkey;
#   if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
              ACE_OS::memset (&localkey, 0, sizeof (ACE_thread_key_t));
              ACE_OS::memcpy (&localkey, &counter_, sizeof (u_int));
#   else
              localkey = counter;
#   endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */
            // If the key is not set as used, we can give out this key, if not
            // we have to search further
            if (tss_keys_used_.is_set(localkey) == 0)
            {
               tss_keys_used_.test_and_set(localkey);
               key = localkey;
               break;
            }
         }

      ++total_keys_;
      return 0;
    }
  else
    {
      key = ACE_OS::NULL_key;
      return -1;
    }
}

int
ACE_TSS_Emulation::release_key (ACE_thread_key_t key)
{
  ACE_OS_Recursive_Thread_Mutex_Guard (
    *ACE_static_cast (ACE_recursive_thread_mutex_t *,
                      ACE_OS_Object_Manager::preallocated_object[
                        ACE_OS_Object_Manager::ACE_TSS_KEY_LOCK]));

  if (tss_keys_used_.test_and_clear (key) == 0)
  {
    --total_keys_;
    return 0;
  }
  return 1;
}

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

  u_int ts_created = 0;
  tss_base (ts_storage, &ts_created);
  if (ts_created)
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

void
ACE_TSS_Emulation::tss_close ()
{
#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  // Free native_tss_key_ here.
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
}

# endif /* ACE_HAS_TSS_EMULATION */

#endif /* WIN32 || ACE_HAS_TSS_EMULATION */

void
ACE_OS::cleanup_tss (const u_int main_thread)
{
#if defined (ACE_HAS_TSS_EMULATION) || defined (ACE_WIN32) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  // Call TSS destructors for current thread.
  ACE_TSS_Cleanup::instance ()->exit (0);
#endif /* ACE_HAS_TSS_EMULATION || ACE_WIN32 || ACE_PSOS_HAS_TSS */

  if (main_thread)
    {
#if !defined (ACE_HAS_TSS_EMULATION)  &&  !defined (ACE_HAS_MINIMAL_ACE_OS)
      // Just close the ACE_Log_Msg for the current (which should be
      // main) thread.  We don't have TSS emulation; if there's native
      // TSS, it should call its destructors when the main thread
      // exits.
      ACE_Base_Thread_Adapter::close_log_msg ();
#endif /* ! ACE_HAS_TSS_EMULATION  &&  ! ACE_HAS_MINIMAL_ACE_OS */

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
#if ! defined (ACE_HAS_TSS_EMULATION)
      // Don't do this with TSS_Emulation, because the the
      // ACE_TSS_Cleanup::instance () has already exited ().  We can't
      // safely access the TSS values that were created by the main
      // thread.

      // Remove all TSS_Info table entries.
      ACE_TSS_Cleanup::instance ()->free_all_keys_left ();
#endif /* ! ACE_HAS_TSS_EMULATION */

      // Finally, free up the ACE_TSS_Cleanup instance.  This method gets
      // called by the ACE_Object_Manager.
      delete ACE_TSS_Cleanup::instance ();
#endif /* WIN32 || ACE_HAS_TSS_EMULATION || ACE_PSOS_HAS_TSS */

#if defined (ACE_HAS_TSS_EMULATION)
      ACE_TSS_Emulation::tss_close ();
#endif /* ACE_HAS_TSS_EMULATION */
    }
}

#if !defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
       (*THR_ID = ::_beginthreadex ((void(_Optlink*)(void*))ENTRY_POINT, STACK, STACKSIZE, ARGS), *THR_ID)
#elif defined(ACE_WIN32) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)

struct __IBMCPP__thread_params {
  __IBMCPP__thread_params(ACE_THR_C_FUNC e, LPVOID a)
    :entry_point(e),args(a) {}
  ACE_THR_C_FUNC entry_point;
  LPVOID args;
};

#pragma handler(initThread)
extern "C" DWORD __stdcall __IBMCPP__initThread(void *arg)
{
  // Must reset 387 since using CreateThread
  _fpreset();

  // Dispatch user function...
  auto_ptr<__IBMCPP__thread_params> parms((__IBMCPP__thread_params *)arg);
  (*parms->entry_point)(parms->args);
  _endthread();
  return 0;
}

HANDLE WINAPI __IBMCPP__beginthreadex(void *stack,
                                      DWORD stacksize,
                                      ACE_THR_C_FUNC entry_point,
                                      LPVOID args,
                                      DWORD flags,
                                      LPDWORD thr_id)
{
  return  CreateThread(0,
                       stacksize,
                       (LPTHREAD_START_ROUTINE)__IBMCPP__initThread,
                       new __IBMCPP__thread_params(entry_point, args),
                       flags,
                       thr_id);
}

#define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
             __IBMCPP__beginthreadex(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID)

#elif defined (ACE_HAS_WINCE) && defined (UNDER_CE) && (UNDER_CE >= 211)
#define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
      CreateThread (0, STACKSIZE, (unsigned long (__stdcall *) (void *)) ENTRY_POINT, ARGS, (FLAGS) & CREATE_SUSPENDED, (unsigned long *) THR_ID)
#elif defined(ACE_HAS_WTHREADS)
  // Green Hills compiler gets confused when __stdcall is imbedded in
  // parameter list, so we define the type ACE_WIN32THRFUNC_T and use it
  // instead.
  typedef unsigned (__stdcall *ACE_WIN32THRFUNC_T)(void*);
#define ACE_BEGINTHREADEX(STACK, STACKSIZE, ENTRY_POINT, ARGS, FLAGS, THR_ID) \
      ::_beginthreadex (STACK, STACKSIZE, (ACE_WIN32THRFUNC_T) ENTRY_POINT, ARGS, FLAGS, (unsigned int *) THR_ID)
#endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
int ACE_SEH_Default_Exception_Selector (void *)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("(%t) Win32 structured exception exiting thread\n")));
#endif /* 0 */
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

int
ACE_OS::thr_create (ACE_THR_FUNC func,
                    void *args,
                    long flags,
                    ACE_thread_t *thr_id,
                    ACE_hthread_t *thr_handle,
                    long priority,
                    void *stack,
                    size_t stacksize,
                    ACE_Base_Thread_Adapter *thread_adapter)
{
  ACE_OS_TRACE ("ACE_OS::thr_create");

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


  ACE_Base_Thread_Adapter *thread_args;
  if (thread_adapter == 0)

# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
    ACE_NEW_RETURN (thread_args,
                    ACE_OS_Thread_Adapter (func, args,
                                           (ACE_THR_C_FUNC) ace_thread_adapter,
                                           ACE_OS_Object_Manager::seh_except_selector(),
                                           ACE_OS_Object_Manager::seh_except_handler()),
                    -1);
# else
  ACE_NEW_RETURN (thread_args,
                  ACE_OS_Thread_Adapter (func, args,
                                         (ACE_THR_C_FUNC) ace_thread_adapter),
                  -1);

# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  else
    thread_args = thread_adapter;

# if defined (ACE_HAS_THREADS)

  // *** Set Stack Size
#   if defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
  if (stacksize < ACE_NEEDS_HUGE_THREAD_STACKSIZE)
    stacksize = ACE_NEEDS_HUGE_THREAD_STACKSIZE;
#   endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

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

#     if defined (CHORUS)
  // If it is a super actor, we can't set stacksize.  But for the time
  // being we are all non-super actors.  Should be fixed to take care
  // of super actors!!!
  if (stacksize == 0)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
  else if (stacksize < ACE_CHORUS_DEFAULT_MIN_STACK_SIZE)
    stacksize = ACE_CHORUS_DEFAULT_MIN_STACK_SIZE;
#     endif /*CHORUS */

# if defined (ACE_HAS_PTHREAD_SETSTACK)
  if ((stacksize != 0) && (stack != 0))
# else
  if (stacksize != 0)
# endif /* ACE_HAS_PTHREAD_SETSTACK */
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
#         if defined (ACE_HAS_PTHREAD_SETSTACK)
        if (ACE_ADAPT_RETVAL(pthread_attr_setstack (&attr, stack, size), result) == -1)
#         else
        if (ACE_ADAPT_RETVAL(pthread_attr_setstacksize (&attr, size), result) == -1)
#         endif /* ACE_HAS_PTHREAD_SETSTACK */
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
#     if !defined (ACE_HAS_PTHREAD_SETSTACK)
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
#     endif /* ACE_HAS_PTHREAD_SETSTACK */

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
          // SunOS, thru version 5.6, only supports SCHED_OTHER.
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
          sparam.sched_priority = ACE_MIN (priority,
                                           (long) PTHREAD_MAX_PRIORITY);
#       elif defined (PTHREAD_MAX_PRIORITY) && !defined(ACE_HAS_PTHREADS_STD)
          /* For MIT pthreads... */
          sparam.prio = ACE_MIN (priority, PTHREAD_MAX_PRIORITY);
#       elif defined(ACE_HAS_PTHREADS_STD) && !defined (ACE_HAS_STHREADS)
          // The following code forces priority into range.
          if (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_FIFO_MAX,
                       ACE_MAX (ACE_THR_PRI_FIFO_MIN, priority));
          else if (ACE_BIT_ENABLED(flags, THR_SCHED_RR))
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_RR_MAX,
                       ACE_MAX (ACE_THR_PRI_RR_MIN, priority));
          else // Default policy, whether set or not
            sparam.sched_priority =
              ACE_MIN (ACE_THR_PRI_OTHER_MAX,
                       ACE_MAX (ACE_THR_PRI_OTHER_MIN, priority));
#       elif defined (PRIORITY_MAX)
          sparam.sched_priority = ACE_MIN (priority,
                                           (long) PRIORITY_MAX);
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
#         if defined (sun)  &&  defined (ACE_HAS_ONLY_SCHED_OTHER)
            // SunOS, through 5.6, POSIX only allows priorities > 0 to
            // ::pthread_attr_setschedparam.  If a priority of 0 was
            // requested, set the thread priority after creating it, below.
            if (priority > 0)
#         endif /* sun && ACE_HAS_ONLY_SCHED_OTHER */
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
#         if defined (ACE_CONFIG_LINUX_H) || defined (HPUX)
          // LinuxThreads do not have support for PTHREAD_SCOPE_PROCESS.
          // Neither does HPUX (up to HP-UX 11.00, as far as I know).
          int scope = PTHREAD_SCOPE_SYSTEM;
#         else /* ACE_CONFIG_LINUX_H */
          int scope = PTHREAD_SCOPE_PROCESS;
#         endif /* ACE_CONFIG_LINUX_H */
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
          // SunOS semantics.
          int lwps = ACE_OS::thr_getconcurrency ();
          if (lwps == -1)
            {
              if (errno == ENOTSUP)
                // Suppress the ENOTSUP because it's harmless.
                errno = 0;
              else
                // This should never happen on SunOS:
                // ::thr_getconcurrency () should always succeed.
                return -1;
            }
          else if (ACE_OS::thr_setconcurrency (lwps + 1) == -1)
            {
              if (errno == ENOTSUP)
                {
                  // Unlikely:  ::thr_getconcurrency () is supported but
                  // ::thr_setconcurrency () is not?
                }
              else
                return -1;
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
      // just given as an argument.  This will cause
      // ACE_Thread_Manager (if it's doing this create) to lose track
      // of the new thread since the ID will be passed back equal to
      // 0.  So give pthread_detach a junker to scribble on.
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

  // This is a SunOS or POSIX implementation of pthreads, where we
  // assume that ACE_thread_t and ACE_hthread_t are the same.  If this
  // *isn't* correct on some platform, please let us know.
  if (result != -1)
    *thr_handle = *thr_id;

#     if defined (sun)  &&  defined (ACE_HAS_ONLY_SCHED_OTHER)
  // SunOS prior to 5.7:

  // If the priority is 0, then we might have to set it now because we
  // couldn't set it with ::pthread_attr_setschedparam, as noted
  // above.  This doesn't provide strictly correct behavior, because
  // the thread was created (above) with the priority of its parent.
  // (That applies regardless of the inherit_sched attribute: if it
  // was PTHREAD_INHERIT_SCHED, then it certainly inherited its
  // parent's priority.  If it was PTHREAD_EXPLICIT_SCHED, then "attr"
  // was initialized by the SunOS ::pthread_attr_init () to contain
  // NULL for the priority, which indicated to SunOS ::pthread_create
  // () to inherit the parent priority.)
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

      // The only policy supported by by SunOS, thru version 5.6,
      // is SCHED_OTHER, so that's hard-coded here.
      policy = ACE_SCHED_OTHER;

      if (sparam.sched_priority != 0)
        {
          ACE_OS::memset ((void *) &sparam, 0, sizeof sparam);
          // The memset to 0 sets the priority to 0, so we don't need
          // to explicitly set sparam.sched_priority.

          ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_setschedparam (*thr_id,
                                                                        policy,
                                                                        &sparam),
                                               result),
                             int, -1);
        }
    }

#       if defined (ACE_NEEDS_LWP_PRIO_SET)
#         if 0
  // It would be useful if we could make this work.  But, it requires
  // a mechanism for determining the ID of an LWP to which another
  // thread is bound.  Is there a way to do that?  Instead, just rely
  // on the code in ACE_Thread_Adapter::invoke () to set the LWP
  // priority.

  // If the thread is bound, then set the priority on its LWP.
  if (ACE_BIT_ENABLED (flags, THR_BOUND))
    {
      ACE_Sched_Params sched_params (ACE_BIT_ENABLED (flags, THR_SCHED_FIFO) ||
                                     ACE_BIT_ENABLED (flags, THR_SCHED_RR)  ?
                                     ACE_SCHED_FIFO  :
                                     ACE_SCHED_OTHER,
                                     priority);
      result = ACE_OS::lwp_setparams (sched_params,
                                      /* ? How do we find the ID of the LWP
                                         to which *thr_id is bound? */);
    }
#         endif /* 0 */
#       endif /* ACE_NEEDS_LWP_PRIO_SET */

#     endif /* sun && ACE_HAS_ONLY_SCHED_OTHER */
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
      // With SunOS threads, ACE_thread_t and ACE_hthread_t are the same.
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
        ::AfxBeginThread ((AFX_THREADPROC) thread_args->entry_point (),
                          thread_args,
                          priority,
                          0,
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

      *thr_handle = (void *) ACE_BEGINTHREADEX (0,
                                                ACE_static_cast
                                                   (u_int, stacksize),
                                                thread_args->entry_point (),
                                                thread_args,
                                                flags,
                                                thr_id);

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

  // set the stacksize to a default value if no size is specified
  if (stacksize == 0)
    stacksize = ACE_PSOS_DEFAULT_STACK_SIZE;

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
               //             T_NOTSLICE |           // Task is not timesliced with other tasks at same priority
               T_TSLICE |             // Task is timesliced with other tasks at same priority
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
            ACE_OS::strsncpy (*thr_id + 1, ::taskTcb (tid)->name, 10);
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
ACE_OS::thr_exit (ACE_THR_FUNC_RETURN status)
{
  ACE_OS_TRACE ("ACE_OS::thr_exit");
#if defined (ACE_HAS_THREADS)
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
    // An ACE_Thread_Descriptor really is an ACE_OS_Thread_Descriptor.
    // But without #including ace/Thread_Manager.h, we don't know that.
    ACE_OS_Thread_Descriptor *td =
      ACE_Base_Thread_Adapter::thr_desc_log_msg ();
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
          ::AfxEndThread (status);
        else
          ACE_ENDTHREADEX (status);
      }
    else
      {
        // Not spawned by ACE_Thread_Manager, use the old buggy
        // version.  You should seriously consider using
        // ACE_Thread_Manager to spawn threads.  The following code is
        // know to cause some problem.
        CWinThread *pThread = ::AfxGetThread ();
        if (!pThread || pThread->m_nThreadID != ACE_OS::thr_self ())
          ACE_ENDTHREADEX (status);
        else
          ::AfxEndThread (status);
      }
#     else
    ACE_ENDTHREADEX (status);
#     endif /* ACE_HAS_MFC && ACE_HAS_MFS != 0*/

#   elif defined (VXWORKS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    *((int *) status) = ::taskDelete (tid);
#   elif defined (ACE_PSOS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);

#     if defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    // Call TSS destructors.
    ACE_OS::cleanup_tss (0 /* not main thread */);
#     endif /* ACE_PSOS && ACE_PSOS_HAS_TSS */

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

      sched_params.policy (ACE_SCHED_OTHER);
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
  // ACE_OS_TRACE ("ACE_OS::thr_setspecific");
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
  // ACE_OS_TRACE ("ACE_OS::thr_setspecific");
#if defined (ACE_HAS_THREADS)
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
#   elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    ACE_hthread_t tid;
    ACE_OS::thr_self (tid);
    if (::tsd_setval (key, tid, data) != 0)
     return -1;
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
#   elif defined (ACE_HAS_WTHREADS)
    ::TlsSetValue (key, data);
    ACE_TSS_Cleanup::instance ()->key_used (key);
    return 0;
#   else
    ACE_UNUSED_ARG (key);
    ACE_UNUSED_ARG (data);
    ACE_NOTSUP_RETURN (-1);
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
  ACE_OS_TRACE ("ACE_OS::thr_keyfree");
# if defined (ACE_HAS_THREADS)
#   if defined (ACE_HAS_TSS_EMULATION)
    // Release the key in the TSS_Emulation administration
    ACE_TSS_Emulation::release_key (key);
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
#   elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)
    // Extract out the thread-specific table instance and free up
    // the key and destructor.
    ACE_TSS_Cleanup::instance ()->remove (key);
    return (::tsd_delete (key) == 0) ? 0 : -1;
#   else
    ACE_UNUSED_ARG (key);
    ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_TSS_EMULATION */
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
  // ACE_OS_TRACE ("ACE_OS::thr_keycreate");
#   if defined (ACE_HAS_THREADS)
#     if defined (ACE_HAS_PTHREADS)
    ACE_UNUSED_ARG (inst);


#       if defined (ACE_HAS_PTHREADS_DRAFT4)
#         if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#         else  /* ! ACE_HAS_STDARG_THR_DEST */
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#         endif /* ! ACE_HAS_STDARG_THR_DEST */
#       elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#       else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#       endif /* ACE_HAS_PTHREADS_DRAFT4 */
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
  // ACE_OS_TRACE ("ACE_OS::thr_keycreate");
#if defined (ACE_HAS_THREADS)
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

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
#       if defined (ACE_HAS_STDARG_THR_DEST)
    ACE_OSCALL_RETURN (::pthread_keycreate (key, (void (*)(...)) dest), int, -1);
#       else  /* ! ACE_HAS_STDARG_THR_DEST */
    ACE_OSCALL_RETURN (::pthread_keycreate (key, dest), int, -1);
#       endif /* ! ACE_HAS_STDARG_THR_DEST */
#     elif defined (ACE_HAS_PTHREADS_DRAFT6)
    ACE_OSCALL_RETURN (::pthread_key_create (key, dest), int, -1);
#     else
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_key_create (key, dest),
                                         ace_result_),
                       int, -1);
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */

#   elif defined (ACE_HAS_STHREADS)
    ACE_UNUSED_ARG (inst);
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::thr_keycreate (key, dest),
                                         ace_result_),
                       int, -1);
#     elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS)

    static u_long unique_name = 0;
    void *tsdanchor;

    ++unique_name;
    if (::tsd_create (ACE_reinterpret_cast (char *, &unique_name),
                      0,
                      TSD_NOALLOC,
                      (void ****) &tsdanchor,
                      key) != 0)
      {
        return -1;
      }

    return ACE_TSS_Cleanup::instance ()->insert (*key, dest, inst);
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
#   else
    ACE_UNUSED_ARG (key);
    ACE_UNUSED_ARG (dest);
    ACE_UNUSED_ARG (inst);
    ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_TSS_EMULATION */
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
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
  ACE_TSS_Cleanup::instance ()->key_used (key);
  return 0;
# else
  ACE_UNUSED_ARG (key);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_WIN32 || ACE_HAS_TSS_EMULATION || ACE_PSOS_HAS_TSS */
}

int
ACE_OS::thr_key_detach (void *inst)
{
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
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

void
ACE_OS::unique_name (const void *object,
                     ACE_TCHAR *name,
                     size_t length)
{
  // The process ID will provide uniqueness between processes on the
  // same machine. The "this" pointer of the <object> will provide
  // uniqueness between other "live" objects in the same process. The
  // uniqueness of this name is therefore only valid for the life of
  // <object>.
  ACE_TCHAR temp_name[ACE_UNIQUE_NAME_LEN];
  ACE_OS::sprintf (temp_name,
                   ACE_LIB_TEXT ("%p%d"),
                   object,
                   ACE_static_cast (int, ACE_OS::getpid ()));
  ACE_OS::strsncpy (name,
                    temp_name,
                    length);
}

int
ACE_OS::argv_to_string (ACE_TCHAR **argv,
                        ACE_TCHAR *&buf,
                        int substitute_env_args)
{
  if (argv == 0 || argv[0] == 0)
    return 0;

  size_t buf_len = 0;

  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
      ACE_TCHAR *temp = 0;

#if !defined (ACE_LACKS_ENV)
      // Account for environment variables.
      if (substitute_env_args
          && (argv[i][0] == '$'
              && (temp = ACE_OS::getenv (&argv[i][1])) != 0))
        buf_len += ACE_OS_String::strlen (temp);
      else
#endif /* ACE_LACKS_ENV */
        buf_len += ACE_OS_String::strlen (argv[i]);

      // Add one for the extra space between each string.
      buf_len++;
    }

  // Step through all argv params and copy each one into buf; separate
  // each param with white space.

  ACE_NEW_RETURN (buf,
                  ACE_TCHAR[buf_len + 1],
                  0);

  // Initial null charater to make it a null string.
  buf[0] = '\0';
  ACE_TCHAR *end = buf;
  int j;

  for (j = 0; argv[j] != 0; j++)
    {
      ACE_TCHAR *temp = 0;

# if !defined (ACE_LACKS_ENV)
      // Account for environment variables.
      if (substitute_env_args
      && (argv[j][0] == '$'
              && (temp = ACE_OS::getenv (&argv[j][1])) != 0))
        end = ACE_OS::strecpy (end, temp);
      else
#endif /* ACE_LACKS_ENV */
        end = ACE_OS::strecpy (end, argv[j]);

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
ACE_OS::string_to_argv (ACE_TCHAR *buf,
                        int &argc,
                        ACE_TCHAR **&argv,
                        int substitute_env_args)
{
  // Reset the number of arguments
  argc = 0;

  if (buf == 0)
    return -1;

  ACE_TCHAR *cp = buf;

  // First pass: count arguments.

  // '#' is the start-comment token..
  while (*cp != ACE_LIB_TEXT ('\0') && *cp != ACE_LIB_TEXT ('#'))
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*cp))
        cp++;

      // Increment count and move to next whitespace..
      if (*cp != ACE_LIB_TEXT ('\0'))
        argc++;

      while (*cp != ACE_LIB_TEXT ('\0') && !ACE_OS::ace_isspace (*cp))
        {
          // Grok quotes....
          if (*cp == ACE_LIB_TEXT ('\'') || *cp == ACE_LIB_TEXT ('"'))
            {
              ACE_TCHAR quote = *cp;

              // Scan past the string..
              for (cp++; *cp != ACE_LIB_TEXT ('\0') && *cp != quote; cp++)
                continue;

              // '\0' implies unmatched quote..
              if (*cp == ACE_LIB_TEXT ('\0'))
                {
                  argc--;
                  break;
                }
              else
                cp++;
            }
          else
            cp++;
        }
    }

  // Second pass: copy arguments.
  ACE_TCHAR arg[ACE_DEFAULT_ARGV_BUFSIZ];
  ACE_TCHAR *argp = arg;

  // Make sure that the buffer we're copying into is always large
  // enough.
  if (cp - buf >= ACE_DEFAULT_ARGV_BUFSIZ)
    ACE_NEW_RETURN (argp,
                    ACE_TCHAR[cp - buf + 1],
                    -1);

  // Make a new argv vector of argc + 1 elements.
  ACE_NEW_RETURN (argv,
                  ACE_TCHAR *[argc + 1],
                  -1);

  ACE_TCHAR *ptr = buf;

  for (int i = 0; i < argc; i++)
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*ptr))
        ptr++;

      // Copy next argument and move to next whitespace..
      cp = argp;
      while (*ptr != ACE_LIB_TEXT ('\0') && !ACE_OS::ace_isspace (*ptr))
        if (*ptr == ACE_LIB_TEXT ('\'') || *ptr == ACE_LIB_TEXT ('"'))
          {
            ACE_TCHAR quote = *ptr++;

            while (*ptr != ACE_LIB_TEXT ('\0') && *ptr != quote)
              *cp++ = *ptr++;

            if (*ptr == quote)
              ptr++;
          }
        else
          *cp++ = *ptr++;

      *cp = ACE_LIB_TEXT ('\0');

#if !defined (ACE_LACKS_ENV)
      // Check for environment variable substitution here.
      if (substitute_env_args) {
          argv[i] = ACE_OS::strenvdup(argp);

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
      }
      else
#endif /* ACE_LACKS_ENV */
        {
          argv[i] = ACE_OS::strdup(argp);

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
        }
    }

  if (argp != arg)
    delete [] argp;

  argv[argc] = 0;
  return 0;
}

// Create a contiguous command-line argument buffer with each arg
// separated by spaces.

pid_t
ACE_OS::fork_exec (ACE_TCHAR *argv[])
{
# if defined (ACE_WIN32)
  ACE_TCHAR *buf;

  if (ACE_OS::argv_to_string (argv, buf) != -1)
    {
      PROCESS_INFORMATION process_info;
#   if !defined (ACE_HAS_WINCE)
      ACE_TEXT_STARTUPINFO startup_info;
      ACE_OS::memset ((void *) &startup_info,
                      0,
                      sizeof startup_info);
      startup_info.cb = sizeof startup_info;

      if (ACE_TEXT_CreateProcess (0,
                                  buf,
                                  0, // No process attributes.
                                  0,  // No thread attributes.
                                  TRUE, // Allow handle inheritance.
                                  0, // Don't create a new console window.
                                  0, // No environment.
                                  0, // No current directory.
                                  &startup_info,
                                  &process_info))
#   else
      if (ACE_TEXT_CreateProcess (0,
                                  buf,
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
              // The OS layer should not print stuff out
              // ACE_ERROR ((LM_ERROR,
              //             "%p Exec failed\n"));

              // If the execv fails, this child needs to exit.
              ACE_OS::exit (errno);
            }
        default:
          // Server process.  The fork succeeded.
          return result;
        }
# endif /* ACE_WIN32 */
}

ssize_t
ACE_OS::read_n (ACE_HANDLE handle,
                void *buf,
                size_t len,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = ACE_OS::read (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred);

      if (n == -1 || n == 0)
        return n;
    }

  return bytes_transferred;
}

// Write <len> bytes from <buf> to <handle> (uses the <write>
// system call on UNIX and the <WriteFile> call on Win32).

ssize_t
ACE_OS::write_n (ACE_HANDLE handle,
                 const void *buf,
                 size_t len,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = ACE_OS::write (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred);

      if (n == -1 || n == 0)
        return n;
    }

  return bytes_transferred;
}

# if defined (ACE_LACKS_WRITEV)

// "Fake" writev for operating systems without it.  Note that this is
// thread-safe.

int
ACE_OS::writev_emulation (ACE_HANDLE handle, ACE_WRITEV_TYPE iov[], int n)
{
  ACE_OS_TRACE ("ACE_OS::writev_emulation");

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
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  char *ptr = buf;

  for (i = 0; i < n; i++)
    {
      ACE_OS::memcpy (ptr, iov[i].iov_base, iov[i].iov_len);
      ptr += iov[i].iov_len;
    }

  ssize_t result = ACE_OS::write (handle, buf, length);
#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return result;
}
# endif /* ACE_LACKS_WRITEV */

# if defined (ACE_LACKS_READV)

// "Fake" readv for operating systems without it.  Note that this is
// thread-safe.

ssize_t
ACE_OS::readv_emulation (ACE_HANDLE handle,
                         ACE_READV_TYPE *iov,
                         int n)
{
  ACE_OS_TRACE ("ACE_OS::readv_emulation");

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
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  length = ACE_OS::read (handle, buf, length);

  if (length != -1)
    {
      char *ptr = buf;
      ssize_t copyn = length;

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

  return ACE_OS::fcntl (handle, F_FREESP, ACE_reinterpret_cast (long, &fl));
}
# endif /* ACE_NEEDS_FTRUNCATE */

# if defined (ACE_LACKS_MKTEMP)
ACE_TCHAR *
ACE_OS::mktemp (ACE_TCHAR *s)
{
  ACE_OS_TRACE ("ACE_OS::mktemp");
  if (s == 0)
    // check for null template string failed!
    return 0;
  else
    {
      ACE_TCHAR *xxxxxx = ACE_OS::strstr (s, ACE_LIB_TEXT ("XXXXXX"));

      if (xxxxxx == 0)
        // the template string doesn't contain "XXXXXX"!
        return s;
      else
        {
          ACE_TCHAR unique_letter = ACE_LIB_TEXT ('a');
          ACE_stat sb;

          // Find an unused filename for this process.  It is assumed
          // that the user will open the file immediately after
          // getting this filename back (so, yes, there is a race
          // condition if multiple threads in a process use the same
          // template).  This appears to match the behavior of the
          // SunOS 5.5 mktemp().
          ACE_OS::sprintf (xxxxxx,
                           ACE_LIB_TEXT ("%05d%c"),
                           ACE_OS::getpid (),
                           unique_letter);
          while (ACE_OS::stat (s, &sb) >= 0)
            {
              if (++unique_letter <= ACE_LIB_TEXT ('z'))
                ACE_OS::sprintf (xxxxxx,
                                 ACE_LIB_TEXT ("%05d%c"),
                                 ACE_OS::getpid (),
                                 unique_letter);
              else
                {
                  // maximum of 26 unique files per template, per process
                  ACE_OS::sprintf (xxxxxx, ACE_LIB_TEXT ("%s"), ACE_LIB_TEXT (""));
                  return s;
                }
            }
        }
      return s;
    }
}
# endif /* ACE_LACKS_MKTEMP */

int
ACE_OS::socket_init (int version_high, int version_low)
{
# if defined (ACE_WIN32)
  if (ACE_OS::socket_initialized_ == 0)
    {
      WORD version_requested = MAKEWORD (version_high, version_low);
      WSADATA wsa_data;
      int error = WSAStartup (version_requested, &wsa_data);

      if (error != 0)
#   if defined (ACE_HAS_WINCE)
        {
          wchar_t fmt[] = ACE_LIB_TEXT ("%s failed, WSAGetLastError returned %d");
          wchar_t buf[80];  // @@ Eliminate magic number.
          ACE_OS::sprintf (buf, fmt, ACE_LIB_TEXT ("WSAStartup"), error);
          ::MessageBox (0, buf, ACE_LIB_TEXT ("WSAStartup failed!"), MB_OK);
        }
#   else
      ACE_OS::fprintf (stderr,
                       "ACE_OS::socket_init; WSAStartup failed, "
                         "WSAGetLastError returned %d\n",
                       error);
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
      if (WSACleanup () != 0)
        {
          int error = ::WSAGetLastError ();
#   if defined (ACE_HAS_WINCE)
          wchar_t fmt[] = ACE_LIB_TEXT ("%s failed, WSAGetLastError returned %d");
          wchar_t buf[80];  // @@ Eliminate magic number.
          ACE_OS::sprintf (buf, fmt, ACE_LIB_TEXT ("WSACleanup"), error);
          ::MessageBox (0, buf , ACE_LIB_TEXT ("WSACleanup failed!"), MB_OK);
#   else
          ACE_OS::fprintf (stderr,
                           "ACE_OS::socket_fini; WSACleanup failed, "
                             "WSAGetLastError returned %d\n",
                           error);
#   endif /* ACE_HAS_WINCE */
        }
      ACE_OS::socket_initialized_ = 0;
    }
# endif /* ACE_WIN32 */
  return 0;
}

# if defined (ACE_LACKS_SYS_NERR)
# if defined (__rtems__)
int sys_nerr = EWOULDBLOCK + 1;  // definitely a hack.
# else
 int sys_nerr = ERRMAX + 1;
# endif /* __rtems__ */
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
                               ACE_NEEDS_HUGE_THREAD_STACKSIZE, // stack size
                               entry,      // entry point
                               argc,       // first argument to main ()
                               (int) argv, // second argument to main ()
                               0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::taskSpawn () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}



// A helper function for the extended spa functions
static void
add_to_argv (int& argc, char** argv, int max_args, char* string)
{
  char indouble   = 0;
  size_t previous = 0;
  size_t length   = ACE_OS_String::strlen (string);

  // We use <= to make sure that we get the last argument
  for (size_t i = 0; i <= length; i++)
    {
      // Is it a double quote that hasn't been escaped?
      if (string[i] == '\"' && (i == 0 || string[i - 1] != '\\'))
        {
          indouble ^= 1;
          if (indouble)
            {
              // We have just entered a double quoted string, so
              // save the starting position of the contents.
              previous = i + 1;
            }
          else
            {
              // We have just left a double quoted string, so
              // zero out the ending double quote.
              string[i] = '\0';
            }
        }
      else if (string[i] == '\\')  // Escape the next character
        {
          // The next character is automatically
          // skipped because of the strcpy
          ACE_OS_String::strcpy (string + i, string + i + 1);
          length--;
        }
      else if (!indouble &&
               (ACE_OS::ace_isspace (string[i]) || string[i] == '\0'))
        {
          string[i] = '\0';
          if (argc < max_args)
            {
              argv[argc] = string + previous;
              argc++;
            }
          else
            {
              ACE_OS::fprintf (stderr, "spae(): number of arguments "
                                       "limited to %d\n", max_args);
            }

          // Skip over whitespace in between arguments
          for(++i; i < length && ACE_OS::ace_isspace (string[i]); ++i)
            {
            }

          // Save the starting point for the next time around
          previous = i;

          // Make sure we don't skip over a character due
          // to the above loop to skip over whitespace
          i--;
        }
    }
}

// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.
//
// usage: -> spae main, "arg1 arg2 \"arg3 with spaces\""
//
// All arguments must be within double quotes, even numbers.
int
spae (FUNCPTR entry, ...)
{
  static const int WINDSH_ARGS = 10;
  static const int MAX_ARGS    = 128;
  static char* argv[MAX_ARGS]  = { "ace_main", 0 };
  va_list pvar;
  int argc = 1;

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.
  va_start (pvar, entry);

  int i = 0;
  for (char* str = va_arg (pvar, char*);
       str != 0 && i < WINDSH_ARGS; str = va_arg (pvar, char*), ++i)
    {
      add_to_argv(argc, argv, MAX_ARGS, str);
    }

  // fill unused argv slots with 0 to get rid of leftovers
  // from previous invocations
  for (i = argc; i < MAX_ARGS; ++i)
    argv[i] = 0;

  // The hard-coded options are what ::sp () uses, except for the
  // larger stack size (instead of ::sp ()'s 20000).
  const int ret = ::taskSpawn (argv[0],    // task name
                               100,        // task priority
                               VX_FP_TASK, // task options
                               ACE_NEEDS_HUGE_THREAD_STACKSIZE, // stack size
                               entry,      // entry point
                               argc,       // first argument to main ()
                               (int) argv, // second argument to main ()
                               0, 0, 0, 0, 0, 0, 0, 0);
  va_end (pvar);

  // ::taskSpawn () returns the taskID on success: return 0 instead if
  // successful
  return ret > 0 ? 0 : ret;
}


// This global function can be used from the VxWorks shell to pass
// arguments to a C main () function.  The function will be run
// within the shells task.
//
// usage: -> spaef main, "arg1 arg2 \"arg3 with spaces\""
//
// All arguments must be within double quotes, even numbers.
// Unlike the spae function, this fuction executes the supplied
// routine in the foreground, rather than spawning it in a separate
// task.
int
spaef (FUNCPTR entry, ...)
{
  static const int WINDSH_ARGS = 10;
  static const int MAX_ARGS    = 128;
  static char* argv[MAX_ARGS]  = { "ace_main", 0 };
  va_list pvar;
  int argc = 1;

  // Peel off arguments to spa () and put into argv.  va_arg () isn't
  // necessarily supposed to return 0 when done, though since the
  // VxWorks shell uses a fixed number (10) of arguments, it might 0
  // the unused ones.
  va_start (pvar, entry);

  int i = 0;
  for (char* str = va_arg (pvar, char*);
       str != 0 && i < WINDSH_ARGS; str = va_arg (pvar, char*), ++i)
    {
      add_to_argv(argc, argv, MAX_ARGS, str);
    }

  // fill unused argv slots with 0 to get rid of leftovers
  // from previous invocations
  for (i = argc; i < MAX_ARGS; ++i)
    argv[i] = 0;

  int ret = entry (argc, argv);

  va_end (pvar);

  // Return the return value of the invoked ace_main routine.
  return ret;
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
ACE_OS::fork (const ACE_TCHAR *program_name)
{
  ACE_OS_TRACE ("ACE_OS::fork");
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

#if !defined (ACE_HAS_MINIMAL_ACE_OS)
  if (pid == 0)
    ACE_Base_Thread_Adapter::sync_log_msg (program_name);
#endif /* ! ACE_HAS_MINIMAL_ACE_OS */

  return pid;
# endif /* ACE_WIN32 */
}

void
ACE_Cleanup::cleanup (void *)
{
  delete this;
}


ACE_Cleanup::~ACE_Cleanup (void)
{
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

void
ACE_Thread_ID::to_string (char* thr_id)
{

  char format[128]; // Converted format string
  char *fp;         // Current format pointer
  fp = format;
  *fp++ = '%';   // Copy in the %

#if defined (ACE_WIN32)
  ACE_OS::strcpy (fp, "u");
  ACE_OS::sprintf (thr_id,
		   format,
		   ACE_static_cast(unsigned,
				   ACE_OS::thr_self ()));
#elif defined (ACE_AIX_VERS) && (ACE_AIX_VERS <= 402)
                  // AIX's pthread_t (ACE_hthread_t) is a pointer, and it's
                  // a little ugly to send that through a %u format.  So,
                  // get the kernel thread ID (tid_t) via thread_self() and
                  // display that instead.
                  // This isn't conditionalized on ACE_HAS_THREAD_SELF because
                  // 1. AIX 4.2 doesn't have that def anymore (it messes up
                  //    other things)
                  // 2. OSF/1 V3.2 has that def, and I'm not sure what affect
                  //   this would have on that.
                  // -Steve Huston, 19-Aug-97
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format, thread_self());
#elif defined (DIGITAL_UNIX)
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format,
#  if defined (ACE_HAS_THREADS)
                                   pthread_getselfseq_np ()
#  else
                                   ACE_Thread::self ()
#  endif /* ACE_HAS_THREADS */
                                          );
#else
                  ACE_hthread_t t_id;
                  ACE_OS::thr_self (t_id);

#  if defined (ACE_HAS_PTHREADS_DRAFT4) && defined (HPUX_10)
                  ACE_OS::strcpy (fp, "u");
                  // HP-UX 10.x DCE's thread ID is a pointer.  Grab the
                  // more meaningful, readable, thread ID.  This will match
                  // the one seen in the debugger as well.
                  ACE_OS::sprintf (thr_id, format,
                                   pthread_getunique_np(&t_id));
#  elif defined (ACE_MVS)
                  // MVS's pthread_t is a struct... yuck. So use the ACE 5.0
                  // code for it.
                  ACE_OS::strcpy (fp, "u");
                  ACE_OS::sprintf (thr_id, format, t_id);
#  else
                  // Yes, this is an ugly C-style cast, but the correct
                  // C++ cast is different depending on whether the t_id
                  // is an integral type or a pointer type. FreeBSD uses
                  // a pointer type, but doesn't have a _np function to
                  // get an integral type, like the OSes above.
                  ACE_OS::strcpy (fp, "lu");
                  ACE_OS::sprintf (thr_id, format, (unsigned long)t_id);
#  endif /* ACE_HAS_PTHREADS_DRAFT4 && HPUX_10 */

#endif /* ACE_WIN32 */
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

// All other platforms have this inlined in OS.i
#if defined (ACE_PSOS)
char *
ACE_OS::inet_ntoa (const struct in_addr addr)
{
  ACE_OS_TRACE ("ACE_OS::inet_ntoa");

  static char addrstr[INET_ADDRSTRLEN + 1] = { 0 };
  ACE_UINT32 ipaddr = ntohl (addr.s_addr);
  //printf("Socket address %X, IP address %X.\n",addr.s_addr,ipaddr);
  sprintf(addrstr, "%d.%d.%d.%d",
          ((ipaddr & 0xff000000) >> 24) & 0x000000ff,
          (ipaddr & 0x00ff0000) >> 16,
          (ipaddr & 0x0000ff00) >> 8,
          (ipaddr & 0x000000ff));
  return addrstr;
}
#endif /* defined (ACE_PSOS) */

int
ACE_OS::inet_aton (const char *host_name, struct in_addr *addr)
{
#if defined (ACE_LACKS_INET_ATON)
  ACE_UINT32 ip_addr = ACE_OS::inet_addr (host_name);

  if (ip_addr == INADDR_NONE
      // Broadcast addresses are weird...
      && ACE_OS::strcmp (host_name, "255.255.255.255") != 0)
    return 0;
  else if (addr == 0)
    return 0;
  else
    {
      addr->s_addr = ip_addr;  // Network byte ordered
      return 1;
    }
#elif defined (VXWORKS)
  // inet_aton() returns OK (0) on success and ERROR (-1) on failure.
  // Must reset errno first. Refer to WindRiver SPR# 34949, SPR# 36026
  ::errnoSet(0);
  int result = ERROR;
  ACE_OSCALL (::inet_aton ((char*)host_name, addr), int, ERROR, result);
  return (result == ERROR) ? 0 : 1;
#else
  // inet_aton() returns 0 upon failure, not -1 since -1 is a valid
  // address (255.255.255.255).
  ACE_OSCALL_RETURN (::inet_aton (host_name, addr), int, 0);
#endif  /* ACE_LACKS_INET_ATON */
}

struct tm *
ACE_OS::localtime_r (const time_t *t, struct tm *res)
{
  ACE_OS_TRACE ("ACE_OS::localtime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
# if defined (DIGITAL_UNIX)
  ACE_OSCALL_RETURN (::_Plocaltime_r (t, res), struct tm *, 0);
# elif defined (HPUX_10)
  return (::localtime_r (t, res) == 0 ? res : (struct tm *)0);
# else
  ACE_OSCALL_RETURN (::localtime_r (t, res), struct tm *, 0);
# endif /* DIGITAL_UNIX */
#elif !defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  ACE_OS_GUARD

  ACE_UNUSED_ARG (res);
  struct tm * res_ptr;
  ACE_OSCALL (::localtime (t), struct tm *, 0, res_ptr);
  if (res_ptr == 0)
    return 0;
  else
    {
      *res = *res_ptr;
      return res;
    }
#elif defined (ACE_HAS_WINCE)
  // This is really stupid, converting FILETIME to timeval back and
  // forth.  It assumes FILETIME and DWORDLONG are the same structure
  // internally.

  TIME_ZONE_INFORMATION pTz;

  const unsigned short int __mon_yday[2][13] =
  {
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
  };

  ULARGE_INTEGER _100ns;
  ::GetTimeZoneInformation (&pTz);

  _100ns.QuadPart = (DWORDLONG) *t * 10000 * 1000 + ACE_Time_Value::FILETIME_to_timval_skew;
  FILETIME file_time;
  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  FILETIME localtime;
  SYSTEMTIME systime;
  FileTimeToLocalFileTime (&file_time, &localtime);
  FileTimeToSystemTime (&localtime, &systime);

  res->tm_hour = systime.wHour;

  if(pTz.DaylightBias!=0)
    res->tm_isdst = 1;
  else
    res->tm_isdst = 1;

   int iLeap;
   iLeap = (res->tm_year % 4 == 0 && (res->tm_year% 100 != 0 || res->tm_year % 400 == 0));
   // based on leap select which group to use

   res->tm_mday = systime.wDay;
   res->tm_min = systime.wMinute;
   res->tm_mon = systime.wMonth;
   res->tm_sec = systime.wSecond;
   res->tm_wday = systime.wDayOfWeek;
   res->tm_yday = __mon_yday[iLeap][systime.wMonth] + systime.wDay;
   res->tm_year = systime.wYear;// this the correct year but bias the value to start at the 1900
   res->tm_year = res->tm_year - 1900;

   return res;
#else
  // @@ Same as ACE_OS::localtime (), you need to implement it
  //    yourself.
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (res);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ssize_t
ACE_OS::pread (ACE_HANDLE handle,
               void *buf,
               size_t nbytes,
               off_t offset)
{
# if defined (ACE_HAS_P_READ_WRITE)
#   if defined (ACE_WIN32)

  ACE_OS_GUARD

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              0,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  DWORD altered_position = ::SetFilePointer (handle,
                                             offset,
                                             0,
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
                            ACE_static_cast (DWORD, nbytes),
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
                            0);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        0,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_read;

#   else /* ACE_WIN32 */

  return ::pread (handle, buf, nbytes, offset);

#   endif /* ACE_WIN32 */

# else /* ACE_HAS_P_READ_WRITE */

  ACE_OS_GUARD

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_CUR);

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

  ACE_OS_GUARD

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              0,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  DWORD altered_position = ::SetFilePointer (handle,
                                             offset,
                                             0,
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
                             ACE_static_cast (DWORD, nbytes),
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
                             0);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        0,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_written;

#   else /* ACE_WIN32 */

  return ::pwrite (handle, buf, nbytes, offset);
#   endif /* ACE_WIN32 */
# else /* ACE_HAS_P_READ_WRITE */

  ACE_OS_GUARD

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_CUR);
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
  ACE_OS_TRACE ("ACE_OS::open");

#if defined (ACE_WIN32)
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

  ACE_MT (ACE_thread_mutex_t *ace_os_monitor_lock = 0;)

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      ACE_MT
        (
          ace_os_monitor_lock = (ACE_thread_mutex_t *)
            ACE_OS_Object_Manager::preallocated_object[
              ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK];
          ACE_OS::thread_mutex_lock (ace_os_monitor_lock);
        )
    }

  DWORD shared_mode = perms;

#if defined (ACE_HAS_WINCE)
  ACE_HANDLE h = ::CreateFileW (ACE_Ascii_To_Wide (filename).wchar_rep (), access,
                                shared_mode,
                                ACE_OS::default_win32_security_attributes (sa),
                                creation,
                                flags,
                                0);
#else /* ACE_HAS_WINCE */
  ACE_HANDLE h = ::CreateFileA (filename, access,
                                shared_mode,
                                ACE_OS::default_win32_security_attributes (sa),
                                creation,
                                flags,
                                0);
#endif /* ACE_HAS_WINCE */

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      if (h != ACE_INVALID_HANDLE)
        {
          ::SetFilePointer (h, 0, 0, FILE_END);
        }

      ACE_MT (ACE_OS::thread_mutex_unlock (ace_os_monitor_lock);)
    }

  if (h == ACE_INVALID_HANDLE)
    ACE_FAIL_RETURN (h);
  else
    return h;
#elif defined (ACE_PSOS)
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
      // We need to clean this up...not 100% correct!
      // To correct we should handle all the cases of TRUNC and CREAT
      if ((result == 0x200B) && (ACE_BIT_ENABLED (mode, O_CREAT)))
        {
          result = ::create_f(ACE_const_cast(char *, filename),1,0);
          if (result != 0)
            {
              errno = result;
              return ACE_static_cast (ACE_HANDLE, -1);
            }
          else  //File created...try to open it again
            {
              result = ::open_f (&handle, ACE_const_cast(char *, filename), 0);
              if (result != 0)
                {
                  errno = result;
                  return ACE_static_cast (ACE_HANDLE, -1);
                }

            }
        }
      else
        {
          errno = result;
          return ACE_static_cast (ACE_HANDLE, -1);
        }
    }
  return ACE_static_cast (ACE_HANDLE, handle);
# endif /* defined (ACE_PSOS_LACKS_PHILE) */
#elif defined (INTEGRITY)
  ACE_UNUSED_ARG (sa);
  if(!strcmp(filename,ACE_DEV_NULL)) {
      ACE_OSCALL_RETURN (::AllocateNullConsoleDescriptor(), ACE_HANDLE, -1);
  }
  else {
      ACE_OSCALL_RETURN (::open (filename, mode, perms), ACE_HANDLE, -1);
  }
#else
  ACE_UNUSED_ARG (sa);
  ACE_OSCALL_RETURN (::open (filename, mode, perms), ACE_HANDLE, -1);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_WCHAR)
ACE_HANDLE
ACE_OS::open (const wchar_t *filename,
              int mode,
              int perms,
              LPSECURITY_ATTRIBUTES sa)
{
#if defined (ACE_WIN32)
  // @@ (brunsch) Yuck, maybe there is a way to combine the code
  // here with the char version

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

  ACE_MT (ACE_thread_mutex_t *ace_os_monitor_lock = 0;)

  if (ACE_BIT_ENABLED (mode, _O_APPEND))
    {
      ACE_MT
        (
          ace_os_monitor_lock = (ACE_thread_mutex_t *)
            ACE_OS_Object_Manager::preallocated_object[
              ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK];
          ACE_OS::thread_mutex_lock (ace_os_monitor_lock);
        )
    }

  DWORD shared_mode = perms;

  ACE_HANDLE h = ::CreateFileW (filename,
                                access,
                                shared_mode,
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

      ACE_MT (ACE_OS::thread_mutex_unlock (ace_os_monitor_lock);)
    }

  if (h == ACE_INVALID_HANDLE)
    ACE_FAIL_RETURN (h);
  else
    return h;
#else /* ACE_WIN32 */
  // Just emulate with ascii version
  return ACE_OS::open (ACE_Wide_To_Ascii (filename).char_rep (),
                       mode,
                       perms,
                       sa);
#endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */

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
ACE_TCHAR *
ACE_OS::ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen)
{
  // buflen must be at least 26 wchar_t long.
  if (buflen < 26)              // Again, 26 is a magic number.
    {
      errno = ERANGE;
      return 0;
    }
  // This is really stupid, converting FILETIME to timeval back and
  // forth.  It assumes FILETIME and DWORDLONG are the same structure
  // internally.
  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (DWORDLONG) *clock * 10000 * 1000
                     + ACE_Time_Value::FILETIME_to_timval_skew;
  FILETIME file_time;
  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  FILETIME localtime;
  SYSTEMTIME systime;
  FileTimeToLocalFileTime (&file_time, &localtime);
  FileTimeToSystemTime (&localtime, &systime);
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

time_t
ACE_OS::mktime (struct tm *t)
{
  ACE_OS_TRACE ("ACE_OS::mktime");
#   if defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (-1);
#   elif defined (ACE_HAS_WINCE)
  SYSTEMTIME t_sys;
  FILETIME t_file;
  t_sys.wSecond = t->tm_sec;
  t_sys.wMinute = t->tm_min;
  t_sys.wHour = t->tm_hour;
  t_sys.wDay = t->tm_mday;
  t_sys.wMonth = t->tm_mon + 1;  // SYSTEMTIME is 1-indexed, tm is 0-indexed
  t_sys.wYear = t->tm_year + 1900; // SYSTEMTIME is real; tm is since 1900
  t_sys.wDayOfWeek = t->tm_wday;  // Ignored in below function call.
  if (SystemTimeToFileTime (&t_sys, &t_file) == 0)
    return -1;
  ACE_Time_Value tv (t_file);
  return tv.sec ();
#   else
#     if defined (ACE_HAS_THREADS)  &&  !defined (ACE_HAS_MT_SAFE_MKTIME)
  ACE_OS_GUARD
#     endif /* ACE_HAS_THREADS  &&  ! ACE_HAS_MT_SAFE_MKTIME */

  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#   endif /* ACE_PSOS && ! ACE_PSOS_HAS_TIME */
}

# if !defined (ACE_HAS_THREADS) || defined (ACE_LACKS_RWLOCK_T)
int
ACE_OS::rwlock_init (ACE_rwlock_t *rw,
                     int type,
                     const ACE_TCHAR *name,
                     void *arg)
{
  // ACE_OS_TRACE ("ACE_OS::rwlock_init");
#   if defined (ACE_HAS_THREADS) && defined (ACE_LACKS_RWLOCK_T)
  // NT, POSIX, and VxWorks don't support this natively.
  ACE_UNUSED_ARG (name);
  int result = -1;

  // Since we cannot use the user specified name for all three
  // objects, we will create three completely new names.
  ACE_TCHAR name1[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name2[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name3[ACE_UNIQUE_NAME_LEN];
  ACE_TCHAR name4[ACE_UNIQUE_NAME_LEN];

  ACE_OS::unique_name ((const void *) &rw->lock_,
                       name1,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_readers_,
                       name2,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_writers_,
                       name3,
                       ACE_UNIQUE_NAME_LEN);
  ACE_OS::unique_name ((const void *) &rw->waiting_important_writer_,
                       name4,
                       ACE_UNIQUE_NAME_LEN);

  ACE_condattr_t attributes;
  if (ACE_OS::condattr_init (attributes, type) == 0)
    {
      if (ACE_OS::mutex_init (&rw->lock_, type, name1,
                              (ACE_mutexattr_t *) arg) == 0
          && ACE_OS::cond_init (&rw->waiting_readers_,
                                attributes, name2, arg) == 0
          && ACE_OS::cond_init (&rw->waiting_writers_,
                                attributes, name3, arg) == 0
          && ACE_OS::cond_init (&rw->waiting_important_writer_,
                                attributes, name4, arg) == 0)
        {
          // Success!
          rw->ref_count_ = 0;
          rw->num_waiting_writers_ = 0;
          rw->num_waiting_readers_ = 0;
          rw->important_writer_ = 0;
          result = 0;
        }
      ACE_OS::condattr_destroy (attributes);
    }

  if (result == -1)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      ACE_OS::mutex_destroy (&rw->lock_);
      ACE_OS::cond_destroy (&rw->waiting_readers_);
      ACE_OS::cond_destroy (&rw->waiting_writers_);
      ACE_OS::cond_destroy (&rw->waiting_important_writer_);
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

#if defined (ACE_LACKS_COND_T) && ! defined (ACE_PSOS_DIAB_MIPS)
// NOTE: The ACE_OS::cond_* functions for some non-Unix platforms are
// defined here either because they're too big to be inlined, or
// to avoid use before definition if they were inline.

int
ACE_OS::cond_destroy (ACE_cond_t *cv)
{
  ACE_OS_TRACE ("ACE_OS::cond_destroy");
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

// @@ The following functions could be inlined if i could figure where
// to put it among the #ifdefs!
int
ACE_OS::condattr_init (ACE_condattr_t &attributes,
                       int type)
{
  attributes.type = type;
  return 0;
}

int
ACE_OS::condattr_destroy (ACE_condattr_t &)
{
  return 0;
}

int
ACE_OS::cond_init (ACE_cond_t *cv,
                   ACE_condattr_t &attributes,
                   const char *name, void *arg)
{
  return ACE_OS::cond_init (cv, attributes.type, name, arg);
}

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::cond_init (ACE_cond_t *cv,
                   ACE_condattr_t &attributes,
                   const wchar_t *name, void *arg)
{
  return ACE_OS::cond_init (cv, attributes.type, name, arg);
}
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::cond_init (ACE_cond_t *cv, short type, const char *name, void *arg)
{
  ACE_OS_TRACE ("ACE_OS::cond_init");
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

#if defined (ACE_HAS_WCHAR)
int
ACE_OS::cond_init (ACE_cond_t *cv, short type, const wchar_t *name, void *arg)
{
  ACE_OS_TRACE ("ACE_OS::cond_init");
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
#endif /* ACE_HAS_WCHAR */

int
ACE_OS::cond_signal (ACE_cond_t *cv)
{
  ACE_OS_TRACE ("ACE_OS::cond_signal");
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
  ACE_OS_TRACE ("ACE_OS::cond_broadcast");
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
  ACE_OS_TRACE ("ACE_OS::cond_wait");
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
  ACE_OS_TRACE ("ACE_OS::cond_timedwait");
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
  ACE_Errno_Guard error (errno, 0);
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
                                  timeout);
#       endif /* ACE_USES_WINCE_SEMA_SIMULATION */
#     elif defined (ACE_PSOS)
      // Inline the call to ACE_OS::sema_wait () because it takes an
      // ACE_Time_Value argument.  Avoid the cost of that conversion . . .
      u_long ticks = (KC_TICKS2SEC * msec_timeout) / ACE_ONE_SECOND_IN_MSECS;
      //Tick set to 0 tells pSOS to wait forever is SM_WAIT is set.
      if(ticks == 0)
        result = ::sm_p (cv->sema_.sema_, SM_NOWAIT, ticks); //no timeout
      else
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
        case ERR_TIMEOUT:  // Timeout occured with SM_WAIT
        case ERR_NOMSG:    // Didn't acquire semaphore w/ SM_NOWAIT (ticks=0)
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
        case S_objLib_OBJ_UNAVAILABLE:
          if (msec_timeout == 0)
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
  // Note that this *must* be an "if" statement rather than an "else
  // if" statement since the caller may have timed out and hence the
  // result would have been -1 above.
  if (last_waiter)
    // Release the signaler/broadcaster if we're the last waiter.
#     if defined (ACE_WIN32)
    ACE_OS::event_signal (&cv->waiters_done_);
#     else
    ACE_OS::sema_post (&cv->waiters_done_);
#     endif /* ACE_WIN32 */

  // We must always regain the <external_mutex>, even when errors
  // occur because that's the guarantee that we give to our callers.
  ACE_OS::mutex_lock (external_mutex);

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
  ACE_OS_TRACE ("ACE_OS::cond_timedwait");
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
#     if defined (ACE_USES_WINCE_SEMA_SIMULATION)
  // Can't use Win32 API on simulated semaphores.
  result = ACE_OS::sema_wait (&cv->sema_,
                              timeout);

  if (result == -1 && errno == ETIME)
    result = WAIT_TIMEOUT;
#     else
  result = ::WaitForSingleObject (cv->sema_, msec_timeout);
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
  ACE_OS_TRACE ("ACE_OS::cond_wait");
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

  if (result != WAIT_OBJECT_0 && errno == ETIME)
    result = WAIT_TIMEOUT;

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
  ACE_OS_TRACE ("ACE_OS::exit");

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
# endif /* ACE_PSOS && ! ACE_PSOS_DIAB_MIPS */

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

# define ACE_OS_PREALLOCATE_OBJECT(TYPE, ID)\
    {\
      TYPE *obj_p = 0;\
      ACE_NEW_RETURN (obj_p, TYPE, -1);\
      preallocated_object[ID] = (void *) obj_p;\
    }
# define ACE_OS_DELETE_PREALLOCATED_OBJECT(TYPE, ID)\
    delete (TYPE *) preallocated_object[ID];\
    preallocated_object[ID] = 0;

ACE_Object_Manager_Base::ACE_Object_Manager_Base (void)
  : object_manager_state_ (OBJ_MAN_UNINITIALIZED)
  , dynamically_allocated_ (0)
  , next_ (0)
{
}

ACE_Object_Manager_Base::~ACE_Object_Manager_Base (void)
{
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
  // Clear the flag so that fini () doesn't delete again.
  dynamically_allocated_ = 0;
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */
}

int
ACE_Object_Manager_Base::starting_up_i ()
{
  return object_manager_state_ < OBJ_MAN_INITIALIZED;
}

int
ACE_Object_Manager_Base::shutting_down_i ()
{
  return object_manager_state_ > OBJ_MAN_INITIALIZED;
}

extern "C"
void
ACE_OS_Object_Manager_Internal_Exit_Hook (void)
{
  if (ACE_OS_Object_Manager::instance_)
    ACE_OS_Object_Manager::instance ()->fini ();
}

ACE_OS_Object_Manager *ACE_OS_Object_Manager::instance_ = 0;

void *ACE_OS_Object_Manager::preallocated_object[
  ACE_OS_Object_Manager::ACE_OS_PREALLOCATED_OBJECTS] = { 0 };

ACE_OS_Object_Manager::ACE_OS_Object_Manager (void)
  // default_mask_ isn't initialized, because it's defined by <init>.
  : thread_hook_ (0)
  , exit_info_ ()
#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
  , seh_except_selector_ (ACE_SEH_Default_Exception_Selector)
  , seh_except_handler_ (ACE_SEH_Default_Exception_Handler)
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
{
  // If instance_ was not 0, then another ACE_OS_Object_Manager has
  // already been instantiated (it is likely to be one initialized by
  // way of library/DLL loading).  Let this one go through
  // construction in case there really is a good reason for it (like,
  // ACE is a static/archive library, and this one is the non-static
  // instance (with ACE_HAS_NONSTATIC_OBJECT_MANAGER, or the user has
  // a good reason for creating a separate one) but the original one
  // will be the one retrieved from calls to
  // ACE_Object_Manager::instance().

  // Be sure that no further instances are created via instance ().
  if (instance_ == 0)
    instance_ = this;

  init ();
}

ACE_OS_Object_Manager::~ACE_OS_Object_Manager (void)
{
  dynamically_allocated_ = 0;   // Don't delete this again in fini()
  fini ();
}

sigset_t *
ACE_OS_Object_Manager::default_mask (void)
{
  return ACE_OS_Object_Manager::instance ()->default_mask_;
}

ACE_Thread_Hook *
ACE_OS_Object_Manager::thread_hook (void)
{
  return ACE_OS_Object_Manager::instance ()->thread_hook_;
}

#if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
ACE_SEH_EXCEPT_HANDLER
ACE_OS_Object_Manager::seh_except_selector (void)
{
  return ACE_OS_Object_Manager::instance ()->seh_except_selector_;
}

ACE_SEH_EXCEPT_HANDLER
ACE_OS_Object_Manager::seh_except_selector (ACE_SEH_EXCEPT_HANDLER n)
{
  ACE_OS_Object_Manager *instance =
    ACE_OS_Object_Manager::instance ();

  ACE_SEH_EXCEPT_HANDLER retv = instance->seh_except_selector_;
  instance->seh_except_selector_ = n;
  return retv;
}

ACE_SEH_EXCEPT_HANDLER
ACE_OS_Object_Manager::seh_except_handler (void)
{
  return ACE_OS_Object_Manager::instance ()->seh_except_handler_;
}

ACE_SEH_EXCEPT_HANDLER
ACE_OS_Object_Manager::seh_except_handler (ACE_SEH_EXCEPT_HANDLER n)
{
  ACE_OS_Object_Manager *instance =
    ACE_OS_Object_Manager::instance ();

  ACE_SEH_EXCEPT_HANDLER retv = instance->seh_except_handler_;
  instance->seh_except_handler_ = n;
  return retv;
}
#endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

ACE_Thread_Hook *
ACE_OS_Object_Manager::thread_hook (ACE_Thread_Hook *new_thread_hook)
{
  ACE_OS_Object_Manager *os_om = ACE_OS_Object_Manager::instance ();
  ACE_Thread_Hook *old_hook = os_om->thread_hook_;
  os_om->thread_hook_ = new_thread_hook;
  return old_hook;
}

ACE_OS_Object_Manager *
ACE_OS_Object_Manager::instance (void)
{
  // This function should be called during construction of static
  // instances, or before any other threads have been created in the
  // process.  So, it's not thread safe.

  if (instance_ == 0)
    {
      ACE_OS_Object_Manager *instance_pointer;

      ACE_NEW_RETURN (instance_pointer,
                      ACE_OS_Object_Manager,
                      0);
      // I (coryan) removed it, using asserts in the OS layer
      // brings down the Log msg stuff
      // ACE_ASSERT (instance_pointer == instance_);

      instance_pointer->dynamically_allocated_ = 1;

    }

  return instance_;
}

int
ACE_OS_Object_Manager::init (void)
{
  if (starting_up_i ())
    {
      // First, indicate that this ACE_OS_Object_Manager instance is being
      // initialized.
      object_manager_state_ = OBJ_MAN_INITIALIZING;

      if (this == instance_)
        {
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
          ACE_CE_Errno::init ();
#   endif /* ACE_HAS_WINCE_BROKEN_ERRNO */
          ACE_OS_PREALLOCATE_OBJECT (ACE_thread_mutex_t, ACE_OS_MONITOR_LOCK)
          if (ACE_OS::thread_mutex_init
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_OS_MONITOR_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_OS_MONITOR_LOCK"));
          ACE_OS_PREALLOCATE_OBJECT (ACE_recursive_thread_mutex_t,
                                     ACE_TSS_CLEANUP_LOCK)
          if (ACE_OS::recursive_mutex_init
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_CLEANUP_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_TSS_CLEANUP_LOCK"));
          ACE_OS_PREALLOCATE_OBJECT (ACE_thread_mutex_t,
                                     ACE_LOG_MSG_INSTANCE_LOCK)
          if (ACE_OS::thread_mutex_init
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_LOG_MSG_INSTANCE_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_LOG_MSG_INSTANCE_LOCK"));
#   if defined (ACE_HAS_TSS_EMULATION)
          ACE_OS_PREALLOCATE_OBJECT (ACE_recursive_thread_mutex_t,
                                     ACE_TSS_KEY_LOCK)
          if (ACE_OS::recursive_mutex_init
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_KEY_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_TSS_KEY_LOCK"));
#     if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
          ACE_OS_PREALLOCATE_OBJECT (ACE_recursive_thread_mutex_t,
                                     ACE_TSS_BASE_LOCK)
          if (ACE_OS::recursive_mutex_init
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_BASE_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_TSS_BASE_LOCK"));
#     endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
#   endif /* ACE_HAS_TSS_EMULATION */
# endif /* ACE_MT_SAFE */

          // Open Winsock (no-op on other platforms).
          ACE_OS::socket_init (ACE_WSOCK_VERSION);

          // Register the exit hook, for use by ACE_OS::exit ().
          ACE_OS::set_exit_hook (&ACE_OS_Object_Manager_Internal_Exit_Hook);
        }

      ACE_NEW_RETURN (default_mask_, sigset_t, -1);
      ACE_OS::sigfillset (default_mask_);

      // Finally, indicate that the ACE_OS_Object_Manager instance has
      // been initialized.
      object_manager_state_ = OBJ_MAN_INITIALIZED;

# if defined (ACE_WIN32)
      ACE_OS::win32_versioninfo_.dwOSVersionInfoSize =
        sizeof (OSVERSIONINFO);
      ::GetVersionEx (&ACE_OS::win32_versioninfo_);
# endif /* ACE_WIN32 */
      return 0;
    } else {
      // Had already initialized.
      return 1;
    }
}

// Clean up an ACE_OS_Object_Manager.  There can be instances of this object
// other than The Instance.  This can happen if a user creates one for some
// reason.  All objects clean up their per-object information and managed
// objects, but only The Instance cleans up the static preallocated objects.
int
ACE_OS_Object_Manager::fini (void)
{
  if (instance_ == 0  ||  shutting_down_i ())
    // Too late.  Or, maybe too early.  Either fini () has already
    // been called, or init () was never called.
    return object_manager_state_ == OBJ_MAN_SHUT_DOWN  ?  1  :  -1;

  // No mutex here.  Only the main thread should destroy the singleton
  // ACE_OS_Object_Manager instance.

  // Indicate that the ACE_OS_Object_Manager instance is being shut
  // down.  This object manager should be the last one to be shut
  // down.
  object_manager_state_ = OBJ_MAN_SHUTTING_DOWN;

  // If another Object_Manager has registered for termination, do it.
  if (next_)
    {
      next_->fini ();
      next_ = 0;  // Protect against recursive calls.
    }

  // Call all registered cleanup hooks, in reverse order of
  // registration.
  exit_info_.call_hooks ();

  // Only clean up preallocated objects when the singleton Instance is being
  // destroyed.
  if (this == instance_)
    {
      // Close down Winsock (no-op on other platforms).
      ACE_OS::socket_fini ();

#if ! defined (ACE_HAS_STATIC_PREALLOCATION)
      // Cleanup the dynamically preallocated objects.
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   if !defined(ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK)
      if (ACE_OS::thread_mutex_destroy
          // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
          (ACE_reinterpret_cast (ACE_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_OS_MONITOR_LOCK])) != 0)
        ACE_OS_Object_Manager::print_error_message (
          __LINE__, ACE_LIB_TEXT ("ACE_OS_MONITOR_LOCK"));
#   endif /* ! ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK */
      ACE_OS_DELETE_PREALLOCATED_OBJECT (ACE_thread_mutex_t,
                                         ACE_OS_MONITOR_LOCK)
#   if !defined(ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK)
      if (ACE_OS::recursive_mutex_destroy
          // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
          (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_CLEANUP_LOCK])) != 0)
        ACE_OS_Object_Manager::print_error_message (
          __LINE__, ACE_LIB_TEXT ("ACE_TSS_CLEANUP_LOCK"));
#   endif /* ! ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK */
      ACE_OS_DELETE_PREALLOCATED_OBJECT (ACE_recursive_thread_mutex_t,
                                         ACE_TSS_CLEANUP_LOCK)
#   if !defined(ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK)
      if (ACE_OS::thread_mutex_destroy
          // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
          (ACE_reinterpret_cast (ACE_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object [ACE_LOG_MSG_INSTANCE_LOCK])) != 0)
        ACE_OS_Object_Manager::print_error_message (
          __LINE__, ACE_LIB_TEXT ("ACE_LOG_MSG_INSTANCE_LOCK "));
#   endif /* ! ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK */
      ACE_OS_DELETE_PREALLOCATED_OBJECT (ACE_thread_mutex_t,
                                         ACE_LOG_MSG_INSTANCE_LOCK)
#   if defined (ACE_HAS_TSS_EMULATION)
#     if !defined(ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK)
        if (ACE_OS::recursive_mutex_destroy
            // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
            (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_KEY_LOCK])) != 0)
          ACE_OS_Object_Manager::print_error_message (
            __LINE__, ACE_LIB_TEXT ("ACE_TSS_KEY_LOCK"));
#     endif /* ! ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK */
      ACE_OS_DELETE_PREALLOCATED_OBJECT (ACE_recursive_thread_mutex_t,
                                         ACE_TSS_KEY_LOCK)
#     if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
#       if !defined(ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK)
          if (ACE_OS::recursive_mutex_destroy
              // This line must not be broken to avoid tickling a bug with SunC++'s preprocessor.
              (ACE_reinterpret_cast (ACE_recursive_thread_mutex_t *, ACE_OS_Object_Manager::preallocated_object[ACE_TSS_BASE_LOCK])) != 0)
            ACE_OS_Object_Manager::print_error_message (
              __LINE__, ACE_LIB_TEXT ("ACE_TSS_BASE_LOCK"));
#       endif /* ! ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK */
      ACE_OS_DELETE_PREALLOCATED_OBJECT (ACE_recursive_thread_mutex_t,
                                         ACE_TSS_BASE_LOCK)
#     endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
#   endif /* ACE_HAS_TSS_EMULATION */
#   if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
          ACE_CE_Errno::fini ();
#   endif /* ACE_HAS_WINCE_BROKEN_ERRNO */
# endif /* ACE_MT_SAFE */
#endif /* ! ACE_HAS_STATIC_PREALLOCATION */
    }

  delete default_mask_;
  default_mask_ = 0;

  // Indicate that this ACE_OS_Object_Manager instance has been shut down.
  object_manager_state_ = OBJ_MAN_SHUT_DOWN;

  if (dynamically_allocated_)
    {
      delete this;
    }

  if (this == instance_)
    instance_ = 0;

  return 0;
}

int ace_exit_hook_marker = 0;

int
ACE_OS_Object_Manager::at_exit (ACE_EXIT_HOOK func)
{
  return exit_info_.at_exit_i (&ace_exit_hook_marker,
                               ACE_reinterpret_cast (ACE_CLEANUP_FUNC, func),
                               0);
}

void
ACE_OS_Object_Manager::print_error_message (u_int line_number,
                                            const ACE_TCHAR *message)
{
  // To avoid duplication of these const strings in OS.o.
#if !defined (ACE_HAS_WINCE)
  fprintf (stderr, "ace/OS.cpp, line %u: %s ",
           line_number,
           message);
  perror ("failed");
#else
  // @@ Need to use the following information.
  ACE_UNUSED_ARG (line_number);
  ACE_UNUSED_ARG (message);

  ACE_TCHAR *lpMsgBuf = 0;
  ::FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM,
                   0,
                   ::GetLastError (),
                   MAKELANGID (LANG_NEUTRAL,
                               SUBLANG_DEFAULT),
                   // Default language
                   (ACE_TCHAR *) &lpMsgBuf,
                   0,
                   0);
  ::MessageBox (NULL,
                lpMsgBuf,
                ACE_LIB_TEXT ("ACE_OS error"),
                MB_OK);
#endif
}

int
ACE_OS_Object_Manager::starting_up (void)
{
  return ACE_OS_Object_Manager::instance_
    ? instance_->starting_up_i ()
    : 1;
}

int
ACE_OS_Object_Manager::shutting_down (void)
{
  return ACE_OS_Object_Manager::instance_
    ? instance_->shutting_down_i ()
    : 1;
}

#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
/**
 * @class ACE_OS_Object_Manager_Manager
 *
 * @brief Ensure that the <ACE_OS_Object_Manager> gets initialized at
 * program startup, and destroyed at program termination.
 *
 * Without ACE_HAS_NONSTATIC_OBJECT_MANAGER, a static instance of this
 * class is created.  Therefore, it gets created before main ()
 * is called.  And it gets destroyed after main () returns.
 */
class ACE_OS_Object_Manager_Manager
{
public:
  /// Constructor.
  ACE_OS_Object_Manager_Manager (void);

  /// Destructor.
  ~ACE_OS_Object_Manager_Manager (void);

private:
  /// Save the main thread ID, so that destruction can be suppressed.
  ACE_thread_t saved_main_thread_id_;
};

ACE_OS_Object_Manager_Manager::ACE_OS_Object_Manager_Manager (void)
  : saved_main_thread_id_ (ACE_OS::thr_self ())
{
  // Ensure that the Object_Manager gets initialized before any
  // application threads have been spawned.  Because this will be called
  // during construction of static objects, that should always be the
  // case.
  (void) ACE_OS_Object_Manager::instance ();
}

ACE_OS_Object_Manager_Manager::~ACE_OS_Object_Manager_Manager (void)
{
  if (ACE_OS::thr_equal (ACE_OS::thr_self (),
                         saved_main_thread_id_))
    {
      delete ACE_OS_Object_Manager::instance_;
      ACE_OS_Object_Manager::instance_ = 0;
    }
  // else if this destructor is not called by the main thread, then do
  // not delete the ACE_OS_Object_Manager.  That causes problems, on
  // WIN32 at least.
}

static ACE_OS_Object_Manager_Manager ACE_OS_Object_Manager_Manager_instance;
#endif /* ! ACE_HAS_NONSTATIC_OBJECT_MANAGER */

// You may be asking yourself, why are we doing this?  Well, in winbase.h,
// MS didn't follow their normal Api_FunctionA and Api_FunctionW style,
// so we have to #undef their define to get access to the unicode version.
// And because we don't want to #undef this for the users code, we keep
// this method in the .cpp file.
#if defined (ACE_WIN32) && defined (UNICODE) && !defined (ACE_USES_TCHAR)
#undef GetEnvironmentStrings
#endif /* ACE_WIN32 && UNICODE !ACE_USES_TCHAR */

ACE_TCHAR *
ACE_OS::getenvstrings (void)
{
#if defined (ACE_LACKS_ENV)
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
# if defined (ACE_USES_WCHAR)
  return ::GetEnvironmentStringsW ();
# else /* ACE_USES_WCHAR */
  return ::GetEnvironmentStrings ();
# endif /* ACE_USES_WCHAR */
#else /* ACE_WIN32 */
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_STRPTIME)
char *
ACE_OS::strptime (char *buf, const char *format, struct tm *tm)
{
#if !defined (ACE_HAS_WINCE)
#if defined (ACE_LACKS_NATIVE_STRPTIME)
  int bi = 0;
  int fi = 0;
  int percent = 0;

  if (!buf || !format)
    return 0;

  while (format[fi] != '\0')
    {
      if (percent)
        {
          percent = 0;
          switch (format[fi])
            {
            case '%':                        // an escaped %
              if (buf[bi] == '%')
                {
                  fi++; bi++;
                }
              else
                return buf + bi;
              break;

              /* not supported yet: weekday via locale long/short names
                 case 'a':                        / * weekday via locale * /
                 / * FALL THROUGH * /
                 case 'A':                        / * long/short names * /
                 break;
                 */

              /* not supported yet:
                 case 'b':                        / * month via locale * /
                 / * FALL THROUGH * /
                 case 'B':                        / * long/short names * /
                 / * FALL THROUGH * /
                 case 'h':
                 break;
                 */

              /* not supported yet:
                 case 'c':                        / * %x %X * /
                 break;
                 */

              /* not supported yet:
                 case 'C':                        / * date & time -  * /
                 / * locale long format * /
                 break;
                 */

            case 'd':                        /* day of month (1-31) */
              /* FALL THROUGH */
            case 'e':
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              break;

            case 'D':                        /* %m/%d/%y */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              fi--;
              tm->tm_mon--;

              if (buf[bi] != '/')
                return buf + bi;

              bi++;

              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              fi--;
              if (buf[bi] != '/')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;
              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'H':                        /* hour (0-23) */
              /* FALL THROUGH */
            case 'k':
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;
              break;

              /* not supported yet:
                 case 'I':                        / * hour (0-12) * /
                 / * FALL THROUGH * /
                 case 'l':
                 break;
                 */

            case 'j':                        /* day of year (0-366) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
                return buf + bi;

              tm->tm_yday--;
              break;

            case 'm':                        /* an escaped % */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              tm->tm_mon--;
              break;

            case 'M':                        /* minute (0-59) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              break;

              /* not supported yet:
                 case 'p':                        / * am or pm for locale * /
                 break;
                 */

              /* not supported yet:
                 case 'r':                        / * %I:%M:%S %p * /
                 break;
                 */

            case 'R':                        /* %H:%M */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              break;

            case 'S':                        /* seconds (0-61) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;
              break;

            case 'T':                        /* %H:%M:%S */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;

              break;

            case 'w':                        /* day of week (0=Sun-6) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
                return buf + bi;

              break;

              /* not supported yet: date, based on locale
                 case 'x':                        / * date, based on locale * /
                 break;
                 */

              /* not supported yet:
                 case 'X':                        / * time, based on locale * /
                 break;
                 */

            case 'y':                        /* the year - 1900 (0-99) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;

              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'Y':                        /* the full year (1999) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
                return buf + bi;

              tm->tm_year -= 1900;
              break;

            default:                        /* unrecognised */
              return buf + bi;
            } /* switch (format[fi]) */

        }
      else
        { /* if (percent) */
          if (format[fi] == '%')
            {
              percent = 1;
              fi++;
            }
          else
            {
              if (format[fi] == buf[bi])
                {
                  fi++;
                  bi++;
                }
              else
                return buf + bi;
            }
        } /* if (percent) */
    } /* while (format[fi] */

  return buf + bi;
#else  /* ! ACE_LACKS_NATIVE_STRPTIME */
  return ::strptime (buf,
                     format,
                     tm);
#endif /* ! ACE_LACKS_NATIVE_STRPTIME */
#else /* ! ACE_HAS_WINCE */
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (tm);

  ACE_NOTSUP_RETURN (0);
#endif /* ! ACE_HAS_WINCE */
}

# if defined (ACE_LACKS_NATIVE_STRPTIME)
int
ACE_OS::strptime_getnum (char *buf,
                         int *num,
                         int *bi,
                         int *fi,
                         int min,
                         int max)
{
  int i = 0, tmp = 0;

  while (isdigit (buf[i]))
    {
      tmp = (tmp * 10) + (buf[i] - '0');
      if (max && (tmp > max))
        return 0;
      i++;
    }

  if (tmp < min)
    return 0;
  else if (i)
    {
      *num = tmp;
      (*fi)++;
      *bi += i;
      return 1;
    }
  else
    return 0;
}
# endif /* ACE_LACKS_NATIVE_STRPTIME */
#endif /* ACE_HAS_STRPTIME */

#if !defined (ACE_HAS_WINCE)
ACE_HANDLE
ACE_OS::accept (ACE_HANDLE handle,
                struct sockaddr *addr,
                int *addrlen,
                const ACE_Accept_QoS_Params &qos_params)
{
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  ACE_SOCKCALL_RETURN (::WSAAccept ((ACE_SOCKET) handle,
                                    addr,
                                    (ACE_SOCKET_LEN *) addrlen,
                                    (LPCONDITIONPROC) qos_params.qos_condition_callback (),
                                    qos_params.callback_data ()),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
# else
  ACE_UNUSED_ARG (qos_params);
  return ACE_OS::accept (handle,
                         addr,
                         addrlen);
# endif /* ACE_HAS_WINSOCK2 */
}

ACE_HANDLE
ACE_OS::join_leaf (ACE_HANDLE socket,
                   const sockaddr *name,
                   int namelen,
                   const ACE_QoS_Params &qos_params)
{
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)

  QOS qos;
  // Construct the WinSock2 QOS structure.

  qos.SendingFlowspec = *(qos_params.socket_qos ()->sending_flowspec ());
  qos.ReceivingFlowspec = *(qos_params.socket_qos ()->receiving_flowspec ());
  qos.ProviderSpecific = (WSABUF) qos_params.socket_qos ()->provider_specific ();

  ACE_SOCKCALL_RETURN (::WSAJoinLeaf ((ACE_SOCKET) socket,
                                      name,
                                      namelen,
                                      (WSABUF *) qos_params.caller_data (),
                                      (WSABUF *) qos_params.callee_data (),
                                      &qos,
                                      (QOS *) qos_params.group_socket_qos (),
                                      qos_params.flags ()),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);

# else
  ACE_UNUSED_ARG (socket);
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (namelen);
  ACE_UNUSED_ARG (qos_params);
  ACE_NOTSUP_RETURN (ACE_INVALID_HANDLE);
# endif /* ACE_HAS_WINSOCK2 */
}

int
ACE_OS::ioctl (ACE_HANDLE socket,
               u_long io_control_code,
               void *in_buffer_p,
               u_long in_buffer,
               void *out_buffer_p,
               u_long out_buffer,
               u_long *bytes_returned,
               ACE_OVERLAPPED *overlapped,
               ACE_OVERLAPPED_COMPLETION_FUNC func)
{
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  ACE_SOCKCALL_RETURN (::WSAIoctl ((ACE_SOCKET) socket,
                                   io_control_code,
                                   in_buffer_p,
                                   in_buffer,
                                   out_buffer_p,
                                   out_buffer,
                                   bytes_returned,
                                   (WSAOVERLAPPED *) overlapped,
                                   func),
                       int,
                       SOCKET_ERROR);
# else
  ACE_UNUSED_ARG (socket);
  ACE_UNUSED_ARG (io_control_code);
  ACE_UNUSED_ARG (in_buffer_p);
  ACE_UNUSED_ARG (in_buffer);
  ACE_UNUSED_ARG (out_buffer_p);
  ACE_UNUSED_ARG (out_buffer);
  ACE_UNUSED_ARG (bytes_returned);
  ACE_UNUSED_ARG (overlapped);
  ACE_UNUSED_ARG (func);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_WINSOCK2 */
}


int
ACE_OS::ioctl (ACE_HANDLE socket,
               u_long io_control_code,
               ACE_QoS &ace_qos,
               u_long *bytes_returned,
               void *buffer_p,
               u_long buffer,
               ACE_OVERLAPPED *overlapped,
               ACE_OVERLAPPED_COMPLETION_FUNC func)
{
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)

  QOS qos;
  u_long qos_len = sizeof (QOS);

  if (io_control_code == SIO_SET_QOS)
    {
      qos.SendingFlowspec = *(ace_qos.sending_flowspec ());
      qos.ReceivingFlowspec = *(ace_qos.receiving_flowspec ());
      qos.ProviderSpecific = (WSABUF) ace_qos.provider_specific ();

      qos_len += ace_qos.provider_specific ().iov_len;

      ACE_SOCKCALL_RETURN (::WSAIoctl ((ACE_SOCKET) socket,
                                       io_control_code,
                                       &qos,
                                       qos_len,
                                       buffer_p,
                                       buffer,
                                       bytes_returned,
                                       (WSAOVERLAPPED *) overlapped,
                                       func),
                           int,
                           SOCKET_ERROR);
    }
  else
    {
      u_long dwBufferLen = 0;

      // Query for the buffer size.
      int result = ::WSAIoctl ((ACE_SOCKET) socket,
                                io_control_code,
                                NULL,
                                0,
                                &dwBufferLen,
                                sizeof (dwBufferLen),
                                bytes_returned,
                                NULL,
                                NULL);


      if (result == SOCKET_ERROR)
          {
                u_long dwErr = ::WSAGetLastError ();

                if (dwErr == WSAEWOULDBLOCK)
                {
                        errno = dwErr;
                        return -1;
                }
                else
                        if (dwErr != WSAENOBUFS)
                        {
                                errno = dwErr;
                                return -1;
                        }
          }

    char *qos_buf;
        ACE_NEW_RETURN (qos_buf,
                                        char [dwBufferLen],
                                        -1);

        QOS *qos = ACE_reinterpret_cast (QOS*,
                                                                         qos_buf);

        result = ::WSAIoctl ((ACE_SOCKET) socket,
                                   io_control_code,
                       NULL,
                       0,
                       qos,
                       dwBufferLen,
                       bytes_returned,
                       NULL,
                       NULL);

    if (result == SOCKET_ERROR)
                return result;

    ACE_Flow_Spec sending_flowspec (qos->SendingFlowspec.TokenRate,
                                    qos->SendingFlowspec.TokenBucketSize,
                                    qos->SendingFlowspec.PeakBandwidth,
                                    qos->SendingFlowspec.Latency,
                                    qos->SendingFlowspec.DelayVariation,
#  if defined(ACE_HAS_WINSOCK2_GQOS)
                                    qos->SendingFlowspec.ServiceType,
                                    qos->SendingFlowspec.MaxSduSize,
                                    qos->SendingFlowspec.MinimumPolicedSize,
#  else /* ACE_HAS_WINSOCK2_GQOS */
                                    0,
                                    0,
                                    0,
#  endif /* ACE_HAS_WINSOCK2_GQOS */
                                    0,
                                    0);

    ACE_Flow_Spec receiving_flowspec (qos->ReceivingFlowspec.TokenRate,
                                      qos->ReceivingFlowspec.TokenBucketSize,
                                      qos->ReceivingFlowspec.PeakBandwidth,
                                      qos->ReceivingFlowspec.Latency,
                                      qos->ReceivingFlowspec.DelayVariation,
#  if defined(ACE_HAS_WINSOCK2_GQOS)
                                      qos->ReceivingFlowspec.ServiceType,
                                      qos->ReceivingFlowspec.MaxSduSize,
                                      qos->ReceivingFlowspec.MinimumPolicedSize,
#  else /* ACE_HAS_WINSOCK2_GQOS */
                                      0,
                                      0,
                                      0,
#  endif /* ACE_HAS_WINSOCK2_GQOS */
                                      0,
                                      0);

       ace_qos.sending_flowspec (&sending_flowspec);
       ace_qos.receiving_flowspec (&receiving_flowspec);
       ace_qos.provider_specific (*((struct iovec *) (&qos->ProviderSpecific)));


      return result;
    }

# else
  ACE_UNUSED_ARG (socket);
  ACE_UNUSED_ARG (io_control_code);
  ACE_UNUSED_ARG (ace_qos);
  ACE_UNUSED_ARG (bytes_returned);
  ACE_UNUSED_ARG (buffer_p);
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (overlapped);
  ACE_UNUSED_ARG (func);
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_WINSOCK2 */
}

int
ACE_OS::connect (ACE_HANDLE handle,
                 const sockaddr *addr,
                 int addrlen,
                 const ACE_QoS_Params &qos_params)
{
  ACE_OS_TRACE ("ACE_OS::connect");
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  ACE_SOCKCALL_RETURN (::WSAConnect ((ACE_SOCKET) handle,
                                     (const sockaddr *) addr,
                                     (ACE_SOCKET_LEN) addrlen,
                                     (WSABUF *) qos_params.caller_data (),
                                     (WSABUF *) qos_params.callee_data (),
                                     (QOS *) qos_params.socket_qos (),
                                     (QOS *) qos_params.group_socket_qos ()),
                       int, -1);
# else
  ACE_UNUSED_ARG (qos_params);
  return ACE_OS::connect (handle,
                          (sockaddr *) addr,
                          addrlen);
# endif /* ACE_HAS_WINSOCK2 */
}
#endif  // ACE_HAS_WINCE

long
ACE_OS::num_processors (void)
{
  ACE_OS_TRACE ("ACE_OS::num_processors");

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return sys_info.dwNumberOfProcessors;
#elif defined (linux) || defined (sun)
  return ::sysconf (_SC_NPROCESSORS_CONF);
#else
  ACE_NOTSUP_RETURN (-1);
#endif
}

long
ACE_OS::num_processors_online (void)
{
  ACE_OS_TRACE ("ACE_OS::num_processors_online");

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return sys_info.dwNumberOfProcessors;
#elif defined (linux) || defined (sun)
  return ::sysconf (_SC_NPROCESSORS_ONLN);
#elif defined (__hpux)
  struct pst_dynamic psd;
  if (::pstat_getdynamic (&psd, sizeof (psd), (size_t) 1, 0) != -1)
    return psd.psd_proc_cnt;
  else
    return -1;
#else
  ACE_NOTSUP_RETURN (-1);
#endif
}

// Include if_arp so that getmacaddr can use the
// arp structure.
#if defined (sun)
# include /**/ <net/if_arp.h>
#endif

int
ACE_OS::getmacaddress (struct macaddr_node_t *node)
{
  ACE_OS_TRACE ("ACE_OS::getmacaddress");

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  /** Define a structure for use with the netbios routine */
  struct ADAPTERSTAT
  {
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
  };

  NCB         ncb;
  LANA_ENUM   lenum;
  unsigned char result;

  ACE_OS::memset (&ncb, 0, sizeof(ncb));
  ncb.ncb_command = NCBENUM;
  ncb.ncb_buffer  = ACE_reinterpret_cast (unsigned char*,&lenum);
  ncb.ncb_length  = sizeof(lenum);

  result = Netbios (&ncb);

  for(int i = 0; i < lenum.length; i++)
    {
      ACE_OS::memset (&ncb, 0, sizeof(ncb));
      ncb.ncb_command  = NCBRESET;
      ncb.ncb_lana_num = lenum.lana [i];

      /** Reset the netbios */
      result = Netbios (&ncb);

      if (ncb.ncb_retcode != NRC_GOODRET)
	{
	  return -1;
	}

      ADAPTERSTAT adapter;
      ACE_OS::memset (&ncb, 0, sizeof (ncb));
# if defined (__BORLANDC__) || defined (__MINGW32__)
      ACE_OS::strcpy (ACE_reinterpret_cast (char*, ncb.ncb_callname), "*");
# else
      ACE_OS::strcpy (ACE_static_cast (char*, ncb.ncb_callname), "*");
# endif /* __BORLANDC__ || __MINGW32__ */
      ncb.ncb_command     = NCBASTAT;
      ncb.ncb_lana_num    = lenum.lana[i];
      ncb.ncb_buffer      = ACE_reinterpret_cast (unsigned char*, &adapter);
      ncb.ncb_length      = sizeof (adapter);

      result = Netbios (&ncb);

      if (result == 0)
	{
	  ACE_OS::memcpy (node->node,
			  adapter.adapt.adapter_address,
			  6);
	  return 0;
	}
    }
  return 0;
#elif defined (sun)

  int result = 0;

  /** obtain the local host name */
  char hostname [MAXHOSTNAMELEN];
  ACE_OS::hostname (hostname, sizeof (hostname));

  /** Get the hostent to use with ioctl */
  struct hostent *phost =
    ACE_OS::gethostbyname (hostname);

  if (phost == 0)
    {
      return -1;
    }

  ACE_HANDLE handle =
    ACE_OS::socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (handle == ACE_INVALID_HANDLE)
    {
      return -1;
    }

  char **paddrs = phost->h_addr_list;

  struct arpreq ar;

  struct sockaddr_in *psa =
    (struct sockaddr_in *)&(ar.arp_pa);

  ACE_OS::memset (&ar,
		  0,
		  sizeof (struct arpreq));

  psa->sin_family = AF_INET;

  ACE_OS::memcpy (&(psa->sin_addr),
		  *paddrs,
		  sizeof (struct in_addr));

  if (ACE_OS::ioctl (handle,
                     SIOCGARP,
                     &ar) == -1)
    {
      return -1;
    }

  ACE_OS::close (handle);

  ACE_OS::memcpy (node->node,
		  ar.arp_ha.sa_data,
		  6);

  return 0;

#elif defined (linux)

  struct ifreq ifr;

  ACE_HANDLE handle =
    ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);

  if (handle == ACE_INVALID_HANDLE)
    {
      return -1;
    }

  ACE_OS::strcpy (ifr.ifr_name, "eth0");

  if (ACE_OS::ioctl (handle/*s*/, SIOCGIFHWADDR, &ifr) < 0)
    {
      ACE_OS::close (handle);
      return -1;
    }

  struct sockaddr* sa =
    (struct sockaddr *) &ifr.ifr_addr;

  ACE_OS::memcpy (node->node,
		  sa->sa_data,
                  6);

  return 0;

#else
  ACE_UNUSED_ARG (node);
  ACE_NOTSUP_RETURN (-1);
#endif
}
