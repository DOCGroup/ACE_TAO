/* -*- C++ -*- */
// $Id$
#include "Repository.h"

ACE_RCSID(ImplRepo_Service, Repository, "$Id$")

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
  // null character (6) and then also the size that the host/port could
  // take up.
  ACE_NEW_RETURN (temp,
                  char [ACE_OS::strlen (rec.comm_line)
                        + ACE_OS::strlen (rec.env)
                        + ACE_OS::strlen (rec.wdir)
                        + ACE_OS::strlen (rec.ping_ior)
                        + ACE_OS::strlen (rec.host)
                        + 40],
                  -1);

  // Put them all in a string
  // Why use the extra space?  Well, strtok doesn't like null strings
  // because they show up as \n\n, which it skips past.
  ACE_OS::sprintf (temp,
                   " %s\n %s\n %s\n %s\n %hu\n %s\n",
                   rec.comm_line,
                   rec.env,
                   rec.wdir,
                   rec.host,
                   rec.port,
                   rec.ping_ior);

  // Store the record in the repository.
  int retval = this->repository_.bind (key, temp);

  // Clean up and exit.
  delete [] temp;

  return retval;
}

int 
Repository::update (const char *key, const Repository::Record &rec)
{
  this->remove (key);
  return this->add (key, rec);
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
  char *value = 0, *type = 0; // Temp variables needed for resolve
  int retval = this->repository_.resolve (key, value, type);

  if (retval == 0)
    {
      // +1 to get rid of the space
      rec.comm_line = ACE::strnew (ACE_OS::strtok (value, "\n") + 1);
      rec.env = ACE::strnew (ACE_OS::strtok (NULL, "\n") + 1);
      rec.wdir = ACE::strnew (ACE_OS::strtok (NULL, "\n") + 1);
      rec.host = ACE::strnew (ACE_OS::strtok (NULL, "\n") + 1);
      ::sscanf (ACE_OS::strtok (NULL, "\n"), "%hu", &rec.port);
      rec.ping_ior = ACE::strnew (ACE_OS::strtok (NULL, "\n") + 1);
    }
  else
    {
      retval = -1;
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

  if (retval == 0)
    {
      comm_line = rec.comm_line;
      delete [] rec.env;
      delete [] rec.wdir;
      delete [] rec.ping_ior;
    }

  return retval;
}


int
Repository::get_env (const char *key, char *&env)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  if (retval == 0)
    {
      delete [] rec.comm_line;
      env = rec.env;
      delete [] rec.wdir;
      delete [] rec.host;
      delete [] rec.ping_ior;
    }

  return retval;
}

int
Repository::get_wdir (const char *key, char *&wdir)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  if (retval == 0)
    {
      delete [] rec.comm_line;
      delete [] rec.env;
      wdir = rec.wdir;
      delete [] rec.host;
      delete [] rec.ping_ior;
    }

  return retval;
}

int
Repository::get_hostport (const char *key, char *&host, unsigned short &port)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  if (retval == 0)
    {
      delete [] rec.comm_line;
      delete [] rec.env;
      delete [] rec.wdir;
      host = rec.host;
      port = rec.port;
      delete [] rec.ping_ior;
    }

  return retval;
}

int
Repository::get_ping_ior (const char *key, char *&ping_ior)
{
  Repository::Record rec;
  int retval = this->resolve (key, rec);

  if (retval == 0)
    {
      delete [] rec.comm_line;
      delete [] rec.env;
      delete [] rec.wdir;
      delete [] rec.host;
      ping_ior = rec.ping_ior;
    }

  return retval;
}

void
Repository::dump (void)
{
  this->repository_.dump ();
}
