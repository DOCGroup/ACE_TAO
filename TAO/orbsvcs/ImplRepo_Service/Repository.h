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

struct Repository_Record
{
  // Constructors
  Repository_Record ();
  Repository_Record (const Repository_Record &);
  Repository_Record (const ASYS_TCHAR *c,
                     const ASYS_TCHAR *e,
                     const ASYS_TCHAR *w,
                     const ASYS_TCHAR *h,
                     const unsigned short p,
                     const ASYS_TCHAR *pi);


  // Destructor
  ~Repository_Record ();

  // Assignment Operator
  Repository_Record &operator= (Repository_Record &);

  // Fields
  ASYS_TCHAR *comm_line;
  ASYS_TCHAR *env;
  ASYS_TCHAR *wdir;
  ASYS_TCHAR *host;
  unsigned short port;
  ASYS_TCHAR *ping_ior;
};

class Repository
{
public:
  Repository ();
  // Default Constructor

  typedef ACE_Hash_Map_Entry<const ASYS_TCHAR *, 
                             Repository_Record *> HASH_STRING_ENTRY;

  typedef ACE_Hash_Map_Manager_Ex<const ASYS_TCHAR *, 
                                  Repository_Record *, 
                                  ACE_Hash<ASYS_TCHAR *>, 
                                  ACE_Equal_To<ASYS_TCHAR *>, 
                                  ACE_Null_Mutex> HASH_STRING_MAP;

  int add (const ASYS_TCHAR *key, const Repository_Record &rec);
  // Add a new server to the Repository

  int update (const ASYS_TCHAR *key, const Repository_Record &rec);
  // Updates an existing key with <rec>
  
  int remove (const ASYS_TCHAR *key);
  // Removes the server from the Repository

  int resolve (const ASYS_TCHAR *key, Repository_Record &rec);
  // Find the key record in the Repository

  // = Accessor methods
  int get_comm_line (const ASYS_TCHAR *key, ASYS_TCHAR *&comm_line);
  int get_env (const ASYS_TCHAR *key, ASYS_TCHAR *&env);
  int get_wdir (const ASYS_TCHAR *key, ASYS_TCHAR *&wdir);
  int get_ping_ior (const ASYS_TCHAR *key, ASYS_TCHAR *&ping_ior);
  int get_hostport (const ASYS_TCHAR *key, ASYS_TCHAR *&host, unsigned short &port);

  // Dump method
  void dump (void);

private:
  HASH_STRING_MAP repository_;
};


#endif /* REPOSITORY_H */
