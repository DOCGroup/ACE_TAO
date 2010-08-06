// $Id$

#include "Messenger_i.h"
#include <iostream>
#include <fstream>
// 1. Define a "task" class for implenting the thread pool threads.
#include "ace/Task.h"

class ORB_Task : public ACE_Task_Base
{
public:
  ORB_Task (CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb)) { }
  virtual ~ORB_Task () { }
  virtual int svc ()
  {
    this->orb_->run();
    return 0;
  }
private:
  CORBA::ORB_var orb_;
};

// 2. Establish the number of threads.
static const int nthreads = 4;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    Messenger_i messenger_servant;

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid =
      poa->activate_object( &messenger_servant );
    CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
    std::ofstream iorFile( "Messenger.ior" );
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file Messenger.ior" << std::endl;

    // 3. Create and activate threads for the thread pool.
    ORB_Task task (orb.in());
    int retval = task.activate (THR_NEW_LWP | THR_JOINABLE, nthreads);
    if (retval != 0) {
      std::cerr << "Failed to activate " << nthreads << " threads." << std::endl;
      return 1;
    }

    // 4. Wait for threads to finish.
    task.wait();

    // Clean up.
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
