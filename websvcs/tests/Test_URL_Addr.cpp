// $Id$

#include "websvcs/lib/URL_Addr.h"

ACE_RCSID(WEBSVCS_Test, Test_URL_Addr, "$Id$")

void test_http_addr (void);
void test_ftp_addr (void);
void test_url_addr (void);

int main (int, char*[])
{
  test_http_addr ();
  test_ftp_addr ();
  return 0;
}

void test_http_addr (void)
{
  static LPCTSTR addresses[] = {
    ASYS_TEXT("http://www.cs.wustl.edu/"),
    ASYS_TEXT("http://www.cs.wustl.edu/index.html"),
    ASYS_TEXT("http://www.cs.wustl.edu/form?var=foo"),
    ASYS_TEXT("http://www.notexist.com:8080/index.html"),
    ASYS_TEXT("http://www.notexist.com:80/index.html"),
    ASYS_TEXT("ftp://foo"),
    ASYS_TEXT("http://www/?kkk//")
  };
  static int naddresses = sizeof(addresses)/sizeof(addresses[0]);
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
      
      ASYS_TCHAR buffer[BUFSIZ];
      if (addr.addr_to_string (buffer, BUFSIZ, i%2) == 0)
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

void test_ftp_addr (void)
{
  static LPCTSTR addresses[] = {
    ASYS_TEXT("ftp://www.cs.wustl.edu/"),
    ASYS_TEXT("ftp://user@www.cs.wustl.edu/"),
    ASYS_TEXT("ftp://user:pass@www.cs.wustl.edu/"),
    ASYS_TEXT("ftp://user:pass@www.cs.wustl.edu/path"),
    ASYS_TEXT("ftp://www.cs.wustl.edu"),
    ASYS_TEXT("http://www.cs.wustl.edu/index.html"),
  };
  static int naddresses = sizeof(addresses)/sizeof(addresses[0]);
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

      ASYS_TCHAR buffer[BUFSIZ];
      if (addr.addr_to_string (buffer, BUFSIZ, i%2) == 0)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
