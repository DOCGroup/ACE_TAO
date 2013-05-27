// $Id$

#include "simple_test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"

#include "tao/ORB_Core.h"
#include "tao/IORTable/Async_IORTable.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"

CORBA::String_var testIOR;

ACE_CString ior_table_name = "IORTable";
bool use_async_locator = false;

//*************************************************************************
class AsyncLocatorTask
  : public virtual IORTable::AsyncLocator,
    public virtual CORBA::LocalObject
{
public:
  AsyncLocatorTask (CORBA::ORB_ptr orb);

  void async_locate (IORTable::Locate_ResponseHandler rh, const char *);
  char * locate (const char *);

private:
  CORBA::ORB_var orb_;
};

void
AsyncLocatorTask::async_locate (IORTable::Locate_ResponseHandler rh, const char * id)
{
  CORBA::Object_var fwdObj = orb_->string_to_object(testIOR.in ());
  CORBA::String_var fwdString = orb_->object_to_string(fwdObj.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("in async_locate!  Forwarding client %C to: %C\n"),
              id, fwdString.in ()));

  rh->forward_ior (fwdString._retn(), false);
}

char *
AsyncLocatorTask::locate (const char * id)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("In async locator::locate, id = %C, throwing\n"),
              id));
  throw IORTable::NotFound ();
  return 0;
}


AsyncLocatorTask::AsyncLocatorTask (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

//*************************************************************************
class SyncLocatorTask
  : public virtual IORTable::Locator,
    public virtual CORBA::LocalObject
{
public:
  SyncLocatorTask (CORBA::ORB_ptr orb);

  char * locate (const char *);

private:
  CORBA::ORB_var orb_;
};

char *
SyncLocatorTask::locate (const char * id)
{
  CORBA::Object_var fwdObj = orb_->string_to_object(testIOR.in ());
  CORBA::String_var fwdString = orb_->object_to_string(fwdObj.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("in synch locate: Forwarding client %C to: %C\n"),
              id,
              fwdString.in ()));

  return fwdString._retn();
}

SyncLocatorTask::SyncLocatorTask (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  for (int c = 1; c < argc; c++)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-aa")) == 0)
        {
          ior_table_name = "AsyncIORTable";
          use_async_locator = true;
        }
      else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-as")) == 0)
        {
          ior_table_name = "AsyncIORTable";
          use_async_locator = false;
        }
      else
        {
          return -1;
        }
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      Simple_Test_i *test_impl = 0;
      ACE_NEW_RETURN (test_impl,
                      Simple_Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      simple::SimpleTest_T_var theServer =
        simple::SimpleTest_T::_narrow (object.in ());

      testIOR = orb->object_to_string (theServer.in ());

      poa_manager->activate ();

      // Create task.
      IORTable::Locator_ptr iorloc_ptr;
      if (use_async_locator)
        {
          ACE_NEW_RETURN (iorloc_ptr,
                          AsyncLocatorTask (orb.in ()),
                          1);
        }
      else
        {
          ACE_NEW_RETURN (iorloc_ptr,
                          SyncLocatorTask (orb.in ()),
                          1);
        }
      IORTable::Locator_var iorloc = iorloc_ptr;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Server - using ior table object: %C\n"),
                  ior_table_name.c_str()));
      // Resolve the IORTable and set the locator
      CORBA::Object_var iorTableObj =
        orb->resolve_initial_references(ior_table_name.c_str());

      IORTable::Table_var local_ior_table =
        IORTable::Table::_narrow (iorTableObj.in ());

      local_ior_table->set_locator (iorloc.in());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Server - event loop finished\n")));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Server - Exception caught:"));
      return 1;
    }

  return 0;
}
