// $Id$

// ================================================================
//
// = FILENAME
//     Dir_Service.h
//
// = DESCRIPTION
//     This is a customised servant-- a dynamic linked library which 
//     provides telephone directory information.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ================================================================

#ifndef DIR_SERVICE_I_H
#define DIR_SERVICE_I_H

#include "Dir_ServiceS.h"

class GENERIC_SERVANT_Export Dir_Service_i : public POA_Dir_Service
{
  //=  TITLE
  //   Directory Service object.
  //
  //=  DESCRIPTION
  //   Provides telephone directory assistance.
 public:
  
  Dir_Service_i (CORBA::ORB_ptr orb,
                 PortableServer::POA_ptr poa);
  // Initialization.
  
  ~Dir_Service_i (void);
  // Destruction.

  PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  // Returns the Default POA of this Servant object  

  CORBA::Long tele_number (const char* name,
                           CORBA::Environment &env );
  // The telephone number is returned.

  void end_note (CORBA::Environment &env);
  // Just a word of thanks.

  void area_codes_info (CORBA::Environment &env);
  // Extra area codes information provided.  

  void shutdown (CORBA::Environment &env);
  // Shutdown the ORB.
 
 private:
  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()

};

#endif /* DIR_SERVICE_I_H */
