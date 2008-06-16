// $Id$

#include "ace/Functor.h"
#include "ace/Task.h"
#include "fooS.h"

/*
  This isn't too complicated, but it is a little convoluted.  So, here's the
  explanation:

  1. Two threads.  One thread is a CORBA server, the other a CORBA
     client to that CORBA server.

  2. the main thread sets up the server-side stuff, and then fires off
     a task to run the server.

  3. once the server thread/task is running, the main thread makes
     invocations to "match_references()" and reports the outcome.

  The CORBA Object compares the stringified representation of two IORs
  both generated from the ORB but in different contexts, and returns the
  result of the comparison as a boolean.
 */

class Foo_Impl : public virtual POA_foo
{
public:
  CORBA::Boolean match_references ();

  CORBA::String_var ior_from_main_;
  ACE_Equal_To<const char*> equal_func;
  ACE_Hash<const char*> hash_func;
};

CORBA::Boolean
Foo_Impl::match_references ()
{
  CORBA::Object_var o = _this ();
  CORBA::ORB_var orb = o->_get_orb ();
  CORBA::String_var ior_from_upcall = orb->object_to_string (o.in());

  CORBA::Boolean r1 = equal_func (this->ior_from_main_.in(), ior_from_upcall.in());

  return r1;
}

class Server_Task : public ACE_Task_Base
{
public:
  Server_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }
  virtual ~Server_Task ();

  virtual int svc ();
private:
  CORBA::ORB_var orb_;
};

Server_Task::~Server_Task() { }

int
Server_Task::svc ()
{
  this->orb_->run ();
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var s_orb;
  try
    {
      s_orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var o = s_orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var rootpoa = PortableServer::POA::_narrow (o.in());
      if (CORBA::is_nil (rootpoa.in()))
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t): failed to get root poa\n"), 1);

      PortableServer::POAManager_var poamgr = rootpoa->the_POAManager();

      Foo_Impl* foo = 0;
      ACE_NEW_RETURN (foo, Foo_Impl, 1);
      PortableServer::ServantBase_var owner_transfer (foo);
      PortableServer::ObjectId_var id = rootpoa->activate_object (foo);
      o = rootpoa->id_to_reference (id.in());

      Foo_Impl* foo2 = 0;
      ACE_NEW_RETURN (foo2, Foo_Impl, 1);
      PortableServer::ServantBase_var foo_owner (foo2);
      foo_var f2 = foo2->_this (); // implicit activation

      poamgr->activate ();

      foo->ior_from_main_ = s_orb->object_to_string (o.in());
      foo2->ior_from_main_ = foo->ior_from_main_;

      Server_Task server(s_orb.in());
      server.activate(1); // activate one thread running the task

      foo_var f = foo::_narrow (o.in());  // ignore the possibility that
                                          // it's not a 'foo' since we
                                          // created it above

      int const iterations = 10;
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: next %d iterations should match\n",
                  iterations));
      for (int i = 0; i < iterations; ++i)
        {
          CORBA::Boolean b = f->match_references ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: iteration %d, match = %d\n",
                      i, b));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client: next %d iterations should NOT match\n",
                  iterations));
      for (int i = 0; i < iterations; ++i)
        {
          CORBA::Boolean b = f2->match_references ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client: iteration %d, match = %d\n",
                      i, b));
        }
    }
  catch (...)
    {
    }

  return 0;
}
