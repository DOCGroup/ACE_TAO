// -*- C++ -*-
// $Id$

#include "ace/OS_NS_sys_utsname.h"

ACE_RCSID(ace, OS_NS_sys_utsname, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_sys_utsname.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

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
