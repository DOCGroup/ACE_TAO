
//=============================================================================
/**
 *  @file    Bug_3729_Regression_Test.cpp
 *
 *  $Id$
 *
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

struct Multicast_Address
{
  const char *addr_;
  bool is_multicast_;
};

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3729_Regression_Test"));

  int status = 0;     // Innocent until proven guilty

  // Test is_multicast()
  Multicast_Address mcast_addresses[] =
    {
      { "223.0.0.5:23006", false },
      { "224.0.0.0:23006", true },
      { "224.0.0.1:23006", true },
      { "239.255.255.255:23006", true },
      { "240.0.0.0:23006", false },
      { "2.0.0.224", false },
      { 0, false }
    };

  for (int i = 0; mcast_addresses[i].addr_; ++i)
    {
      ACE_INET_Addr addr (mcast_addresses[i].addr_);

      if (addr.is_multicast () != mcast_addresses[i].is_multicast_)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ACE_INET_Addr::is_multicast() for ")
                      ACE_TEXT ("\"%C\" incorrectly returned %d\n"),
                      mcast_addresses[i].addr_,
                      (int)addr.is_multicast ()));
          status = 1;
        }
    }

  ACE_END_TEST;

  return status;
}
