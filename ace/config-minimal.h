/* -*- C++ -*- */
// $Id$

// This configuration file is designed to build only the minimal
// ACE_OS adaptation layer.

#ifndef ACE_CONFIG_MINIMAL_H
#define ACE_CONFIG_MINIMAL_H

#define ACE_HAS_MINIMAL_ACE_OS

// Only instantiate the ACE_OS_Object_Manager.
#define ACE_MAIN_OBJECT_MANAGER \
  ACE_OS_Object_Manager ace_os_object_manager;

// So that ACE_Thread_Exit isn't required . . .
#define ACE_USE_THREAD_MANAGER_ADAPTER

#if defined (ACE_ASSERT)
# undef ACE_ASSERT
#endif /* ACE_ASSERT */
#define ACE_ASSERT(x)

#if defined (ACE_DEBUG)
# undef ACE_DEBUG
#endif /* ACE_DEBUG */
#define ACE_DEBUG(x)

#if defined (ACE_ERROR)
# undef ACE_ERROR
#endif /* ACE_ERROR */
#define ACE_ERROR(x)

#endif /* ACE_CONFIG_MINIMAL_H */
