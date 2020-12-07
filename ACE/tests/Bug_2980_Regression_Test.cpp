//FUZZ: disable check_for_lack_ACE_OS

#include "ace/config-lite.h"

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

// This is a non-ACE driver program which loads an ACE-based DLL.  The
// usual ACE-related defines will not apply and we must use
// platform-specific ones.
//
// This test has not been made to work on Windows and vxWorks, yet ...

#if defined (ACE_HAS_THREADS)
#  define CAN_USE_THREADS
#else
#  undef CAN_USE_THREADS
#endif

#if !(defined (WIN32) || defined (ACE_VXWORKS) || defined (ACE_HAS_LYNXOS_178))\
    && !defined ACE_FACE_SAFETY_EXTENDED
#  define CAN_RUN_TEST

#  include <dlfcn.h>

namespace {
#  if defined (ACE_DLL_SUFFIX)
  const char *DllTestName = "./libBug_2980_Regression" ACE_DLL_SUFFIX;
#else
  const char *DllTestName = "./libBug_2980_Regression.so";
#endif /* ACE_DLL_SUFFIX */
}

#  if defined CAN_USE_THREADS
#    include <pthread.h>
#  endif

#else
#  undef CAN_RUN_TEST
#endif

#if defined (__BORLANDC__)
#  define PRINTF std::printf
#else
#  define PRINTF printf
#endif


typedef int (* voidfunction)(void);


#if defined (CAN_RUN_TEST)
static void * dllHandle;
static voidfunction   capi_init = nullptr;
static voidfunction   capi_fini = nullptr;
static voidfunction   capi_dosomething = nullptr;
#endif /* defined (CAN_RUN_TEST) */


extern "C"
void* loadDll(void*)
{
#if defined (CAN_RUN_TEST)

  PRINTF ("loadDll - entered\n");
  const char *subdir_env = getenv ("ACE_EXE_SUB_DIR");
  if (subdir_env)
    {
      char *dllFile =
        (char *) malloc (2 + strlen (subdir_env) + strlen (DllTestName));
      strcpy (dllFile, subdir_env);
      strcat (dllFile, "/");
      strcat (dllFile, DllTestName);
      dllHandle = dlopen (dllFile, RTLD_NOW);
      free (dllFile);
    }
  else
    {
      dllHandle = dlopen (DllTestName, RTLD_NOW);
    }

  if (dllHandle == nullptr)
  {
    PRINTF ("unable to load library: %s\n", dlerror());
    assert(dllHandle != nullptr);
  }

  void* temp = dlsym (dllHandle, "capi_init");
  memcpy (&capi_init, &temp, sizeof (temp));
  if (capi_init == nullptr)
  {
    PRINTF ("unable to resolve symbol capi_init: %s\n", dlerror());
    assert(capi_init != nullptr);
  }

  temp = dlsym (dllHandle, "capi_fini");
  memcpy (&capi_fini, &temp, sizeof (temp));
  if (capi_fini == nullptr)
  {
    PRINTF ("unable to resolve symbol capi_fini: %s\n", dlerror());
    assert(capi_fini != nullptr);
  }

  temp = dlsym (dllHandle, "capi_dosomething");
  memcpy (&capi_dosomething, &temp, sizeof (temp));
  if (capi_dosomething == nullptr)
  {
    PRINTF ("unable to resolve symbol capi_dosomething: %s\n", dlerror());
    assert(capi_dosomething != nullptr);
  }
  capi_init();
  PRINTF ("loadDll - leaving\n");
#endif /* defined (CAN_RUN_TEST) */
  return nullptr;
}

extern "C"
void* unloadDll(void*)
{
#if defined (CAN_RUN_TEST)
  PRINTF ("unloadDll - entered\n");
  capi_fini();
  dlclose(dllHandle);
  PRINTF ("unloadDll - leaving\n");
#endif /* defined (CAN_RUN_TEST) */
  return nullptr;
}

void * loadunloadDll(void *pp)
{
  loadDll(pp);

#if defined (CAN_RUN_TEST)
  assert(capi_dosomething != nullptr);
  capi_dosomething();
#endif /* defined (CAN_RUN_TEST) */

  unloadDll(pp);

  return nullptr;
}
// FUZZ: disable check_for_improper_main_declaration
int main (int, char *[])
{
#if !defined (CAN_RUN_TEST)
# ifndef ACE_FACE_SAFETY_EXTENDED
  PRINTF ("Terminating because this test has not been designed "
          "to run on WIN32 or VXWORKS.\n");
# endif
#else
  PRINTF ("main called\n");
  PRINTF ("main - calling loadDll\n");

#  if defined (CAN_USE_THREADS)
  int result = 0;
  pthread_t tid1;
  result = pthread_create(&tid1, nullptr, &loadDll, nullptr);
  if (result != 0)
  {
    PRINTF ("pthread_create() failed: %d\n", result);
    return result;
  }

  pthread_join(tid1, nullptr);
  PRINTF ("loadDll thread finished and re-joined\n");

#  else

  loadDll(0);
  PRINTF ("loadDll finished\n");

#  endif /* defined (CAN_USE_THREADS) */

  PRINTF ("main - calling unloadDll\n");

#  if defined (CAN_USE_THREADS)
  pthread_t tid2;
  result = pthread_create(&tid2, nullptr, &unloadDll, nullptr);
  if (result != 0)
  {
    PRINTF ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid2, nullptr);
  PRINTF ("unloadDll thread finished and re-joined\n");

#  else

  unloadDll(0);
  PRINTF ("unloadDll finished\n");

#  endif /* defined (CAN_USE_THREADS) */

  PRINTF ("main finished\n");
#endif /* defined (CAN_RUN_TEST) */

  return 0;

}
  //FUZZ: enable check_for_lack_ACE_OS
