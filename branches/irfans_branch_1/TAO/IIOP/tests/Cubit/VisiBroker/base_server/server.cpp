//**************************************************************************
//
// NAME :   tpr_server.cpp 
// DESCRIPTION:  
//
// Server mainline
//
//****************************************************************************
#include "cubit_impl.h"		// server header file

int 
main (int argc, char** argv)
{

  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  CORBA::BOA_var boa = orb->BOA_init(argc, argv);

#ifdef Cubit_USE_BOA
  Cubit_Impl cb("Cubit");
  cout << "Using BOA approach" << endl;
#else
  Cubit_Impl tied("Cubit");
  _tie_Cubit<Cubit_Impl> cb(tied, "Cubit");

  cout << "Using TIE approach" << endl;
#endif /* Cubit_USE_BOA */
  
  //
  // Go get some work to do....
  //
  try {

    boa->obj_is_ready(&cb);

    boa->impl_is_ready();

  } catch (const CORBA::Exception &excep) {
    cerr << "Server error: " << excep << endl;
    return -1;
  } catch (...) {
    cerr << "Unknown exception" << endl;
    return -1;
  }

  cout << "Cubit server is exiting." << endl;


  return 0;
}
