#include "ace/Naming_Context.h"
// $Id$

#include "ace/Name_Options.h"
#include "nametest.h"

void 
Nametest::listopt (void)
{
  cout << "serverport is " 
       << ACE_Name_Options::instance ()->nameserver_port() 
       << endl;
  cout << "serverhost is " 
       << ACE_Name_Options::instance ()->nameserver_host() 
       << endl;
  cout << "process_name is " 
       << ACE_Name_Options::instance ()->process_name() 
       << endl;
  cout << "namespace_dir is " 
       << ACE_Name_Options::instance ()->namespace_dir() 
       << endl;
}

int 
Nametest::init (int argc, char *argv[])
{
  ACE_Server_Record *sr;
  ACE_Service_Config::svc_rep ()->find ("Name_Server_Proxy", &sr);
  ACE_Service_Type *st = sr->type ();
  ACE_Server_Object *so = st->object ();
dynamic_cast<ACE_Name_Server_Proxy *> (so);

  ACE_Name_Server_Proxy *ns_proxy = ACE_Service_Config::name_server_proxy ();
  
  ns_proxy->bind (...);

  this->listopt ();

  ACE_Naming_Context ns_context;

  if (ns_context.open (ACE_Naming_Context::NET_LOCAL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ns_context.open"), -1);

  const char *mykey = argv[0];
  char *myvalue = argv[1];
  char *ns_value = 0;
  char *ns_type = 0;

  if (ns_context.bind (mykey, myvalue, "ottotype") == -1)
    cout << "bind failed" << endl;
  else
    cout << "bind succeeded" << endl;


  if (ns_context.resolve (mykey, ns_value, ns_type) == -1)
    cout << "resolve of " << mykey << " failed" << endl;
  else
    cout << "resolve of " << mykey << " succeeded, value = " 
	 << ns_value << ", type = " << ns_type << endl;

  delete [] ns_value;
  delete [] ns_type;
  ns_value = 0;
  ns_type = 0;

  if (ns_context.rebind (mykey, myvalue, "newottotype") == -1)
    cout << "rebind failed" << endl;
  else
    cout << "rebind succeeded" << endl;

  if (ns_context.resolve (mykey, ns_value, ns_type) == -1)
    cout << "resolve of " << mykey << " failed" << endl;
  else
    cout << "resolve of " << mykey << " succeeded, value = " 
	 << ns_value << ", type = " << ns_type << endl;

  delete [] ns_value;
  delete [] ns_type;
  ns_value = 0;
  ns_type = 0;

  if (ns_context.unbind (mykey) == -1)
    cout << "unbind failed" << endl;
  else
    cout << "unbind succeeded" << endl;

  return 0;
}

int 
Nametest::fini (void)
{
  cout << "Nametest::fini called" << endl;
  return 0;
}

int 
Nametest::info (char **, unsigned) const
{
  cout << "Nametest::info called" << endl;
  return 0;
}

extern "C" ACE_Service_Object *_alloc(void);

// Factory function that is called automatically when the ACE
// framework dynamically links this shared object file.

ACE_Service_Object *_alloc (void) 
{
  return new Nametest;
}
