// DLL_Manager.cpp
// $Id$

#include "ace/DLL_Manager.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/Framework_Component.h"

#include "ace/OS.h"
#include "ace/Lib_Find.h"
#include "ace/Object_Manager.h"
#include "ace/SString.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Guard_T.h"

ACE_RCSID (ace,
	   DLL_Manager,
	   "$Id$")

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
  delete[] this->dll_name_;
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
                           ACE_LIB_TEXT ("ACE_DLL_Handle::open: error, ")
                           ACE_LIB_TEXT ("tried to reopen %s with name %s\n"),
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
          if (ACE::debug ())
            ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("ACE_DLL_Handle::open: calling dlopen on ")
                        ACE_LIB_TEXT ("\"%s\"\n"), dll_name));
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
                                 ACE_LIB_TEXT ("ACE_DLL_Handle::open: Invalid handle when opening DLL %s: %s\n"),
                                 this->dll_name_,
                                 this->error ()->c_str ()),
                                -1);
            }
        }
    }
  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("ACE_DLL_Handle::open: loading %s (%d)\n"),
                this->dll_name_,
                this->handle_));

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
      if (ACE::debug ())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT ("ACE_DLL_Handle::close: unloading %s (%d)\n"),
                    this->dll_name_,
                    this->handle_));
      // First remove any associated Framework Components.
      ACE_Framework_Repository * frPtr= ACE_Framework_Repository::instance ();

      if (frPtr)
      {
         frPtr->remove_dll_components (this->dll_name_);
      }

      retval = ACE_OS::dlclose (this->handle_);
      this->handle_ = ACE_SHLIB_INVALID_HANDLE;
    }

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Handle::close error: \"%s\".\n"),
                       this->error ()->c_str ()),
                      retval);
  return retval;
}

sig_atomic_t
ACE_DLL_Handle::refcount (void) const
{
  return this->refcount_;
}

void *
ACE_DLL_Handle::symbol (const ACE_TCHAR *sym_name, int ignore_errors)
{
  ACE_TRACE ("ACE_DLL_Handle::symbol");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_Auto_Array_Ptr <ACE_TCHAR> auto_name (ACE_Lib_Find::ldname (sym_name));

  void *sym =  ACE_OS::dlsym (this->handle_, auto_name.get ());

  // Linux says that the symbol could be null and that it isn't an error.
  // So you should check the error message also, but since null symbols
  // won't do us much good anyway, let's still report an error.
  if (!sym && ignore_errors != 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Handle::symbol (\"%s\") \"%s\".\n"),
                       auto_name.get (), this->error ()->c_str ()),
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
                       ACE_LIB_TEXT ("ACE_DLL_Handle::get_handle: ")
                       ACE_LIB_TEXT ("cannot become owner, refcount == 0.\n")),
                      ACE_SHLIB_INVALID_HANDLE);

  handle = this->handle_;

  if (become_owner != 0)
    {
      if (--this->refcount_ == 0)
        this->handle_ = ACE_SHLIB_INVALID_HANDLE;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("ACE_DLL_Handle::get_handle: ")
              ACE_LIB_TEXT ("post call: handle %s, refcount %d\n"),
              this->handle_ == ACE_SHLIB_INVALID_HANDLE ?
                ACE_LIB_TEXT ("invalid") : ACE_LIB_TEXT ("valid"),
              this->refcount_));

  return handle;
}

// This method is used return the last error of a library operation.

auto_ptr <ACE_TString>
ACE_DLL_Handle::error (void)
{
  ACE_TRACE ("ACE_DLL_Handle::error");
  const ACE_TCHAR *error = ACE_OS::dlerror ();
  auto_ptr<ACE_TString> str
    (new ACE_TString (error ? error : ACE_LIB_TEXT ("no error")));
  return str;
}

/******************************************************************/

// Pointer to the Singleton instance.
ACE_DLL_Manager *ACE_DLL_Manager::instance_ = 0;


ACE_DLL_Manager *
ACE_DLL_Manager::instance (int size)
{
  ACE_TRACE ("ACE_DLL_Manager::instance");

  if (ACE_DLL_Manager::instance_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (ACE_DLL_Manager::instance_ == 0)
        {
          ACE_NEW_RETURN (ACE_DLL_Manager::instance_,
                          ACE_DLL_Manager (size),
                          0);
        }
    }

  return ACE_DLL_Manager::instance_;
}

void
ACE_DLL_Manager::close_singleton (void)
{
  ACE_TRACE ("ACE_DLL_Manager::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  delete ACE_DLL_Manager::instance_;
  ACE_DLL_Manager::instance_ = 0;
}

ACE_DLL_Manager::ACE_DLL_Manager (int size)
  : handle_vector_ (0),
    current_size_ (0),
    total_size_ (0),
    unload_policy_ (ACE_DLL_UNLOAD_POLICY_PER_DLL)
{
  ACE_TRACE ("ACE_DLL_Manager::ACE_DLL_Manager");

  if (this->open (size) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_DLL_Manager ctor failed to allocate ")
                ACE_LIB_TEXT ("handle_vector_.\n")));
}

ACE_DLL_Manager::~ACE_DLL_Manager (void)
{
  ACE_TRACE ("ACE_DLL_Manager::~ACE_DLL_Manager");

  if (this->close () != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_DLL_Manager dtor failed to close ")
                ACE_LIB_TEXT ("properly.\n")));
}

ACE_DLL_Handle *
ACE_DLL_Manager::open_dll (const ACE_TCHAR *dll_name,
                           int open_mode,
                           ACE_SHLIB_HANDLE handle)
{
  ACE_TRACE ("ACE_DLL_Manager::open_dll");
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
                             ACE_LIB_TEXT ("ACE_DLL_Manager::open_dll: Could not ")
                             ACE_LIB_TEXT ("open dll %s.\n"),
                             dll_name),
                            0);
        }
    }
  return dll_handle;
}

int
ACE_DLL_Manager::close_dll (const ACE_TCHAR *dll_name)
{
  ACE_TRACE ("ACE_DLL_Manager::close_dll");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_DLL_Handle *handle = this->find_dll (dll_name);
  if (handle)
    {
      return this->unload_dll (handle, 0);
    }

  return -1;
}

u_long
ACE_DLL_Manager::unload_policy (void) const
{
  ACE_TRACE ("ACE_DLL_Manager::unload_policy");
  return this->unload_policy_;
}

void
ACE_DLL_Manager::unload_policy (u_long unload_policy)
{
  ACE_TRACE ("ACE_DLL_Manager::unload_policy");
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->lock_));

  u_long old_policy = this->unload_policy_;
  this->unload_policy_ = unload_policy;

  // If going from LAZY to EAGER or from PER_DLL to PER_PROCESS|EAGER,
  // call close(1) on all the ACE_DLL_Handle objects with refcount == 0
  // which will force those that are still loaded to be unloaded.
  if (this->handle_vector_)
    if (( ACE_BIT_ENABLED (old_policy, ACE_DLL_UNLOAD_POLICY_LAZY) &&
          ACE_BIT_DISABLED (this->unload_policy_, ACE_DLL_UNLOAD_POLICY_LAZY) ) ||
        ( ACE_BIT_DISABLED (this->unload_policy_, ACE_DLL_UNLOAD_POLICY_LAZY) &&
          ACE_BIT_ENABLED (old_policy, ACE_DLL_UNLOAD_POLICY_PER_DLL) &&
          ACE_BIT_DISABLED (this->unload_policy_, ACE_DLL_UNLOAD_POLICY_PER_DLL) ))
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
ACE_DLL_Manager::open (int size)
{
  ACE_TRACE ("ACE_DLL_Manager::open");

  ACE_DLL_Handle **temp;

  ACE_NEW_RETURN (temp,
                  ACE_DLL_Handle *[size],
                  -1);

  this->handle_vector_ = temp;
  this->total_size_ = size;
  return 0;
}

int
ACE_DLL_Manager::close (void)
{
  ACE_TRACE ("ACE_DLL_Manager::close");

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
ACE_DLL_Manager::find_dll (const ACE_TCHAR *dll_name) const
{
  ACE_TRACE ("ACE_DLL_Manager::find_dll");

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
ACE_DLL_Manager::unload_dll (ACE_DLL_Handle *dll_handle, int force_unload)
{
  ACE_TRACE ("ACE_DLL_Manager::unload_dll");

  if (dll_handle)
    {
      int unload = force_unload;
      if (unload == 0)
        {
          // apply strategy
          if (ACE_BIT_DISABLED (this->unload_policy_,
                                ACE_DLL_UNLOAD_POLICY_PER_DLL))
            {
              unload = ACE_BIT_DISABLED (this->unload_policy_,
                                         ACE_DLL_UNLOAD_POLICY_LAZY);
            }
          else
            {
              // Declare the type of the symbol:
              typedef int (*dll_unload_policy)(void);
              dll_unload_policy the_policy = 0;
              void *unload_policy_ptr =
                dll_handle->symbol (ACE_TEXT ("_get_dll_unload_policy"), 1);
              ptrdiff_t temp_p =
                ACE_reinterpret_cast (ptrdiff_t, unload_policy_ptr);
              the_policy =
                ACE_reinterpret_cast (dll_unload_policy, temp_p);
              if (the_policy != 0)
                unload = ACE_BIT_DISABLED (the_policy (),
                                           ACE_DLL_UNLOAD_POLICY_LAZY);
              else
                unload = ACE_BIT_DISABLED (this->unload_policy_,
                                           ACE_DLL_UNLOAD_POLICY_LAZY);
            }
        }

      if (dll_handle->close (unload) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("ACE_DLL_Manager::unload error.\n")),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("ACE_DLL_Manager::unload_dll called with ")
                       ACE_LIB_TEXT ("null pointer.\n")),
                      -1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<ACE_TString>;
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
template class ACE_Auto_Basic_Ptr<ACE_TString>;
#  endif  /* ACE_LACKS_AUTO_PTR */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<ACE_TString>
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
#    pragma instantiate ACE_Auto_Basic_Ptr<ACE_TString>
#  endif  /* ACE_LACKS_AUTO_PTR */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
