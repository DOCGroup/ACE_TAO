// $Id$
#if !defined (REPOSITORY_H)
#define REPOSITORY_H

#include "ace/Naming_Context.h"

class Repository
{
public:
  Repository ();
  // Default Constructor

  int add (const char *key, const char *comm_line, const char *env, const char *wdir);
  // Add a new server to the Repository
  
  int remove (const char *key);
  // Removes the server from the Repository

  int resolve (const char *key, char *&comm_line, char *&env, char *&wdir);
  // Find the key record in the Repository

  // = Accessor methods
  int get_comm_line (const char *key, char *&comm_line);
  int get_env (const char *key, char *&env);
  int get_wdir (const char *key, char *&wdir);

  // Dump method
  void dump (void);

private:
  ACE_Naming_Context repository_;
};


#endif /* REPOSITORY_H */
