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

#include "ace/Naming_Context.h"

class Repository
{
public:
  Repository ();
  // Default Constructor

  struct Record
  {
    char *comm_line;
    char *env;
    char *wdir;
    char *host;
    unsigned short port;
    char *ping_ior;
  };

  int add (const char *key, const Repository::Record &rec);
  // Add a new server to the Repository

  int update (const char *key, const Repository::Record &rec);
  // Updates an existing key with <rec>
  
  int remove (const char *key);
  // Removes the server from the Repository

  int resolve (const char *key, Repository::Record &rec);
  // Find the key record in the Repository

  // = Accessor methods
  int get_comm_line (const char *key, char *&comm_line);
  int get_env (const char *key, char *&env);
  int get_wdir (const char *key, char *&wdir);
  int get_ping_ior (const char *key, char *&ping_ior);
  int get_hostport (const char *key, char *&host, unsigned short &port);

  // Dump method
  void dump (void);

private:
  ACE_Naming_Context repository_;
};


#endif /* REPOSITORY_H */
