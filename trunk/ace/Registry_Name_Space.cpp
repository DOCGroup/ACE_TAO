#define ACE_BUILD_DLL

#include "ace/Registry_Name_Space.h"

#if (defined (ACE_WIN32) && defined (UNICODE))
// This only works on Win32 platforms when UNICODE is turned on

ACE_Registry_Name_Space::ACE_Registry_Name_Space (void)
{
}

ACE_Registry_Name_Space::ACE_Registry_Name_Space (ACE_Name_Options *name_options)
{
  if (this->open (name_options) != 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Registry_Name_Space::open"));
}


ACE_Registry_Name_Space::~ACE_Registry_Name_Space (void)
{
}


int 
ACE_Registry_Name_Space::open (ACE_Name_Options *name_options)
{
  const char *host = name_options->nameserver_host ();
  ACE_Registry::Naming_Context predefined;
  
  int result = ACE_Predefined_Naming_Contexts::connect (predefined,
							HKEY_LOCAL_MACHINE,
							ACE_WIDE_STRING (host));
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Predefined_Naming_Context::connect"), result);
  else
    {
      // Directory
      ACE_Registry::Istring name = name_options->namespace_dir ();
      // Separator
      name += ACE_Registry::STRING_SEPARATOR;
      // Filename
      name += name_options->database ();

      // Create new context or bind to existing one
      result = predefined.bind_context (name,
					this->context_);
      if (result != 0)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::bind_context"), result);
    }
  return 0;
}


int 
ACE_Registry_Name_Space::bind (const ACE_WString &name, 
			       const ACE_WString &value, 
			       const char *type)
{
  // Pointer to data
  const ACE_USHORT16 *data = value.fast_rep ();
  
  // Size
  u_long size = value.length () * sizeof (ACE_USHORT16);
  
  // Represent value as an ACE_Registry::Object
  ACE_Registry::Object object ((void *) data,
			       size,
			       REG_SZ);
  // Add new <key>/<value> pair
  return this->context_.bind (name.fast_rep(), 
			      object);
}


int 
ACE_Registry_Name_Space::rebind (const ACE_WString &name, 
				 const ACE_WString &value, 
				 const char *type)
{
  // Pointer to data
  const ACE_USHORT16 *data = value.fast_rep ();
  
  // Size
  u_long size = value.length () * sizeof (ACE_USHORT16);
  
  // Represent value as an ACE_Registry::Object
  ACE_Registry::Object object ((void *) data,
			       size,
			       REG_SZ);
  // Add new <key>/<value> pair
  return this->context_.rebind (name.fast_rep (), 
				object);
}


int 
ACE_Registry_Name_Space::unbind (const ACE_WString &name)
{
  return this->context_.unbind (name.fast_rep ());
}


int 
ACE_Registry_Name_Space::resolve (const ACE_WString &name, 
				  ACE_WString &value, 
				  char *&type)
{
  ACE_Registry::Object object;
  int result = this->context_.resolve (name.fast_rep (), object);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::resolve"), result);

  ACE_WString string ((ACE_USHORT16 *) object.data (),
		      object.size () / sizeof (ACE_USHORT16));
  value = string;
  return 0;
}

  
int 
ACE_Registry_Name_Space:: list_names (ACE_WSTRING_SET &set, 
				      const ACE_WString &pattern)
{
  ACE_BINDING_SET binding_set;
  int result = this->list_name_entries (binding_set,
					pattern);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::list"), result);    
  
  ACE_BINDING_ITERATOR iterator (binding_set);

  for (ACE_Name_Binding *entry = 0;
       iterator.next (entry) !=0;
       iterator.advance())
    {
      set.insert (entry->name_);
    }
  return 0;
}


int 
ACE_Registry_Name_Space::list_values (ACE_WSTRING_SET &set, 
				      const ACE_WString &pattern)
{
  ACE_BINDING_SET binding_set;
  int result = this->list_name_entries (binding_set,
					pattern);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::list"), result);    
  
  ACE_BINDING_ITERATOR iterator (binding_set);

  for (ACE_Name_Binding *entry = 0;
       iterator.next (entry) !=0;
       iterator.advance())
    {
      set.insert (entry->value_);
    }
  return 0;
}


int 
ACE_Registry_Name_Space::list_types (ACE_WSTRING_SET &set, 
				     const ACE_WString &pattern)
{
  return 0;
}


int 
ACE_Registry_Name_Space::list_name_entries (ACE_BINDING_SET &set, 
					    const ACE_WString &pattern)
{
  ACE_Registry::Binding_List list;
  int result = this->context_.list (list);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::list"), result);

  // Iterator through all entries
  for (ACE_Registry::Binding_List::iterator i = list.begin ();
       i != list.end ();
       i++)
    {
      // Yeeesss! STL rules!
      ACE_Registry::Binding &binding = *i;
      
      if (binding.type () == ACE_Registry::OBJECT)
	{	  
	  // Key
	  ACE_Registry::Istring string = binding.name ();
	  ACE_WString key (string.c_str ());
	  
	  // Value
	  ACE_WString value;
	  char *type = 0;
	  result = this->resolve (key,
				  value,
				  type);	  
	  if (result != 0)
	    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Registry::Naming_Context::resolve"), result);
	  
	  // Complete binding
	  ACE_Name_Binding binding (key, value, type);
	  set.insert (binding);
	}
    }
  return 0;
}


int 
ACE_Registry_Name_Space::list_value_entries (ACE_BINDING_SET &set, 
					     const ACE_WString &pattern)
{
  return this->list_name_entries (set, pattern);
}


int 
ACE_Registry_Name_Space::list_type_entries (ACE_BINDING_SET &set, 
					    const ACE_WString &pattern)
{
  return this->list_name_entries (set, pattern);
}


void 
ACE_Registry_Name_Space::dump (void) const
{
}


#endif /* ACE_WIN32 && UNICODE */
