/* -*- C++ -*- */
// $Id$
#include "Repository.h"
#include "Options.h"
#include "ace/ACE.h"

ACE_RCSID(ImplRepo_Service, Repository, "$Id$")

Repository_Record::Repository_Record ()
: comm_line (ACE::strnew ("")),
  env (ACE::strnew ("")),
  wdir (ACE::strnew ("")),
  host (ACE::strnew ("")),
  port (0),
  ping_ior (ACE::strnew (""))
{
  // Nothing
}

Repository_Record::Repository_Record (const Repository_Record &r)
: comm_line (ACE::strnew (r.comm_line)),
  env (ACE::strnew (r.env)),
  wdir (ACE::strnew (r.wdir)),
  host (ACE::strnew (r.host)),
  port (r.port),
  ping_ior (ACE::strnew (r.ping_ior))
{
  // Nothing
}

Repository_Record::Repository_Record (const ASYS_TCHAR *c,
                                      const ASYS_TCHAR *e,
                                      const ASYS_TCHAR *w,
                                      const ASYS_TCHAR *h,
                                      const unsigned short p,
                                      const ASYS_TCHAR *pi)
: comm_line (ACE::strnew (c)),
  env (ACE::strnew (e)),
  wdir (ACE::strnew (w)),
  host (ACE::strnew (h)),
  port (p),
  ping_ior (ACE::strnew (pi))
{
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

  this->comm_line = ACE::strnew (r.comm_line);
  this->env = ACE::strnew (r.env);
  this->wdir = ACE::strnew (r.wdir);
  this->host = ACE::strnew (r.host);
  this->port = r.port;
  this->ping_ior = ACE::strnew (r.ping_ior);
  
  return *this;
}

// Default Constructor
Repository::Repository ()
{
  // Nothing
}

// Add a new server to the Repository
int
Repository::add (ACE_TString key, const Repository_Record &rec)
{
  if (OPTIONS::instance ()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Repository: Adding key %s\n", key.c_str ()));

  Repository_Record *new_rec = new Repository_Record (rec);

  // Store the record in the repository.
  return this->repository_.bind (key, new_rec);
}

int 
Repository::update (ACE_TString key, const Repository_Record &rec)
{
  if (OPTIONS::instance ()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Repository: Updating key %s\n", key.c_str ()));

  Repository_Record *new_rec = new Repository_Record (rec);
  Repository_Record *old_rec;

  // Store the record in the repository.
  int retval = this->repository_.rebind (key, new_rec, old_rec);
  
  if (retval == 1)
    delete old_rec;

  return retval >= 0 ? 0 : -1;
}

// Removes the server from the Repository
int
Repository::remove (ACE_TString key)
{
  if (OPTIONS::instance ()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Repository: Removing key %s\n", key.c_str ()));
  return this->repository_.unbind (key.c_str ());
}


// Find the key record in the Repository
int
Repository::resolve (ACE_TString key, Repository_Record &rec)
{
  if (OPTIONS::instance ()->debug () >= 2)
    ACE_DEBUG ((LM_DEBUG, "Repository: Resolving key %s\n", key.c_str ()));

  Repository_Record *rep_rec;
  int retval = this->repository_.find (key, rep_rec);

  if (retval == 0)
    rec = *rep_rec;

  return retval;
}
// = Accessor methods

int
Repository::get_comm_line (ACE_TString key, ASYS_TCHAR *&comm_line)
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
Repository::get_env (ACE_TString key, ASYS_TCHAR *&env)
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
Repository::get_wdir (ACE_TString key, ASYS_TCHAR *&wdir)
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
Repository::get_hostport (ACE_TString key, ASYS_TCHAR *&host, unsigned short &port)
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
Repository::get_ping_ior (ACE_TString key, ASYS_TCHAR *&ping_ior)
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
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("Total size: %d\n"), this->repository_.total_size ()));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("Current size: %d\n"), this->repository_.current_size ()));
  HASH_IR_ENTRY *entry;
  size_t i = 0;

  for (HASH_IR_ITER hash_iter (this->repository_);
       hash_iter.next (entry) != 0;
       hash_iter.advance ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("Record %d has server name \"%s\"\n"),
                  i,
                  ((ACE_TString) entry->ext_id_).c_str ()));
      i++;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<ACE_TString, Repository_Record *>;
template class ACE_Hash_Map_Manager_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<ACE_TString, Repository_Record *>;
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

