/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// 6.4 platforms using one of these compilers:
// 1) The GNU g++ compiler that is shipped with VxWorks 6.4

#ifndef ACE_CONFIG_H

# include "ace/config-vxworks6.3.h"

#if defined __RTP__
  // Needed include to get all VxWorks CPU types
  #include "types/vxCpu.h"
  #if defined (_VX_CPU) && (_VX_CPU == _VX_PENTIUM || _VX_CPU == _VX_PENTIUM2 || _VX_CPU == _VX_PENTIUM3 || _VX_CPU == _VX_PENTIUM4)
    // If running an Intel Pentium the
    // ACE_OS::gethrtime () can use the RDTSC instruction.
    # define ACE_HAS_PENTIUM
  #endif
#else
  // Needed include to get all VxWorks CPU types
  #include "types/vxCpu.h"
  #if defined (CPU) && (CPU == PENTIUM || CPU == PENTIUM2 || CPU == PENTIUM3 || CPU == PENTIUM4)
   // If running an Intel Pentium the
   // ACE_OS::gethrtime () can use the RDTSC instruction.
    # define ACE_HAS_PENTIUM
  #endif
  #define ACE_LACKS_INTPTR_T
#endif

#endif /* ACE_CONFIG_H */
