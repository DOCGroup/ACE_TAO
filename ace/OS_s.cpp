// $Id$

#define ACE_BUILD_DLL

#include "ace/OS.h"

#if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION)
  ACE_TSS_Cleanup *ACE_TSS_Cleanup::instance_ = 0;

# if defined (ACE_HAS_TSS_EMULATION)
    u_int ACE_TSS_Emulation::total_keys_ = 0;

    ACE_TSS_Emulation::ACE_TSS_DESTRUCTOR
    ACE_TSS_Emulation::tss_destructor_
      [ACE_TSS_Emulation::ACE_TSS_THREAD_KEYS_MAX] = { 0 };

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
      int ACE_TSS_Emulation::key_created_ = 0;

      ACE_OS_thread_key_t ACE_TSS_Emulation::native_tss_key_;
#   endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */

# endif /* ACE_HAS_TSS_EMULATION */
#endif /* WIN32 || ACE_HAS_TSS_EMULATION */

ACE_OS_Object_Manager *ACE_OS_Object_Manager::instance_ = 0;

void *ACE_OS_Object_Manager::preallocated_object[
  ACE_OS_Object_Manager::ACE_OS_PREALLOCATED_OBJECTS] = { 0 };

ACE_EXIT_HOOK ACE_OS::exit_hook_ = 0;
