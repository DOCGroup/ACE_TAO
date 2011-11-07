// $Id$

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_netdb.h"

#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
#define THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(X) \
  ((X)                                                          \
   ? static_cast<void>(0)                                       \
   : ACE_VERSIONED_NAMESPACE_NAME::__ace_assert(__FILE__, __LINE__, ACE_TEXT_CHAR_TO_TCHAR (#X)))

int
getmacaddress_test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing getmacaddress\n")));

#if defined (ACE_LACKS_NETWORKING)
  return 0;
#else /* ACE_LACKS_NETWORKING */
  struct ACE_OS::macaddr_node_t node;

  ACE_OS::memset (&node, 0x0, sizeof (node));
  int retval = ACE_OS::getmacaddress (&node);
  if (retval == 0)
    {
      if (node.node[0] == 0x0 && node.node[1] == 0x0 && node.node[2] == 0x0 &&
          node.node[3] == 0x0 && node.node[4] == 0x0 && node.node[5] == 0x0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE_OS::getmacaddress() failed to get MAC address\n")));
          ++retval;
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_OS::getmacaddress() returned %d, should be 0\n"),
                  retval));
    }
  return retval;
#endif /* ACE_LACKS_NETWORKING */
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3744_Regression_Test"));

  int status = 0;
  int result;

  if ((result = getmacaddress_test ()) != 0)
      status = result;

  ACE_END_TEST;
  return status;
}
#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
