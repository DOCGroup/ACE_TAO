// Local_Name_Space_T.cpp
// $Id$

#if !defined (ACE_LOCAL_NAME_SPACE_T_C)
#define ACE_LOCAL_NAME_SPACE_T_C

#define ACE_BUILD_DLL
#include "ace/ACE.h"
#include "ace/Local_Name_Space.h"

template <class ALLOCATOR>
ACE_Name_Space_Map<ALLOCATOR>::ACE_Name_Space_Map (ALLOCATOR *alloc)
  : MAP_MANAGER (alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::ACE_Name_Space_Map");
}

template <class ALLOCATOR> int 
ACE_Name_Space_Map<ALLOCATOR>::close (ALLOCATOR* alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::close");

  this->allocator_ = alloc;
  return this->close_i ();
}

template <class ALLOCATOR> int 
ACE_Name_Space_Map<ALLOCATOR>::bind (const ACE_NS_String &ext_id,
				     const ACE_NS_Internal &int_id,
				     ALLOCATOR *alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::bind");

  this->allocator_ = alloc;
  return this->bind_i (ext_id, int_id);
}

template <class ALLOCATOR> int 
ACE_Name_Space_Map<ALLOCATOR>::unbind (const ACE_NS_String &ext_id, 
				       ACE_NS_Internal &int_id,
				       ALLOCATOR *alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::unbind");

  this->allocator_ = alloc;
  return this->unbind_i (ext_id, int_id);
}

template <class ALLOCATOR> int 
ACE_Name_Space_Map<ALLOCATOR>::rebind (const ACE_NS_String &ext_id,
				       const ACE_NS_Internal &int_id,
				       ACE_NS_String &old_ext_id, 
				       ACE_NS_Internal &old_int_id,
				       ALLOCATOR *alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::rebind");

  this->allocator_ = alloc;
  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id);
}

template <class ALLOCATOR> int 
ACE_Name_Space_Map<ALLOCATOR>::find (const ACE_NS_String &ext_id,
				     ACE_NS_Internal &int_id,
				     ALLOCATOR *alloc)
{
  ACE_TRACE ("ACE_Name_Space_Map::find");

  this->allocator_ = alloc;
  return this->find_i (ext_id, int_id);
}

#if defined (ACE_WIN32)
template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::remap (EXCEPTION_POINTERS *ep)
{
  ACE_TRACE ("ACE_Name_Space_Map::remap");

  void *addr = (void *) ep->ExceptionRecord->ExceptionInformation[1];

  // The following requires Memory Pool to have ::remap()
  // defined. Thus currently this will only work for
  // ACE_MMap_Memory_Pool.
  if (this->allocator_->alloc ().memory_pool ().remap (addr) == -1)
    // Kick it upstairs...
    return (DWORD) EXCEPTION_CONTINUE_SEARCH;

#if __X86__
  // This is 80x86-specific.
  ep->ContextRecord->Edi = (DWORD) addr;
#elif __MIPS__
  ep->ContextRecord->IntA0 =
    ep->ContextRecord->IntV0 = (DWORD) addr;
  ep->ContextRecord->IntT5 = ep->ContextRecord->IntA0 + 3;
#endif /* __X86__ */
    // Resume execution at the original point of "failure."
  return (DWORD) EXCEPTION_CONTINUE_EXECUTION; 
}
#endif /* ACE_WIN32 */

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::shared_bind (const ACE_WString &name,
							 const ACE_WString &value, 
							 const char *type,
							 int rebind)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->shared_bind_i (name, value, type, rebind);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::shared_bind_i (const ACE_WString &name,
							   const ACE_WString &value, 
							   const char *type,
							   int rebind)
{

  ACE_TRACE ("ACE_Local_Name_Space::shared_bind");
  size_t name_len = (name.length () + 1) * sizeof (ACE_USHORT16);
  size_t value_len = (value.length () + 1) * sizeof (ACE_USHORT16);
  size_t type_len = ACE_OS::strlen (type) + 1;
  size_t total_len = name_len + value_len + type_len;
  char *ptr = (char *) this->allocator_->malloc (total_len);

  if (ptr == 0)
    return -1;
  else
    {
      // Note that the value_rep *must* come first to make sure we can
      // retrieve this pointer later on in unbind().
      ACE_USHORT16 *value_rep = (ACE_USHORT16 *) (ptr);
      ACE_USHORT16 *name_rep = (ACE_USHORT16 *) (ptr + value_len);
      char *new_type = (char *) (ptr + value_len + name_len);
      ACE_NS_String new_name (name_rep, name.fast_rep (), name_len);
      ACE_NS_String new_value (value_rep, value.fast_rep (), value_len);
      ACE_OS::strcpy (new_type, type);
      ACE_NS_Internal new_internal (new_value, new_type);
      int result = -1;

      if (rebind == 0)
	{
	  // Do a normal bind.  This will fail if there's already an
	  // <new_internal> with the same name.
	  result = this->name_space_map_->bind (new_name, new_internal, this->allocator_);

	  if (result == 1)
	    {
	      // Entry already existed so bind failed. Free our dynamically allocated memory.
	      this->allocator_->free ((void *) ptr);
	      return result;
	    }		
	}
      else
	{
	  // Do a rebind.  If there's already any entry, this will return the existing 
	  // <new_name> and <new_internal> and overwrite the existing name binding.
	  ACE_NS_String old_name;
	  ACE_NS_Internal old_internal;
	  
	  result = this->name_space_map_->rebind (new_name, new_internal,
						  old_name, old_internal,
						  this->allocator_);
	  if (result == 1)
	    {
	      // Free up the memory we allocated in shared_bind().  Note that this
	      // assumes that the "value" pointer comes first and that the value, 
	      // name, and type are contiguously allocated (see above for details)
	      this->allocator_->free ((void *) (old_internal.value ()).fast_rep ());
	    }
	}

      if (result == -1)
	// Free our dynamically allocated memory.
	this->allocator_->free ((void *) ptr);
      else
	// If bind() or rebind() succeed, they will automatically sync
	// up the map manager entry.  However, we must sync up our
	// name/value memory. 
	this->allocator_->sync (ptr, total_len);

      return result;
    }
}

template <ACE_MEM_POOL_1, class LOCK> int  
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::unbind (const ACE_WString &name)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->unbind_i (name);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }

}

template <ACE_MEM_POOL_1, class LOCK> int  
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::unbind_i (const ACE_WString &name)
{
  ACE_TRACE ("ACE_Local_Name_Space::unbind");

  ACE_WRITE_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);
  ACE_NS_String ns_name (name);
  ACE_NS_Internal ns_internal;
  if (this->name_space_map_->unbind (ns_name, ns_internal, this->allocator_) != 0)
    return -1;
  else
    {
      // Free up the memory we allocated in shared_bind().  Note that
      // this assumes that the "value" pointer comes first and that
      // the value, name and type are contiguously allocated (see
      // shared_bind() for details)
      this->allocator_->free ((void *) (ns_internal.value ()).fast_rep ());
      return 0;
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::bind (const ACE_WString &name,
					    const ACE_WString &value, 
					    const char *type)
{
  ACE_TRACE ("ACE_Local_Name_Space::bind");
  ACE_WRITE_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  return this->shared_bind (name, value, type, 0);
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::rebind (const ACE_WString &name, 
					      const ACE_WString &value, 
					      const char *type)
{
  ACE_TRACE ("ACE_Local_Name_Space::rebind");
  ACE_WRITE_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  return this->shared_bind (name, value, type, 1);
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::resolve (const ACE_WString &name,
						     ACE_WString &value, 
						     char *&type)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->resolve_i (name, value, type);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}


template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::resolve_i (const ACE_WString &name,
						       ACE_WString &value, 
						       char *&type)
{
  ACE_TRACE ("ACE_Local_Name_Space::resolve");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  ACE_NS_String ns_name (name);
  ACE_NS_Internal ns_internal;
  ACE_NS_String nbc_string; // Note the classy variable name! :)

  if (this->name_space_map_->find (ns_name, ns_internal, this->allocator_) != 0)
    return -1;
  else
    {
      // Calls conversion operator and then calls the ACE_WString
      // assignment operator to get a fresh copy.  (*#*(@#&!*@!!*@&(
      // HP compiler causes us to add an extra copy explicitly !! :)
      nbc_string = ns_internal.value ();
      value = nbc_string;

      // Gets type and then the actual reprsentation which is a ACE_USHORT16
      const char *temp = ns_internal.type ();

      size_t len = ACE_OS::strlen (ns_internal.type ());
      // Makes a copy here. Caller needs to call delete to free up memory
      char *new_type;
      ACE_NEW_RETURN (new_type, char [len + 1], -1);

      ACE_OS::strncpy (new_type, temp, len);
      new_type[len] = '\0';  // Null terminate the string
      type = new_type;
      return 0;
    }
}

template <ACE_MEM_POOL_1, class LOCK> int
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::open (ACE_Naming_Context::Context_Scope_Type scope_in) 
{
  ACE_TRACE ("ACE_Local_Name_Space::open");
  this->ns_scope_ = scope_in;

  return this->create_manager ();
}

template <ACE_MEM_POOL_1, class LOCK> 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::ACE_Local_Name_Space (void)
  : allocator_ (0),
    name_space_map_ (0),
    name_options_ (0)
{
  ACE_TRACE ("ACE_Local_Name_Space::ACE_Local_Name_Space");
}

template <ACE_MEM_POOL_1, class LOCK> 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::ACE_Local_Name_Space (ACE_Naming_Context::Context_Scope_Type scope_in, 
							    ACE_Name_Options *name_options)
  : name_options_ (name_options)
{  
  ACE_TRACE ("ACE_Local_Name_Space::ACE_Local_Name_Space");
  if (this->open (scope_in) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Local_Name_Space::ACE_Local_Name_Space"));
}

template <ACE_MEM_POOL_1, class LOCK> 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::~ACE_Local_Name_Space (void)
{
  ACE_TRACE ("ACE_Local_Name_Space::~ACE_Local_Name_Space");

  // Remove the map.
  delete this->allocator_;
  delete this->lock_;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::create_manager (void)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->create_manager_i ();
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}


template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::create_manager_i (void)
{
  ACE_TRACE ("ACE_Local_Name_Space::create_manager");
  // Get directory name
  LPCTSTR dir = this->name_options_->namespace_dir ();
  LPCTSTR database = this->name_options_->database ();

  // Use process name as the file name.
  size_t len = ACE_OS::strlen (dir);
  len += ACE_OS::strlen (database) + 1;

  if (len >= MAXNAMELEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

  ACE_OS::strcpy (this->context_file_, dir);
  ACE_OS::strcat (this->context_file_, ACE_DIRECTORY_SEPARATOR_STR);
  ACE_OS::strcat (this->context_file_, database);

  ACE_MEM_POOL_OPTIONS options (this->name_options_->base_address ());

  TCHAR lock_name_for_local_name_space [MAXNAMELEN];
  TCHAR lock_name_for_backing_store [MAXNAMELEN];
  LPCTSTR postfix = database;

  size_t length = 0;
  length = sizeof lock_name_for_local_name_space / sizeof (TCHAR);
  ACE_OS::strncpy (lock_name_for_local_name_space, 
		   __TEXT ("name_space_"),
		   length);
  ACE_OS::strncat (lock_name_for_local_name_space, 
		   postfix,
		   length - ACE_OS::strlen (__TEXT ("name_space_")));
  
  length = sizeof lock_name_for_backing_store / sizeof (TCHAR);
  ACE_OS::strncpy (lock_name_for_backing_store, 
		   __TEXT ("backing_store_"),
		   length);
  ACE_OS::strncat (lock_name_for_backing_store, 
		   postfix,
		   length - ACE_OS::strlen (__TEXT ("backing_store_")));

  // Create the allocator with the appropriate options.
  ACE_NEW_RETURN (this->allocator_, 
		  ALLOCATOR (this->context_file_, 
			     lock_name_for_backing_store,
			     &options), -1);

  if (ACE_LOG_MSG->op_status ())
    ACE_ERROR_RETURN ((LM_ERROR, "Allocator::Allocator\n"), -1);    

  ACE_NEW_RETURN (this->lock_, LOCK (lock_name_for_local_name_space), -1);
  
  // Now check if the backing store has been created successfully
  if (ACE_OS::access (this->context_file_, F_OK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "create_manager\n"), -1);
  
  void *ns_map = 0;

  // This is the easy case since if we find the Name Server Map
  // Manager we know it's already initialized.
  if (this->allocator_->find (ACE_NAME_SERVER_MAP, ns_map) == 0)
    {
      this->name_space_map_ = (ACE_Name_Space_Map <ALLOCATOR> *) ns_map;
      ACE_DEBUG ((LM_DEBUG, "name_space_map_ = %d, ns_map = %d\n",
		  this->name_space_map_, ns_map));
    }

  // This is the hard part since we have to avoid potential race
  // conditions...  We will use the double check here
  else
    {
      ACE_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

      // This is the easy case since if we find the Name Server Map
      // Manager we know it's already initialized.
      if (this->allocator_->find (ACE_NAME_SERVER_MAP, ns_map) == 0)
	{
	  this->name_space_map_ = (ACE_Name_Space_Map <ALLOCATOR> *) ns_map;
	  ACE_DEBUG ((LM_DEBUG, "name_space_map_ = %d, ns_map = %d\n",
		      this->name_space_map_, ns_map));
	}
      else
	{
	  size_t map_size = sizeof *this->name_space_map_;
	  ns_map = this->allocator_->malloc (map_size);

	  // Initialize the map into its memory location (e.g., shared memory).
	  ACE_NEW_RETURN (this->name_space_map_,
			  (ns_map) ACE_Name_Space_Map <ALLOCATOR> (this->allocator_),
			  -1);

	  if (this->allocator_->bind (ACE_NAME_SERVER_MAP, ns_map) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "create_manager\n"), -1);
	}

      ACE_DEBUG ((LM_DEBUG, "name_space_map_ = %d, ns_map = %d\n",
		  this->name_space_map_, ns_map));
    }

  return 0;
}


template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_names_i (ACE_PWSTRING_SET &set,
							  const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_names");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);
  
  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;
  
  int result = 1;
  
  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance())
    {
      if (map_entry->ext_id_.strstr (pattern) != -1)
	{
	  ACE_WString entry (map_entry->ext_id_ );
	  
	  if (set.insert (entry) == -1)
	    {
	      result = -1;
	      break;
	    }
	  else 
	    result = 0;
	}
    }

  return result;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_values_i (ACE_PWSTRING_SET &set,
							   const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_values");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;

  int result = 1;

  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance ())
    {
      if (map_entry->int_id_.value ().strstr (pattern) != -1)
	{
	  ACE_WString entry (map_entry->int_id_.value ());

	  if (set.insert (entry) == -1)
	    {
  	      result = -1;
	      break;
            }
	  else 
	    result = 0;
	}
    }

  return result;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_types_i (ACE_PWSTRING_SET &set,
							  const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_types");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;

  char *compiled_regexp = 0;

  // Note that char_rep() allocates memory so we need to delete it
  char *pattern_rep = pattern.char_rep ();
  
  // Check for wildcard case first.
  if (ACE_OS::strcmp ("", pattern_rep) == 0)
    compiled_regexp = ACE_OS::strdup ("");
  else 
    // Compile the regular expression (the 0's cause ACE_OS::compile to allocate space).
#if defined (ACE_HAS_REGEX)
    compiled_regexp = ACE_OS::compile (pattern_rep, 0, 0);
#else /* If we don't have regular expressions just use the pattern directly. */
  compiled_regexp = pattern_rep;
#endif /* ACE_HAS_REGEX */
  
  int result = 1;
  
  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance ())
    {
      // Get the type
      const char *type = map_entry->int_id_.type ();
      
      if (ACE_OS::strcmp ("", pattern_rep) == 0 // Everything matches the wildcard.
#if defined (ACE_HAS_REGEX)
	  || ACE_OS::step (type, compiled_regexp) != 0)
#else /* If we don't have regular expressions just use strstr() for substring matching. */
	|| ACE_OS::strstr (type, compiled_regexp) != 0)
#endif /* ACE_HAS_REGEX */

        {
          ACE_WString entry (type);
	  
	  if (set.insert (entry) == -1)
	    {
	      result = -1;
	      break;
	    }
	  else 
	    result = 0;
	}
    }
#if defined (ACE_HAS_REGEX)
  if (compiled_regexp)
    ACE_OS::free ((void *) compiled_regexp);
#endif /* ACE_HAS_REGEX */
  delete [] pattern_rep;  // delete pattern_rep; 
  return result;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space <ACE_MEM_POOL_2, LOCK>::list_name_entries_i (ACE_BINDING_SET &set,
                                                                  const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_name_entries");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;

  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance())
    {
      if (map_entry->ext_id_.strstr (pattern) != -1)
	{
	  ACE_Name_Binding entry (map_entry->ext_id_,
	                          map_entry->int_id_.value (),
				  map_entry->int_id_.type ());

	  if (set.insert (entry) == -1)
	    return -1;
	}
    }

  return 0;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_value_entries_i (ACE_BINDING_SET &set,
					                          const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_value_entries");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;

  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance ())
    {
      if (map_entry->int_id_.value ().strstr (pattern) != -1)
	{
	  ACE_Name_Binding entry (map_entry->ext_id_,
	                          map_entry->int_id_.value (),
				  map_entry->int_id_.type ());

	  if (set.insert (entry) == -1)
	    return -1;
	}
    }
  return 0;
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_type_entries_i (ACE_BINDING_SET &set,
							         const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Local_Name_Space::list_type_entries");
  ACE_READ_GUARD_RETURN (ACE_RW_Process_Mutex, ace_mon, *this->lock_, -1);

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;

  char *compiled_regexp = 0;
  // Note that char_rep() allocates memory so we need to delete it
  char *pattern_rep = pattern.char_rep ();

  // Check for wildcard case first.
  if (ACE_OS::strcmp ("", pattern_rep) == 0)
    compiled_regexp = ACE_OS::strdup ("");
  else 
    // Compile the regular expression (the 0's cause ACE_OS::compile to allocate space).
#if defined (ACE_HAS_REGEX)
    compiled_regexp = ACE_OS::compile (pattern_rep, 0, 0);
#else /* If we don't have regular expressions just the pattern directly. */
  compiled_regexp = pattern_rep;
#endif /* ACE_HAS_REGEX */

  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance ())
    {
      // Get the type.
      const char *type = map_entry->int_id_.type ();

      if (ACE_OS::strcmp ("", pattern_rep) == 0 // Everything matches the wildcard.
#if defined (ACE_HAS_REGEX)
	  || ACE_OS::step (type, compiled_regexp) != 0)
#else /* If we don't have regular expressions just use strstr() for substring matching. */
	|| ACE_OS::strstr (type, compiled_regexp) != 0)
#endif /* ACE_HAS_REGEX */
        {
  	  ACE_Name_Binding entry (map_entry->ext_id_,
				  map_entry->int_id_.value (),
				  map_entry->int_id_.type ());

	  if (set.insert (entry) == -1)
	    return -1;
	}
    }
#if defined (ACE_HAS_REGEX)
  if (compiled_regexp)
    ACE_OS::free ((void *) compiled_regexp);
#endif /* ACE_HAS_REGEX */
  delete [] pattern_rep;  // delete pattern_rep; 
  return 0;
}

template <ACE_MEM_POOL_1, class LOCK> void
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::dump_i (void) const
{
  ACE_TRACE ("ACE_Local_Name_Space::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  MAP_ITERATOR map_iterator (*this->name_space_map_);
  MAP_ENTRY *map_entry;
  
  for (map_entry = 0;
       map_iterator.next (map_entry) != 0;
       map_iterator.advance())
    {
      char *key = map_entry->ext_id_.char_rep ();
      char *value = map_entry->int_id_.value ().char_rep ();
      const char *type = map_entry->int_id_.type ();

      ACE_DEBUG ((LM_DEBUG, "key=%s\nvalue=%s\ntype=%s\n",
		  key, value, type));
      // We need to delete key and value since char_rep allocates memory for them
      delete [] key;
      delete [] value;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_names (ACE_PWSTRING_SET &set,
							const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_names_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_values (ACE_PWSTRING_SET &set,
						         const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_values_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_types (ACE_PWSTRING_SET &set,
							const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_types_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space <ACE_MEM_POOL_2, LOCK>::list_name_entries (ACE_BINDING_SET &set,
								const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_name_entries_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_value_entries (ACE_BINDING_SET &set,
								const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_value_entries_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> int 
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::list_type_entries (ACE_BINDING_SET &set,
							       const ACE_WString &pattern)
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.
  ACE_SEH_TRY 
    {
      return this->list_type_entries_i (set, pattern);
    }
  ACE_SEH_EXCEPT (this->remap (GetExceptionInformation ())) 
    {
    }
}

template <ACE_MEM_POOL_1, class LOCK> void
ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK>::dump (void) const
{
  // Note that we *must* use structured exception handling here
  // because (1) we may need to commit virtual memory pages and (2)
  // C++ exception handling doesn't support resumption.

  // This should really be a const cast
  ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK> *fake_this = 
    (ACE_Local_Name_Space<ACE_MEM_POOL_2, LOCK> *) this;
  ACE_UNUSED_ARG (fake_this);

  ACE_SEH_TRY 
    {
      this->dump_i ();
    }
  ACE_SEH_EXCEPT (fake_this->remap (GetExceptionInformation ())) 
    {
    }
}

#endif /* ACE_LOCAL_NAME_SPACE_T_C */
