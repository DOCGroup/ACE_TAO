// DLL.cpp
// $Id$

#include "ace/DLL.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/DLL_Manager.h"

ACE_RCSID(ace, DLL, "$Id$")

// Default constructor. Also, by default, the object will be closed
// before it is destroyed.

ACE_DLL::ACE_DLL (int close_on_destruction)
  : open_mode_ (0),
    dll_name_ (0),
    close_on_destruction_ (close_on_destruction),
    dll_handle_ (0),
    error_ (0)
{
  ACE_TRACE ("ACE_DLL::ACE_DLL (int)");
}

ACE_DLL::ACE_DLL (const ACE_DLL &rhs)
{
  ACE_TRACE ("ACE_DLL::ACE_DLL (const ACE_DLL &)");

  // Have to do this since open() calls close()...
  this->dll_handle_ = 0;
  this->dll_name_ = 0;
  this->close_on_destruction_ = 1;
  this->error_ = 0;

  if (rhs.dll_name_)
    // This will automatically up the refcount.
    if (this->open (rhs.dll_name_, rhs.open_mode_, this->close_on_destruction_) != 0)
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("ACE_DLL::copy_ctor: error: %s\n"),
                  this->error ()));
}

// If the library name and the opening mode are specified than on
// object creation the library is implicitly opened.

ACE_DLL::ACE_DLL (const ACE_TCHAR *dll_name,
                  int open_mode,
                  int close_on_destruction)
  : open_mode_ (open_mode),
    dll_name_ (0),
    close_on_destruction_ (close_on_destruction),
    dll_handle_ (0),
    error_ (0)
{
  ACE_TRACE ("ACE_DLL::ACE_DLL");

  if (this->open (dll_name, this->open_mode_, close_on_destruction) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_DLL::open: error calling open: %s\n"),
                this->error ()));
}

// The library is closed before the class gets destroyed depending on
// the close_on_destruction value specified which is stored in
// close_on_destruction_.

ACE_DLL::~ACE_DLL (void)
{
  ACE_TRACE ("ACE_DLL::~ACE_DLL");

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
ACE_DLL::open (const ACE_TCHAR *dll_filename,
               int open_mode,
               int close_on_destruction)
{
  ACE_TRACE ("ACE_DLL::open");

  return open_i (dll_filename, open_mode, close_on_destruction);
}

int
ACE_DLL::open_i (const ACE_TCHAR *dll_filename,
                 int open_mode,
                 int close_on_destruction,
                 ACE_SHLIB_HANDLE handle)
{
  ACE_TRACE ("ACE_DLL::open_i");

  this->error_ = 0;

  if (!dll_filename)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL::open_i: dll_name: %s\n"),
                       this->dll_name_),
                      -1);

  if (this->dll_handle_)
    {
      // If we have a good handle and its the same name, just return.
      if (ACE_OS_String::strcmp (this->dll_name_, dll_filename) == 0)
        return 0;
      else
        this->close ();
    }
  if (!this->dll_name_)
    {
      this->dll_name_ = ACE::strnew (dll_filename);
    }
  this->open_mode_ = open_mode;
  this->close_on_destruction_ = close_on_destruction;

  this->dll_handle_ = ACE_DLL_Manager::instance()->open_dll (this->dll_name_,
                                                             this->open_mode_,
                                                             handle);

  if (!this->dll_handle_)
    this->error_ = 1;

  return this->error_ ? -1 : 0;
}

// The symbol refernce of the name specified is obtained.

void *
ACE_DLL::symbol (const ACE_TCHAR *sym_name, int ignore_errors)
{
  ACE_TRACE ("ACE_DLL::symbol");

  this->error_ = 0;

  void *sym = this->dll_handle_->symbol (sym_name, ignore_errors);

  if (!sym)
    this->error_ = 1;

  return sym;
}

// The library is closed using the ACE_SHLIB_HANDLE obejct.  i.e. The
// shared object is now disassociated form the current process.

int
ACE_DLL::close (void)
{
  ACE_TRACE ("ACE_DLL::close");
  int retval = 0;

  if (this->close_on_destruction_ && this->dll_name_ &&
      (retval = ACE_DLL_Manager::instance ()->close_dll (this->dll_name_)) != 0)
    this->error_ = 1;

  // Even if close_dll() failed, go ahead and cleanup.
  this->dll_handle_ = 0;
  delete[] this->dll_name_;
  this->dll_name_ = 0;
  this->close_on_destruction_ = 0;

  return retval;
}

// This method is used return the last error of a library operation.

ACE_TCHAR *
ACE_DLL::error (void) const
{
  ACE_TRACE ("ACE_DLL::error");
  if (this->error_)
    return ACE_const_cast (ACE_TCHAR *,
                           ACE_LIB_TEXT ("Error:  check log for details."));

  return 0;
}

// Return the handle to the user either temporarily or forever, thus
// orphaning it. If 0 means the user wants the handle forever and if 1
// means the user temporarily wants to take the handle.

ACE_SHLIB_HANDLE
ACE_DLL::get_handle (int become_owner) const
{
  ACE_TRACE ("ACE_DLL::get_handle");

  ACE_SHLIB_HANDLE handle = ACE_SHLIB_INVALID_HANDLE;

  if (this->dll_handle_)
    handle = this->dll_handle_->get_handle (become_owner);

  return handle;
}

// Set the handle for the DLL. By default, the object will be closed
// before it is destroyed.

int
ACE_DLL::set_handle (ACE_SHLIB_HANDLE handle,
                     int close_on_destruction)
{
  ACE_TRACE ("ACE_DLL::set_handle");

  // Create a unique name.  Note that this name is only quaranteed
  // to be unique for the life of this object.
  ACE_TCHAR temp[ACE_UNIQUE_NAME_LEN];
  ACE_OS::unique_name (this, temp, ACE_UNIQUE_NAME_LEN);

  return this->open_i (temp, 1, close_on_destruction, handle);
}
