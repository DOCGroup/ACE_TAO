#include <iostream>
#include <dlfcn.h>
#include <pthread.h>
#include <assert.h>


static void * dllHandle;
typedef void (* voidfunction)(void);

static voidfunction   capi_init = 0;
static voidfunction   capi_fini = 0;
static voidfunction   capi_dosomething = 0;


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

int main(int argc, char ** argv)
{
  int result = 0;

  printf ("main - entered\n");

#ifdef USE_THREAD
  pthread_t tid1;
  result = pthread_create(&tid1, NULL, &loadDll, 0);
  if (result != 0)
  {
    printf ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid1, 0);
  printf ("loadDll thread finished\n");
#else
  loadDll(0);
  printf ("loadDll finished"\n);
#endif

#ifdef USE_THREAD
  pthread_t tid2;
  result = pthread_create(&tid2, NULL, &unloadDll, 0);
  if (result != 0)
  {
    printf ("pthread_create() failed: %d\n", result);
    return 1;
  }
  pthread_join(tid2, 0);
  printf ("unloadDll thread finished\n");
#else
  unloadDll(0);
  printf ("unloadDll finished\n");
#endif

  printf ("main - leaving\n");

  return 0;
}
