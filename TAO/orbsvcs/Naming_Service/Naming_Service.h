// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/Naming_Service/Naming_Service
//
// = FILENAME
//    Naming_Service.h
//
// = DESCRIPTION
//      This class implements the functionality of a Naming_Service in
//      a stand-alone process.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//    Marina Spivak <marina@cs.wustl.edu>
// ============================================================================

#ifndef TAO_NAMING_SERVICE_H
#define TAO_NAMING_SERVICE_H

#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class TAO_Naming_Service
{
  // = TITLE
  //   Defines a class that encapsulates the implementation of the
  //   COS Naming Service.
  //
  // = DESCRIPTION
  //   This class makes use of the <TAO_Naming_Server> and
  //   <TAO_ORB_Manager> class to implement the COS Naming Service.
public:
  TAO_Naming_Service (void);
  // Default Constructor.

  TAO_Naming_Service (int argc, ACE_TCHAR* argv[]);
  // Constructor taking the command-line arguments.

  virtual int init (int argc, ACE_TCHAR* argv[]);
  // Initialize the Naming Service with the arguments.

  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Run the TAO_Naming_Service.

  virtual ~TAO_Naming_Service (void);
  // Destructor.

protected:

  int parse_args (int &argc, ACE_TCHAR* argv[]);
  // Parse the command line arguments to find
  // the timeout period.

  CORBA::ORB_var orb_;
  // The ORB.

  //  PortableServer::POA_var root_poa_;
  // The Root POA.

  TAO_Naming_Server my_naming_server_;
  // Naming Server instance.

  long time_;
  // After how long the server should stop listening to requests (in
  // seconds).
};

#endif /* TAO_NAMING_SERVICE_H */
