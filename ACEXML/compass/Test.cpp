// $Id$

#include "ace/Thread_Manager.h"
#include "ace/DLL.h"
#include "ace/ARGV.h"
#include "ace/SString.h"
#include "tao/ORB_Core.h"

static ACE_ARGV myargv;

void*
create_dll (void* name)
{
  typedef int (*dll_func) (int, char**);
  const ACE_TCHAR* dllname = ACE_reinterpret_cast (const ACE_TCHAR*, name);
  if (dllname == 0)
    {
      ACE_ERROR ((LM_ERROR, "Trying to create a dll with null name"));
      return (void*)-1;
    }
  ACE_DLL dll;
  if (dll.open (dllname) == -1)
    {
     ACE_ERROR ((LM_ERROR, "Failed to open Service: %m\n"));
     return (void*)-1;
    }

  void *foo;
  const ACE_TCHAR* create = "create";
  ACE_CString entrypoint (create);
  entrypoint += dllname;
  foo = dll.symbol (entrypoint.c_str());

  // Cast the void* to long first.
  long tmp = ACE_reinterpret_cast (long, foo);
  dll_func factory = ACE_reinterpret_cast (dll_func, tmp);
  if (factory == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to resolve factory: %s: %m\n"),
                  dll.error ()));
      return (void*)-1;
    }
  ACE_ARGV serviceargv;
  if (ACE_OS::strcmp (dllname, "Service") == 0)
    {
      serviceargv.add (myargv.argv());
      serviceargv.add ("-ECDispatching");
      serviceargv.add ("mt");
      return (void*)factory (serviceargv.argc(), serviceargv.argv());
    }
  return (void*)factory (myargv.argc(), myargv.argv());
}

int
main (int argc, char* argv[])
{
  myargv.add (argv);

  const ACE_TCHAR* services[] = { "Service", "Consumer", "Supplier" };

  ACE_Thread_Manager* thr_mgr = ACE_Thread_Manager::instance();
  for (size_t i = 0; i < 3; ++i)
    {
      if (thr_mgr->spawn (create_dll,(void*)services[i]) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Unable to spawn dll %s\n"),
                             services[i]), -1);
        }
      ACE_OS::sleep (3);
    }
  thr_mgr->wait();
  return 0;
}
