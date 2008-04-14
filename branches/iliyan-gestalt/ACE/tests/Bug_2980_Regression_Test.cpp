// $Id$

#include <iostream>
#include <assert.h>
#include <stdio.h>

#include "ace/config-lite.h"

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

#if  !(defined (WIN32) || defined (ACE_VXWORKS))
#  define CAN_RUN_TEST

#  include <dlfcn.h>


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
static voidfunction   capi_init = 0;
static voidfunction   capi_fini = 0;
static voidfunction   capi_dosomething = 0;
#endif /* defined (CAN_RUN_TEST) */


extern "C"
void* loadDll(void*)
{
  PRINTF ("loadDll - entered\n");

#if defined (CAN_RUN_TEST)
  dllHandle = dlopen("./libBug_2980_Regression.so", RTLD_NOW);
  if (dllHandle == 0)
  {
    PRINTF ("unable to load library: %s\n", dlerror());
    assert(dllHandle != 0);
  }

  capi_init = (voidfunction) dlsym(dllHandle, "capi_init");
  if (capi_init == 0)
  {
    PRINTF ("unable to resolve symbol capi_init: %s\n", dlerror());
    assert(capi_init != 0);
  }

  capi_fini = (voidfunction) dlsym(dllHandle, "capi_fini");
  if (capi_init == 0)
  {
    PRINTF ("unable to resolve symbol capi_fini: %s\n", dlerror());
    assert(capi_init != 0);
  }

  capi_dosomething = (voidfunction) dlsym(dllHandle, "capi_dosomething");
  if (capi_dosomething == 0)
  {
    PRINTF ("unable to resolve symbol capi_dosomething: %s\n", dlerror());
    assert(capi_dosomething != 0);
  }
  capi_init();
#endif /* defined (CAN_RUN_TEST) */

  PRINTF ("loadDll - leaving\n");

  return 0;
}

extern "C"
void* unloadDll(void*)
{
  PRINTF ("unloadDll - entered\n");

#if defined (CAN_RUN_TEST)
  capi_fini();
  dlclose(dllHandle);
#endif /* defined (CAN_RUN_TEST) */

  PRINTF ("unloadDll - leaving\n");
  return 0;
}




void * loadunloadDll(void *pp)
{
  loadDll(pp);

#if defined (CAN_RUN_TEST)
  assert(capi_dosomething != 0);
  capi_dosomething();
#endif /* defined (CAN_RUN_TEST) */

  unloadDll(pp);

  return 0;
}

int main(int, char **)
{
  PRINTF ("main called\n");

#if !defined (CAN_RUN_TEST)

  PRINTF ("Terminating because this test has not been designed "
          "to run on WIN32 or VXWORKS.\n");

#else

  PRINTF ("main - calling loadDll\n");

#  if defined (CAN_USE_THREADS)
  int result = 0;
  pthread_t tid1;
  result = pthread_create(&tid1, 0, &loadDll, 0);
  if (result != 0)
  {
    PRINTF ("pthread_create() failed: %d\n", result);
    return result;
  }

  pthread_join(tid1, 0);
  PRINTF ("loadDll thread finished and re-joined\n");

#  else

  loadDll(0);
  PRINTF ("loadDll finished\n");

#  endif /* defined (CAN_USE_THREADS) */

  PRINTF ("main - calling unloadDll\n");

#  if defined (CAN_USE_THREADS)
  pthread_t tid2;
  result = pthread_create(&tid2, 0, &unloadDll, 0);
  if (result != 0)
  {
    PRINTF ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid2, 0);
  PRINTF ("unloadDll thread finished and re-joined\n");

#  else

  unloadDll(0);
  PRINTF ("unloadDll finished\n");

#  endif /* defined (CAN_USE_THREADS) */

#endif /* defined (CAN_RUN_TEST) */

  PRINTF ("main finished\n");
  return 0;

}
