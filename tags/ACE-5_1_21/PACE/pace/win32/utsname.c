/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utsname.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/sys/utsname.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/utsname.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_SP_UOF)
int
uname (pace_utsname * name)
{
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);

#if 0  /* Emulation: Not bug free! :-/ */

  size_t maxnamelen = sizeof name->nodename;
  pace_strcpy (name->sysname, "Win32");

  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx (&vinfo);

  SYSTEM_INFO sinfo;
  GetSystemInfo(&sinfo);
  pace_strcpy (name->sysname, "Win32");
  
  if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
      /* Get information from the two structures */
      pace_sprintf (name->release, "Windows NT %d.%d",
                    vinfo.dwMajorVersion,
                    vinfo.dwMinorVersion);
      pace_sprintf (name->version,
                    "Build %d %s",
                    vinfo.dwBuildNumber,
                    vinfo.szCSDVersion);
      
      /* We have to make sure that the size of (processor + subtype) is
         not greater than the size of name->machine.  So we give half
         the space to the processor and half the space to subtype.  The
         -1 is necessary for because of the space between processor and
         subtype in the machine name.
      */
      const int bufsize = ((sizeof (name->machine) / sizeof (ACE_TCHAR)) / 2) - 1;
      char processor[bufsize] = "Unknown";
      char subtype[bufsize] = "Unknown";
      
      WORD arch = sinfo.wProcessorArchitecture;
      
      switch (arch)
        {

        case PROCESSOR_ARCHITECTURE_INTEL: {
          
          pace_strcpy (processor, "Intel");
          if (sinfo.wProcessorLevel == 3)
            {
              pace_strcpy (subtype, "80386");
            }
          else if (sinfo.wProcessorLevel == 4)
            {
              pace_strcpy (subtype, "80486");
            }
          else if (sinfo.wProcessorLevel == 5)
            {
              pace_strcpy (subtype, "Pentium");
            }
          else if (sinfo.wProcessorLevel == 6)
            {
              pace_strcpy (subtype, "Pentium Pro");
            }
          else if (sinfo.wProcessorLevel == 7)  /* guessing here */
            {
              pace_strcpy (subtype, "Pentium II");
            }
          break;
        }
        
        case PROCESSOR_ARCHITECTURE_MIPS: {
          pace_strcpy (processor, "MIPS");
          pace_strcpy (subtype, "R4000");
          break;
        }
        
        case PROCESSOR_ARCHITECTURE_ALPHA: {
          pace_strcpy (processor, "Alpha");
          pace_sprintf (subtype, "%d", sinfo.wProcessorLevel);
          break;
        }
        
        case PROCESSOR_ARCHITECTURE_PPC: {
          pace_strcpy (processor, "PPC");
          if (sinfo.wProcessorLevel == 1)
            {
              pace_strcpy (subtype, "601");
            }
          else if (sinfo.wProcessorLevel == 3)
            {
              pace_strcpy (subtype, "603");
            }
          else if (sinfo.wProcessorLevel == 4)
            {
              pace_strcpy (subtype, "604");
            }
          else if (sinfo.wProcessorLevel == 6)
            {
              pace_strcpy (subtype, "603+");
            }
          else if (sinfo.wProcessorLevel == 9)
            {
              pace_strcpy (subtype, "804+");
            }
          else if (sinfo.wProcessorLevel == 20)
            {
              pace_strcpy (subtype, "620");
            }
          break;
        }
        
        case PROCESSOR_ARCHITECTURE_UNKNOWN: {
          
        default:
          {
            /* @@ We could provide WinCE specific info here.  But let's
               defer that to some later point.
            */
            pace_strcpy (processor, "Unknown");
            break;
          }
        }
        }
      pace_sprintf(name->machine, "%s %s", processor, subtype);
    }
  else if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
      // Get Windows 95 Information
      pace_strcpy (name->release, "Windows 95");
      pace_sprintf (name->version, "%d", LOWORD (vinfo.dwBuildNumber));
      if (sinfo.dwProcessorType == PROCESSOR_INTEL_386)
        {
          pace_strcpy (name->machine, "Intel 80386");
        }
      else if (sinfo.dwProcessorType == PROCESSOR_INTEL_486)
        {
          pace_strcpy (name->machine, "Intel 80486");
        }
      else if (sinfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM)
        {
          pace_strcpy (name->machine, "Intel Pentium");
        }
    }
  else
    {
      /* We don't know what this is! */
      pace_strcpy (name->release, "???");
      pace_strcpy (name->version, "???");
      pace_strcpy (name->machine, "???");
    }
  
  /* ick */
  return ACE_OS::hostname (name->nodename, maxnamelen);

#endif /* emulation */

}
#endif /* PACE_HAS_POSIX_SP_UOF */
