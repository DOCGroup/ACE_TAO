#include "websvcs/lib/URL_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include <memory>

void test_http_addr ();
void test_ftp_addr ();
void test_mailto_addr ();
void test_url_addr ();
bool test_url_buffer_aliasing ();
bool test_ftp_set_components ();
bool test_url_self_set ();
bool test_url_component_aliasing ();

int ACE_TMAIN (int, ACE_TCHAR*[])
{
  if (!test_url_buffer_aliasing ())
    {
      ACE_ERROR ((LM_ERROR, "URL buffer aliasing test failed\n"));
      return 1;
    }
  if (!test_ftp_set_components ())
    {
      ACE_ERROR ((LM_ERROR, "FTP set components test failed\n"));
      return 1;
    }
  if (!test_url_self_set ())
    {
      ACE_ERROR ((LM_ERROR, "URL self-set test failed\n"));
      return 1;
    }
  if (!test_url_component_aliasing ())
    {
      ACE_ERROR ((LM_ERROR, "URL component aliasing test failed\n"));
      return 1;
    }

  test_http_addr ();
  test_ftp_addr ();
  test_mailto_addr ();
  test_url_addr ();
  return 0;
}

bool test_url_buffer_aliasing ()
{
  ACE_HTTP_Addr addr;
  ACE_TCHAR const url[] = ACE_TEXT ("http://www.cs.wustl.edu/");

  return addr.string_to_addr (url) == 0
    && addr.string_to_addr (addr.get_url ()) == 0
    && ACE_OS::strcmp (addr.get_url (), url) == 0;
}

bool test_ftp_set_components ()
{
  ACE_FTP_Addr addr;
  ACE_TCHAR const host[] = ACE_TEXT ("ftp.example.org");
  ACE_TCHAR const path[] = ACE_TEXT ("x");
  ACE_TCHAR const user[] = ACE_TEXT ("a-long-user-name-that-is-longer-than-the-path");
  ACE_TCHAR const password[] = ACE_TEXT ("secret");
  ACE_TCHAR buffer[BUFSIZ];

  if (addr.set (host, path, user, password) != 0
      || ACE_OS::strcmp (addr.get_user (), user) != 0
      || ACE_OS::strcmp (addr.get_passwd (), password) != 0
      || addr.addr_to_string (buffer, BUFSIZ, 1) != 0)
  {
    return false;
  }

  ACE_TCHAR const expected[] =
    ACE_TEXT ("ftp://a-long-user-name-that-is-longer-than-the-path:secret@ftp.example.org/x");
  return ACE_OS::strcmp (buffer, expected) == 0;
}

bool test_url_self_set ()
{
  ACE_HTTP_Addr http;
  ACE_FTP_Addr ftp;
  ACE_Mailto_Addr mailto;

  ACE_TCHAR const http_url[] = ACE_TEXT ("http://example.org/path?query");
  ACE_TCHAR const ftp_url[] = ACE_TEXT ("ftp://user:password@example.org/path");
  ACE_TCHAR const mailto_url[] = ACE_TEXT ("mailto:user@example.org?subject=test");

  return http.string_to_addr (http_url) == 0
    && http.set (http) == 0
    && ACE_OS::strcmp (http.get_url (), http_url) == 0
    && ftp.string_to_addr (ftp_url) == 0
    && ftp.set (ftp) == 0
    && ACE_OS::strcmp (ftp.get_url (), ftp_url) == 0
    && mailto.string_to_addr (mailto_url) == 0
    && mailto.set (mailto) == 0
    && ACE_OS::strcmp (mailto.get_url (), mailto_url) == 0;
}

bool test_url_component_aliasing ()
{
  ACE_HTTP_Addr http;
  ACE_FTP_Addr ftp;
  ACE_Mailto_Addr mailto;
  ACE_TCHAR const http_url[] = ACE_TEXT ("http://example.org/path?query");
  ACE_TCHAR const ftp_url[] = ACE_TEXT ("ftp://user:password@example.org/path");
  ACE_TCHAR const mailto_url[] = ACE_TEXT ("mailto:user@example.org?subject=test");

  return http.string_to_addr (http_url) == 0
    && http.set (http.get_hostname (), http.get_path (), http.get_query (),
                 http.get_port_number ()) == 0
    && ACE_OS::strcmp (http.get_url (), http_url) == 0
    && ftp.string_to_addr (ftp_url) == 0
    && ftp.set (ftp.get_hostname (), ftp.get_path (), ftp.get_user (),
                ftp.get_passwd ()) == 0
    && ACE_OS::strcmp (ftp.get_url (), ftp_url) == 0
    && mailto.string_to_addr (mailto_url) == 0
    && mailto.set (mailto.get_user (), mailto.get_hostname (),
                   mailto.get_headers ()) == 0
    && ACE_OS::strcmp (mailto.get_url (), mailto_url) == 0;
}

#define HTTP_TEST_ARRAY \
    ACE_TEXT("http://www.cs.wustl.edu/"), \
    ACE_TEXT("http://www.cs.wustl.edu/index.html"), \
    ACE_TEXT("http://www.cs.wustl.edu/form?var=foo"), \
    ACE_TEXT("http://www.notexist.com:8080/index.html"), \
    ACE_TEXT("http://www.notexist.com:80/index.html"), \
    ACE_TEXT("ftp://foo"), \
    ACE_TEXT("http://www/?kkk//")

#define FTP_TEST_ARRAY \
    ACE_TEXT("ftp://www.cs.wustl.edu/"), \
    ACE_TEXT("ftp://user@www.cs.wustl.edu/"), \
    ACE_TEXT("ftp://user:pass@www.cs.wustl.edu/"), \
    ACE_TEXT("ftp://user:pass@www.cs.wustl.edu/path"), \
    ACE_TEXT("ftp://www.cs.wustl.edu"), \
    ACE_TEXT("http://www.cs.wustl.edu/index.html")

#define MAILTO_TEST_ARRAY \
    ACE_TEXT("mailto:ace-users@cs.wustl.edu"), \
    ACE_TEXT("mailto:majordomo@cs.wustl.edu?Subject: subscribe ace-users"), \
    ACE_TEXT("mailto:nobody"), \
    ACE_TEXT("http://www.cs.wustl.edu")

#define URL_TEST_ARRAY \
    ACE_TEXT("file:/etc/passwd")

void test_http_addr ()
{
  static ACE_TCHAR const *const addresses[] = {
    HTTP_TEST_ARRAY
  };
  static constexpr int naddresses = sizeof (addresses) / sizeof (addresses[0]);
  for (int i = 0; i < naddresses; ++i)
    {
      ACE_HTTP_Addr addr;
      if (addr.string_to_addr (addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "HTTP[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
          continue;
        }

      ACE_TCHAR buffer[BUFSIZ];
      if (addr.addr_to_string (buffer, BUFSIZ, i % 2) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "HTTP[%d]\n"
                      "    \"%s\"\n"
                      "    <%s>\n"
                      "    <%s>\n",
                      i, addresses[i],
                      addr.get_url (),
                      buffer));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "HTTP[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
        }
    }
}

void test_ftp_addr ()
{
  static ACE_TCHAR const *const addresses[] = {
    FTP_TEST_ARRAY
  };
  static constexpr int naddresses = sizeof (addresses) / sizeof (addresses[0]);
  for (int i = 0; i < naddresses; ++i)
    {
      ACE_FTP_Addr addr;
      if (addr.string_to_addr (addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "FTP[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
          continue;
        }

      ACE_TCHAR buffer[BUFSIZ];
      if (addr.addr_to_string (buffer, BUFSIZ, i % 2) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "FTP[%d]\n"
                      "    \"%s\"\n"
                      "    <%s>\n"
                      "    <%s>\n",
                      i, addresses[i],
                      addr.get_url (),
                      buffer));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "FTP[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
        }
    }
}

void test_mailto_addr ()
{
  static ACE_TCHAR const *const addresses[] = {
    MAILTO_TEST_ARRAY
  };
  static constexpr int naddresses = sizeof (addresses) / sizeof (addresses[0]);
  for (int i = 0; i < naddresses; ++i)
    {
      ACE_Mailto_Addr addr;
      if (addr.string_to_addr (addresses[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Mailto[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
          continue;
        }

      ACE_TCHAR buffer[BUFSIZ];
      if (addr.addr_to_string (buffer, BUFSIZ, i % 2) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Mailto[%d]\n"
                      "    \"%s\"\n"
                      "    <%s>\n"
                      "    <%s>\n",
                      i, addresses[i],
                      addr.get_url (),
                      buffer));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Mailto[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
        }
    }
}

void test_url_addr ()
{
  static ACE_TCHAR const *const addresses[] = {
    HTTP_TEST_ARRAY,
    FTP_TEST_ARRAY,
    MAILTO_TEST_ARRAY,
    URL_TEST_ARRAY
  };
  static constexpr int naddresses = sizeof (addresses) / sizeof (addresses[0]);
  for (int i = 0; i < naddresses; ++i)
    {
      std::unique_ptr<ACE_URL_Addr> const addr {
        ACE_URL_Addr::create_address (addresses[i])
      };
      if (!addr)
        {
          ACE_ERROR ((LM_ERROR,
                      "URL[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
          continue;
        }

      ACE_TCHAR buffer[BUFSIZ];
      if (addr->addr_to_string (buffer, BUFSIZ, i % 2) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "URL[%d]\n"
                      "    \"%s\"\n"
                      "    <%s>\n"
                      "    <%s>\n",
                      i, addresses[i],
                      addr->get_url (),
                      buffer));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "URL[%d]\n"
                      "    \"%s\" ERROR\n",
                      i, addresses[i]));
        }
    }
}

