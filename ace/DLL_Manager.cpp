// DLL_Manager.cpp
// $Id$

#include "ace/DLL_Manager.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/Framework_Component.h"

#include "ace/OS.h"
#include "ace/Lib_Find.h"

ACE_RCSID(ace, DLL, "$Id$")

// Uncomment these lines to turn tracing on for this file.
//#undef ACE_TRACE
//#define ACE_TRACE(X) ACE_TRACE_IMPL(X)

/******************************************************************/

sig_atomic_t ACE_DLL_Handle::open_called_ = 0;

ACE_DLL_Handle::ACE_DLL_Handle (void)
  : refcount_ (0),
    dll_name_ (0),
    handle_ (ACE_SHLIB_INVALID_HANDLE)
{
  ACE_TRACE ("ACE_DLL_Handle::ACE_DLL_Handle");
}

ACE_DLL_Handle::~ACE_DLL_Handle (void)
{
  ACE_TRACE ("ACE_DLL_Handle::~ACE_DLL_Handle");
  this->close (1);
  delete this->dll_name_;
}

const ACE_TCHAR  *
ACE_DLL_Handle::dll_name (void) const
{
  ACE_TRACE ("ACE_DLL_Handle::dll_name");
  return this->dll_name_;
}

int
ACE_DLL_Handle::open (const ACE_TCHAR *dll_name, 
                      int open_mode,
                      ACE_SHLIB_HANDLE handle)
{
  ACE_TRACE ("ACE_DLL_Handle::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  //ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("dll_name: %s; open_mode: %d \n"),
  //            dll_name,
  //            open_mode));

  if (this->dll_name_) 
    {
      // Once dll_name_ has been set, it can't be changed..
      if (ACE_OS_String::strcmp (this->dll_name_, dll_name) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           ACE_LIB_TEXT ("ACE_DLL_Handle::open: error, "
                                         "tried to reopen %s with name %s\n"),
                           this->dll_name_, dll_name),
                          -1);
    }
  else
    this->dll_name_ = ACE::strnew (dll_name);  

  if (!this->open_called_)
    this->open_called_ = 1;

  // If it hasn't been loaded yet, go ahead and do that now.
  if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
    {
      if (handle)
        this->handle_ = handle;
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("ACE_DLL_Handle::open: calling dlopen on "
                                              "\"%s\"\n"), dll_name));
          // Find out where the library is
          ACE_TCHAR dll_pathname[MAXPATHLEN + 1];

          // Transform the pathname into the appropriate dynamic link library
          // by searching the ACE_LD_SEARCH_PATH.
          ACE_Lib_Find::ldfind (dll_name,
                                dll_pathname,
                                (sizeof dll_pathname / sizeof (ACE_TCHAR)));
          
          // The ACE_SHLIB_HANDLE object is obtained.
          this->handle_ = ACE_OS::dlopen (dll_pathname,
                                          open_mode);

#if defined (AIX)
          if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
            {
              // AIX often puts the shared library file (most often named shr.o)
              // inside an archive library. If this is an archive library
              // name, then try appending [shr.o] and retry.
              if (0 != ACE_OS_String::strstr (dll_pathname, ACE_LIB_TEXT (".a")))
                {
                  ACE_OS_String::strcat (dll_pathname, ACE_LIB_TEXT ("(shr.o)"));
                  open_mode |= RTLD_MEMBER;
                  this->handle_ = ACE_OS::dlopen (dll_pathname, open_mode);
                }
            }        
#endif /* AIX */

          if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_LIB_TEXT ("%s\n"), this->error ()->c_str ()),
                                -1);
            }
        }
    }

  ++this->refcount_;
  return 0;
}

int
ACE_DLL_Handle::close (int unload)
{
  ACE_TRACE ("ACE_DLL_Handle::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  int retval = 0;

  // Since we don't actually unload the dll as soon as the refcount
  // reaches zero, we need to make sure we don't decrement it below
  // zero.
  if (this->refcount_ > 0)
    --this->refcount_;
  else
    this->refcount_ = 0;

  if (this->refcount_ == 0 && 
      this->handle_ != ACE_SHLIB_INVALID_HANDLE && 
      unload == 1)
    {
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_LIB_TEXT ("ACE_DLL_Handle::close: unloading %s\n"),
                  this->dll_name_));
      // First remove any associated Framework Components.
      ACE_Framework_Repository::instance ()->remove_dll_components (this->dll_name_);

      retval = ACE_OS::dlclose (this->handle_);
      this->handle_ = ACE_SHLIB_INVALID_HANDLE;
    }

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Handle::close error: \"%s\"."),
                       this->error ()->c_str ()),
                      retval);
  return retval;
}

const sig_atomic_t
ACE_DLL_Handle::refcount (void) const
{
  return this->refcount_;
}

void *
ACE_DLL_Handle::symbol (const ACE_TCHAR *sym_name)
{
  ACE_TRACE ("ACE_DLL_Handle::symbol");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  auto_ptr <ACE_TCHAR> auto_name (ACE_Lib_Find::ldname (sym_name));

  void *sym =  ACE_OS::dlsym (this->handle_, auto_name.get ());

  auto_ptr <ACE_TString> error = this->error ();

  // Linux says that the symbol could be null and that it isn't an error.
  // So you should check the error message also, but since null symbols
  // won't do us much good anyway, let's still report an error.
  if (!sym)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Handle::symbol error: \"%s\"."),
                       error->c_str ()),
                      0);

  return sym;
}

ACE_SHLIB_HANDLE 
ACE_DLL_Handle::get_handle (int become_owner)
{
  ACE_TRACE ("ACE_DLL_Handle::get_handle");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_SHLIB_HANDLE handle = ACE_SHLIB_INVALID_HANDLE;

  if (this->refcount_ == 0 && become_owner != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Handle::get_handle: "
                                     "cannot become owner, refcount == 0.\n")),
                      ACE_SHLIB_INVALID_HANDLE);
  else if (become_owner != 0)
    {
      handle = this->handle_;
      if (--this->refcount_ == 0)
        this->handle_ = ACE_SHLIB_INVALID_HANDLE;
    }

  ACE_DEBUG ((LM_DEBUG, 
              ACE_LIB_TEXT ("ACE_DLL_Handle::get_handle: "
                            "handle %s, refcount %d\n"),
              this->handle_ == ACE_SHLIB_INVALID_HANDLE ? "invalid" : "valid",
              this->refcount_));                        

  return handle;
}

// This method is used return the last error of a library operation.

auto_ptr <ACE_TString>
ACE_DLL_Handle::error (void) const
{
  ACE_TRACE ("ACE_DLL_Handle::error");

  ACE_TString *str = new ACE_TString (ACE_OS::dlerror ());
  return auto_ptr <ACE_TString> (str);
}

/******************************************************************/

ACE_DLL_Manager_Ex::ACE_DLL_Manager_Ex (int size)
  : handle_vector_ (0),
    current_size_ (0),
    total_size_ (0),
    unload_strategy_ (0)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::ACE_DLL_Manager_Ex");

  if (this->open (size) != 0)
    ACE_ERROR ((LM_ERROR, 
                ACE_LIB_TEXT ("ACE_DLL_Manager_Ex ctor failed to allocate "
                              "handle_vector_.\n")));
}

ACE_DLL_Manager_Ex::~ACE_DLL_Manager_Ex (void)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::~ACE_DLL_Manager_Ex");

  if (this->close () != 0)
    ACE_ERROR ((LM_ERROR, 
                ACE_LIB_TEXT ("ACE_DLL_Manager_Ex dtor failed to close "
                              "properly.\n")));
}

ACE_DLL_Handle *
ACE_DLL_Manager_Ex::open_dll (const ACE_TCHAR *dll_name, 
                              int open_mode,
                              ACE_SHLIB_HANDLE handle)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::open_dll");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_DLL_Handle *dll_handle = this->find_dll (dll_name);
  if (!dll_handle)
    {
      if (this->current_size_ < this->total_size_)
        {
          ACE_NEW_RETURN (dll_handle,
                          ACE_DLL_Handle,
                          0);
  
          this->handle_vector_[this->current_size_] = dll_handle;
          this->current_size_++;
        }
    }

  if (dll_handle)
    {
      if (dll_handle->open (dll_name, open_mode, handle) != 0)
        {
          // Don't worry about freeing the memory right now, since
          // the handle_vector_ will be cleaned up automatically later.
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("ACE_DLL_Manager::open_dll: Could not "
                                           "open dll.\n")),
                            0);          
        }
    }
  return dll_handle;
}

int
ACE_DLL_Manager_Ex::close_dll (const ACE_TCHAR *dll_name)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::close_dll");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_DLL_Handle *handle = this->find_dll (dll_name);
  if (handle)
    {
      return this->unload_dll (handle, 0);
    }

  return -1;
}

u_long
ACE_DLL_Manager_Ex::unload_strategy (void) const
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::unload_strategy");
  return this->unload_strategy_;
}

void
ACE_DLL_Manager_Ex::unload_strategy (u_long unload_strategy)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::unload_strategy");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_));

  u_long old_strategy = this->unload_strategy_;
  this->unload_strategy_ = unload_strategy;

  // If going from LAZY to EAGER, inform all the ACE_DLL_Handle 
  // objects with 
  if (this->handle_vector_ &&
      ACE_BIT_ENABLED (old_strategy, LAZY) &&
      ACE_BIT_DISABLED (unload_strategy, LAZY))
    {
      for (int i = this->current_size_ - 1; i >= 0; i--)
        {
          if (this->handle_vector_[i] &&
              this->handle_vector_[i]->refcount () == 0)
            this->handle_vector_[i]->close (1);
        }
    }
}

int
ACE_DLL_Manager_Ex::open (int size)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::open");

  ACE_DLL_Handle **temp;

  ACE_NEW_RETURN (temp,
                  ACE_DLL_Handle *[size],
                  -1);

  this->handle_vector_ = temp;
  this->total_size_ = size;
  return 0;
}

int
ACE_DLL_Manager_Ex::close (void)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::close");

  int force_close = 1;

  if (this->handle_vector_ != 0)
    {
      // Delete components in reverse order.
      for (int i = this->current_size_ - 1; i >= 0; i--)
        {
          if (this->handle_vector_[i])
            {
              ACE_DLL_Handle *s = ACE_const_cast (ACE_DLL_Handle *,
                                                  this->handle_vector_[i]);
              this->handle_vector_[i] = 0;
              this->unload_dll (s, force_close);
              delete s;
            }
        }

      delete [] this->handle_vector_;
      this->handle_vector_ = 0;
      this->current_size_ = 0;
    }
  return 0;
}

ACE_DLL_Handle *
ACE_DLL_Manager_Ex::find_dll (const ACE_TCHAR *dll_name) const
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::find_dll");

  int i;
  for (i = 0; i < this->current_size_; i++)
    if (this->handle_vector_[i] &&
        ACE_OS_String::strcmp (this->handle_vector_[i]->dll_name (), dll_name) == 0)
      {
        return this->handle_vector_[i];
      }

  return 0;
}

int 
ACE_DLL_Manager_Ex::unload_dll (ACE_DLL_Handle *dll_handle, int force_close)
{
  ACE_TRACE ("ACE_DLL_Manager_Ex::unload_dll");

  if (dll_handle)
    {
      int close = force_close;
      if (close == 0)
        {
          // apply strategy
          if (ACE_BIT_DISABLED (this->unload_strategy_, PER_DLL))
            {
              close = ACE_BIT_DISABLED (this->unload_strategy_, LAZY);
            }
          else
            {
              // need to get it from the dll, so just dummy it up for now
              close = ACE_BIT_DISABLED (this->unload_strategy_, LAZY);
            }
        }

      if (dll_handle->close (close) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           ACE_LIB_TEXT ("ACE_DLL_Manager_Ex::unload error.\n")),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Manager_Ex::unload_dll called with "
                                     "null pointer.\n")),
                      -1);

  return 0;
}
