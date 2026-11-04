#include "simple_test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"

#include "tao/ORB_Core.h"
#include "tao/IORTable/Async_IORTable.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include <string>

struct Server_Options
{
  std::string ior_table_name {"IORTable"};
  bool use_async_locator {};
};

//*************************************************************************
class AsyncLocatorTask
  : public virtual IORTable::AsyncLocator,
    public virtual CORBA::LocalObject
{
public:
  explicit AsyncLocatorTask (CORBA::ORB_ptr orb, std::string const &test_ior);
  AsyncLocatorTask (AsyncLocatorTask const &) = delete;
  AsyncLocatorTask (AsyncLocatorTask &&) = delete;
  AsyncLocatorTask &operator= (AsyncLocatorTask const &) = delete;
  AsyncLocatorTask &operator= (AsyncLocatorTask &&) = delete;

  void async_locate (IORTable::Locate_ResponseHandler rh, char const *) override;
  char *locate (char const *) override;

private:
  CORBA::ORB_var orb_;
  std::string test_ior_;
};

void
AsyncLocatorTask::async_locate (IORTable::Locate_ResponseHandler rh, char const *id)
{
  CORBA::Object_var const fwd_object = this->orb_->string_to_object (this->test_ior_.c_str ());
  CORBA::String_var fwd_string = this->orb_->object_to_string (fwd_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("in async_locate!  Forwarding client %C to: %C\n"),
              id, fwd_string.in ()));

  rh->forward_ior (fwd_string._retn (), false);
}

char *
AsyncLocatorTask::locate (char const *id)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("In async locator::locate, id = %C, throwing\n"),
              id));
  throw IORTable::NotFound ();
}


AsyncLocatorTask::AsyncLocatorTask (CORBA::ORB_ptr orb, std::string const &test_ior)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    test_ior_ (test_ior)
{
}

//*************************************************************************
class SyncLocatorTask
  : public virtual IORTable::Locator,
    public virtual CORBA::LocalObject
{
public:
  explicit SyncLocatorTask (CORBA::ORB_ptr orb, std::string const &test_ior);
  SyncLocatorTask (SyncLocatorTask const &) = delete;
  SyncLocatorTask (SyncLocatorTask &&) = delete;
  SyncLocatorTask &operator= (SyncLocatorTask const &) = delete;
  SyncLocatorTask &operator= (SyncLocatorTask &&) = delete;

  char *locate (char const *) override;

private:
  CORBA::ORB_var orb_;
  std::string test_ior_;
};

char *
SyncLocatorTask::locate (char const *id)
{
  CORBA::Object_var const fwd_object = this->orb_->string_to_object (this->test_ior_.c_str ());
  CORBA::String_var fwd_string = this->orb_->object_to_string (fwd_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("in synch locate: Forwarding client %C to: %C\n"),
              id,
              fwd_string.in ()));

  return fwd_string._retn ();
}

SyncLocatorTask::SyncLocatorTask (CORBA::ORB_ptr orb, std::string const &test_ior)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    test_ior_ (test_ior)
{
}

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  for (int c = 1; c < argc; c++)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-aa")) == 0)
        {
          options.ior_table_name = "AsyncIORTable";
          options.use_async_locator = true;
        }
      else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-as")) == 0)
        {
          options.ior_table_name = "AsyncIORTable";
          options.use_async_locator = false;
        }
      else
        {
          return -1;
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) == -1)
        return -1;

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      Simple_Test_i *test_impl {};
      ACE_NEW_RETURN (test_impl,
                      Simple_Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      simple::SimpleTest_T_var const the_server =
        simple::SimpleTest_T::_narrow (object.in ());

      CORBA::String_var const test_ior = orb->object_to_string (the_server.in ());

      poa_manager->activate ();

      // Create task.
      IORTable::Locator_ptr iorloc_ptr {};
      if (options.use_async_locator)
        {
          ACE_NEW_RETURN (iorloc_ptr,
                          AsyncLocatorTask (orb.in (), test_ior.in ()),
                          1);
        }
      else
        {
          ACE_NEW_RETURN (iorloc_ptr,
                          SyncLocatorTask (orb.in (), test_ior.in ()),
                          1);
        }
      IORTable::Locator_var const iorloc (iorloc_ptr);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Server - using ior table object: %C\n"),
                  options.ior_table_name.c_str()));
      // Resolve the IORTable and set the locator
      CORBA::Object_var const ior_table_object =
        orb->resolve_initial_references (options.ior_table_name.c_str ());

      IORTable::Table_var const local_ior_table = IORTable::Table::_narrow (ior_table_object.in ());

      local_ior_table->set_locator (iorloc.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Server - event loop finished\n")));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Server - Exception caught:"));
      return 1;
    }

  return 0;
}
