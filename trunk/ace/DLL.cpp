// DLL.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/DLL.h"

ACE_RCSID(ace, DLL, "$Id$")
  
// Implementation of the methods which provide an abstraction of
// dynamically linked library.

// The close_on_destruction flag feature is added on object constuction.

ACE_DLL::ACE_DLL (int close_on_destruction)
{
  // This flag closes the library automagically on destruction of the
  // object.  The default value is zero.

  // @@ Kirthika, please move this into the "base/member"
  // initialization section...
  this->close_mode_ = close_on_destruction;
}
  
// The library is closed before the class gets destroyed depending on
// the close_on_destruction value specified which is stored in
// close_mode_.

ACE_DLL::~ACE_DLL (void)
{
  // CLose the library only if it hasnt been already.
  if (this->close_mode_ == 0 && this->handle_ != 0)
    this->close ();
}
  
// This method opens the library based on the mode specified using the
// ACE_SHLIB_HANDLE which is obtained on making the ACE_OS::dlopen call.
// The default mode is:
// RTLD_LAZY     Only references to data symbols are relocate when the 
//               object is first loaded.
// The other modes include:
//  RTLD_NOW     All necessary relocations are performed when the
//               object is first loaded.
//  RTLD_GLOBAL  The object symbols are made available for the
//               relocation processing of any other object. 

int 
ACE_DLL::open (ACE_DL_TYPE dll_name,
               int mode)
{
  // The ACE_SHLIB_HANDLE object is obtained.
  this->handle_ = ACE_OS::dlopen (dll_name, mode);

  if (this->handle_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s\n", this->error ()),
                      -1);
  return 0;
}

// The symbol refernce of teh name specified is obtained. 

void *
ACE_DLL::symbol (ACE_DL_TYPE sym_name)
{
  return ACE_OS::dlsym (this->handle_, sym_name);
}

// The library is closed using the ACE_SHLIB_HANDLE obejct.  i.e. The
// shared object is now disassociated form the current process.

int 
ACE_DLL::close (void)
{
  // The handle is checked to see whether the library is closed
  // already.  If not, it is closed an dthe handle is made 0 which
  // portrays that it is closed.
  if (this->handle_ != 0)
    {
      int retval = ACE_OS::dlclose (this->handle_);
      this->handle_ = 0;
      return retval;
    }
  else
    return 0;
}

// This method is used on error in an library operation.

char *
ACE_DLL::error (void)
{
  return ACE_OS::dlerror();
} 
