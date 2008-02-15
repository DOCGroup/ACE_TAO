// $Id$

#include "Server.h"

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_strings.h"

void replace (ACE_TCHAR** a, ACE_TCHAR** b)
  {
    ACE_TCHAR* tmp = *a;

    *a = *b;
    *b = tmp;
  }

Server::Server (int , ACE_TCHAR* argv[])
  : init_ (false), shutdown_ (false)
  , orb_shutdown_ (false), management_orb_shutdown_ (false)
{
  try {
    ACE_ARGV args_1 (argv);
    ACE_ARGV args_2 (argv);

    /*
      for (int my_count = 0; my_count < args_1.argc(); my_count++) {
      ACE_DEBUG ((LM_DEBUG, "<%d, %s> ", my_count, (args_1.argv())[my_count]));
      }
      ACE_DEBUG ((LM_DEBUG, "\n"));
    */

    ACE_TCHAR** my_argv = args_1.argv ();
    int my_argc = args_1.argc ();
    int count = 0;
    for (count = my_argc-1; count > -1; count--) {
      if (ACE_OS::strcasecmp ("-ORBEndpoint", my_argv[count]) == 0) {
        break;
      }
    }
    if (count > my_argc-2){
      // check if less than 2-last position.
      return;
    }
    replace (&(my_argv[count]), &(my_argv[my_argc-2]));
    replace (&(my_argv[count+1]), &(my_argv[my_argc-1]));

    my_argc--; my_argc--;
    orb_ = CORBA::ORB_init (my_argc, my_argv, "Server");

    my_argv = args_2.argv();
    my_argc = args_2.argc();
    for (count = 0; count < my_argc; count++) {
      if (ACE_OS::strcasecmp ("-ORBEndpoint", my_argv[count]) == 0) {
        break;
      }
    }
    if (count > my_argc-2) {
      // check if less than 2-last position.
      return;
    }
    replace (&(my_argv[count]), &(my_argv[my_argc-2]));
    replace (&(my_argv[count+1]), &(my_argv[my_argc-1]));

    // initialize management ORB
    my_argc--; my_argc--;
    management_orb_ = CORBA::ORB_init (my_argc, my_argv, "Management");

    if (!this->parse_args (my_argc, my_argv))
      return;

    CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (obj.in ());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

    ACE_auto_ptr_reset (test_i_, new Test_i (orb_.in()));
    PortableServer::ObjectId_var oid =
      root_poa->activate_object (test_i_.get());
    obj = root_poa->id_to_reference (oid.in());
    Test_var test_obj = Test::_narrow (obj.in());

    std::string ior = orb_->object_to_string (test_obj.in ());
    obj = orb_->resolve_initial_references("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow (obj.in());
    table->bind ("Test", ior.c_str());

    poa_manager->activate ();

    // initialize management objects

    obj = management_orb_->resolve_initial_references ("RootPOA");
    PortableServer::POA_var management_root_poa =
      PortableServer::POA::_narrow (obj.in ());
    PortableServer::POAManager_var management_poa_manager =
      management_root_poa->the_POAManager ();

    oid = management_root_poa->activate_object (test_i_.get());
    obj = management_root_poa->id_to_reference (oid.in());
    test_obj = Test::_narrow (obj.in());

    ior = orb_->object_to_string (test_obj.in ());
    obj = management_orb_->resolve_initial_references("IORTable");
    table = IORTable::Table::_narrow (obj.in());
    table->bind ("Management", ior.c_str());

    management_poa_manager->activate ();
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Server> Caught CORBA::Exception %s"
                , ex._info().c_str()));
    return;
  }

  init_ = true;
}

Server::~Server ()
{
  this->shutdown ();
}

bool
Server::parse_args (int , ACE_TCHAR* [])
{
  /*
    ACE_Get_Opt get_opts (argc, argv, "");
    int c;

    while ((c = get_opts ()) != -1) {
    switch (c)
    {
    default:
    ACE_ERROR_RETURN ((LM_ERROR, "Invalid option \'-%c\'\n", c)
    , false);
    }
    }
  */

  return true;
}

bool
Server::run (bool management)
{
  bool status = true;

  try {
    if (!management) {
      orb_->run ();
      orb_shutdown_ = true;
    }
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Server::run> Caught CORBA::Exception %s in worker ORB\n"
                , ex._info().c_str()));
    status = false;
  }

  try {
    if (management) {
      management_orb_->run();
      management_orb_shutdown_ = true;
    }
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Server::run> Caught CORBA::Exception %s in management ORB\n"
                , ex._info().c_str()));
    status = false;
  }

  return status;
}

void
Server::shutdown ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (shutdown_) {
    return;
  }

  try {
    management_orb_->shutdown (1);

    ACE_Time_Value sleep_time;
    sleep_time.msec (10);
    while (true) {
      ACE_OS::sleep (sleep_time);
      if (management_orb_shutdown_ && orb_shutdown_) {
        break;
      }
    }

    orb_->destroy ();
    orb_ = CORBA::ORB::_nil();
    management_orb_->destroy ();
    management_orb_ = CORBA::ORB::_nil();
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Server::run> Caught CORBA::Exception %s"
                , ex._info().c_str()));

  }

  shutdown_ = true;
}
