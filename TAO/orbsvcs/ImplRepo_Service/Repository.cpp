/* -*- C++ -*- */
// $Id$
#include "Repository.h"

ACE_RCSID(ImplRepo, Repository, "$Id$")

// Default Constructor
Repository::Repository ()
{
  this->repository_.open (ACE_Naming_Context::PROC_LOCAL);
}

// Add a new server to the Repository
int
Repository::add (const char *key, const Repository::Record &rec)
{
  char *temp; // Temporary string to hold all the variables in.

  // Needs to be as long as all the lengths plus the separators and the
  // null character.
  ACE_NEW_RETURN (temp,
                  char [ACE_OS::strlen (rec.comm_line)
                        + ACE_OS::strlen (rec.env)
                        + ACE_OS::strlen (rec.wdir)
                        + ACE_OS::strlen (rec.ior)
                        + 4],
                  -1);

  //Put them all in a string
  ACE_OS::sprintf (temp,
                   "%s\n%s\n%s\n%s",
                   rec.comm_line,
                   rec.env,
                   rec.wdir,
                   rec.ior);

  // Store the record in the repository.
  int retval = this->repository_.bind (key, temp);

  // Clean up and exit.
  delete [] temp;

  return retval;
}

// Removes the server from the Repository
int
Repository::remove (const char *key)
{
  return this->repository_.unbind (key);
}


// Find the key record in the Repository
int
Repository::resolve (const char *key, Repository::Record &rec)
{
  char *value, *type; // Temp variables needed for resolve
  char *last, *temp ; // For fields
  int retval = this->repository_.resolve (key, value, type);

  // If successful, return what we need.
  while (retval == 0)
  {
    temp = value;
    last = ACE_OS::strstr (temp, "\n");
    if (last != 0 && *last == '\n')
    {
      *last = '\0';
      ACE_NEW_RETURN (rec.comm_line, char [strlen (temp) + 1], -1);

      // Copy to the comm_line argument
      strcpy (rec.comm_line, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    temp = last + 1;
    last = ACE_OS::strstr (temp, "\n");
    if (last != 0 && *last == '\n')
    {
      *last = '\0';
      ACE_NEW_RETURN (rec.env, char [strlen (temp) + 1], -1);

      // Copy to the env argument
      strcpy (rec.env, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    temp = last + 1;
    if (last != 0)
    {
      ACE_NEW_RETURN (rec.wdir, char [strlen (temp) + 1], -1);

      // Copy to the env argument
      strcpy (rec.wdir, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    temp = last + 1;
    if (last != 0)
    {
      ACE_NEW_RETURN (rec.ior, char [strlen (temp) + 1], -1);

      // Copy to the env argument
      strcpy (rec.ior, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    // Now exit out.
    break;
  }

  delete [] value;
  delete [] type;
  return retval;
}
// = Accessor methods

int
Repository::get_comm_line (const char *key, char *&comm_line)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  comm_line = rec.comm_line;
  delete [] rec.env;
  delete [] rec.wdir;
  delete [] rec.ior;

  return retval;
}


int
Repository::get_env (const char *key, char *&env)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  delete [] rec.comm_line;
  env = rec.env;
  delete [] rec.wdir;
  delete [] rec.ior;

  return retval;
}

int
Repository::get_wdir (const char *key, char *&wdir)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  delete [] rec.comm_line;
  delete [] rec.env;
  wdir = rec.wdir;
  delete [] rec.ior;

  return retval;
}

int
Repository::get_ior (const char *key, char *&ior)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  delete [] rec.comm_line;
  delete [] rec.env;
  delete [] rec.wdir;
  ior = rec.ior;

  return retval;
}

void
Repository::dump (void)
{
  this->repository_.dump ();
}
