//**************************************************************************
//
// NAME :   tpr_server.cpp 
// DESCRIPTION:  
//
// Server mainline
//
//****************************************************************************
#define IT_EX_MACROS

#include "cubit_impl.h"		// server header file

int 
main (int , char**)
{

#ifdef Cubit_USE_BOA
  Cubit_Factory_var cf = new Cubit_Factory_Impl;
  cout << "Using BOA approach" << endl;
#else
  Cubit_Factory_var cf = new TIE_Cubit_Factory (Cubit_Factory) (new Cubit_Factory);
  cout << "Using TIE approach" << endl;
#endif /* Cubit_USE_BOA */
  
  //
  // Go get some work to do....
  //
  IT_TRY {
      CORBA::Orbix.impl_is_ready("Cubit_Factory", IT_X);
  }
  IT_CATCHANY {
     cout << IT_X << endl;
  }
  IT_ENDTRY;

  cout << "Cubit server is exiting." << endl;


  return 0;
}
