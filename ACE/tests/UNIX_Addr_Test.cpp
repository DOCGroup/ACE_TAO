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
  ACE_TEST_ASSERT (strcmp(path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp(path, addr.get_path_name ()) == 0);

  // Set longer path by ACE_UNIX_Addr::string_to_addr
  origin = addr.get_size ();
  path = "/tmp/unix_addr_test";
  addr.string_to_addr (path);
  ACE_TEST_ASSERT (addr.get_size () > origin);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp(path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp(path, addr.get_path_name ()) == 0);

  // Set shorter path by ACE_UNIX_Addr::string_to_addr
  origin = addr.get_size ();
  path = "/tmp/test";
  addr.string_to_addr (path);
  ACE_TEST_ASSERT (addr.get_size () < origin);
  ACE_TEST_ASSERT (addr.addr_to_string (buf, sizeof (buf)) == 0);
  ACE_TEST_ASSERT (strcmp(path, ACE_TEXT_ALWAYS_CHAR(buf)) == 0);
  ACE_TEST_ASSERT (strcmp(path, addr.get_path_name ()) == 0);
#endif // ! ACE_LACKS_UNIX_DOMAIN_SOCKETS

  ACE_END_TEST;

  return 0;
}
// vim: set ts=2 sw=2 sts=2 et:
