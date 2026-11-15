#include "URL_Addr.h"

#if !defined (__ACE_INLINE__)
#include "URL_Addr.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Global_Macros.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"

constexpr ACE_TCHAR http[] = ACE_TEXT ("http:");
constexpr size_t http_size = sizeof (http) / sizeof (http[0]) - 1;
constexpr ACE_TCHAR ftp[] = ACE_TEXT ("ftp:");
constexpr size_t ftp_size = sizeof (ftp) / sizeof (ftp[0]) - 1;
constexpr ACE_TCHAR mailto[] = ACE_TEXT ("mailto:");
constexpr size_t mailto_size = sizeof (mailto) / sizeof (mailto[0]) - 1;

constexpr ACE_TCHAR file[] = ACE_TEXT ("file:");
constexpr size_t file_size = sizeof (file) / sizeof (file[0]) - 1;
constexpr ACE_TCHAR afs[] = ACE_TEXT ("afs:");
constexpr size_t afs_size = sizeof (afs) / sizeof (afs[0]) - 1;
constexpr ACE_TCHAR news[] = ACE_TEXT ("news:");
constexpr size_t news_size = sizeof (news) / sizeof (news[0]) - 1;
constexpr ACE_TCHAR nntp[] = ACE_TEXT ("nntp:");
constexpr size_t nntp_size = sizeof (nntp) / sizeof (nntp[0]) - 1;
constexpr ACE_TCHAR cid[] = ACE_TEXT ("cid:");
constexpr size_t cid_size = sizeof (cid) / sizeof (cid[0]) - 1;
constexpr ACE_TCHAR mid[] = ACE_TEXT ("mid:");
constexpr size_t mid_size = sizeof (mid) / sizeof (mid[0]) - 1;
constexpr ACE_TCHAR wais[] = ACE_TEXT ("wais:");
constexpr size_t wais_size = sizeof (wais) / sizeof (wais[0]) - 1;
constexpr ACE_TCHAR prospero[] = ACE_TEXT ("prospero:");
constexpr size_t prospero_size = sizeof (prospero) / sizeof (prospero[0]) - 1;
constexpr ACE_TCHAR telnet[] = ACE_TEXT ("telnet:");
constexpr size_t telnet_size = sizeof (telnet) / sizeof (telnet[0]) - 1;
constexpr ACE_TCHAR rlogin[] = ACE_TEXT ("rlogin:");
constexpr size_t rlogin_size = sizeof (rlogin) / sizeof (rlogin[0]) - 1;
constexpr ACE_TCHAR tn3270[] = ACE_TEXT ("tn3270:");
constexpr size_t tn3270_size = sizeof (tn3270) / sizeof (tn3270[0]) - 1;
constexpr ACE_TCHAR gopher[] = ACE_TEXT ("gopher:");
constexpr size_t gopher_size = sizeof (gopher) / sizeof (gopher[0]) - 1;

ACE_URL_Addr::ACE_URL_Addr ()
  = default;

void
ACE_URL_Addr::URL_Deleter::operator () (ACE_TCHAR *url) const
{
  ACE_OS::free (url);
}

template <typename String_Owner>
static bool
duplicate_url_string (String_Owner &destination, const ACE_TCHAR *source)
{
  ACE_TCHAR * const copy = ACE_OS::strdup (source);
  if (copy == nullptr)
    {
      errno = ENOMEM;
      return false;
    }

  destination.reset (copy);
  return true;
}

ACE_URL_Addr::~ACE_URL_Addr () = default;

int
ACE_URL_Addr::set (const ACE_URL_Addr& address)
{
  return this->ACE_URL_Addr::string_to_addr (address.url_.get ());
}

int
ACE_URL_Addr::string_to_addr (const ACE_TCHAR *address)
{
  if (address == nullptr)
    {
      this->url_.reset ();
      return -1;
    }

  URL_String url (ACE_OS::strdup (address));
  if (url == nullptr)
    {
      errno = ENOMEM;
      return -1;
    }
  this->url_ = std::move (url);
  return 0;
}

int
ACE_URL_Addr::addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int) const
{
  if (size < ACE_OS::strlen (this->url_.get ()))
    return -1;
  ACE_OS::strcpy (s, this->url_.get ());
  return 0;
}

int
ACE_URL_Addr::accept (ACE_URL_Addr_Visitor* visitor)
{
  return visitor->visit (this);
}

// The factory method to create URL addresses.
ACE_URL_Addr*
ACE_URL_Addr::create_address (const ACE_TCHAR *url)
{
  if (url == nullptr)
    return nullptr;

  ACE_URL_Addr *raw_addr {};
  if (ACE_OS::strncasecmp (http, url, http_size) == 0)
    ACE_NEW_RETURN (raw_addr, ACE_HTTP_Addr (), nullptr);
  else if (ACE_OS::strncasecmp (ftp, url, ftp_size) == 0)
    ACE_NEW_RETURN (raw_addr, ACE_FTP_Addr (), nullptr);
  else if (ACE_OS::strncasecmp (mailto, url, mailto_size) == 0)
    ACE_NEW_RETURN (raw_addr, ACE_Mailto_Addr (), nullptr);
  else
    ACE_NEW_RETURN (raw_addr, ACE_URL_Addr (), nullptr);

  std::unique_ptr<ACE_URL_Addr> addr (raw_addr);
  if (addr->string_to_addr (url) != 0)
    return nullptr;
  return addr.release ();
}

bool
ACE_URL_Addr::known_scheme (const ACE_TCHAR *url)
{
  if (ACE_OS::strncasecmp (http, url, http_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (ftp, url, ftp_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (mailto, url, mailto_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (file, url, file_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (afs, url, afs_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (news, url, news_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (nntp, url, nntp_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (cid, url, cid_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (mid, url, mid_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (wais, url, wais_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (prospero, url, prospero_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (telnet, url, telnet_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (rlogin, url, rlogin_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (tn3270, url, tn3270_size) == 0)
    return true;
  else if (ACE_OS::strncasecmp (gopher, url, gopher_size) == 0)
    return true;

  return false;
}

// ****************************************************************

ACE_URL_Addr_Visitor::~ACE_URL_Addr_Visitor ()
{
}

int
ACE_URL_Addr_Visitor::visit (ACE_URL_Addr*)
{
  return 0;
}

int
ACE_URL_Addr_Visitor::visit (ACE_HTTP_Addr*)
{
  return 0;
}

int
ACE_URL_Addr_Visitor::visit (ACE_FTP_Addr*)
{
  return 0;
}

int
ACE_URL_Addr_Visitor::visit (ACE_Mailto_Addr*)
{
  return 0;
}

// ****************************************************************

ACE_HTTP_Addr::ACE_HTTP_Addr ()
  :  hostname_ (nullptr),
     port_number_ (0),
     path_ (nullptr),
     query_ (nullptr)
{
}

ACE_HTTP_Addr::ACE_HTTP_Addr (const ACE_TCHAR *host_name,
                              const ACE_TCHAR *path,
                              const ACE_TCHAR *query,
                              u_short port)
  :  hostname_ (nullptr),
     port_number_ (0),
     path_ (nullptr),
     query_ (nullptr)
{
  this->set (host_name, path, query, port);
}

ACE_HTTP_Addr::ACE_HTTP_Addr (const ACE_HTTP_Addr &addr)
  :  ACE_URL_Addr (),
     hostname_ (nullptr),
     port_number_ (0),
     path_ (nullptr),
     query_ (nullptr)
{
  this->set (addr);
}

ACE_HTTP_Addr::~ACE_HTTP_Addr ()
  = default;

int
ACE_HTTP_Addr::set (const ACE_TCHAR *host_name,
                    const ACE_TCHAR *path,
                    const ACE_TCHAR *query,
                    u_short port)
{
  if (host_name == nullptr || path == nullptr)
    return -1;

  URL_String hostname;
  URL_String path_copy;
  URL_String query_copy;
  if (!duplicate_url_string (hostname, host_name)
      || !duplicate_url_string (path_copy, path))
    return -1;
  if (query != nullptr)
    {
      if (!duplicate_url_string (query_copy, query))
        return -1;
    }

  this->hostname_ = std::move (hostname);
  this->path_ = std::move (path_copy);
  this->query_ = std::move (query_copy);
  this->port_number_ = port;

  size_t size = this->url_size (1);

  ACE_TCHAR *raw_buffer {};
  ACE_ALLOCATOR_RETURN (raw_buffer,
                        reinterpret_cast<ACE_TCHAR *> (ACE_OS::malloc (size)),
                        -1);
  URL_String buffer (raw_buffer);
  if (this->addr_to_string (buffer.get (), size, 1) == -1)
    return -1;
  this->set_url (buffer.release ());
  return 0;
}

int
ACE_HTTP_Addr::set (const ACE_HTTP_Addr &addr)
{
  if (this == std::addressof (addr))
    return 0;

  if (this->ACE_URL_Addr::set (addr) != 0)
    return -1;
  if (addr.hostname_ != nullptr)
    {
      if (!duplicate_url_string (this->hostname_, addr.hostname_.get ()))
        return -1;
    }
  else
    {
      if (!duplicate_url_string (this->hostname_, ACE_TEXT ("")))
        return -1;
    }
  if (addr.path_ != nullptr)
    {
      if (!duplicate_url_string (this->path_, addr.path_.get ()))
        return -1;
    }
  else
    {
      if (!duplicate_url_string (this->path_, ACE_TEXT ("")))
        return -1;
    }
  this->port_number_ = addr.port_number_;
  if (addr.query_ != nullptr)
    {
      if (!duplicate_url_string (this->query_, addr.query_.get ()))
        return -1;
    }
  else
    this->query_.reset ();
  return 0;
}

void
ACE_HTTP_Addr::clear ()
{
  this->hostname_.reset ();
  this->path_.reset ();
  this->query_.reset ();
}

size_t
ACE_HTTP_Addr::url_size (int flags) const
{
  // Notice that we cannot hard-code the value because the size in
  // wchar's may be different.
  size_t size =
    + sizeof (ACE_TEXT ("http://"))
    + sizeof (ACE_TEXT ("/:?")); // separators

  size_t chars =
    + (this->path_?ACE_OS::strlen (this->path_.get ()):0)
    + (this->query_?ACE_OS::strlen (this->query_.get ()):0);

  if (flags == 0)
    {
      size += sizeof (ACE_TEXT("255.255.255.255"));
    }
  else
    {
      chars += ACE_OS::strlen (this->hostname_.get ());
    }

  if (this->port_number_ != ACE_DEFAULT_HTTP_PORT)
    {
      size += sizeof (ACE_TEXT(":65335"));
    }

  size += chars * sizeof(ACE_TCHAR);

  return size;
}

inline int
path_copy (const ACE_TCHAR *begin,
           const ACE_TCHAR * /* end */,
           ACE_TCHAR *& target,
           const ACE_TCHAR *src)
{
  // Copy one character at a time, if we find a /../ we go back to the
  // previous '/'
  for (; *src != 0; ++src)
    {
      ACE_TCHAR c = *src;

      switch (c)
        {
        case '/':
          if (src[1] == '.' && src[2] == '.' && src[3] == '/')
            {
              while (target != begin && *(--target) != '/')
                {
                  // No action.
                }

              src += 3;
            }
          else if (src[1] == '.' && src[2] == '/')
            {
              src += 2;
            }
          else
            {
              *target = c;
              ++target;
            }
          break;
        default:
          *target = c;
          ++target;
        }
    }
  return 0;
}

ACE_URL_Addr*
ACE_HTTP_Addr::create_relative_address (const ACE_TCHAR *url) const
{
  if (ACE_URL_Addr::known_scheme (url))
    return ACE_URL_Addr::create_address (url);

  ACE_HTTP_Addr *addr {};
  if (url[0] == '/')
    {
      ACE_NEW_RETURN (addr, ACE_HTTP_Addr (this->get_hostname (),
                                           url + 1,
                                           nullptr,
                                           this->get_port_number ()),
                      nullptr);
    }
  else
    {
      const ACE_TCHAR *path = this->get_path ();
      ACE_TCHAR *raw_buffer {};
      size_t n = ACE_OS::strlen (url)
        + ACE_OS::strlen (path)
        + 2;
      ACE_NEW_RETURN (raw_buffer,
                      ACE_TCHAR [n],
                      nullptr);
      std::unique_ptr<ACE_TCHAR[]> buffer (raw_buffer);

      // We copy the contens of <path> into <buf>; but simplifying the
      // path, to avoid infinite loop like:
      // "foo/../foo/../foo/../foo/../foo/index.html"
      //
      ACE_TCHAR *target = buffer.get ();

      // Copy the path
      path_copy (buffer.get (), buffer.get () + n, target, path);

      if (url[0] == '#')
        {
          // Remove any # from the path
          ACE_TCHAR *p = target;

          while (p != buffer.get () && *(--p) != '#')
            {
              // No action.
            }

          if (p != buffer.get ())
            target = p;
        }
      else
        {
          // Go back to the last / to remove the basename.
          while (target != buffer.get () && *(--target) != '/')
            {
              // No action.
            }

          // Go back if we begin with '../'
          while ((url[0] == '.' && url[1] == '.' && url[2] == '/')
                 || (url[0] == '.' && url[1] == '/'))
            {
              if (url[1] == '.')
                {
                  // A ../ go back
                  while (target != buffer.get () && *(--target) != '/')
                    {
                      // No action.
                    }

                  url += 3;
                }
              else
                {
                  // A ./ remove
                  url += 2;
                }
            }

          *target = '/'; ++target;
        }
      // Copy the url
      path_copy (buffer.get (), buffer.get () + n, target, url);
      // null terminate.
      *target = 0;
      ACE_NEW_RETURN (addr, ACE_HTTP_Addr (this->get_hostname (),
                                           buffer.get (),
                                           nullptr,
                                           this->get_port_number ()),
                      nullptr);
    }

  return addr;
}

int
ACE_HTTP_Addr::string_to_addr (const ACE_TCHAR *address)
{
  if (address == nullptr)
    return -1;

  if (ACE_OS::strncasecmp (http, address, http_size) != 0)
    return -1;

  this->clear ();

  // Save the original URL....
  if (this->ACE_URL_Addr::string_to_addr (address) != 0)
    return -1;

  const ACE_TCHAR *string = address;
  string += http_size;
  string += 2; // == strlen ("//");

  // Make a copy of the string to manipulate it.
  if (!duplicate_url_string (this->hostname_, string))
    return -1;

  ACE_TCHAR *path_start = ACE_OS::strchr (this->hostname_.get (), '/');
  if (path_start != nullptr)
    {
      // terminate the host:port substring
      path_start[0] = '\0';
      ++path_start;
      ACE_TCHAR *query_start = ACE_OS::strchr (path_start, '?');
      if (query_start != nullptr)
        {
          query_start[0] = '\0';
          ++query_start;
          if (!duplicate_url_string (this->query_, query_start))
            return -1;
        }
      if (!duplicate_url_string (this->path_, path_start))
        return -1;
    }

  // By now t is null terminated at the start of the path, find the
  // port (if present).
  ACE_TCHAR *port_start = ACE_OS::strchr (this->hostname_.get (), ':');
  this->port_number_ = ACE_DEFAULT_HTTP_PORT;
  if (port_start != nullptr)
    {
      // terminate the ipaddr.
      port_start[0] = '\0';
      ++port_start;
      this->port_number_ = ACE_OS::atoi (port_start);
    }

  return 0;
}

int
ACE_HTTP_Addr::addr_to_string (ACE_TCHAR *buffer,
                               size_t size,
                               int flags) const
{
  if (size < this->url_size (flags))
    return -1;

  if (this->hostname_ == nullptr)
    return -1;

  size_t n = ACE_OS::sprintf (buffer,
                              ACE_TEXT ("http://"));
  if (flags == 0)
    {
      ACE_INET_Addr inet = this->get_inet_address ();
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("%") ACE_TEXT_PRIs, inet.get_host_addr ());
    }
  else
    {
     n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("%") ACE_TEXT_PRIs, this->hostname_.get ());
    }

  if (this->port_number_ != ACE_DEFAULT_HTTP_PORT)
    {
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT (":%d"), this->port_number_);
    }
  if (this->path_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n,
                            ACE_TEXT ("/%") ACE_TEXT_PRIs,
                            this->path_.get ());
    }
  if (this->query_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n,
                            ACE_TEXT ("?%") ACE_TEXT_PRIs,
                            this->query_.get ());
    }
  return 0;
}

int
ACE_HTTP_Addr::accept (ACE_URL_Addr_Visitor *visitor)
{
  return visitor->visit (this);
}

// ****************************************************************

ACE_FTP_Addr::ACE_FTP_Addr ()
  :  user_ (nullptr),
     password_ (nullptr),
     hostname_ (nullptr),
     path_ (nullptr)
{
}

ACE_FTP_Addr::ACE_FTP_Addr (const ACE_TCHAR *host_name,
                            const ACE_TCHAR *path,
                            const ACE_TCHAR *user,
                            const ACE_TCHAR *password)
  :  user_ (nullptr),
     password_ (nullptr),
     hostname_ (nullptr),
     path_ (nullptr)
{
  this->set (host_name, path, user, password);
}

ACE_FTP_Addr::ACE_FTP_Addr (const ACE_FTP_Addr& addr)
  :  ACE_URL_Addr (),
     user_ (nullptr),
     password_ (nullptr),
     hostname_ (nullptr),
     path_ (nullptr)
{
  this->set (addr);
}

ACE_FTP_Addr::~ACE_FTP_Addr ()
  = default;

int
ACE_FTP_Addr::set (const ACE_TCHAR *host_name,
                   const ACE_TCHAR *path,
                   const ACE_TCHAR *user,
                   const ACE_TCHAR *password)
{
  if (host_name == nullptr || path == nullptr)
    return -1;

  URL_String hostname;
  URL_String path_copy;
  URL_String user_copy;
  URL_String password_copy;
  if (!duplicate_url_string (hostname, host_name)
      || !duplicate_url_string (path_copy, path))
    return -1;
  if (user != nullptr)
    {
      if (!duplicate_url_string (user_copy, user))
        return -1;
    }
  if (password != nullptr)
    {
      if (!duplicate_url_string (password_copy, password))
        return -1;
    }

  this->hostname_ = std::move (hostname);
  this->path_ = std::move (path_copy);
  this->user_ = std::move (user_copy);
  this->password_ = std::move (password_copy);

  size_t size = this->url_size (1);

  ACE_TCHAR *raw_buffer {};
  ACE_ALLOCATOR_RETURN (raw_buffer,
                        reinterpret_cast<ACE_TCHAR *> (ACE_OS::malloc (size)),
                        -1);
  URL_String buffer (raw_buffer);
  if (this->addr_to_string (buffer.get (), size, 1) == -1)
    return -1;
  this->set_url (buffer.release ());
  return 0;
}

int
ACE_FTP_Addr::set (const ACE_FTP_Addr& addr)
{
  if (this == std::addressof (addr))
    return 0;

  if (this->ACE_URL_Addr::set (addr) != 0)
    return -1;
  if (!duplicate_url_string (this->hostname_, addr.hostname_.get ())
      || !duplicate_url_string (this->path_, addr.path_.get ()))
    return -1;
  if (addr.user_ != nullptr)
    {
      if (!duplicate_url_string (this->user_, addr.user_.get ()))
        return -1;
    }
  else
    this->user_.reset ();
  if (addr.password_ != nullptr)
    {
      if (!duplicate_url_string (this->password_, addr.password_.get ()))
        return -1;
    }
  else
    this->password_.reset ();
  return 0;
}

void
ACE_FTP_Addr::clear ()
{
  this->hostname_.reset ();
  this->path_.reset ();
  this->user_.reset ();
  this->password_.reset ();
}

size_t
ACE_FTP_Addr::url_size (int flags) const
{
  // Notice that we cannot hard-code the value because the size in
  // wchar's may be different.
  size_t size =
    + sizeof (ACE_TEXT ("ftp://"))
    + sizeof (ACE_TEXT ("@:/")); // separators

  size_t chars =
    + (this->user_?ACE_OS::strlen (this->user_.get ()):0)
    + (this->password_?ACE_OS::strlen (this->password_.get ()):0)
    + (this->path_?ACE_OS::strlen (this->path_.get ()):0);

  if (flags == 0)
    {
      size += sizeof (ACE_TEXT("255.255.255.255"));
    }
  else
    {
      chars += ACE_OS::strlen (this->hostname_.get ());
    }

  size += chars * sizeof(ACE_TCHAR);
  return size;
}

int
ACE_FTP_Addr::addr_to_string (ACE_TCHAR *buffer,
                              size_t size,
                              int flags) const
{
  if (size < this->url_size (flags))
    return -1;

  size_t n = ACE_OS::sprintf (buffer,
                              ACE_TEXT ("ftp://"));

  if (this->user_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("%") ACE_TEXT_PRIs, this->user_.get ());
    }
  if (this->password_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n,
                            ACE_TEXT (":%") ACE_TEXT_PRIs,
                            this->password_.get ());
    }

  if (this->user_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("@"));
    }

  if (flags == 0)
    {
      ACE_INET_Addr inet = this->get_inet_address ();
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("%") ACE_TEXT_PRIs, inet.get_host_addr ());
    }
  else
    {
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("%") ACE_TEXT_PRIs, this->hostname_.get ());
    }
  if (this->path_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n,
                            ACE_TEXT ("/%") ACE_TEXT_PRIs,
                            this->path_.get ());
    }
  return 0;
}

int
ACE_FTP_Addr::string_to_addr (const ACE_TCHAR *address)
{
  if (address == nullptr)
    return -1;
  if (ACE_OS::strncasecmp (ftp, address, ftp_size) != 0)
    return -1;

  this->clear ();
  // Save the original URL....
  if (this->ACE_URL_Addr::string_to_addr (address) != 0)
    return -1;

  const ACE_TCHAR *string = address;
  string += ftp_size;
  string += 2; // == strlen ("//");

  // Make a copy of the string to manipulate it.
  URL_String t (ACE_OS::strdup (string));
  if (t == nullptr)
    {
      errno = ENOMEM;
      return -1;
    }

  ACE_TCHAR *path_start = ACE_OS::strchr (t.get (), '/');
  if (path_start != nullptr)
    {
      // terminate the host:port substring
      path_start[0] = '\0';
      ++path_start;
      if (!duplicate_url_string (this->path_, path_start))
        return -1;
    }

  ACE_TCHAR *host_start = ACE_OS::strchr (t.get (), '@');
  if (host_start != nullptr)
    {
      host_start[0] = '\0';
      ++host_start;
      if (!duplicate_url_string (this->hostname_, host_start))
        return -1;
      ACE_TCHAR *pass_start = ACE_OS::strchr (t.get (), ':');
      if (pass_start != nullptr)
        {
          pass_start[0] = '\0';
          ++pass_start;
          if (!duplicate_url_string (this->password_, pass_start))
            return -1;
        }
      this->user_ = std::move (t);
    }
  else
    {
      this->hostname_ = std::move (t);
    }

  return 0;
}

int
ACE_FTP_Addr::accept (ACE_URL_Addr_Visitor* visitor)
{
  return visitor->visit (this);
}

// ****************************************************************

ACE_Mailto_Addr::ACE_Mailto_Addr ()
  :  user_ (nullptr),
     hostname_ (nullptr),
     headers_ (nullptr)
{
}

ACE_Mailto_Addr::ACE_Mailto_Addr (const ACE_TCHAR *user,
                                  const ACE_TCHAR *hostname,
                                  const ACE_TCHAR *headers)
  :  user_ (nullptr),
     hostname_ (nullptr),
     headers_ (nullptr)
{
  this->set (user, hostname, headers);
}

ACE_Mailto_Addr::ACE_Mailto_Addr (const ACE_Mailto_Addr &addr)
  :  ACE_URL_Addr (addr),
     user_ (nullptr),
     hostname_ (nullptr),
     headers_ (nullptr)
{
  this->set (addr);
}

ACE_Mailto_Addr::~ACE_Mailto_Addr ()
  = default;

int
ACE_Mailto_Addr::set (const ACE_TCHAR *user,
                      const ACE_TCHAR *hostname,
                      const ACE_TCHAR *headers)
{
  if (user == nullptr || hostname == nullptr)
    return -1;

  URL_String user_copy;
  URL_String hostname_copy;
  URL_String headers_copy;
  if (!duplicate_url_string (user_copy, user)
      || !duplicate_url_string (hostname_copy, hostname))
    return -1;
  if (headers != nullptr)
    {
      if (!duplicate_url_string (headers_copy, headers))
        return -1;
    }

  this->user_ = std::move (user_copy);
  this->hostname_ = std::move (hostname_copy);
  this->headers_ = std::move (headers_copy);
  size_t size = this->url_size (1);
  ACE_TCHAR *raw_buffer {};
  ACE_ALLOCATOR_RETURN (raw_buffer,
                        reinterpret_cast<ACE_TCHAR *> (ACE_OS::malloc (size)),
                        -1);
  URL_String buffer (raw_buffer);
  if (this->addr_to_string (buffer.get (), size, 1) == -1)
    return -1;
  this->set_url (buffer.release ());
  return 0;
}

int
ACE_Mailto_Addr::set (const ACE_Mailto_Addr &addr)
{
  if (this == std::addressof (addr))
    return 0;

  if (this->ACE_URL_Addr::set (addr) != 0)
    return -1;
  if (!duplicate_url_string (this->user_, addr.user_.get ())
      || !duplicate_url_string (this->hostname_, addr.hostname_.get ()))
    return -1;
  if (addr.headers_ != nullptr)
    {
      if (!duplicate_url_string (this->headers_, addr.headers_.get ()))
        return -1;
    }
  else
    this->headers_.reset ();
  return 0;
}

void
ACE_Mailto_Addr::clear ()
{
  this->user_.reset ();
  this->hostname_.reset ();
  this->headers_.reset ();
}

size_t
ACE_Mailto_Addr::url_size (int) const
{
  // Notice that we cannot hard-code the value because the size in
  // wchar's may be different.
  size_t size = sizeof (ACE_TEXT ("mailto:"))
    + sizeof (ACE_TEXT ("@?")); // separators

  size_t chars =
    + (this->user_?ACE_OS::strlen (this->user_.get ()):0)
    + (this->hostname_?ACE_OS::strlen (this->hostname_.get ()):0)
    + (this->headers_?ACE_OS::strlen (this->headers_.get ()):0);
  size += chars * sizeof (ACE_TCHAR);

  return size;
}

int
ACE_Mailto_Addr::addr_to_string (ACE_TCHAR *buffer,
                                 size_t size,
                                 int flags) const
{
  if (size < this->url_size (flags))
    return -1;
  if (this->user_ == nullptr || this->hostname_ == nullptr)
    return -1;

  size_t n = ACE_OS::sprintf (buffer,
                              ACE_TEXT ("mailto:%") ACE_TEXT_PRIs
                              ACE_TEXT ("@%") ACE_TEXT_PRIs,
                              this->user_.get (), this->hostname_.get ());
  if (this->headers_ != nullptr)
    {
      n += ACE_OS::sprintf (buffer + n, ACE_TEXT ("?%") ACE_TEXT_PRIs,
                            this->headers_.get ());
    }

  return 0;
}

int
ACE_Mailto_Addr::string_to_addr (const ACE_TCHAR *address)
{
  if (address == nullptr)
    return -1;

  if (ACE_OS::strncasecmp (mailto, address, mailto_size) != 0)
    return -1;

  this->clear ();
  // Save the original URL....
  if (this->ACE_URL_Addr::string_to_addr (address) != 0)
    return -1;

  const ACE_TCHAR *string = address;
  string += mailto_size;

  // Make a copy of the string to manipulate it.
  URL_String t (ACE_OS::strdup (string));
  if (t == nullptr)
    {
      errno = ENOMEM;
      return -1;
    }

  ACE_TCHAR *host_start = ACE_OS::strchr (t.get (), '@');
  if (host_start != nullptr)
    {
      // terminate the host:port substring
      host_start[0] = '\0';
      ++host_start;
      ACE_TCHAR *headers_start = ACE_OS::strchr (host_start, '?');
      if (headers_start != nullptr)
        {
          headers_start[0] = '\0';
          ++headers_start;
          if (!duplicate_url_string (this->headers_, headers_start))
            return -1;
        }
      if (!duplicate_url_string (this->hostname_, host_start))
        return -1;
    }
  else
    return -1;
  this->user_ = std::move (t);

  return 0;
}

int
ACE_Mailto_Addr::accept (ACE_URL_Addr_Visitor* visitor)
{
  return visitor->visit (this);
}
