// $Id$

// AddrServerMain.cpp
// Main program for a common Address server instance to be shared by all
// event channels

#include "SimpleAddressServer.h"
#include "BarrierS.h"

#include "orbsvcs/CosNamingC.h"

#include "ace/Barrier.h"
#include "ace/Task.h"
#include "ace/Auto_Ptr.h"
#include <fstream>

class Barrier_i : public virtual POA_Barrier
{
public:
  Barrier_i (int count) : barrier_(count)
  {
    ACE_DEBUG ((LM_DEBUG,"Barrier, count = %d\n", count));
  }

  void wait (void)
  {
    ACE_DEBUG ((LM_DEBUG,"%t entering wait%d\n"));
    this->barrier_.wait();
    ACE_DEBUG ((LM_DEBUG,"%t exiting wait%d\n"));
  }

private:
  ACE_Barrier barrier_;
};

class ORB_Task : public ACE_Task_Base
{
public:
  ORB_Task (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb)) {}

  int svc () { this->orb_->run(); return 1; }

private:
  CORBA::ORB_var orb_;
};

int main (int argc, char* argv[])
{
  try
  {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    const char* address = "localhost";
    int num_nodes = 2;

    u_short port = 12345;

    for (int i = 0; argv[i] != 0; i++)
      {
        if (strcasecmp(argv[i], "-address") == 0)
          {
            if (argv[i+1] != 0)
              address = argv[++i];
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing address\n"),0);
          }
        else if (strcasecmp(argv[i], "-port") == 0)
          {
            if (argv[i+1] != 0)
              port = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing port\n"),0);
          }
        else if (strcasecmp(argv[i], "-nodes") == 0)
          {
            if (argv[i+1] != 0)
              num_nodes = ACE_OS::atoi(argv[++i]);
            else
              ACE_ERROR_RETURN ((LM_ERROR, "Missing node count\n"),0);
          }
      }

    // Get the POA
    CORBA::Object_var tmpobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (tmpobj.in ());
    PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    // Find the Naming Service.
    tmpobj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root_context =
      CosNaming::NamingContextExt::_narrow(tmpobj.in());

    // Initialize the address server with the desired address.
    // This will be used by the sender object and the multicast
    // receiver.
    ACE_INET_Addr send_addr (port, address);
    SimpleAddressServer addr_srv_impl (send_addr);

    PortableServer::ObjectId_var oid = poa->activate_object(&addr_srv_impl);
    tmpobj = poa->id_to_reference(oid.in());

    // Bind the Addr server using Naming Services
    CosNaming::Name_var name = root_context->to_name("Echo_address");
    root_context->rebind(name.in(), tmpobj.in());

    // Activate a barrier object
    Barrier_i barrier_impl(num_nodes);

    oid = poa->activate_object(&barrier_impl);
    tmpobj = poa->id_to_reference(oid.in());

    name = root_context->to_name ("Echo_barrier");
    root_context->rebind(name.in(), tmpobj.in());

    // run the ORB
    ORB_Task orb_task(orb.in());
    orb_task.activate ( THR_NEW_LWP | THR_JOINABLE,
                        num_nodes+1);
    orb_task.wait();
    orb->destroy();
    return 0;
  }
  catch (CORBA::Exception& exc)
  {
    ACE_ERROR ((LM_ERROR,
    "Caught CORBA::Exception\n%s (%s)\n",
    exc._name (), exc._rep_id ()));
  }
  return 1;
}
