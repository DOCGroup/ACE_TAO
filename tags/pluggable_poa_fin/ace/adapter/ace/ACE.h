// $Id$

#ifndef ACE_ADAPTER_ACE_H
#define ACE_ADAPTER_ACE_H
#include "ace/pre.h"

#include "../../ACE.h"

class ACE_W : public ACE
{
public:
  // @@ Memory Leaks?
  static const wchar_t *compiler_name (void)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE::compiler_name ())); }

  static int get_bcast_addr (ACE_UINT32 &bcast_addr,
                             const wchar_t *hostname = 0,
                             ACE_UINT32 host_addr = 0,
                             ACE_HANDLE handle = ACE_INVALID_HANDLE)
  { return ACE::get_bcast_addr (bcast_addr, ACE_TEXT_WCHAR_TO_CHAR (hostname), host_addr, handle); }

  static ACE_HANDLE handle_timed_open (ACE_Time_Value *timeout, const wchar_t *name, int flags, int perms)
  { return ACE::handle_timed_open (timeout, ACE_TEXT_WCHAR_TO_CHAR (name), flags, perms); }


#if !defined (ACE_HAS_WINCE)
  static wchar_t *strenvdup (const wchar_t *str)
  { return ACE_OS::strdup (ACE_TEXT_CHAR_TO_WCHAR (ACE::strenvdup (ACE_TEXT_WCHAR_TO_CHAR (str)))); }
#endif /* ACE_HAS_WINCE */

  static const wchar_t *execname (const wchar_t *pathname)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE::execname (ACE_TEXT_WCHAR_TO_CHAR (pathname)))); }

  static const wchar_t *basename (const wchar_t *pathname, wchar_t delim = ACE_DIRECTORY_SEPARATOR_CHAR)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE::basename (ACE_TEXT_WCHAR_TO_CHAR (pathname), delim))); }

  static const wchar_t *dirname (const wchar_t *pathname, wchar_t delim = ACE_DIRECTORY_SEPARATOR_CHAR)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE::dirname (ACE_TEXT_WCHAR_TO_CHAR (pathname), delim))); }

  static wchar_t *timestamp (wchar_t date_and_time[], int time_len)
  { 
    char *result;
    char *char_buf;
    ACE_NEW_RETURN (char_buf, char[time_len], 0);

    result = ACE::timestamp (char_buf, time_len);
    ACE_OS::strcpy (date_and_time, ACE_TEXT_CHAR_TO_WCHAR (char_buf));
    delete [] char_buf;
    
    if (result == 0)
      return 0;
    
    // @@ Don't think this is actually the same behavior
    return date_and_time;
  }

  static pid_t fork (const wchar_t *program_name = L"<unknown>", int avoid_zombies = 0)
  { return ACE::fork (ACE_TEXT_WCHAR_TO_CHAR (program_name), avoid_zombies); }

  static int daemonize (const wchar_t pathname[] = L"/",
                        int close_all_handles = ACE_DEFAULT_CLOSE_ALL_HANDLES,
                        const wchar_t program_name[] = L"<unknown>")
  { return ACE::daemonize (ACE_TEXT_WCHAR_TO_CHAR (pathname), 
                           close_all_handles,
                           ACE_TEXT_WCHAR_TO_CHAR (program_name)); }

  static int ldfind (const wchar_t *filename, wchar_t *pathname, size_t maxlen)
  {
    int result;
    char *char_buf;
    ACE_NEW_RETURN (char_buf, char[maxlen], 0);

    result = ACE::ldfind (ACE_TEXT_WCHAR_TO_CHAR (filename), char_buf, maxlen);
    ACE_OS::strcpy (pathname, ACE_TEXT_CHAR_TO_WCHAR (char_buf));
    delete [] char_buf;
    return result;
  }

  static FILE *ldopen (const wchar_t *filename, const wchar_t *type)
  { return ACE::ldopen (ACE_TEXT_WCHAR_TO_CHAR (filename), ACE_TEXT_WCHAR_TO_CHAR (type)); }

  static int get_temp_dir (wchar_t *buffer, size_t buffer_len)
  {
    int result;
    char *char_buf;
    ACE_NEW_RETURN (char_buf, char [buffer_len], -1);
   
    result = ACE::get_temp_dir (char_buf, buffer_len);
    ACE_OS::strcpy (buffer, ACE_TEXT_CHAR_TO_WCHAR (char_buf));
    delete [] char_buf;
    return result;
  }

  static ACE_HANDLE open_temp_file (const wchar_t *name, int mode, int perm = 0)
  { return ACE::open_temp_file (ACE_TEXT_WCHAR_TO_CHAR (name), mode, perm); }

  static int format_hexdump (const char *buffer, int size, wchar_t *obuf, int obuf_sz)
  {
    int result;
    char *char_buf;
    ACE_NEW_RETURN (char_buf, char [obuf_sz], -1);
   
    result = ACE::format_hexdump (buffer, size, char_buf, obuf_sz);
    ACE_OS::strcpy (obuf, ACE_TEXT_CHAR_TO_WCHAR (char_buf));
    delete [] char_buf;
    return result;
  }

  static const wchar_t *sock_error (int error)
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE::sock_error (error))); }

  static void unique_name (const void *object, wchar_t *name, size_t length)
  {
    char *char_buf;
    ACE_NEW (char_buf, char [length]);
   
    ACE::unique_name (object, char_buf, length);
    ACE_OS::strcpy (name, ACE_TEXT_CHAR_TO_WCHAR (char_buf));
    delete [] char_buf;
  }

  static wchar_t nibble2hex (u_int n)
  { return ACE::nibble2hex (n); }
 
  static u_char hex2byte (wchar_t c)
  { return ACE::hex2byte (c); }
};

#include "ace/post.h"
#endif  /* ACE_ADAPTER_ACE_H */
