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
                        + 40],
                  -1);

  //Put them all in a string
  ACE_OS::sprintf (temp,
                   "%s\n%s\n%s\n%lu %hu\n%s\n",
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
  char *value, *type; // Temp variables needed for resolve
  char *last, *temp ; // For fields
  int retval = this->repository_.resolve (key, value, type);

  // If successful, return what we need.
  while (retval == 0) // Why a while statement?  So we can break out of it.
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
    last = ACE_OS::strstr (temp, "\n");
    if (last != 0)
    {
      *last = '\0';
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
    last = ACE_OS::strstr (temp, "\n");
    if (last != 0)
    {
      *last = '\0';
      ::sscanf (temp, "%lu %hu", &rec.host, &rec.port);
    }
    else
    {
      retval = -1;
      break;
    }

    temp = last + 1;
    last = ACE_OS::strstr (temp, "\n");
    if (last != 0)
    {
      *last = '\0';
      ACE_NEW_RETURN (rec.ping_ior, char [strlen (temp) + 1], -1);

      // Copy to the ping_ior argument
      strcpy (rec.ping_ior, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    delete [] value;
    delete [] type;

    // Now exit out.
    break;
  }

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
      delete [] rec.ping_ior;
    }

  return retval;
}

int
Repository::get_hostport (const char *key, unsigned long &host, unsigned short &port)
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
      ping_ior = rec.ping_ior;
    }

  return retval;
}

void
Repository::dump (void)
{
  this->repository_.dump ();
}
