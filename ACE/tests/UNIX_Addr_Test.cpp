/**
 * @file UNIX_Addr_Test.cpp
 * @brief Performs serveral tests on ACE_UNIX_Addr class.
 * @author Like Ma <likemartinma@gmail.com>
 */
#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/UNIX_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("UNIX_Addr_Test"));

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
  // Default ctor and empty path.
  ACE_UNIX_Addr addr;
  ACE_UNIX_Addr empty("");
  ACE_TEST_ASSERT (addr == empty);
  ACE_TEST_ASSERT (addr.get_type () == AF_UNIX);
  ACE_TEST_ASSERT (addr.get_size () == empty.get_size ());
  ACE_TEST_ASSERT (addr.hash () == empty.hash ());
  ACE_TEST_ASSERT (*addr.get_path_name () == '\0');
  ACE_TEST_ASSERT (*empty.get_path_name () == '\0');

  ACE_TCHAR buf[MAXPATHLEN];
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (*buf == '\0');

  // Set longer path by ACE_UNIX_Addr::set
  int origin = addr.get_size ();
  const char* path = "/tmp/ace.test";
  addr.set (path);
  ACE_TEST_ASSERT (addr.get_size () > origin);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, addr.get_path_name ()) == 0);

  // Set longer path by ACE_UNIX_Addr::string_to_addr
  origin = addr.get_size ();
  path = "/tmp/unix_addr_test";
  addr.string_to_addr (path);
  ACE_TEST_ASSERT (addr.get_size () > origin);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, addr.get_path_name ()) == 0);

  // Set shorter path by ACE_UNIX_Addr::string_to_addr
  origin = addr.get_size ();
  path = "/tmp/test";
  addr.string_to_addr (path);
  ACE_TEST_ASSERT (addr.get_size () < origin);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, addr.get_path_name ()) == 0);

  // Bounds checking
  path = "/tmp/bounds.test";
  addr.set (path);
  ACE_TEST_ASSERT (addr.addr_to_string (nullptr, sizeof (buf)) == -1);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, 0) == -1);

  ACE_UNIX_Addr a1 ("/tmp/testA");
  ACE_UNIX_Addr a2 ("/tmp/testB");
  ACE_TEST_ASSERT (a1 != a2);

  // Test 1: Regular paths with difference at the end
  {
    char path1[108] = "/test/path/";
    char path2[108] = "/test/path/";
    // Fill paths with 'a' up to index 106 (leaving room for null terminator)
    ACE_OS::memset(path1 + 11, 'a', 95);
    ACE_OS::memset(path2 + 11, 'a', 95);
    path1[106] = '1';
    path2[106] = '2';
    path1[107] = '\0';
    path2[107] = '\0';

    ACE_UNIX_Addr addr1(path1);
    ACE_UNIX_Addr addr2(path2);

    // Should detect difference at the last valid character
    ACE_TEST_ASSERT(!(addr1 == addr2));
  }

  // Regular paths comparison
  {
    ACE_UNIX_Addr addr1 ("/tmp/test1");
    ACE_UNIX_Addr addr2 ("/tmp/test1");
    ACE_UNIX_Addr addr3 ("/tmp/test2");

    ACE_TEST_ASSERT (addr1 == addr2);
    ACE_TEST_ASSERT (!(addr1 == addr3));
  }

  {
    ACE_UNIX_Addr addr1 ("/tmp/test1");
    ACE_UNIX_Addr addr2 ("/tmp/test2");

    ACE_TEST_ASSERT (!(addr1 == addr2));
  }

#if defined(ACE_LINUX)
  // Abstract paths comparison
  {
    ACE_UNIX_Addr addr1 ("@test1");
    ACE_UNIX_Addr addr2 ("@test1");
    ACE_UNIX_Addr addr3 ("@test2");

    ACE_TEST_ASSERT (addr1 == addr2);
    ACE_TEST_ASSERT (!(addr1 == addr3));
  }

  // Mixed comparison (abstract vs regular)
  {
    ACE_UNIX_Addr addr1 ("@test1");
    ACE_UNIX_Addr addr2 ("/test1");

    ACE_TEST_ASSERT (!(addr1 == addr2));
  }

  // Bounds checking for abstract path
  path = "@/tmp/bounds.test";
  addr.set (path);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, 1) == -1);

  a1.set ("@/tmp/boundA");
  a2.set ("@/tmp/boundB");
  ACE_TEST_ASSERT (a1 != a2);
  a2.set ("/tmp/boundA");
  ACE_TEST_ASSERT (a1 != a2);
  a1.set ("@/tmp/bound");
  ACE_TEST_ASSERT (a1 != a2);

  // Set abstract path by set.
  path = "@/tmp/ace.test";
  addr.set (path);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (*addr.get_path_name () == '\0');
  ACE_TEST_ASSERT (strcmp (path + 1, addr.get_path_name () + 1) == 0);

  // Set abstract path by string_to_addr.
  path = "@/tmp/unix_addr_test";
  addr.string_to_addr (path);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp (path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (*addr.get_path_name () == '\0');
  ACE_TEST_ASSERT (strcmp (path + 1, addr.get_path_name () + 1) == 0);

  // Test maximum length abstract path
  char max_path[108] = "@";  // sizeof(sun_path) is typically 108
  ACE_OS::memset(max_path + 1, 'a', sizeof(max_path) - 2);
  max_path[sizeof(max_path) - 1] = '\0';
  addr.set(max_path);
  ACE_TEST_ASSERT(addr.addr_to_string(buf, sizeof(buf)) == 0);
  ACE_TEST_ASSERT(strcmp(max_path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);

  // Test comparison of abstract paths
  ACE_UNIX_Addr addr2("@/tmp/ace.test");
  ACE_TEST_ASSERT(addr2 == addr2);
  ACE_TEST_ASSERT(!(addr2 != addr2));

  // Test invalid abstract paths
  ACE_TEST_ASSERT(addr.set("@") == 0);  // Empty abstract path
  ACE_TEST_ASSERT(addr.set("@@/tmp/test") == 0);  // Multiple @ prefixes


  // Test path with embedded null
  char null_path[] = "@/tmp/te\0st";
  ACE_TEST_ASSERT(addr.set(null_path) == 0);
  ACE_TEST_ASSERT(ACE_OS::strcmp(addr.get_path_name() + 1, "/tmp/te") == 0);

  // Abstract paths with difference at the end
  {
    char path1[108] = "@/test/";
    char path2[108] = "@/test/";
    // Fill paths with 'a' up to index 106
    ACE_OS::memset(path1 + 7, 'a', 99);
    ACE_OS::memset(path2 + 7, 'a', 99);
    path1[106] = '1';
    path2[106] = '2';
    path1[107] = '\0';
    path2[107] = '\0';

    ACE_UNIX_Addr addr1(path1);
    ACE_UNIX_Addr addr2(path2);

    // Should detect difference at the last valid character
    ACE_TEST_ASSERT(!(addr1 == addr2));
  }
#endif // ACE_LINUX

#endif // ! ACE_LACKS_UNIX_DOMAIN_SOCKETS

  ACE_END_TEST;

  return 0;
}
// vim: set ts=2 sw=2 sts=2 et:
