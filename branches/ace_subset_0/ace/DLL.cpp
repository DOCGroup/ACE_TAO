// DLL.cpp
// $Id$

#include "ace/DLL.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"

ACE_RCSID(ace, DLL, "$Id$")

// Default constructor. Also, by default, the object will be closed
// before it is destroyed.

ACE_DLL::ACE_DLL (int close_on_destruction)
  : handle_ (ACE_SHLIB_INVALID_HANDLE),
    close_on_destruction_ (close_on_destruction)
{
}

// If the library name and the opening mode are specified than on
// object creation the library is implicitly opened.

ACE_DLL::ACE_DLL (const ACE_TCHAR *dll_name,
                  int open_mode,
                  int close_on_destruction)
  : handle_ (ACE_OS::dlopen (dll_name,
                             open_mode)),
    close_on_destruction_ (close_on_destruction)
{
  if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%s\n"),
                this->error ()));
}

// The library is closed before the class gets destroyed depending on
// the close_on_destruction value specified which is stored in
// close_on_destruction_.

ACE_DLL::~ACE_DLL (void)
{
  // CLose the library only if it hasn't been already.
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
  // This check is necessary as the library could be opened more than
  // once without closing it which would cause handle memory leaks.
  this->close ();

  // Reset the flag
  this->close_on_destruction_ = close_on_destruction;

  // Find out where the library is
  ACE_TCHAR dll_pathname[MAXPATHLEN + 1];

  // Transform the pathname into the appropriate dynamic link library
  // by searching the ACE_LD_SEARCH_PATH.
  int result = ACE_Lib_Find::ldfind (dll_filename,
                                     dll_pathname,
                                     (sizeof dll_pathname / sizeof (ACE_TCHAR)));
  // Check for errors
  if (result != 0)
    return result;

  // The ACE_SHLIB_HANDLE object is obtained.
  this->handle_ = ACE_OS::dlopen (dll_pathname,
                                  open_mode);

  if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
    {
#if defined (AIX)
      do
        {
          // AIX often puts the shared library file (most often named shr.o)
          // inside an archive library. If this is an archive library
          // name, then try appending [shr.o] and retry.
          if (0 != ACE_OS_String::strstr (dll_pathname, ACE_LIB_TEXT (".a")))
            {
              ACE_OS_String::strcat (dll_pathname, ACE_LIB_TEXT ("(shr.o)"));
              open_mode |= RTLD_MEMBER;
              this->handle_ = ACE_OS::dlopen (dll_pathname, open_mode);
              if (this->handle_ != ACE_SHLIB_INVALID_HANDLE)
                break;  // end up returning 0
            }
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%s\n"), this->error ()),
                            -1);
        }
      while (0);
#else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%s\n"), this->error ()),
                        -1);
#endif /* AIX */
    }

  return 0;
}

// The symbol refernce of the name specified is obtained.

void *
ACE_DLL::symbol (const ACE_TCHAR *sym_name)
{
  return ACE_OS::dlsym (this->handle_, sym_name);
}

// The library is closed using the ACE_SHLIB_HANDLE obejct.  i.e. The
// shared object is now disassociated form the current process.

int
ACE_DLL::close (void)
{
  int retval = 0;

  // The handle is checked to see whether the library is closed
  // already and the <close_on_destruction_> flag is specified.  If
  // not, it is closed and the handle is made invalid to indicate that
  // it's now closed.
  if (this->close_on_destruction_ != 0 &&
      this->handle_ != ACE_SHLIB_INVALID_HANDLE)
    {
      retval = ACE_OS::dlclose (this->handle_);
    }

  this->handle_ = ACE_SHLIB_INVALID_HANDLE;
  return retval;
}

// This method is used on error in an library operation.

ACE_TCHAR *
ACE_DLL::error (void)
{
  return ACE_OS::dlerror ();
}

// Return the handle to the user either temporarily or forever, thus
// orphaning it. If 0 means the user wants the handle forever and if 1
// means the user temporarily wants to take the handle.

ACE_SHLIB_HANDLE
ACE_DLL::get_handle (int become_owner)
{
  // Since the caller is becoming the owner of the handle we lose
  // rights to close it on destruction.  The new controller has to do
  // it explicitly.
  if (become_owner)
    this->close_on_destruction_ = 0;

  // Return the handle requested by the user.
  return this->handle_;
}

// Set the handle for the DLL. By default, the object will be closed
// before it is destroyed.

int
ACE_DLL::set_handle (ACE_SHLIB_HANDLE handle,
                     int close_on_destruction)
{
  // Close the handle in use before accepting the next one.
  if (this->close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%s\n"), this->error ()),
                      -1);

  this->handle_ = handle;
  this->close_on_destruction_ = close_on_destruction;

  return 0;
}
