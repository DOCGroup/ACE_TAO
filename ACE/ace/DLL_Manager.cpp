#include "ace/DLL_Manager.h"

#include "ace/Log_Category.h"
#include "ace/ACE.h"
#include "ace/Framework_Component.h"

#include "ace/Lib_Find.h"
#include "ace/Object_Manager.h"
#include "ace/SString.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Guard_T.h"
#include "ace/OS_NS_dlfcn.h"
#include "ace/OS_NS_string.h"
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

sig_atomic_t ACE_DLL_Handle::open_called_ = 0;

ACE_DLL_Handle::ACE_DLL_Handle ()
  : refcount_ (0),
    dll_name_ (0),
    handle_ (ACE_SHLIB_INVALID_HANDLE)
{
  ACE_TRACE ("ACE_DLL_Handle::ACE_DLL_Handle");
}

ACE_DLL_Handle::~ACE_DLL_Handle ()
{
  ACE_TRACE ("ACE_DLL_Handle::~ACE_DLL_Handle");
  this->close (1);
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free(this->dll_name_);
#else
  delete[] this->dll_name_;
#endif /* ACE_HAS_ALLOC_HOOKS */
}

ACE_ALLOC_HOOK_DEFINE(ACE_DLL_Handle)

const ACE_TCHAR  *
ACE_DLL_Handle::dll_name () const
{
  ACE_TRACE ("ACE_DLL_Handle::dll_name");
  return this->dll_name_;
}

int
ACE_DLL_Handle::open (const ACE_TCHAR *dll_name,
                      int open_mode,
                      ACE_SHLIB_HANDLE handle,
                      ERROR_STACK *errors)
{
  ACE_TRACE ("ACE_DLL_Handle::open");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  if (this->dll_name_)
    {
      // Once dll_name_ has been set, it can't be changed..
      if (ACE_OS::strcmp (this->dll_name_, dll_name) != 0)
        {
          if (ACE::debug ())
            ACELIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE (%P|%t) DLL_Handle::open: error, ")
                        ACE_TEXT ("tried to reopen <%s> with name <%s>\n"),
                        this->dll_name_,
                        dll_name));

          return -1;
        }
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
          /*
          ** Get the set of names to try loading. We need to do this to
          ** properly support the ability for a user to specify a simple,
          ** unadorned name (for example, "ACE") that will work across
          ** platforms. We apply platform specifics to get a name that will
          ** work (e.g. libACE, ACEd.dll, ACE.dll, etc.) We rely on the
          ** underlying dlopen() implementation to "Do The Right Thing" in
          ** terms of using relative paths, LD_LIBRARY_PATH, system security
          ** rules, etc. except when ACE_MUST_HELP_DLOPEN_SEARCH_PATH is set.
          ** If it is set, then ACE::ldfind() scans the configured path
          ** looking for a match on the name and prefix/suffix applications.
          ** NOTE: having ACE scan for a file and then pass a fully-qualified
          ** pathname to dlopen() is a potential security hole; therefore,
          ** do not use ACE_MUST_HELP_DLOPEN_SEARCH_PATH unless necessary
          ** and only after considering the risks.
          */
          ACE_Array<ACE_TString> dll_names;
          dll_names.max_size (10);    // Decent guess to avoid realloc later

#if defined (ACE_MUST_HELP_DLOPEN_SEARCH_PATH)
          // Find out where the library is
          ACE_TCHAR dll_pathname[MAXPATHLEN + 1];

          // Transform the pathname into the appropriate dynamic link library
          // by searching the ACE_LD_SEARCH_PATH.
          ACE::ldfind (dll_name,
                       dll_pathname,
                       (sizeof dll_pathname / sizeof (ACE_TCHAR)));
          ACE_TString dll_str (dll_pathname);
          dll_names.size (1);
          dll_names.set (dll_str, 0);
#else
          this->get_dll_names (dll_name, dll_names);
#endif

          ACE_TString *name = 0;
          for (ACE_Array_Iterator<ACE_TString> name_iter (dll_names);
               name_iter.next (name); name_iter.advance ())
            {
              if (this->open_i (name->c_str (), open_mode, errors))
                break;
            }

          if (this->handle_ == ACE_SHLIB_INVALID_HANDLE)
            {
              if (ACE::debug ())
                {
                  ACE_TString errtmp;
                  this->error (errtmp);
                  ACELIB_ERROR ((LM_ERROR,
                                ACE_TEXT ("ACE (%P|%t) DLL_Handle::open (\"%s\"): ")
                                ACE_TEXT ("Invalid handle error: %s\n"),
                                this->dll_name_,
                                errtmp.c_str ()));
                }
              return -1;
            }
        }
    }

  ++this->refcount_;

  if (ACE::debug ())
    ACELIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ACE (%P|%t) DLL_Handle::open - <%s> (%d), refcount=%d\n"),
                this->dll_name_,
                this->handle_,
                this->refcount_));
  return 0;
}


int
ACE_DLL_Handle::close (int unload)
{
  ACE_TRACE ("ACE_DLL_Handle::close");

  int retval = 0;
  ACE_SHLIB_HANDLE h = ACE_SHLIB_INVALID_HANDLE;

  // Only hold the lock until it comes time to dlclose() the DLL. Closing
  // the DLL can cause further shutdowns as DLLs and their dependents are
  // unloaded.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

    // Since we don't actually unload the dll as soon as the refcount
    // reaches zero, we need to make sure we don't decrement it below
    // zero.
    if (this->refcount_ > 0)
      --this->refcount_;
    else
      this->refcount_ = 0;

    if (ACE::debug ())
      ACELIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ACE (%P|%t) DLL_Handle::close - ")
                  ACE_TEXT ("<%s> (handle=%d, refcount=%d)\n"),
                  this->dll_name_,
                  this->handle_,
                  this->refcount_));

    if (this->refcount_ == 0 &&
        this->handle_ != ACE_SHLIB_INVALID_HANDLE &&
        unload == 1)
      {
        if (ACE::debug ())
          ACELIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ACE (%P|%t) DLL_Handle::close: ")
                      ACE_TEXT ("Unloading <%s> (handle=%d)\n"),
                      this->dll_name_,
                      this->handle_));

        // First remove any associated Framework Components.
        ACE_Framework_Repository *frPtr= ACE_Framework_Repository::instance ();
        if (frPtr)
          {
            frPtr->remove_dll_components (this->dll_name_);
          }

        h = this->handle_;
        this->handle_ = ACE_SHLIB_INVALID_HANDLE;
      }
  } // Release lock_ here

  if (h != ACE_SHLIB_INVALID_HANDLE)
    {
      retval = ACE_OS::dlclose (h);

      if (retval != 0 && ACE::debug ())
        {
          ACE_TString err;
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE (%P|%t) DLL_Handle::close - ")
                      ACE_TEXT ("Failed with: <%s>\n"),
                      this->error (err).c_str ()));
        }
    }

  return retval;
}

sig_atomic_t
ACE_DLL_Handle::refcount () const
{
  return this->refcount_;
}

void *
ACE_DLL_Handle::symbol (const ACE_TCHAR *sym_name, bool ignore_errors)
{
  ACE_TString error;
  return this->symbol (sym_name, ignore_errors, error);
}

void *
ACE_DLL_Handle::symbol (const ACE_TCHAR *sym_name, bool ignore_errors, ACE_TString &error)
{
  ACE_TRACE ("ACE_DLL_Handle::symbol");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  std::unique_ptr <ACE_TCHAR[]> auto_name (ACE::ldname (sym_name));
  // handle_ can be invalid especially when ACE_DLL_Handle resigned ownership
  // BTW. Handle lifecycle management is a little crazy in ACE
  if (this->handle_ != ACE_SHLIB_INVALID_HANDLE)
    {
      void *sym =  ACE_OS::dlsym (this->handle_, auto_name.get ());

      // Linux says that the symbol could be null and that it isn't an
      // error.  So you should check the error message also, but since
      // null symbols won't do us much good anyway, let's still report
      // an error.
      if (!sym && !ignore_errors)
        {
          this->error (error);

          if (ACE::debug ())
            ACELIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE (%P|%t) DLL_Handle::symbol <%s>")
                        ACE_TEXT (" failed with <%s>\n"),
                        auto_name.get (),
                        error.c_str ()));

          return nullptr;
        }
      return sym;
    }
  return nullptr;
}

ACE_SHLIB_HANDLE
ACE_DLL_Handle::get_handle (bool become_owner)
{
  ACE_TRACE ("ACE_DLL_Handle::get_handle");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));

  if (this->refcount_ == 0 && become_owner)
    {
      if (ACE::debug ())
        ACELIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("ACE (%P|%t) DLL_Handle::get_handle: ")
                    ACE_TEXT ("cannot become owner, refcount == 0.\n")));

      return ACE_SHLIB_INVALID_HANDLE;
    }

  ACE_SHLIB_HANDLE handle = this->handle_;

  if (become_owner)
    {
      if (--this->refcount_ == 0)
        this->handle_ = ACE_SHLIB_INVALID_HANDLE;
    }

  if (ACE::debug ())
    ACELIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ACE (%P|%t) DLL_Handle::get_handle: ")
                ACE_TEXT ("post call: handle %s, refcount %d\n"),
                this->handle_ == ACE_SHLIB_INVALID_HANDLE ?
                ACE_TEXT ("invalid") : ACE_TEXT ("valid"),
                this->refcount_));

  return handle;
}

// This method is used return the last error of a library operation.
ACE_TString &
ACE_DLL_Handle::error (ACE_TString &err)
{
  ACE_TRACE ("ACE_DLL_Handle::error");
  const ACE_TCHAR *error = ACE_OS::dlerror ();
  if (error)
    {
      err = error;
    }
  return err;
}

void
ACE_DLL_Handle::get_dll_names (const ACE_TCHAR *dll_name,
                               ACE_Array<ACE_TString> &try_names)
{
  // Build the array of DLL names to try on this platform by applying the
  // proper prefixes and/or suffixes to the specified dll_name.
  ACE_TString base (dll_name);
  ACE_TString base_dir, base_file, base_suffix;

  // 1. Separate the dll_name into the dir part and the file part. We
  // only decorate the file part to determine the names to try loading.
  ACE_TString::size_type pos = base.rfind (ACE_DIRECTORY_SEPARATOR_CHAR);
  if (pos != ACE_TString::npos)
    {
      base_dir = base.substr (0, pos + 1);
      base_file = base.substr (pos + 1);
    }
  else
    base_file = base;

  // 2. Locate the file suffix, if there is one. Move the '.' and the
  // suffix to base_suffix.
  if ((pos = base_file.rfind (ACE_TEXT ('.'))) != ACE_TString::npos)
    {
      base_suffix = base_file.substr (pos);
      base_file = base_file.substr (0, pos);
    }

  // 3. Build the combinations to try for this platform.
  // Try these combinations:
  //   - name with platform's dll prefix (if it has one) and suffix
  //   - name with platform's dll prefix, decorator, and suffix.
  //   - name with decorator and platform's suffix appended (if not supplied)
  //   - name with platform's suffix appended (if not supplied)
  //   - name as originally given
  // We first try to find the file using the decorator so that when a
  // filename with and without decorator is used, we get the file with
  // the same decorator as the ACE dll has and then as last resort
  // the one without. For example with msvc, the debug build has a "d"
  // decorator, but the release build has none and we really want to get
  // the debug version of the library in a debug application instead
  // of the release one.
  // So we need room for 5 entries in try_names.
  try_names.size (0);
  if ((try_names.max_size () - try_names.size ()) < 5)
    try_names.max_size (try_names.max_size () + 5);
#if defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
  ACE_TString decorator (ACE_LD_DECORATOR_STR);
#endif
  ACE_TString suffix (ACE_DLL_SUFFIX);
  ACE_TString prefix (ACE_DLL_PREFIX);

  for (size_t i = 0; i < 5 && try_names.size () < try_names.max_size (); ++i)
    {
      ACE_TString try_this;
      size_t const j = try_names.size ();
      switch (i)
        {
        case 0:        // Prefix + name + decorator + suffix
        case 1:        // Prefix + name + suffix
        case 2:        // Name + decorator + suffix
        case 3:        // Name + suffix
          if (
              base_suffix.length () > 0
#if !(defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK))
              || (i == 1 || i == 3)    // No decorator desired; skip
#endif
              )
            break;
          try_this = base_dir;
          if (i < 2)
            try_this += prefix;
          try_this += base_file;
          if (base_suffix.length () > 0)
            try_this += base_suffix;
          else
            {
#if defined (ACE_LD_DECORATOR_STR) && !defined (ACE_DISABLE_DEBUG_DLL_CHECK)
              try_this += decorator;
#endif
              try_this += suffix;
            }
          break;
        case 4:
          try_this = dll_name;
          break;
        }

      if (try_this.length ())
        {
          try_names.size (j + 1);
          try_names.set (try_this, j);
        }
    }
  return;
}

bool
ACE_DLL_Handle::open_i (const ACE_TCHAR *dll_name, int open_mode, ERROR_STACK* errors)
{
  // The ACE_SHLIB_HANDLE object is obtained.
  this->handle_ = ACE_OS::dlopen (dll_name, open_mode);

  if (errors || ACE::debug ())
    {
      ACE_TString err;
      this->error (err);
      if (errors && err.length () > 0)
        {
          errors->push (err);
        }
      if (ACE::debug ())
        {
          ACELIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ACE (%P|%t) DLL_Handle::open_i ")
                      ACE_TEXT ("<%s>, 0x%x) -> <%s>: <%s>\n"),
                      dll_name,
                      open_mode,
                      ((this->handle_ != ACE_SHLIB_INVALID_HANDLE)
                      ? ACE_TEXT ("succeeded")
                      : ACE_TEXT ("failed")),
                      err.c_str ()));
        }
    }

  return this->handle_ != ACE_SHLIB_INVALID_HANDLE;
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
ACE_DLL_Manager::close_singleton ()
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

  if (this->open (size) != 0 && ACE::debug ())
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE (%P|%t) DLL_Manager ctor failed to allocate ")
                ACE_TEXT ("handle_vector_.\n")));
}

ACE_DLL_Manager::~ACE_DLL_Manager ()
{
  ACE_TRACE ("ACE_DLL_Manager::~ACE_DLL_Manager");

  if (this->close () != 0 && ACE::debug ())
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE (%P|%t) DLL_Manager dtor failed to close ")
                ACE_TEXT ("properly.\n")));
}

ACE_ALLOC_HOOK_DEFINE(ACE_DLL_Manager)

ACE_DLL_Handle *
ACE_DLL_Manager::open_dll (const ACE_TCHAR *dll_name,
                           int open_mode,
                           ACE_SHLIB_HANDLE handle,
                           ACE_DLL_Handle::ERROR_STACK *errors)
{
  ACE_TRACE ("ACE_DLL_Manager::open_dll");

  ACE_DLL_Handle *temp_handle = 0;
  ACE_DLL_Handle *dll_handle = 0;
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
    dll_handle = this->find_dll (dll_name);
    if (!dll_handle)
      {
        if (this->current_size_ < this->total_size_)
          {
            ACE_NEW_RETURN (temp_handle,
                            ACE_DLL_Handle,
                            0);

            dll_handle = temp_handle;
          }
      }
  }

  if (dll_handle)
    {
      if (dll_handle->open (dll_name, open_mode, handle, errors) != 0)
        {
          // Error while opening dll. Free temp handle
          if (ACE::debug ())
            ACELIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE (%P|%t) DLL_Manager::open_dll: Could not ")
                        ACE_TEXT ("open dll <%s>\n"),
                        dll_name));

          delete temp_handle;
          return 0;
        }

      // Add the handle to the vector only if the dll is successfully
      // opened.
      if (temp_handle != 0)
        {
          ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
          this->handle_vector_[this->current_size_] = dll_handle;
          ++this->current_size_;
        }
    }

  return dll_handle;
}

int
ACE_DLL_Manager::close_dll (const ACE_TCHAR *dll_name)
{
  ACE_TRACE ("ACE_DLL_Manager::close_dll");
  ACE_DLL_Handle *handle = 0;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0));
    handle = this->find_dll (dll_name);
  }

  if (handle)
    {
      return this->unload_dll (handle, 0);
    }

  return -1;
}

u_long
ACE_DLL_Manager::unload_policy () const
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

  ACE_DLL_Handle **temp = 0;

#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_ALLOCATOR_RETURN (temp,
                        static_cast<ACE_DLL_Handle**> (ACE_Allocator::instance()->malloc(sizeof (ACE_DLL_Handle*) * size)),
                        -1);
#else
  ACE_NEW_RETURN (temp,
                  ACE_DLL_Handle *[size],
                  -1);
#endif /* ACE_HAS_ALLOC_HOOKS */

  this->handle_vector_ = temp;
  this->total_size_ = size;
  return 0;
}

int
ACE_DLL_Manager::close ()
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
              ACE_DLL_Handle *s =
                const_cast<ACE_DLL_Handle *> (this->handle_vector_[i]);
              this->handle_vector_[i] = 0;
              this->unload_dll (s, force_close);
              delete s;
            }
        }

#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance()->free(this->handle_vector_);
#else
      delete [] this->handle_vector_;
#endif /* ACE_HAS_ALLOC_HOOKS */

      this->handle_vector_ = 0;
      this->current_size_ = 0;
    }
  return 0;
}

ACE_DLL_Handle *
ACE_DLL_Manager::find_dll (const ACE_TCHAR *dll_name) const
{
  ACE_TRACE ("ACE_DLL_Manager::find_dll");

  for (int i = 0; i < this->current_size_; i++)
    if (this->handle_vector_[i] &&
        ACE_OS::strcmp (this->handle_vector_[i]->dll_name (), dll_name) == 0)
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
              using dll_unload_policy = int (*)();

              void * const unload_policy_ptr =
                dll_handle->symbol (ACE_TEXT ("_get_dll_unload_policy"), 1);
              intptr_t const temp_p = reinterpret_cast<intptr_t> (unload_policy_ptr);

              dll_unload_policy const the_policy =
                reinterpret_cast<dll_unload_policy> (temp_p);

              if (the_policy != 0)
                unload = ACE_BIT_DISABLED (the_policy (),
                                           ACE_DLL_UNLOAD_POLICY_LAZY);
              else
                unload = ACE_BIT_DISABLED (this->unload_policy_,
                                           ACE_DLL_UNLOAD_POLICY_LAZY);
            }
        }

      if (dll_handle->close (unload) != 0)
        {
          if (ACE::debug ())
            ACELIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE (%P|%t) DLL_Manager::unload error.\n")));

          return -1;
        }
    }
  else
    {
      if (ACE::debug ())
        ACELIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("ACE (%P|%t) DLL_Manager::unload_dll called with ")
                    ACE_TEXT ("null pointer.\n")));

      return -1;
    }

  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
