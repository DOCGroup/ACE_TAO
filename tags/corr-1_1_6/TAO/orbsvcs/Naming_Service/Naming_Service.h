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

#include "tao/TAO.h"
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

  TAO_Naming_Service (int argc, char *argv[]);
  // Constructor taking the command-line arguments.

  int init (int argc, char *argv[]);
  // Initialize the Naming Service with the arguments.

  int run (CORBA_Environment& ACE_TRY_ENV);
  // Run the TAO_Naming_Service.

  ~TAO_Naming_Service (void);
  // Destructor.

protected:
  int parse_args (int argc, char *argv[]);
  // parses the arguments.

  CORBA::ORB_var orb_;
  // The ORB.

  PortableServer::POA_var root_poa_;
  // The Root POA.

  PortableServer::POA_var ns_poa_;
  // The Naming Service POA.

  TAO_Naming_Server my_naming_server_;
  // Naming Server instance.

  FILE *ior_output_file_;
  // File to output the Naming Service IOR.

  const char *pid_file_name_;
  // File to output the process id.

  size_t context_size_;
  // Size of the hash_table allocated upon the creation of the Naming
  // Service context (if one is created).  Note: all the contexts
  // created under the given context will use the same size for their
  // initial hash table allocations.

  const ACE_TCHAR *persistence_file_name_;
  // Path to the file to be used to store/read in Naming Service
  // persistent state.

  void *base_address_;
  // Address to be used for memory mapping Naming Service state file,
  // identified by the <persistence_file_name_>.

  size_t time_;
  // After how long the server should stop listening to requests (in
  // seconds).

  int multicast_;
  // If not zero multicast is enabled.
};

#endif /* TAO_NAMING_SERVICE_H */
