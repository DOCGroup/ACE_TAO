// $Id$
#include "Repository.h"

// Default Constructor
Repository::Repository ()
{
  this->repository_.open (ACE_Naming_Context::PROC_LOCAL);
}

// Add a new server to the Repository
int 
Repository::add (const char *key, const char *comm_line, const char *env, const char *wdir)
{
  char *temp; // Temporary string to hold all the variables in.

  // Needs to be as long as all the lengths plus the 2 separators and the 
  // null character.
  ACE_NEW_RETURN (temp, char [ACE_OS::strlen (comm_line) + ACE_OS::strlen (env) 
                              + ACE_OS::strlen (wdir) + 10], -1);
  
  ACE_OS::sprintf (temp, "%s\n%s\n%s", comm_line, env, wdir);
  
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
Repository::resolve (const char *key, char *&comm_line, char *&env, char *&wdir)
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
      ACE_NEW_RETURN (comm_line, char [strlen (temp) + 1], -1);
    
      // Copy to the comm_line argument
      strcpy (comm_line, temp);
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
      ACE_NEW_RETURN (env, char [strlen (temp) + 1], -1);
    
      // Copy to the env argument
      strcpy (env, temp);
    }
    else
    {
      retval = -1;
      break;
    }

    temp = last + 1;
    if (last != 0)
    {
      ACE_NEW_RETURN (wdir, char [strlen (temp) + 1], -1);
    
      // Copy to the env argument
      strcpy (wdir, temp);
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
  char *value, *type;  // Temp variables needed for resolve
  char *last, *temp; // For fields
  int retval = this->repository_.resolve (key, value, type);
  
  // If successful, return what we need.
  if (retval == 0)
  {
    temp = value;
    // Null terminate the first field
    last = ACE_OS::strstr (value, "\n");
    if (last != 0 && *last == '\n')
    {
      *last = '\0';
      ACE_NEW_RETURN (comm_line, char [strlen (temp) + 1], -1);
    
      // Copy to the comm_line argument
      strcpy (comm_line, temp);

      delete [] value;
      delete [] type;
      return 0;
    }
    else
    {
      delete [] value;
      delete [] type;
      return -1;
    }
  }
  
  return retval;
}


int 
Repository::get_env (const char *key, char *&env)
{
  char *value, *type;  // Temp variables needed for resolve
  char *last, *temp; // For field position
  int retval = this->repository_.resolve (key, value, type);
  
  // If successful, return what we need.
  if (retval == 0)
  {
    last = ACE_OS::strstr (value, "\n");
    temp = last + 1;
    last = ACE_OS::strstr (temp, "\n");
    
    if (last != 0 && *last == '\n')
    {
      *last = '\0';
      ACE_NEW_RETURN (env, char [strlen (temp) + 1], -1);
    
      // Copy to the env argument
      strcpy (env, temp);
      delete [] value;
      delete [] type;
      return 0;
    }
    else
    {
      delete [] value;
      delete [] type;
      return -1;
    }
  }
  
  return retval;
}

int 
Repository::get_wdir (const char *key, char *&wdir)
{
  char *value, *type;  // Temp variables needed for resolve
  char *last, *temp ; // For fields
  int retval = this->repository_.resolve (key, value, type);
  
  // If successful, return what we need.
  if (retval == 0)
  {
    last = ACE_OS::strstr (value, "\n");
    last = ACE_OS::strstr (last + 1, "\n");
    temp = last + 1;

    ACE_NEW_RETURN (wdir, char [strlen (temp) + 1], -1);
    
    // Copy to the wdir argument
    strcpy (wdir, temp);
    delete [] value;
    delete [] type;
  }
  return retval;
}

void
Repository::dump (void)
{
  this->repository_.dump ();
}
