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
//      This class implements the functionality of a Naming_Service.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#if !defined (_NAMING_SERVICE_H)
#define _NAMING_SERVICE_H

#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Naming_Service
{
  // = TITLE
  //   Defines a class that encapsulates the implementation of the
  //   COS Naming Service.
  //
  // = DESCRIPTION
  //   This class makes use of the <TAO_Naming_Server> and
  //   <TAO_ORB_Manager> class to implement the COS <Naming_Service>.
public:
  Naming_Service (void);
  // Default Constructor.

  Naming_Service (int argc, char *argv[]);
  // Constructor taking the command-line arguments.

  int init (int argc, char *argv[]);
  // Initialize the Naming Service with the arguments.

  int run (CORBA_Environment& env);
  // Run the Naming_Service.

  ~Naming_Service (void);
  // Destructor.
	
private:
  int parse_args (int argc, char *argv[]);
  // parses the arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  TAO_Naming_Server my_naming_server_;
  // Naming Server instance.

  FILE *ior_output_file_;
  // File to output the Naming Service IOR.

  const char *pid_file_name_;
  // File to output the process id.
};

#endif /* _NAMING_SERVICE_H */
