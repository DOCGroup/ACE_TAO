// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

// ACE includes.
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"

// TAO includes.
#include "tao/corba.h"

// MT Cubit application includes.
#include "cubit_i.h"

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* VME_DRIVER */

// Global Variables
static CORBA::String key = CORBA::String ("Cubit");

// Size of the string for holding the stringified object reference.
// Sumedh, what is the rationale for a string of size 30?  Can you use
// a more general parameter instead?  Perhaps this needs to go into
// TAO somewhere?
const u_int OBJECT_STRING_SIZE = 30;

class Cubit_Task : public ACE_Task<ACE_NULL_SYNCH>
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
{
public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs);
  // Constructor.

  virtual int svc (void);
  // Active Object entry point.

protected:
  Cubit_Task (void);
  // No-op constructor.

private:
  int initialize_orb (void);
  // Initialize the ORB, and POA.

  int create_servants (void);
  // Create the servants

  char *orbname_;
  // Name of the ORB.

  char *orbargs_;
  // ORB arguments.

  u_int num_of_objs_;
  // Number of objects we're managing.

  CORBA::ORB_ptr orb_ptr_;
  // Pointer to the ORB

  CORBA::POA_ptr oa_ptr_;
  // Pointer to the POA

  Cubit_i **servants_;
  // Array to hold the servants

};
