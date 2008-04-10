// $Id$

#include <iostream>
#include <assert.h>
#include <stdio.h>

// This is a non-ACE driver program which loads an ACE-based DLL.
// The usual ACE-related defines will not apply and we must use
// platform-specific ones. Luckily, it is only Windows where this
// test has not been made to work yet ...


#if  !(defined (WIN32) || defined (ACE_VXWORKS))

#  if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#    define USE_THREAD
#  else
#    undef USE_THREAD
#  endif

#  define CAN_RUN_TEST
#endif

#if defined (CAN_RUN_TEST)

#include <dlfcn.h>

#if defined USE_THREAD
#  include <pthread.h>
#endif


static void * dllHandle;
typedef int (* voidfunction)(void);

static voidfunction   capi_init = 0;
static voidfunction   capi_fini = 0;
static voidfunction   capi_dosomething = 0;

extern "C"
void* loadDll(void*)
{
  printf ("loadDll - entered\n");
  dllHandle = dlopen("./libBug_2980_Regression.so", RTLD_NOW);
  if (dllHandle == 0)
  {
    printf ("unable to load library: %s\n", dlerror());
    assert(dllHandle != 0);
  }

  capi_init = (voidfunction) dlsym(dllHandle, "capi_init");
  if (capi_init == 0)
  {
    printf ("unable to resolve symbol capi_init: %s\n", dlerror());
    assert(capi_init != 0);
  }

  capi_fini = (voidfunction) dlsym(dllHandle, "capi_fini");
  if (capi_init == 0)
  {
    printf ("unable to resolve symbol capi_fini: %s\n", dlerror());
    assert(capi_init != 0);
  }

  capi_dosomething = (voidfunction) dlsym(dllHandle, "capi_dosomething");
  if (capi_dosomething == 0)
  {
    printf ("unable to resolve symbol capi_dosomething: %s\n", dlerror());
    assert(capi_dosomething != 0);
  }

  capi_init();

  printf ("loadDll - leaving\n");

  return 0;
}

void* unloadDll(void*)
{
  printf ("unloadDll - entered\n");

  capi_fini();

  dlclose(dllHandle);

  printf ("unloadDll - leaving\n");

  return 0;
}

void * loadunloadDll(void *pp)
{
  loadDll(pp);

  assert(capi_dosomething != 0);

  capi_dosomething();

  unloadDll(pp);

  return 0;
}
#endif /* defined (CAN_RUN_TEST) */

int main(int, char **)
{

#if !defined (CAN_RUN_TEST)

#if defined (__BORLANDC__)
  std::printf ("Terminating because this test has not been designed "
               "to run on platforms where WIN32 has been defined.\n");
#else
  printf ("Terminating because this test has not been designed "
          "to run on platforms where WIN32 has been defined.\n");
#endif

#else
  printf ("main - entered\n");

#  ifdef USE_THREAD
  int result = 0;
  pthread_t tid1;
  result = pthread_create(&tid1, 0, &loadDll, 0);
  if (result != 0)
  {
    printf ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid1, 0);
  printf ("loadDll thread finished\n");
#  else
  loadDll(0);
  printf ("loadDll finished\n");
#  endif

#  ifdef USE_THREAD
  pthread_t tid2;
  result = pthread_create(&tid2, 0, &unloadDll, 0);
  if (result != 0)
  {
    printf ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid2, 0);
  printf ("unloadDll thread finished\n");
#  else
  unloadDll(0);
  printf ("unloadDll finished\n");
#  endif

  printf ("main - leaving\n");

#endif /* defined (CAN_RUN_TEST) */

  return 0;
}
