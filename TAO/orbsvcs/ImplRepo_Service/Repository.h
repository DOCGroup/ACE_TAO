/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
// 
// = FILENAME
//    Repository.h
//
// = DESCRIPTION
//    This class implements the Repository for the Implementation Repository.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (REPOSITORY_H)
#define REPOSITORY_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"


class Server_Info
  // = TITLE
  //   Information about IR registered servers.
  //
  // = DESCRIPTION
  //   Contains all the necessary information about the server including
  //   Information on how to start it up and where it is running.
{
public:
  // = Constructors
  
  Server_Info (const ACE_TString POA_name,
               const ACE_TString logical_server_name,
               const ACE_TString startup_command,
               const ACE_TString working_dir);
  // Initialize the command_line and working_dir.

  // = Destructors

  ~Server_Info ();
  // The only destructor there is.
  
  void update_running_info (const ACE_TString host,
                            const unsigned short port,
                            const ACE_TString ping_ior);
  // Updates information that is relevant only when an instance
  // of the server is running.

  void get_startup_info (ACE_TString &logical_server_name,
                         ACE_TString &startup_command,
                         ACE_TString &working_dir);
  // Returns startup information.

  void get_running_info (ACE_TString &host,
                         unsigned short &port,
                         ACE_TString &ping_ior);
  // Returns information about a running instance.

  // @@ Does this belong here?
  //  int startup ();
  // Starts up the server based on the information.
  // Returns:  0  if successful
  //           -1 if there is no registration command (it has to be manually 
  //              restarted)

  int starting_up_;
  // This is a flag to determine if the process has already been spawned
  // and we are just waiting for it to start up.

private:
  ACE_TString logical_server_name_;
  // Which server process this poa is grouped in. 

  ACE_TString POA_name_;
  // The name of the POA.

  ACE_TString startup_command_;
  // The command line startup command (program and arguments).

  ACE_TString working_dir_;
  // The working directory.

  ACE_TString host_;
  // Current hostname used by the server.

  unsigned short port_;
  // Current port used by the server.

  ACE_TString ping_ior_;
  // IOR of the ping object in the server.

  // No copying allowed.
  void operator= (Server_Info &);
  Server_Info (Server_Info &);
};

class Server_Repository
  // = TITLE
  //   Repository of Server_Infos.
  //
  // = DESCRIPTION
  //   Handles the storage, updating, and startup of servers.
{
public:
  Server_Repository ();
  // Default Constructor

  typedef ACE_Hash_Map_Entry<ACE_TString, 
                             Server_Info *> HASH_IR_ENTRY;

  typedef ACE_Hash_Map_Manager_Ex<ACE_TString, 
                                  Server_Info *, 
                                  ACE_Hash<ACE_TString>, 
                                  ACE_Equal_To<ACE_TString>, 
                                  ACE_Null_Mutex> HASH_IR_MAP;

  typedef ACE_Hash_Map_Iterator_Ex<ACE_TString,
                                   Server_Info *, 
                                   ACE_Hash<ACE_TString>, 
                                   ACE_Equal_To<ACE_TString>, 
                                   ACE_Null_Mutex> HASH_IR_ITER;

  int add (const ACE_TString POA_name,
           const ACE_TString logical_server_name,
           const ACE_TString startup_command,
           const ACE_TString working_dir);
  // Add a new server to the Repository

  int update (const ACE_TString POA_name,
              const ACE_TString host,
              const unsigned short port,
              const ACE_TString ping_ior);
  // Update the associated process information.

  int get_startup_info (const ACE_TString POA_name,
                        ACE_TString &logical_server_name,
                        ACE_TString &startup_command,
                        ACE_TString &working_dir);
  // Returns information related to startup.
  
  int get_running_info (const ACE_TString POA_name,
                        ACE_TString &host,
                        unsigned short &port,
                        ACE_TString &ping_ior);
  // Returns information related to a running copy.
  
  int starting_up (const ACE_TString POA_name, int new_value);
  // Checks the starting_up_ variable in the Server_Info and 
  // returns the previous value or -1 if the POA_name wasn't found

  int starting_up (const ACE_TString POA_name);
  // Same as above but does not alter the value.

  int remove (const ACE_TString POA_name);
  // Removes the server from the Repository.

  HASH_IR_ITER *new_iterator ();
  // Returns a new iterator that travels over the repository.

  size_t get_repository_size ();
  // Returns the number of entries in the repository.

private:
  HASH_IR_MAP repository_;
};


#endif /* REPOSITORY_H */
