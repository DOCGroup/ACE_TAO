// DLL.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/DLL.h"

ACE_RCSID(ace, DLL, "$Id$")
  
// Implementation of the methods which provide an abstraction of
// dynamically linked library.

// If the library name and the opening mode are specified than on object
// creation the library is implicitly opened.
  
ACE_DLL::ACE_DLL (ACE_DL_TYPE dll_name,
                  int mode)
  :close_mode_ (0)
{
  // If the library name is not given dont open the file.
  if (dll_name !=0)
   {
     this->handle_ = ACE_OS::dlopen (dll_name, mode);
     // The ACE_SHLIB_HANDLE object is obtained.
     if (this->handle_ == 0)
       ACE_ERROR ((LM_ERROR,
                   "%s\n", this->error ()));
   }
}

// The library is closed before the class gets destroyed depending on
// the close_on_destruction value specified which is stored in
// close_mode_.

ACE_DLL::~ACE_DLL (void)
{
  // CLose the library only if it hasnt been already.
  if (this->close_mode_ == 0 && this->handle_ != ACE_SHLIB_INVALID_HANDLE)
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
  // This check is necessary as the library could be opened
  // more than once without closing it which would cause handle
  // memory leaks.
  if (this->handle_ != ACE_SHLIB_INVALID_HANDLE)
    this->close ();

  // The ACE_SHLIB_HANDLE object is obtained.
  this->handle_ = ACE_OS::dlopen (dll_name, mode);

  if (this->handle_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s\n", this->error ()),
                      -1);
  return 0;
}

// The symbol refernce of the name specified is obtained. 

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
  // already.  If not, it is closed and the handle is made invalid which
  // portrays that it is closed.
  if (this->handle_ != ACE_SHLIB_INVALID_HANDLE)
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

// Return the handle to the user either temporarily or forever,
// thus orphaning it. If 0 means the user wants the handle forever
// and if 1 means the user temporarily wants to take the handle.

ACE_SHLIB_HANDLE
ACE_DLL::get_handle (int orphan_mode)
{
  // Since the handle is getting orphaned we lose rights to close
  // it on destruction. The new controller has to do it explicitly.

  if (orphan_mode == 0)
    this->close_mode_ = 1;

  // Return the handle as requested by the user.
  return this->handle_;
}
