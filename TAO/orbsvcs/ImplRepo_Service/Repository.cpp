/* -*- C++ -*- */
// $Id$
#include "Repository.h"

ACE_RCSID(ImplRepo_Service, Repository, "$Id$")

  ASYS_TCHAR *comm_line;
  ASYS_TCHAR *env;
  ASYS_TCHAR *wdir;
  ASYS_TCHAR *host;
  unsigned short port;
  ASYS_TCHAR *ping_ior;

Repository_Record::Repository_Record ()
: port (0)
{
  ACE_NEW (this->comm_line, ASYS_TCHAR [1]);
  ACE_NEW (this->env, ASYS_TCHAR [1]);
  ACE_NEW (this->wdir, ASYS_TCHAR [1]);
  ACE_NEW (this->host, ASYS_TCHAR [1]);
  ACE_NEW (this->ping_ior, ASYS_TCHAR [1]);

  ACE_OS::strcpy (this->comm_line, ASYS_TEXT (""));
  ACE_OS::strcpy (this->env, ASYS_TEXT (""));
  ACE_OS::strcpy (this->wdir, ASYS_TEXT (""));
  ACE_OS::strcpy (this->host, ASYS_TEXT (""));
  ACE_OS::strcpy (this->ping_ior, ASYS_TEXT (""));
}

Repository_Record::Repository_Record (const Repository_Record &r)
{
  ACE_NEW (this->comm_line, ASYS_TCHAR [ACE_OS::strlen (r.comm_line) + 1]);
  ACE_NEW (this->env, ASYS_TCHAR [ACE_OS::strlen (r.env) + 1]);
  ACE_NEW (this->wdir, ASYS_TCHAR [ACE_OS::strlen (r.wdir) + 1]);
  ACE_NEW (this->host, ASYS_TCHAR [ACE_OS::strlen (r.host) + 1]);
  ACE_NEW (this->ping_ior, ASYS_TCHAR [ACE_OS::strlen (r.ping_ior) + 1]);

  ACE_OS::strcpy (this->comm_line, r.comm_line);
  ACE_OS::strcpy (this->env, r.env);
  ACE_OS::strcpy (this->wdir, r.wdir);
  ACE_OS::strcpy (this->host, r.host);
  ACE_OS::strcpy (this->ping_ior, r.ping_ior);

  this->port = r.port;
}

Repository_Record::Repository_Record (const ASYS_TCHAR *c,
                                      const ASYS_TCHAR *e,
                                      const ASYS_TCHAR *w,
                                      const ASYS_TCHAR *h,
                                      const unsigned short p,
                                      const ASYS_TCHAR *pi)
{
  ACE_NEW (this->comm_line, ASYS_TCHAR [ACE_OS::strlen (c) + 1]);
  ACE_NEW (this->env, ASYS_TCHAR [ACE_OS::strlen (e) + 1]);
  ACE_NEW (this->wdir, ASYS_TCHAR [ACE_OS::strlen (w) + 1]);
  ACE_NEW (this->host, ASYS_TCHAR [ACE_OS::strlen (h) + 1]);
  ACE_NEW (this->ping_ior, ASYS_TCHAR [ACE_OS::strlen (pi) + 1]);

  ACE_OS::strcpy (this->comm_line, c);
  ACE_OS::strcpy (this->env, e);
  ACE_OS::strcpy (this->wdir, w);
  ACE_OS::strcpy (this->host, h);
  ACE_OS::strcpy (this->ping_ior, pi);

  this->port = p;
}

Repository_Record::~Repository_Record ()
{
  delete [] this->comm_line;
  delete [] this->env;
  delete [] this->wdir;
  delete [] this->host;
  delete [] this->ping_ior;
}

Repository_Record &
Repository_Record::operator= (Repository_Record &r)
{
  if (this == &r)
    return *this;

  delete [] this->comm_line;
  delete [] this->env;
  delete [] this->wdir;
  delete [] this->host;
  delete [] this->ping_ior;

  ACE_NEW_RETURN (this->comm_line, ASYS_TCHAR [ACE_OS::strlen (r.comm_line) + 1], *this);
  ACE_NEW_RETURN (this->env, ASYS_TCHAR [ACE_OS::strlen (r.env) + 1], *this);
  ACE_NEW_RETURN (this->wdir, ASYS_TCHAR [ACE_OS::strlen (r.wdir) + 1], *this);
  ACE_NEW_RETURN (this->host, ASYS_TCHAR [ACE_OS::strlen (r.host) + 1], *this);
  ACE_NEW_RETURN (this->ping_ior, ASYS_TCHAR [ACE_OS::strlen (r.ping_ior) + 1], *this);

  ACE_OS::strcpy (this->comm_line, r.comm_line);
  ACE_OS::strcpy (this->env, r.env);
  ACE_OS::strcpy (this->wdir, r.wdir);
  ACE_OS::strcpy (this->host, r.host);
  ACE_OS::strcpy (this->ping_ior, r.ping_ior);

  this->port = r.port;

  return *this;
}

// Default Constructor
Repository::Repository ()
{
  // Nothing
}

// Add a new server to the Repository
int
Repository::add (const ASYS_TCHAR *key, const Repository_Record &rec)
{
  Repository_Record *new_rec = new Repository_Record (rec);

  // Store the record in the repository.
  int retval = this->repository_.bind (key, new_rec);

  return retval;
}

int 
Repository::update (const ASYS_TCHAR *key, const Repository_Record &rec)
{
  this->remove (key);
  return this->add (key, rec);
}

// Removes the server from the Repository
int
Repository::remove (const ASYS_TCHAR *key)
{
  return this->repository_.unbind (key);
}


// Find the key record in the Repository
int
Repository::resolve (const ASYS_TCHAR *key, Repository_Record &rec)
{
  Repository_Record *rep_rec;
  int retval = this->repository_.find (key, rep_rec);

  if (retval == 0)
    rec = *rep_rec;

  return retval;
}
// = Accessor methods

int
Repository::get_comm_line (const ASYS_TCHAR *key, ASYS_TCHAR *&comm_line)
{
  Repository_Record *rec;
  int retval = this->repository_.find (key, rec);

  if (retval == 0)
    {
      ACE_NEW_RETURN (comm_line, ASYS_TCHAR [ACE_OS::strlen (rec->comm_line) + 1], -1);
      ACE_OS::strcpy (comm_line, rec->comm_line);
    }

  return retval;
}


int
Repository::get_env (const ASYS_TCHAR *key, ASYS_TCHAR *&env)
{
  Repository_Record *rec;
  int retval = this->repository_.find (key, rec);

  if (retval == 0)
    {
      ACE_NEW_RETURN (env, ASYS_TCHAR [ACE_OS::strlen (rec->env) + 1], -1);
      ACE_OS::strcpy (env, rec->env);
    }

  return retval;
}

int
Repository::get_wdir (const ASYS_TCHAR *key, ASYS_TCHAR *&wdir)
{
  Repository_Record *rec;
  int retval = this->repository_.find (key, rec);

  if (retval == 0)
    {
      ACE_NEW_RETURN (wdir, ASYS_TCHAR [ACE_OS::strlen (rec->wdir) + 1], -1);
      ACE_OS::strcpy (wdir, rec->wdir);
    }

  return retval;
}

int
Repository::get_hostport (const ASYS_TCHAR *key, ASYS_TCHAR *&host, unsigned short &port)
{
  Repository_Record *rec;
  int retval = this->repository_.find (key, rec);

  if (retval == 0)
    {
      host = rec->host;
      port = rec->port;
    }

  return retval;
}

int
Repository::get_ping_ior (const ASYS_TCHAR *key, ASYS_TCHAR *&ping_ior)
{
  Repository_Record *rec;
  int retval = this->repository_.find (key, rec);

  if (retval == 0)
    {
      ACE_NEW_RETURN (ping_ior, ASYS_TCHAR [ACE_OS::strlen (rec->ping_ior) + 1], -1);
      ACE_OS::strcpy (ping_ior, rec->ping_ior);
    }

  return retval;
}

void
Repository::dump (void)
{
  this->repository_.dump ();
}
