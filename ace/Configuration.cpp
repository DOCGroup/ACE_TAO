// $Id$
#include "ace/Configuration.h"

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
  // Make a copy of key
  ACE_Configuration_Section_Key current_section = key;
  ACE_TString data (path_in);
  ACE_TCHAR *pData = (ACE_TCHAR *) data.c_str ();
  ACE_Tokenizer parser (pData);
  parser.delimiter_replace ('\\', '\0');
  parser.delimiter_replace ('/', '\0');

  for (ACE_TCHAR *temp = 0; temp = parser.next (); current_section = key_out)
    // Open the section
    if (open_section (current_section,
                      temp,
                      create,
                      key_out))
      return -1;

  return 0;
}

int
ACE_Configuration::validate_name (const ACE_TCHAR *name)
{
  const ACE_TCHAR *pos;

  for (pos = name;
       // Make sure it doesn't contain any invalid characters
       *pos != '\0';
       pos++)
    if (ACE_OS::strchr (ACE_LIB_TEXT ("\\]["), *pos))
      return -1;

  // Make sure its not too long.
  if (pos - name > ACE_Configuration::MAX_NAME_LEN)
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
      ACE_TString header = ACE_LIB_TEXT ("[");
      header += path;
      header += ACE_LIB_TEXT ("]");
      header += ACE_LIB_TEXT (" \n");
      if (ACE_OS::fputs (header.fast_rep (), out) < 0)
        return -1;

      // Write out each value
      int index = 0;
      ACE_TString name;
      VALUETYPE type;
      ACE_TString line;
      ACE_TCHAR int_value[32];
      ACE_TCHAR bin_value[3];
      void* binary_data;
      u_int binary_length;
      ACE_TString string_value;

      while (!enumerate_values (section, index, name, type))
        {
          line = ACE_LIB_TEXT ("\"") + name + ACE_LIB_TEXT ("\"=");
          switch (type)
            {
            case INTEGER:
              {
                u_int value;
                if (get_integer_value (section, name.fast_rep (), value))
                  return -2;

                ACE_OS::sprintf (int_value, ACE_LIB_TEXT ("%08x"), value);
                line += ACE_LIB_TEXT ("dword:");
                line += int_value;
                break;
              }
            case STRING:
              {
                if (get_string_value (section,
                                      name.fast_rep (),
                                      string_value))
                  return -2;

                line += ACE_LIB_TEXT ("\"");
                line += string_value + ACE_LIB_TEXT ("\"");
                break;
              }
#if defined (ACE_WIN32)
            case INVALID:
#endif /* ACE_WIN32 */
            case BINARY:
              {
                // not supported yet - maybe use BASE64 codeing?
                if (get_binary_value (section,
                                      name.fast_rep (),
                                      binary_data,
                                      binary_length))
                  return -2;

                line += ACE_LIB_TEXT ("hex:");

                u_char *ptr = (u_char *) binary_data;
                while (binary_length)
                  {
                    if (ptr != binary_data)
                      line += ACE_LIB_TEXT (",");

                    ACE_OS::sprintf (bin_value, ACE_LIB_TEXT ("%02x"), *ptr);
                    line += bin_value;
                    --binary_length;
                    ++ptr;
                  }
                delete (char *) binary_data;
                break;
              }
            default:
              return -3;
            }

          line += ACE_LIB_TEXT ("\n");
          if (ACE_OS::fputs (line.fast_rep (), out) < 0)
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
        sub_section += ACE_LIB_TEXT ("\\");

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
ACE_Configuration::export_config (const ACE_TCHAR* filename)
{
  FILE *out = ACE_OS::fopen (filename, ACE_LIB_TEXT ("w"));
  if (!out)
    return -1;

  int result = export_section (root_, ACE_LIB_TEXT (""), out);
  ACE_OS::fclose (out);
  return result;
}

int
ACE_Configuration::import_config (const ACE_TCHAR* filename)
{
  FILE* in = ACE_OS::fopen (filename, ACE_LIB_TEXT ("r"));
  if (!in)
    return -1;

  // @@ XXX - change this to a dynamic buffer
  ACE_TCHAR buffer[4096];
  ACE_Configuration_Section_Key section;
  while (ACE_OS::fgets (buffer, 4096, in))
    {
      // Check for a comment
      if (buffer[0] == ACE_LIB_TEXT (';') || buffer[0] == ACE_LIB_TEXT ('#'))
        continue;

      if (buffer[0] == ACE_LIB_TEXT ('['))
        {
          // We have a new section here, strip out the section name
          ACE_TCHAR* end = ACE_OS::strrchr (buffer, ACE_LIB_TEXT (']'));
          if (!end)
            {
              ACE_OS::fclose (in);
              return -3;
            }
          *end = 0;

          if (expand_path (root_, buffer + 1, section, 1))
            {
              ACE_OS::fclose (in);
              return -3;
            }

          continue;
        }

      if (buffer[0] == ACE_LIB_TEXT ('"'))
        {
          // we have a value
          ACE_TCHAR* end = ACE_OS::strchr (buffer+1, '"');
          if (!end)  // no closing quote, not a value so just skip it
            continue;

          // null terminate the name
          *end = 0;
          ACE_TCHAR* name = buffer + 1;
          end+=2;
          // determine the type
          if (*end == '\"')
            {
              // string type
              // truncate trailing "
              ++end;
              ACE_TCHAR* trailing = ACE_OS::strrchr (end, '"');
              if (trailing)
                *trailing = 0;
              if (set_string_value (section, name, end))
                {
                  ACE_OS::fclose (in);
                  return -4;
                }
            }
          else if (ACE_OS::strncmp (end, ACE_LIB_TEXT ("dword:"), 6) == 0)
            {
              // number type
              ACE_TCHAR* endptr = 0;
              u_int value = ACE_OS::strtoul (end + 6, &endptr, 16);
              if (set_integer_value (section, name, value))
                {
                  ACE_OS::fclose (in);
                  return -4;
                }
            }
          else if (ACE_OS::strncmp (end, ACE_LIB_TEXT ("hex:"), 4) == 0)
            {
              // binary type
              u_int string_length = ACE_OS::strlen (end + 4);
              // divide by 3 to get the actual buffer length
              u_int length = string_length / 3;
              u_int remaining = length;
              u_char* data = new u_char[length];
              u_char* out = data;
              ACE_TCHAR* inb = end + 4;
              ACE_TCHAR* endptr = 0;
              while (remaining)
                {
                  u_char charin = (u_char) ACE_OS::strtoul (inb, &endptr, 16);
                  *out = charin;
                  ++out;
                  --remaining;
                  inb += 3;
                }
              if (set_binary_value (section, name, data, length))
                {
                  ACE_OS::fclose (in);
                  return -4;
                }
            }
          else
            // invalid type, ignore
            continue;
        }
    }

  if (ferror (in))
    {
      ACE_OS::fclose (in);
      return -1;
    }

  ACE_OS::fclose (in);
  return 0;
}

const ACE_Configuration_Section_Key&
ACE_Configuration::root_section (void)
{
  return root_;
}

//////////////////////////////////////////////////////////////////////////////

#if defined (WIN32)

static const int ACE_DEFAULT_BUFSIZE = ACE_Configuration::MAX_NAME_LEN + 1;

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
                                               const ACE_TCHAR* sub_section,
                                               int create,
                                               ACE_Configuration_Section_Key& result)
{
  if (validate_name (sub_section))
    return -1;

  HKEY base_key;
  if (load_key (base, base_key))
    return -1;

  HKEY result_key;
  if (ACE_TEXT_RegOpenKeyEx (base_key,
                             sub_section,
                             0,
                             KEY_ALL_ACCESS,
                             &result_key) != ERROR_SUCCESS)
    {
      if (!create)
        return -2;

      if (ACE_TEXT_RegCreateKeyEx (base_key,
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
                                                 const ACE_TCHAR* sub_section,
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

      ACE_TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
      DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
      // Note we don't increment the index because the
      // enumeration becomes invalid if we change the
      // subkey, which we do when we delete it.  By leaving
      // it 0, we always delete the top entry
      while (ACE_TEXT_RegEnumKeyEx (sub_key,
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

  if (ACE_TEXT_RegDeleteKey (base_key, sub_section) != ERROR_SUCCESS)
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

  ACE_TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
  DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
  DWORD value_type;
  if (ACE_TEXT_RegEnumValue (base_key,
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

  ACE_TCHAR name_buffer[ACE_DEFAULT_BUFSIZE];
  DWORD buffer_size = ACE_DEFAULT_BUFSIZE;
  if (ACE_TEXT_RegEnumKeyEx (base_key,
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
                                                   const ACE_TCHAR* name,
                                                   const ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (ACE_TEXT_RegSetValueEx (base_key,
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
                                                    const ACE_TCHAR* name,
                                                    u_int value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (ACE_TEXT_RegSetValueEx (base_key,
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
                                                   const ACE_TCHAR* name,
                                                   const void* data,
                                                   u_int length)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (ACE_TEXT_RegSetValueEx (base_key,
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
                                                   const ACE_TCHAR* name,
                                                   ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  // Get the size of the binary data from windows
  DWORD buffer_length = 0;
  DWORD type;
  if (ACE_TEXT_RegQueryValueEx (base_key,
                                name,
                                NULL,
                                &type,
 (BYTE*)0,
                                &buffer_length) != ERROR_SUCCESS)
    return -2;

  if (type != REG_SZ)
    return -3;

  ACE_TCHAR* buffer;
  ACE_NEW_RETURN (buffer, ACE_TCHAR[buffer_length], -4);

  if (ACE_TEXT_RegQueryValueEx (base_key,
                                name,
                                NULL,
                                &type,
 (BYTE*)buffer,
                                &buffer_length) != ERROR_SUCCESS)
  {
    delete[] buffer;
    return -5;
  }

  value = buffer;
  delete[] buffer;
  return 0;
}

int
ACE_Configuration_Win32Registry::get_integer_value (const ACE_Configuration_Section_Key& key,
                                                    const ACE_TCHAR* name,
                                                    u_int& value)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  DWORD length = sizeof (value);
  DWORD type;
  if (ACE_TEXT_RegQueryValueEx (base_key,
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
                                                   const ACE_TCHAR* name,
                                                   void*& data,
                                                   u_int& length)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  // Get the size of the binary data from windows
  DWORD buffer_length = 0;
  DWORD type;
  if (ACE_TEXT_RegQueryValueEx (base_key,
                                name,
                                NULL,
                                &type,
 (BYTE*)0,
                                &buffer_length) != ERROR_SUCCESS)
    return -2;

  if (type != REG_BINARY)
    return -3;

  length = buffer_length;

  ACE_NEW_RETURN (data, u_char[length], -4);

  if (ACE_TEXT_RegQueryValueEx (base_key,
                                name,
                                NULL,
                                &type,
 (BYTE*)data,
                                &buffer_length) != ERROR_SUCCESS)
  {
    delete[] data;
    data = 0;
    return -5;
  }

  return 0;
}

int ACE_Configuration_Win32Registry::find_value (const ACE_Configuration_Section_Key& key,
                         const ACE_TCHAR* name,
                         VALUETYPE& type_out)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  DWORD buffer_length=0;
  DWORD type;
  int result=ACE_TEXT_RegQueryValueEx (base_key,
                                name,
                                NULL,
                                &type,
                                NULL,
                                &buffer_length);
  if (result != ERROR_SUCCESS)
  {
    return -1;
  }

  switch (type)
  {
  case REG_SZ:
    type_out = STRING;
    break;
  case REG_DWORD:
    type_out = INTEGER;
    break;
  case REG_BINARY:
    type_out = BINARY;
    break;
  default:
    return -1; // unknown type
  }

  return 0;
}

int
ACE_Configuration_Win32Registry::remove_value (const ACE_Configuration_Section_Key& key,
                                               const ACE_TCHAR* name)
{
  if (validate_name (name))
    return -1;

  HKEY base_key;
  if (load_key (key, base_key))
    return -1;

  if (ACE_TEXT_RegDeleteValue (base_key, name) != ERROR_SUCCESS)
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
                                              const ACE_TCHAR* path,
                                              int create)
{
  const ACE_TCHAR* begin = path;
  const ACE_TCHAR* end = 0;
  HKEY result = 0;

  // Make a copy of hKey
  if (::RegOpenKey (hKey, NULL, &result) != ERROR_SUCCESS)
    return 0;

  // recurse through the path
  while (1)
    {
      // Detmine the begin/ending of the key name
      end = ACE_OS::strchr (begin, ACE_LIB_TEXT ('\\'));
      size_t length = end ? (size_t) (end-begin) : ACE_OS::strlen (begin);

      // Make sure length is not 0
      if (!length)
        return 0;

      // Open the key
      ACE_TString key (begin, length);
      HKEY subkey;
      if (ACE_TEXT_RegOpenKey (result,
                               key.fast_rep (),
                               &subkey) != ERROR_SUCCESS)
        {
          // try creating it
          if (!create || ACE_TEXT_RegCreateKeyEx (result,
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
#if !defined (ghs)
  return 0;
#endif // ghs
}

#endif // WIN_32

///////////////////////////////////////////////////////////////

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (void)
  : type_ (ACE_Configuration::INVALID),
    data_ (0),
    length_ (0)
{
}

ACE_Configuration_Value_IntId::ACE_Configuration_Value_IntId (ACE_TCHAR* string)
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
  if (this->type_ == ACE_Configuration::STRING
      || this->type_ == ACE_Configuration::BINARY)
    {
      allocator->free ((void *) (data_));
    }
  // Do nothing in other cases...
}

/////////////////////////////////////////////////////////////////////////////

ACE_Configuration_ExtId::ACE_Configuration_ExtId (void)
  : name_ (0)
{
}

ACE_Configuration_ExtId::ACE_Configuration_ExtId (const ACE_TCHAR* name)
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

const ACE_TCHAR*
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
  allocator->free ((void *) (value_hash_map_));
  allocator->free ((void *) (section_hash_map_));
}

ACE_Configuration_Section_Key_Heap::ACE_Configuration_Section_Key_Heap (const ACE_TCHAR* path)
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
    default_map_size_ (0),
    persistent_ (0)
{
  ACE_Configuration_Section_Key_Heap *temp = 0;

  ACE_NEW (temp, ACE_Configuration_Section_Key_Heap (ACE_LIB_TEXT ("")));
  root_ = ACE_Configuration_Section_Key (temp);
}

ACE_Configuration_Heap::~ACE_Configuration_Heap (void)
{
  if (allocator_)
  {
    allocator_->sync ();
    if (!persistent_)
      allocator_->remove ();
  }
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
  persistent_ = 0;
  return create_index ();
}


int
ACE_Configuration_Heap::open (const ACE_TCHAR* file_name,
                              void* base_address,
                              int default_map_size)
{
  persistent_ = 1;
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
                       ACE_LIB_TEXT ("create_index\n")),
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
                      ACE_LIB_TEXT ("create_index\n")));
          this->allocator_->remove ();
          return -1;
        }
      // Add the root section
      return new_section (ACE_LIB_TEXT (""), root_);
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
                                     const ACE_TCHAR* sub_section,
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
    section += ACE_LIB_TEXT ("\\");

  section += sub_section;

  // Add it to the base section
  ACE_TCHAR* pers_name = (ACE_TCHAR*)allocator_->malloc ((ACE_OS::strlen (sub_section) + 1) * sizeof (ACE_TCHAR));
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
  ACE_TCHAR *ptr = (ACE_TCHAR*) this->allocator_->malloc (section_len * sizeof (ACE_TCHAR));

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
        // If bind () succeed, it will automatically sync
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
                                      const ACE_TCHAR* sub_section,
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
    section += ACE_LIB_TEXT ("\\");

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
                                        const ACE_TCHAR* sub_section,
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
    section += ACE_LIB_TEXT ("\\");

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
  if (( (SUBSECTION_HASH*)ParentIntId.section_hash_map_)->
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
      ACE_Hash_Map_Manager_Ex<ACE_Configuration_ExtId ,
                              ACE_Configuration_Value_IntId,
                              ACE_Hash<ACE_Configuration_ExtId>,
                              ACE_Equal_To<ACE_Configuration_ExtId>,
                              ACE_Null_Mutex>* hash_map = IntId.value_hash_map_;
      delete pKey->value_iter_;

      ACE_NEW_RETURN (pKey->value_iter_, VALUE_HASH::ITERATOR (hash_map->begin ()), -3);
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
      delete pKey->section_iter_;

      ACE_NEW_RETURN (pKey->section_iter_,
                      SUBSECTION_HASH::ITERATOR (IntId.section_hash_map_->begin ()),
                      -3);
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
                                          const ACE_TCHAR* name,
                                          const ACE_TString& value)
{
  if (validate_name (name))
    return -1;

  ACE_TString section;
  if (load_key (key, section))
    return -1;

  ACE_Configuration_ExtId section_ext (section.fast_rep ());
  ACE_Configuration_Section_IntId section_int;
  if (index_->find (section_ext, section_int, allocator_))
    return -2;

  // Get the entry for this item (if it exists)
  VALUE_ENTRY* entry;
  ACE_Configuration_ExtId item_name (name);
  if (section_int.value_hash_map_->VALUE_HASH::find (item_name, entry) == 0)
    {
      // found item, replace it
      // Free the old value
      entry->int_id_.free (allocator_);
      // Allocate the new value in this heap
      ACE_TCHAR* pers_value =
        (ACE_TCHAR*) allocator_->malloc ((value.length () + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_value, value.fast_rep ());
      ACE_Configuration_Value_IntId new_value_int (pers_value);
      entry->int_id_ = new_value_int;
    }
  else
    {
      // it doesn't exist, bind it
      ACE_TCHAR* pers_name =
        (ACE_TCHAR*) allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_name, name);
      ACE_TCHAR* pers_value =
        (ACE_TCHAR*) allocator_->malloc ((value.length () + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_value, value.fast_rep ());
      ACE_Configuration_ExtId item_name (pers_name);
      ACE_Configuration_Value_IntId item_value (pers_value);
      if (section_int.value_hash_map_->bind (item_name, item_value, allocator_))
        {
          allocator_->free (pers_value);
          allocator_->free (pers_name);
          return -3;
        }
      return 0;
    }

  return 0;
}

int
ACE_Configuration_Heap::set_integer_value (const ACE_Configuration_Section_Key& key,
                                           const ACE_TCHAR* name,
                                           u_int value)
{
  if (validate_name (name))
    return -1;

  // Get the section name from the key
  ACE_TString section;
  if (load_key (key, section))
    return -1;

  // Find this section
  ACE_Configuration_ExtId section_ext (section.fast_rep ());
  ACE_Configuration_Section_IntId section_int;
  if (index_->find (section_ext, section_int, allocator_))
    return -2;  // section does not exist

  // Get the entry for this item (if it exists)
  VALUE_ENTRY* entry;
  ACE_Configuration_ExtId item_name (name);
  if (section_int.value_hash_map_->VALUE_HASH::find (item_name, entry) == 0)
    {
      // found item, replace it
      ACE_Configuration_Value_IntId new_value_int (value);
      entry->int_id_ = new_value_int;
    }
  else
    {
      // it doesn't exist, bind it
      ACE_TCHAR* pers_name =
        (ACE_TCHAR*) allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_name, name);
      ACE_Configuration_ExtId item_name (pers_name);
      ACE_Configuration_Value_IntId item_value (value);
      if (section_int.value_hash_map_->bind (item_name, item_value, allocator_))
        {
          allocator_->free (pers_name);
          return -3;
        }
      return 0;
    }

  return 0;
}

int
ACE_Configuration_Heap::set_binary_value (const ACE_Configuration_Section_Key& key,
                                          const ACE_TCHAR* name,
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
  ACE_Configuration_ExtId section_ext (section.fast_rep ());
  ACE_Configuration_Section_IntId section_int;
  if (index_->find (section_ext, section_int, allocator_))
    return -2;    // section does not exist

  // Get the entry for this item (if it exists)
  VALUE_ENTRY* entry;
  ACE_Configuration_ExtId item_name (name);
  if (section_int.value_hash_map_->VALUE_HASH::find (item_name, entry) == 0)
    {
      // found item, replace it
      // Free the old value
      entry->int_id_.free (allocator_);
      // Allocate the new value in this heap
      ACE_TCHAR* pers_value = (ACE_TCHAR*)allocator_->malloc (length);
      ACE_OS::memcpy (pers_value, data, length);
      ACE_Configuration_Value_IntId new_value_int (pers_value, length);
      entry->int_id_ = new_value_int;
    }
  else
    {
      // it doesn't exist, bind it
      ACE_TCHAR* pers_name =
        (ACE_TCHAR*) allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_name, name);
      ACE_TCHAR* pers_value = (ACE_TCHAR*)allocator_->malloc (length);
      ACE_OS::memcpy (pers_value, data, length);
      ACE_Configuration_ExtId item_name (pers_name);
      ACE_Configuration_Value_IntId item_value (pers_value, length);
      if (section_int.value_hash_map_->bind (item_name, item_value, allocator_))
        {
          allocator_->free (pers_value);
          allocator_->free (pers_name);
          return -3;
        }
      return 0;
    }

/*
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
      ACE_TCHAR* pers_name =
        (ACE_TCHAR*) allocator_->malloc ((ACE_OS::strlen (name) + 1) * sizeof (ACE_TCHAR));
      ACE_OS::strcpy (pers_name, name);
      ACE_TCHAR* pers_value = (ACE_TCHAR*)allocator_->malloc (length);
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
      ACE_TCHAR* pers_value = (ACE_TCHAR*)allocator_->malloc (length);
      ACE_OS::memcpy (pers_value, data, length);
      VIntIdFind = ACE_Configuration_Value_IntId (pers_value, length);
    }
*/
  return 0;
}

int
ACE_Configuration_Heap::get_string_value (const ACE_Configuration_Section_Key& key,
                                          const ACE_TCHAR* name,
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
  value = (ACE_TCHAR*)VIntId.data_;
  return 0;
}

int
ACE_Configuration_Heap::get_integer_value (const ACE_Configuration_Section_Key& key,
                                           const ACE_TCHAR* name,
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
  value = (u_int) ((long)VIntId.data_);
  return 0;
}

int
ACE_Configuration_Heap::get_binary_value (const ACE_Configuration_Section_Key& key,
                                          const ACE_TCHAR* name,
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
ACE_Configuration_Heap::find_value (const ACE_Configuration_Section_Key& key,
                         const ACE_TCHAR* name,
                         VALUETYPE& type_out)
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
    return -1;    // section does not exist

  // Find it
  ACE_Configuration_ExtId ValueExtId (name);
  VALUE_ENTRY* value_entry;
  if (( (VALUE_HASH*)IntId.value_hash_map_)->find (ValueExtId, value_entry))
    return -1;  // value does not exist

  type_out = value_entry->int_id_.type_;
  return 0;
}


int
ACE_Configuration_Heap::remove_value (const ACE_Configuration_Section_Key& key,
                                      const ACE_TCHAR* name)
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
  if (( (VALUE_HASH*)IntId.value_hash_map_)->find (ValueExtId, value_entry))
    return -4;

  // free it
  value_entry->ext_id_.free (allocator_);
  value_entry->int_id_.free (allocator_);

  // Unbind it
  if (IntId.value_hash_map_->unbind (ValueExtId, allocator_))
    return -3;

  return 0;
}

int
ACE_Configuration::import_config_as_strings (const ACE_TCHAR* filename)
{
  FILE* in = ACE_OS::fopen (filename, ACE_LIB_TEXT ("r"));
  if (!in)
    return -1;

  // @@ Make this a dynamic size!
  ACE_TCHAR buffer[4096];
  ACE_Configuration_Section_Key section;
  while (ACE_OS::fgets (buffer, sizeof buffer, in))
    {
      // Check for a comment and blank line
      if (buffer[0] == ACE_LIB_TEXT (';')
          || buffer[0] == ACE_LIB_TEXT ('#')
          || buffer[0] == ACE_LIB_TEXT ('\r')
          || buffer[0] == ACE_LIB_TEXT ('\n'))
        continue;

      if (buffer[0] == ACE_LIB_TEXT ('['))
        {
          // We have a new section here, strip out the section name
          ACE_TCHAR* end = ACE_OS::strrchr (buffer, ACE_LIB_TEXT (']'));
          if (!end)
            {
              ACE_OS::fclose (in);
              return -3;
            }
          *end = 0;

          if (expand_path (root_, buffer + 1, section, 1))
            {
              ACE_OS::fclose (in);
              return -3;
            }

          continue;
        }

      // we have a line
      const ACE_TCHAR *name = this->skip_whitespace (buffer);
      if (name)
        {
          ACE_TCHAR *end = (ACE_TCHAR *) ACE_OS::strpbrk (name, ACE_LIB_TEXT ("= \t\n\r"));

          // locate equal sign after name and retrieve value
          const ACE_TCHAR *value = ACE_OS::strrchr (name, ACE_LIB_TEXT ('='));
          if (value)
            {
              value++;  // jump over equal sign
              value = this->skip_whitespace (value);
              ACE_TCHAR *value_end;
              if (value[0] != ACE_LIB_TEXT ('"'))
                value_end = (ACE_TCHAR *) ACE_OS::strpbrk (value, ACE_LIB_TEXT (" \t\n\r"));
              else
                {
                  // double quote delimited allows spaces and tabs in string
                  value++;
                  value_end = (ACE_TCHAR *) ACE_OS::strpbrk (value, ACE_LIB_TEXT ("\"\n\r"));
                }
              if (value_end)
                *value_end = '\0'; // terminate value
            }
          else
            value = ACE_LIB_TEXT ("");

          if (end)
            *end = '\0';     // terminate name now

          if (set_string_value (section, name, value))
            {
              ACE_OS::fclose (in);
              return -4;
            }
        }
    }

  if (ferror (in))
    {
      ACE_OS::fclose (in);
      return -1;
    }

  ACE_OS::fclose (in);
  return 0;
}

const ACE_TCHAR *
ACE_Configuration::skip_whitespace (const ACE_TCHAR *src)
{
  const ACE_TCHAR *cp;

  for (cp = src;
       (*cp != '\0') && ((*cp == ' ') || (*cp == '\t'));
       cp++)
    continue;

  return cp;
}
