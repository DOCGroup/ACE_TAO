// $Id$
#define ACE_BUILD_DLL
#include "ace/Configuration.h"

#if !defined (__ACE_INLINE__)
#include "ace/Configuration.i"
#endif /* __ACE_INLINE__ */

ACE_Section_Key_Internal::ACE_Section_Key_Internal (void)
  : ref_count_ (0)
{
}

ACE_Section_Key_Internal::~ACE_Section_Key_Internal (void)
{
}

int
ACE_Section_Key_Internal::add_ref (void)
{
  ++ref_count_;
  return 0;
}

int
ACE_Section_Key_Internal::dec_ref (void)
{
  if (!--ref_count_)
    delete this;
  return 0;
}

ACE_Configuration_Section_Key::ACE_Configuration_Section_Key (void)
  : key_ (0)
{
}

ACE_Configuration_Section_Key::~ACE_Configuration_Section_Key (void)
{
  if (key_)
    key_->dec_ref ();
}

ACE_Configuration_Section_Key::ACE_Configuration_Section_Key (ACE_Section_Key_Internal* key)
  : key_ (key)
{
  if (key_)
    key_->add_ref ();
}

ACE_Configuration_Section_Key::ACE_Configuration_Section_Key (const ACE_Configuration_Section_Key& rhs)
  : key_ (rhs.key_)
{
  if (key_)
    key_->add_ref ();
}

ACE_Configuration_Section_Key&
ACE_Configuration_Section_Key::operator= (const ACE_Configuration_Section_Key& rhs)
{
  if (this != &rhs)
    {
      if (key_)
        key_->dec_ref ();

      key_ = rhs.key_;

      if (key_)
        key_->add_ref ();
    }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////

ACE_Configuration::ACE_Configuration (void)
  : root_ ()
{
}

ACE_Configuration::~ACE_Configuration (void)
{
}

ACE_Section_Key_Internal*
ACE_Configuration::get_internal_key (const ACE_Configuration_Section_Key& key)
{
  return key.key_;
}

int
ACE_Configuration::expand_path (const ACE_Configuration_Section_Key& key,
                                const ACE_TString& path_in,
                                ACE_Configuration_Section_Key& key_out,
                                int create)
{
  const TCHAR* begin = path_in.fast_rep ();
  const TCHAR* end = 0;

  // Make a copy of key
  ACE_Configuration_Section_Key current_section = key;

  // recurse through the path
  while (1)
    {
      // Detmine the begin/ending of the key name
      end = ACE_OS::strchr (begin, '\\');
      size_t length = end ? (size_t)(end-begin) : ACE_OS::strlen (begin);

      // Make sure length is not 0
      if (!length)
        return -1;

      ACE_TString section (begin, length);

      // Open the section
      ACE_Configuration_Section_Key child_section;
      if (open_section (current_section,
                        section.fast_rep (),
                        create,
                        child_section))
        return -1;

      current_section = child_section;

      // If end is NULL, we are done, return the result
      if (!end)
        {
          key_out = current_section;
          break;
        }
      begin = end + 1;
    }

  return 0;
}

int
ACE_Configuration::validate_name (const TCHAR* name)
{
  const TCHAR* pos = name;
  // make sure it doesn't contain any invalid characters
  while (*pos)
    {
      if (ACE_OS::strchr (ACE_TEXT ("\\]["), *pos))
        return -1;

      pos++;
    }

  // Make sure its not too long
  if (pos - name > 255)
    return -2;

  return 0;
}

int
ACE_Configuration::export_section (const ACE_Configuration_Section_Key& section,
                                   const ACE_TString& path,
                                   FILE* out)
{
  // don't export the root
  if (path.length ())
    {
      // Write out the section header
      ACE_TString header = ACE_TEXT ("[");
      header += path;
      header += "]";
      header +="\n";
      if (ACE_OS::fputs (ACE_MULTIBYTE_STRING (header.fast_rep ()), out) < 0)
        return -1;

      // Write out each value
      int index = 0;
      ACE_TString name;
      VALUETYPE type;
      ACE_TString line;
      TCHAR int_value[32];
      ACE_TString string_value;
      while (!enumerate_values (section, index, name, type))
        {
          line = name + ACE_TEXT ("=");
          switch (type)
            {
            case INTEGER:
              {
                u_int value;
                if (get_integer_value (section, name.fast_rep (), value))
                  return -2;

                ACE_OS::sprintf (ACE_MULTIBYTE_STRING (int_value), "#%d", value);
                line += int_value;
                break;
              }
            case STRING:
              {
                if (get_string_value (section,
                                      name.fast_rep (),
                                      string_value))
                  return -2;

                line += ACE_TEXT ("\"");
                line += string_value;
                break;
              }
            case BINARY:
              {
                // not supported yet - maybe use BASE64 codeing?
                break;
              }
            default:
              return -3;
            }

          line += ACE_TEXT ("\n");
          if (ACE_OS::fputs (ACE_MULTIBYTE_STRING (line.fast_rep ()), out) < 0)
            return -4;

          index++;
        }
    }

  // Export all sub sections
  int index = 0;
  ACE_TString name;
  ACE_Configuration_Section_Key sub_key;
  ACE_TString sub_section;

  while (!enumerate_sections (section, index, name))
    {
      ACE_TString sub_section (path);
      if (path.length ())
        sub_section += ACE_TEXT ("\\");

      sub_section += name;
      if (open_section (section, name.fast_rep (), 0, sub_key))
        return -5;

      if (export_section (sub_key, sub_section.fast_rep (), out))
        return -6;

      index++;
    }

  return 0;
}

int
ACE_Configuration::export_config (const TCHAR* filename)
{
  FILE* out = ACE_OS::fopen (ACE_MULTIBYTE_STRING (filename), "w");
  if (!out)
    return -1;

  int result = export_section (root_, "", out);
  ACE_OS::fclose (out);
  return result;
}

int
ACE_Configuration::import_config (const TCHAR* filename)
{
  FILE* in = ACE_OS::fopen (ACE_MULTIBYTE_STRING (filename), "r");
  if (!in)
    return -1;

  // XXX - change this to a dynamic buffer
  TCHAR buffer[4096];
  ACE_Configuration_Section_Key section;
  while (ACE_OS::fgets (buffer, 4096, in))
    {
      // Check for a comment
      if (buffer[0] == ';' || buffer[0] == '#')
        continue;

      if (buffer[0] == '[')
        {
          // We have a new section here, strip out the section name
          int length = ACE_OS::strlen (buffer);
          buffer[length - 2] = 0;

          if (expand_path (root_, buffer + 1, section, 1))
            return -3;

          continue;
        }

      // assume this is a value, read in the value name
      TCHAR* end = ACE_OS::strchr (buffer, '=');
      if (!end)  // no =, not a value so just skip it
        continue;

      // null terminate the name
      *end = 0;
      end++;
      // determine the type
      if (*end == '\"')
        {
          // string type
          if (set_string_value (section, buffer, end + 1))
            return -4;
        }
      else if (*end == '#')
        {
          // number type
          u_int value = atoi (ACE_MULTIBYTE_STRING (end + 1));
          if (set_integer_value (section, buffer, value))
            return -4;
        }
      else
        {
          // invalid type, ignore
          continue;
        }
    }

  if (ferror (in))
    return -1;

  return 0;
}

const ACE_Configuration_Section_Key&
ACE_Configuration::root_section (void)
{
  return root_;
}

//////////////////////////////////////////////////////////////////////////////

#if defined (WIN32)

static const int ACE_DEFAULT_BUFSIZE = 256;

ACE_Section_Key_Win32::ACE_Section_Key_Win32 (HKEY hKey)
  : hKey_ (hKey)
{
}

ACE_Section_Key_Win32::~ACE_Section_Key_Win32 (void)
{
  ::RegCloseKey (hKey_);
}

//////////////////////////////////////////////////////////////////////////////

ACE_Configuration_Win32Registry::ACE_Configuration_Win32Registry (HKEY hKey)
{
  ACE_Section_Key_Win32 *temp;

  ACE_NEW (temp, ACE_Section_Key_Win32 (hKey));

  root_ = ACE_Configuration_Section_Key (temp);
}


ACE_Configuration_Win32Registry::~ACE_Configuration_Win32Registry (void)
{
}

int
ACE_Configuration_Win32Registry::open_section (const ACE_Configuration_Section_Key& base,
                                               const TCHAR* sub_section,
                                               int create,
                                               ACE_Configuration_Section_Key& result)
{
  if (validate_name (sub_section))
    return -1;

  HKEY base_key;
  if (load_key (base, base_key))
    return -1;

  HKEY result_key;
  if (::RegOpenKeyEx (base_key,
                      sub_section,
                      0,
                      KEY_ALL_ACCESS,
                      &result_key) != ERROR_SUCCESS)
    {
      if (!create)
        return -2;

      if (::RegCreateKeyEx (base_key,
                            sub_section,
                            0,
                            NULL,
                            REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS,
                            NULL,
                            &result_key,
                            NULL) != ERROR_SUCCESS)
        return -3;
    }

  ACE_Section_Key_Win32 *temp;

  ACE_NEW_RETURN (temp, ACE_Section_Key_Win32 (result_key), -4);
  result = ACE_Configuration_Section_Key (temp);
  return 0;
}

int
ACE_Configuration_Win32Registry::remove_section (const ACE_Configuration_Section_Key& key,
                                                 const TCHAR* sub_section,
                                                 int recursive)
{
  if (validate_name (sub_section))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (recursive)
    {
      ACE_Configuration_Section_Key section;
      if (open_section (key, sub_section, 0, section))
        return -2;

      HKEY sub_key;
      if (load_key (section, sub_key))
        return -3;

      TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
      DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
      // Note we don't increment the index because the
      // enumeration becomes invalid if we change the
      // subkey, which we do when we delete it.  By leaving
      // it 0, we always delete the top entry
      while (::RegEnumKeyEx (sub_key,
                             0,
                             name_buffer,
                             &buffer_size,
                             NULL,
                             NULL,
                             NULL,
                             NULL) == ERROR_SUCCESS)
        {
          remove_section (section, name_buffer, 1);
          buffer_size = ACE_DEFAULT_BUFSIZE;
        }
    }

  if (::RegDeleteKey (base_key, sub_section) != ERROR_SUCCESS)
    return -2;

  return 0;
}

int
ACE_Configuration_Win32Registry::enumerate_values (const ACE_Configuration_Section_Key& key,
                                                   int Index,
                                                   ACE_TString& name,
                                                   VALUETYPE& type)
{
  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
  DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
  DWORD value_type;
  if (::RegEnumValue (base_key,
                      Index,
                      name_buffer,
                      &buffer_size,
                      NULL,
                      &value_type,
                      NULL,
                      NULL) != ERROR_SUCCESS)
    return -2;

  name = name_buffer;

  switch (value_type)
    {
    case REG_BINARY:
      type = BINARY;
      break;
    case REG_SZ:
      type = STRING;
      break;
    case REG_DWORD:
      type = INTEGER;
      break;
    default:
      type = INVALID;
    }

  return 0;
}

int
ACE_Configuration_Win32Registry::enumerate_sections (const ACE_Configuration_Section_Key& key,
                                                     int Index,
                                                     ACE_TString& name)
{
  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
  DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
  if (::RegEnumKeyEx (base_key,
                      Index,
                      name_buffer,
                      &buffer_size,
                      NULL,
                      NULL,
                      NULL,
                      NULL) != ERROR_SUCCESS)
    return -2;

  name = name_buffer;

  return 0;
}

int
ACE_Configuration_Win32Registry::set_string_value (const ACE_Configuration_Section_Key& key,
                                                   const TCHAR* name,
                                                   const ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (::RegSetValueEx (base_key,
                       name,
                       0,
                       REG_SZ,
                       (BYTE *) value.fast_rep (),
                       value.length () + 1) != ERROR_SUCCESS)
    return -2;

  return 0;
}

int
ACE_Configuration_Win32Registry::set_integer_value (const ACE_Configuration_Section_Key& key,
                                                    const TCHAR* name,
                                                    u_int value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (::RegSetValueEx (base_key,
                       name,
                       0,
                       REG_DWORD,
                       (BYTE *) &value,
                       sizeof (value)) != ERROR_SUCCESS)
    return -2;

  return 0;
}

int
ACE_Configuration_Win32Registry::set_binary_value (const ACE_Configuration_Section_Key& key,
                                                   const TCHAR* name,
                                                   const void* data,
                                                   u_int length)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (::RegSetValueEx (base_key,
                       name,
                       0,
                       REG_BINARY,
                       (BYTE*)data,
                       length) != ERROR_SUCCESS)
    return -2;

  return 0;
}

int
ACE_Configuration_Win32Registry::get_string_value (const ACE_Configuration_Section_Key& key,
                                                   const TCHAR* name,
                                                   ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  TCHAR buffer[ACE_DEFAULT_BUFSIZE];
  DWORD length = ACE_DEFAULT_BUFSIZE;
  DWORD type;
  if (::RegQueryValueEx (base_key,
                         name,
                         NULL,
                         &type,
                         (BYTE*)buffer,
                         &length) != ERROR_SUCCESS)
    return -2;

  if (type != REG_SZ)
    return -3;

  value = buffer;
  return 0;
}

int
ACE_Configuration_Win32Registry::get_integer_value (const ACE_Configuration_Section_Key& key,
                                                    const TCHAR* name,
                                                    u_int& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  DWORD length = sizeof (value);
  DWORD type;
  if (::RegQueryValueEx (base_key,
                         name,
                         NULL,
                         &type,
                         (BYTE*)&value,
                         &length) != ERROR_SUCCESS)
    return -2;

  if (type != REG_DWORD)
    return -3;

  return 0;
}

int
ACE_Configuration_Win32Registry::get_binary_value (const ACE_Configuration_Section_Key& key,
                                                   const TCHAR* name,
                                                   void*& data,
                                                   u_int& length)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  unsigned char buffer[ACE_DEFAULT_BUFSIZE];
  DWORD buffer_length = ACE_DEFAULT_BUFSIZE;
  DWORD type;
  if (::RegQueryValueEx (base_key,
                         name,
                         NULL,
                         &type,
                         (BYTE*)&buffer,
                         &buffer_length) != ERROR_SUCCESS)
    return -2;

  if (type != REG_BINARY)
    return -3;

  length = buffer_length;

  char* new_data;
  ACE_NEW_RETURN (new_data, char[length], -4);

  ACE_OS::memcpy (new_data, buffer, length);
  data = new_data;
  return 0;
}

int
ACE_Configuration_Win32Registry::remove_value (const ACE_Configuration_Section_Key& key,
                                               const TCHAR* name)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (::RegDeleteValue (base_key, name) != ERROR_SUCCESS)
    return -2;

  return 0;
}


int
ACE_Configuration_Win32Registry::load_key (const ACE_Configuration_Section_Key& key,
                                           HKEY& hKey)
{
  ACE_Section_Key_Win32* pKey = ACE_dynamic_cast (ACE_Section_Key_Win32*,
                                                  get_internal_key (key));
  if (!pKey)
    return -1;

  hKey = pKey->hKey_;
  return 0;
}

HKEY
ACE_Configuration_Win32Registry::resolve_key (HKEY hKey,
                                              const TCHAR* path,
                                              int create)
{
  const TCHAR* begin = path;
  const TCHAR* end = 0;
  HKEY result = 0;

  // Make a copy of hKey
  if (::RegOpenKey (hKey, NULL, &result) != ERROR_SUCCESS)
    return 0;

  // recurse through the path
  while (1)
    {
      // Detmine the begin/ending of the key name
      end = ACE_OS::strchr (begin, '\\');
      size_t length = end ? (size_t)(end-begin) : ACE_OS::strlen (begin);

      // Make sure length is not 0
      if (!length)
        return 0;

      // Open the key
      ACE_TString key (begin, length);
      HKEY subkey;
      if (::RegOpenKey (result,
                        key.fast_rep (),
                        &subkey) != ERROR_SUCCESS)
        {
          // try creating it
          if (!create || ::RegCreateKeyEx (result,
                                           key.fast_rep (),
                                           NULL,
                                           NULL,
                                           NULL,
                                           KEY_ALL_ACCESS,
                                           NULL,
                                           &subkey,
                                           NULL) != ERROR_SUCCESS)
            {
              // error
              ::RegCloseKey (result);
              return 0;
            }
        }
      // release our open key handle
      ::RegCloseKey (result);
      result = subkey;

      // If end is NULL, we are done, return the result
      if (!end)
        return result;

      begin = end + 1;
    }

  return 0;
}



#endif // WIN_32




///////////////////////////////////////////////////////////////

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (void)
  : type_ (ACE_Configuration::INVALID),
    data_ (0),
    length_ (0)
{
}

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (TCHAR* string)
  : type_ (ACE_Configuration::STRING),
    data_ (string),
    length_ (0)
{
}

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (u_int integer)
  : type_ (ACE_Configuration::INTEGER),
    data_ ((void*) integer),
    length_ (0)
{
}

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (void* data, u_int length)
  : type_ (ACE_Configuration::BINARY),
    data_ (data),
    length_ (length)
{
}

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (const ACE_Configuration_Value_IntId& rhs)
  : type_ (rhs.type_),
    data_ (rhs.data_),
    length_ (rhs.length_)
{
}


ACE_Configuration_Value_IntId::~ACE_Configuration_Value_IntId (void)
{
}

ACE_Configuration_Value_IntId& ACE_Configuration_Value_IntId::operator= (const ACE_Configuration_Value_IntId& rhs)
{
  if (this != &rhs)
    {
      type_ = rhs.type_;
      data_ = rhs.data_;
      length_ = rhs.length_;
    }
  return *this;
}

void
ACE_Configuration_Value_IntId::free (ACE_Allocator* allocator)
{
  switch (type_)
    {
    case ACE_Configuration::STRING:
    case ACE_Configuration::BINARY:

      allocator->free ((void *) (data_));
      break;

    case ACE_Configuration::INTEGER:
    case ACE_Configuration::INVALID:
      // Do nothing
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

ACE_Configuration_ExtId::ACE_Configuration_ExtId (void)
  : name_ (0)
{
}

ACE_Configuration_ExtId::ACE_Configuration_ExtId (const TCHAR* name)
  : name_ (name)
{
}

ACE_Configuration_ExtId::ACE_Configuration_ExtId (const ACE_Configuration_ExtId& rhs)
  : name_ (rhs.name_)
{
}

ACE_Configuration_ExtId::~ACE_Configuration_ExtId (void)
{
}

ACE_Configuration_ExtId& ACE_Configuration_ExtId::operator= (const ACE_Configuration_ExtId& rhs)
{
  if (this != &rhs)
    name_ = rhs.name_;

  return *this;
}

int
ACE_Configuration_ExtId::operator == (const ACE_Configuration_ExtId& rhs) const
{
  return (ACE_OS::strcmp (name_, rhs.name_) == 0);
}

int
ACE_Configuration_ExtId::operator != (const ACE_Configuration_ExtId& rhs) const
{
  return (ACE_OS::strcmp (name_, rhs.name_) != 0);
}

u_long
ACE_Configuration_ExtId::hash (void) const
{
  ACE_TString temp (name_);
  return temp.hash ();
}

const TCHAR*
ACE_Configuration_ExtId::name (void)
{
  return name_;
}

void
ACE_Configuration_ExtId::free (ACE_Allocator* allocator)
{
  allocator->free ((void *) (name_));
}

///////////////////////////////////////////////////////////////////////

ACE_Configuration_Section_IntId::ACE_Configuration_Section_IntId (void)
  : value_hash_map_ (0),
    section_hash_map_ (0)
{
}

ACE_Configuration_Section_IntId::ACE_Configuration_Section_IntId (VALUE_MAP* value_hash_map, SUBSECTION_MAP* section_hash_map)
  : value_hash_map_ (value_hash_map),
    section_hash_map_ (section_hash_map)
{
}

ACE_Configuration_Section_IntId::ACE_Configuration_Section_IntId (const ACE_Configuration_Section_IntId& rhs)
  : value_hash_map_ (rhs.value_hash_map_),
    section_hash_map_ (rhs.section_hash_map_)
{

}

ACE_Configuration_Section_IntId::~ACE_Configuration_Section_IntId ()
{
}

ACE_Configuration_Section_IntId&
ACE_Configuration_Section_IntId::operator= (const ACE_Configuration_Section_IntId& rhs)
{
  if (this != &rhs)
    {
      value_hash_map_ = rhs.value_hash_map_;
      section_hash_map_ = rhs.section_hash_map_;
    }
  return *this;
}

void
ACE_Configuration_Section_IntId::free (ACE_Allocator* allocator)
{
  allocator->free ((void *)(value_hash_map_));
  allocator->free ((void *)(section_hash_map_));
}

ACE_Configuration_Section_Key_Heap::ACE_Configuration_Section_Key_Heap (const TCHAR* path)
  : path_ (0),
    value_iter_ (0),
    section_iter_ (0)
{
  path_ = ACE_OS::strdup (path);
}

ACE_Configuration_Section_Key_Heap::~ACE_Configuration_Section_Key_Heap ()
{
  delete value_iter_;
  delete section_iter_;
  ACE_OS::free (path_);
}

//////////////////////////////////////////////////////////////////////////////

ACE_Configuration_Heap::ACE_Configuration_Heap (void)
  : allocator_ (0),
    index_ (0),
    default_map_size_ (0)
{
  ACE_Configuration_Section_Key_Heap *temp = 0;

  ACE_NEW (temp, ACE_Configuration_Section_Key_Heap (ACE_TEXT ("")));
  root_ = ACE_Configuration_Section_Key (temp);
}

ACE_Configuration_Heap::~ACE_Configuration_Heap (void)
{
  if (allocator_)
    allocator_->sync ();

  delete allocator_;
}

int
ACE_Configuration_Heap::open (int default_map_size)
{
  default_map_size_ = default_map_size;
  // Create the allocator with the appropriate options.
  // The name used for  the lock is the same as one used
  // for the file.
  ACE_NEW_RETURN (this->allocator_,
                  HEAP_ALLOCATOR (),
                  -1);
  return create_index ();
}


int
ACE_Configuration_Heap::open (const TCHAR* file_name,
                              void* base_address,
                              int default_map_size)
{
  default_map_size_ = default_map_size;

  // Make sure that the file name is of the legal length.
  if (ACE_OS::strlen (file_name) >= MAXNAMELEN + MAXPATHLEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

#if !defined (CHORUS)
  ACE_MMAP_Memory_Pool::OPTIONS options (base_address);
#else
  // Use base address == 0, don't use a fixed address.
  ACE_MMAP_Memory_Pool::OPTIONS options (0,
                                         0,
                                         0,
                                         ACE_CHORUS_LOCAL_NAME_SPACE_T_SIZE);
#endif /* CHORUS */

  // Create the allocator with the appropriate options.  The name used
  // for  the lock is the same as one used for the file.
  ACE_NEW_RETURN (this->allocator_,
                  PERSISTENT_ALLOCATOR (file_name,
                                        file_name,
                                        &options),
                  -1);

#if !defined (ACE_LACKS_ACCESS)
  // Now check if the backing store has been created successfully.
  if (ACE_OS::access (file_name, F_OK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("create_index\n")),
                      -1);
#endif /* ACE_LACKS_ACCESS */

  return create_index ();
}

int
ACE_Configuration_Heap::create_index (void)
{
  void *section_index = 0;

  // This is the easy case since if we find hash table in the
  // memory-mapped file we know it's already initialized.
  if (this->allocator_->find (ACE_CONFIG_SECTION_INDEX, section_index) == 0)
    this->index_ = (SECTION_MAP *) section_index;

  // Create a new <index_> (because we've just created a new
  // memory-mapped file).
  else
    {
      size_t index_size = sizeof (SECTION_MAP);
      section_index = this->allocator_->malloc (index_size);

      if (section_index == 0
          || create_index_helper (section_index) == -1
          || this->allocator_->bind (ACE_CONFIG_SECTION_INDEX,
                                     section_index) == -1)
        {
          // Attempt to clean up.
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT("create_index\n")));
          this->allocator_->remove ();
          return -1;
        }
      // Add the root section
      return new_section (ACE_TEXT (""), root_);
    }
  return 0;
}

int
ACE_Configuration_Heap::create_index_helper (void *buffer)
{
  ACE_NEW_RETURN (this->index_,
                  (buffer) SECTION_MAP (this->allocator_),
                  -1);
  return 0;
}

int
ACE_Configuration_Heap::load_key (const ACE_Configuration_Section_Key& key,
                                  ACE_TString& name)
{
  ACE_Configuration_Section_Key_Heap* pKey =
    ACE_dynamic_cast (ACE_Configuration_Section_Key_Heap*,
                      get_internal_key (key));
  if (!pKey)
    return -1;

  name = pKey->path_;
  return 0;
}


int
ACE_Configuration_Heap::add_section (const ACE_Configuration_Section_Key& base,
                                     const TCHAR* sub_section,
                                     ACE_Configuration_Section_Key& result)
{

  ACE_TString section;
  if (load_key (base, section))
    return -1;

  // Find the base section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;

  // See if this section already exists
  ACE_Configuration_ExtId SubSectionExtId (sub_section);
  int ignored = 0;

  if (!IntId.section_hash_map_->find (SubSectionExtId, ignored, allocator_))
    // already exists!
    return -3;

  // Create the new section name
  // only prepend a separater if were not at the root
  if (section.length ())
    section += "\\";

  section += sub_section;

  // Add it to the base section
  TCHAR* pers_name = (TCHAR*)allocator_->malloc ((ACE_OS::strlen (sub_section) + 1) * sizeof (TCHAR));
  ACE_OS::strcpy (pers_name, sub_section);
  ACE_Configuration_ExtId SSExtId (pers_name);
  if (IntId.section_hash_map_->bind (SSExtId, ignored, allocator_))
    {
      allocator_->free (pers_name);
      return -4;
    }
  return (new_section (section, result));
}

int
ACE_Configuration_Heap::new_section (const ACE_TString& section,
                                     ACE_Configuration_Section_Key& result)
{
  // Create a new section and add it to the global list

  // Allocate memory for items to be stored in the table.
  size_t section_len = section.length () + 1;
  TCHAR *ptr = (TCHAR*) this->allocator_->malloc (section_len);

  int return_value = -1;

  if (ptr == 0)
    return -1;
  else
    {
      // Populate memory with data.
      ACE_OS::strcpy (ptr, section.fast_rep ());

      void *value_hash_map = 0;
      size_t map_size = sizeof (VALUE_MAP);
      value_hash_map = this->allocator_->malloc (map_size);

      // If allocation failed ...
      if (value_hash_map == 0)
        return -1;

      // Initialize allocated hash map through placement new.
      if (value_open_helper (default_map_size_, value_hash_map ) == -1)
        {
          this->allocator_->free (value_hash_map );
          return -2;
        }

      // create the section map
      void* section_hash_map = 0;
      map_size = sizeof (SUBSECTION_MAP);
      section_hash_map = this->allocator_->malloc (map_size);

      // If allocation failed
      if (section_hash_map == 0)
        return -1;

      // initialize allocated hash map through placement new
      if (section_open_helper (default_map_size_, section_hash_map) == -1)
        {
          this->allocator_->free (value_hash_map );
          this->allocator_->free (section_hash_map);
          return -2;
        }

      ACE_Configuration_ExtId name (ptr);
      ACE_Configuration_Section_IntId entry ((VALUE_MAP*)value_hash_map ,
                                             (SUBSECTION_MAP*)section_hash_map);

      // Do a normal bind.  This will fail if there's already an
      // entry with the same name.
      return_value = this->index_->bind (name, entry, this->allocator_);

      if (return_value == 1)
        {
          // Entry already existed so bind failed. Free our dynamically
          // allocated memory.
          this->allocator_->free ((void *) ptr);
          return return_value;
        }

      if (return_value == -1)
        // Free our dynamically allocated memory.
        this->allocator_->free ((void *) ptr);
      else
        // If bind() succeed, it will automatically sync
        // up the map manager entry.  However, we must sync up our
        // name/value memory.
        this->allocator_->sync (ptr, section_len);

    }

  // set the result
  ACE_Configuration_Section_Key_Heap *temp;
  ACE_NEW_RETURN (temp, ACE_Configuration_Section_Key_Heap (section.fast_rep ()), -2);
  result = ACE_Configuration_Section_Key (temp);
  return return_value;
}

int
ACE_Configuration_Heap::value_open_helper (size_t hash_table_size,
                                          void *buffer)
{
  ACE_NEW_RETURN (buffer,
                  (buffer) VALUE_MAP (hash_table_size, this->allocator_),
                  -1);
  return 0;
}

int
ACE_Configuration_Heap::section_open_helper (size_t hash_table_size,
                                             void *buffer)
{
  ACE_NEW_RETURN (buffer,
                  (buffer) SUBSECTION_MAP (hash_table_size, this->allocator_),
                  -1);
  return 0;
}

int
ACE_Configuration_Heap::open_section (const ACE_Configuration_Section_Key& base,
                                      const TCHAR* sub_section,
                                      int create,
                                      ACE_Configuration_Section_Key& result)
{
  if (validate_name (sub_section))
    return -1;

  ACE_TString section;
  if (load_key (base, section))
    return -1;

  // Only add the \\ if were not at the root
  if (section.length ())
    section += ACE_TEXT ("\\");

  section += sub_section;

  // resolve the section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    {
      if (!create)
        return -2;

      return add_section (base, sub_section, result);
    }

  ACE_Configuration_Section_Key_Heap *temp;

  ACE_NEW_RETURN (temp, ACE_Configuration_Section_Key_Heap (section.fast_rep ()), -3);
  result = ACE_Configuration_Section_Key (temp);

  return 0;
}

int
ACE_Configuration_Heap::remove_section (const ACE_Configuration_Section_Key& key,
                                        const TCHAR* sub_section,
                                        int recursive)
{
  if (validate_name (sub_section))
    return -1;

  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this key
  ACE_Configuration_ExtId ParentExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId ParentIntId;
  if (index_->find (ParentExtId, ParentIntId, allocator_))
    return -2;// no parent key

  // Find this subkey
  if (section.length ())
    section += ACE_TEXT ("\\");

  section += sub_section;
  ACE_Configuration_ExtId SectionExtId (section.fast_rep ());
  SECTION_ENTRY* section_entry;
  SECTION_HASH* hashmap = index_;
  if (hashmap->find (SectionExtId, section_entry))
    return -2;

  if (recursive)
    {
      ACE_Configuration_Section_Key section;
      if (open_section (key, sub_section, 0, section))
        return -3;

      int index = 0;
      ACE_TString name;
      while (!enumerate_sections (section, index, name))
        {
          if (remove_section (section, name.fast_rep (), 1))
            return -4;

          index++;
        }
    }

  // Now make sure we dont have any subkeys
  if (section_entry->int_id_.section_hash_map_->current_size ())
    return -3;

  // Now remove subkey from parent key
  ACE_Configuration_ExtId SubSExtId (sub_section);
  SUBSECTION_ENTRY* subsection_entry;
  if (((SUBSECTION_HASH*)ParentIntId.section_hash_map_)->
      find (SubSExtId, subsection_entry))
    return -4;

  if (ParentIntId.section_hash_map_->unbind (SubSExtId, allocator_))
    return -5;

  subsection_entry->ext_id_.free (allocator_);

  // Remember the pointers so we can free them after we unbind
  ACE_Configuration_ExtId ExtIdToFree (section_entry->ext_id_);
  ACE_Configuration_Section_IntId IntIdToFree (section_entry->int_id_);

  // iterate over all values and free memory
  VALUE_HASH* value_hash_map = section_entry->int_id_.value_hash_map_;
  VALUE_HASH::ITERATOR value_iter = value_hash_map->begin ();
  while (!value_iter.done ())
    {
      VALUE_ENTRY* value_entry;
      if (!value_iter.next (value_entry))
        return 1;

      value_entry->ext_id_.free (allocator_);
      value_entry->int_id_.free (allocator_);

      value_iter.advance ();
    }

  // remove it
  if (index_->unbind (SectionExtId, allocator_))
    return -5;

  // Free the memory
  ExtIdToFree.free (allocator_);
  IntIdToFree.free (allocator_);

  return 0;
}

int
ACE_Configuration_Heap::enumerate_values (const ACE_Configuration_Section_Key& key,
                                          int index,
                                          ACE_TString& name,
                                          VALUETYPE& type)
{
  ACE_Configuration_Section_Key_Heap* pKey =
    ACE_dynamic_cast (ACE_Configuration_Section_Key_Heap*,
                      get_internal_key (key));
  if (!pKey)
    return -1;

  name = pKey->path_;

  // resolve the section
  ACE_Configuration_ExtId ExtId (pKey->path_);
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;

  // Handle iterator resets
  if (index == 0)
    {
      ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId , ACE_Configuration_Value_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>* hash_map = IntId.value_hash_map_;
      // @@ This zero pointer check is redundant  -Ossama
      // if (pKey->value_iter_)
      delete pKey->value_iter_;

      ACE_NEW_RETURN (pKey->value_iter_, VALUE_HASH::ITERATOR(hash_map->begin()), -3);
    }

  // Get the next entry
  ACE_Hash_Map_Entry<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId>* entry;
  if (!pKey->value_iter_->next (entry))
    return 1;

  // Return the value of the iterator and advance it
  name = entry->ext_id_.name_;
  type = entry->int_id_.type_;
  pKey->value_iter_->advance ();

  return 0;
}

int
ACE_Configuration_Heap::enumerate_sections (const ACE_Configuration_Section_Key& key,
                                            int index,
                                            ACE_TString& name)
{
  // cast to a heap section key
  ACE_Configuration_Section_Key_Heap* pKey =
    ACE_dynamic_cast (ACE_Configuration_Section_Key_Heap*,
                      get_internal_key (key));
  if (!pKey)
    return -1;  // not a heap key!

  // resolve the section
  ACE_Configuration_ExtId ExtId (pKey->path_);
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2; // unknown section

  // Handle iterator resets
  if (index == 0)
    {
      if (pKey->section_iter_)
        delete pKey->section_iter_;

      ACE_NEW_RETURN (pKey->section_iter_, SUBSECTION_HASH::ITERATOR (IntId.section_hash_map_->begin ()), -3);
    }

  // Get the next entry
  ACE_Hash_Map_Entry<ACE_Configuration_ExtId, int>* entry;
  if (!pKey->section_iter_->next (entry))
    return 1;

  // Return the value of the iterator and advance it
  pKey->section_iter_->advance ();
  name = entry->ext_id_.name_;

  return 0;
}

int
ACE_Configuration_Heap::set_string_value (const ACE_Configuration_Section_Key& key,
                                          const TCHAR* name,
                                          const ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  ACE_TString section;
  if (load_key (key, section))
    return -1;

  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;

  ACE_Configuration_ExtId VExtIdFind (name);
  ACE_Configuration_Value_IntId VIntIdFind;
  // See if it exists first
  if (IntId.value_hash_map_->find (VExtIdFind, VIntIdFind, allocator_))
    {
      // it doesn't exist, bind it
      TCHAR* pers_name = (TCHAR*)allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (TCHAR));
      ACE_OS::strcpy (pers_name, name);
      TCHAR* pers_value = (TCHAR*)allocator_->malloc ((value.length () + 1) * sizeof (TCHAR));
      ACE_OS::strcpy (pers_value, value.fast_rep ());
      ACE_Configuration_ExtId VExtId (pers_name);
      ACE_Configuration_Value_IntId VIntId (pers_value);
      if (IntId.value_hash_map_->bind (VExtId, VIntId, allocator_))
        {
          allocator_->free (pers_value);
          allocator_->free (pers_name);
          return -3;
        }
      return 0;
    }
  else
    {
      // Free the old value memory
      VIntIdFind.free (allocator_);
      // Assign a new value
      TCHAR* pers_value = (TCHAR*)allocator_->malloc ((value.length () + 1) * sizeof (TCHAR));
      ACE_OS::strcpy (pers_value, value.fast_rep ());
      VIntIdFind = ACE_Configuration_Value_IntId (pers_value);
    }

  return 0;
}

int
ACE_Configuration_Heap::set_integer_value (const ACE_Configuration_Section_Key& key,
                                           const TCHAR* name,
                                           u_int value)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;  // section does not exist

  // See if it exists first
  ACE_Configuration_ExtId VExtId (name);
  ACE_Configuration_Value_IntId VIntId;
  if (IntId.value_hash_map_->find (VExtId, VIntId, allocator_))
    {
      // it doesn't exist, bind it
      TCHAR* pers_name = (TCHAR*)allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (TCHAR));
      ACE_OS::strcpy (pers_name, name);
      ACE_Configuration_ExtId VExtId (pers_name);
      ACE_Configuration_Value_IntId VIntId (value);
      if (IntId.value_hash_map_->bind (VExtId, VIntId, allocator_))
        return -3;

      return 0;
    }
  else
    {
      // rebind it
      VIntId = ACE_Configuration_Value_IntId (value);
    }
  return 0;
}

int
ACE_Configuration_Heap::set_binary_value (const ACE_Configuration_Section_Key& key,
                                          const TCHAR* name,
                                          const void* data,
                                          u_int length)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;    // section does not exist

  // See if the value exists first
  ACE_Configuration_ExtId VExtIdFind (name);
  ACE_Configuration_Value_IntId VIntIdFind;
  if (IntId.value_hash_map_->find (VExtIdFind, VIntIdFind, allocator_))
    {
      // it doesn't exist, bind it
      TCHAR* pers_name =  (TCHAR*)allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (TCHAR));
      ACE_OS::strcpy (pers_name, name);
      TCHAR* pers_value =  (TCHAR*)allocator_->malloc (length);
      ACE_OS::memcpy (pers_value, data, length);
      ACE_Configuration_ExtId VExtId (pers_name);
      ACE_Configuration_Value_IntId VIntId (pers_value, length);
      if (IntId.value_hash_map_->bind (VExtId, VIntId, allocator_))
        {
          allocator_->free (pers_value);
          allocator_->free (pers_name);
          return -3;
        }
      return 0;
    }
  else
    {
      // it does exist, free the old value memory
      VIntIdFind.free (allocator_);
      // Assign a new value
      TCHAR* pers_value =  (TCHAR*)allocator_->malloc (length);
      ACE_OS::memcpy (pers_value, data, length);
      VIntIdFind = ACE_Configuration_Value_IntId (pers_value, length);
    }

  return 0;
}

int
ACE_Configuration_Heap::get_string_value (const ACE_Configuration_Section_Key& key,
                                          const TCHAR* name,
                                          ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;    // section does not exist

  // See if it exists first
  ACE_Configuration_ExtId VExtId (name);
  ACE_Configuration_Value_IntId VIntId;
  if (IntId.value_hash_map_->find (VExtId, VIntId, allocator_))
    return -3;    // unknown value

  // Check type
  if (VIntId.type_ != ACE_Configuration::STRING)
    return -4;

  // everythings ok, return the data
  value = (TCHAR*)VIntId.data_;
  return 0;
}

int
ACE_Configuration_Heap::get_integer_value (const ACE_Configuration_Section_Key& key,
                                           const TCHAR* name,
                                           u_int& value)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;    // section does not exist


  // See if it exists first
  ACE_Configuration_ExtId VExtId (name);
  ACE_Configuration_Value_IntId VIntId;
  if (IntId.value_hash_map_->find (VExtId, VIntId, allocator_))
    return -3;    // unknown value

  // Check type
  if (VIntId.type_ != ACE_Configuration::INTEGER)
    return -4;

  // Everythings ok, return the data
  value = (u_int)((long)VIntId.data_);
  return 0;
}

int
ACE_Configuration_Heap::get_binary_value (const ACE_Configuration_Section_Key& key,
                                          const TCHAR* name,
                                          void*& data,
                                          u_int& length)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;    // section does not exist

  ACE_Configuration_ExtId VExtId (name);
  ACE_Configuration_Value_IntId VIntId;
  // See if it exists first
  if (IntId.value_hash_map_->find (VExtId, VIntId, allocator_))
    return -3;    // unknown value

  // Check type
  if (VIntId.type_ != ACE_Configuration::BINARY)
    return -4;

  // Make a copy
  ACE_NEW_RETURN (data, char[VIntId.length_], -5);
  ACE_OS::memcpy (data, VIntId.data_, VIntId.length_);
  length = VIntId.length_;
  return 0;
}

int
ACE_Configuration_Heap::remove_value (const ACE_Configuration_Section_Key& key,
                                      const TCHAR* name)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId ExtId (section.fast_rep ());
  ACE_Configuration_Section_IntId IntId;
  if (index_->find (ExtId, IntId, allocator_))
    return -2;    // section does not exist

  // Find it
  ACE_Configuration_ExtId ValueExtId (name);
  VALUE_ENTRY* value_entry;
  if (((VALUE_HASH*)IntId.value_hash_map_)->find (ValueExtId, value_entry))
    return -4;

  // free it
  value_entry->ext_id_.free (allocator_);
  value_entry->int_id_.free (allocator_);

  // Unbind it
  if (IntId.value_hash_map_->unbind (ValueExtId, allocator_))
    return -3;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if defined (ACE_HAS_THREADS)
// ACE_SYNCH_MUTEX should not be used in the template instantiations
// because the resulting template instantiation for the
// single-threaded case already exists in ACE.
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex> >;
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex>;
template class ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex, ACE_Control_Block>;
#endif /* ACE_HAS_THREADS */
template class ACE_Hash_Map_Entry<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId>;
template class ACE_Hash_Map_Entry<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId>;
template class ACE_Hash_Map_Entry<ACE_Configuration_ExtId, int>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, int, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;


// Added to fix problems in SunOS CC5.0
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Value_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Value_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,int,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Section_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Equal_To<ACE_Configuration_ExtId>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Section_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,int,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>;
template class ACE_Hash<ACE_Configuration_ExtId>;

template class ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, int, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_Configuration_ExtId, int, ACE_Null_Mutex>;
template class ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId>;
template class ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId>;
template class ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, int>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if defined (ACE_HAS_THREADS)
// ACE_SYNCH_MUTEX should not be used in the template instantiations
// because the resulting template instantiation for the
// single-threaded case already exists in ACE.
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex> >
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex>
#pragma instantiate ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Thread_Mutex, ACE_Control_Block>
#endif /* ACE_HAS_THREADS */
#pragma instantiate ACE_Hash_Map_Entry<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId>
#pragma instantiate ACE_Hash_Map_Entry<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId>
#pragma instantiate ACE_Hash_Map_Entry<ACE_Configuration_ExtId, int>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_Configuration_ExtId, int, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Value_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Value_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,int,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Section_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Equal_To<ACE_Configuration_ExtId>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,ACE_Configuration_Section_IntId,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_Configuration_ExtId,int,ACE_Hash<ACE_Configuration_ExtId>,ACE_Equal_To<ACE_Configuration_ExtId>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash<ACE_Configuration_ExtId>

#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId, int, ACE_Hash<ACE_Configuration_ExtId>, ACE_Equal_To<ACE_Configuration_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Configuration_ExtId, int, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, ACE_Configuration_Section_IntId>
#pragma instantiate ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, ACE_Configuration_Value_IntId>
#pragma instantiate ACE_Hash_Map_With_Allocator<ACE_Configuration_ExtId, int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
