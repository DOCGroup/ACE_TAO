/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Enum_interfaces.cpp
//
// = DESCRIPTION
//     This is a simple test of <ACE::get_ip_interfaces>.  This call
//     retrieves the IP addresses assigned to the host by
//     interrogating the kernel.  Network applications typically
//     assume gethostbyname(uname()) will work, but this is just a
//     convention. It is also problematic if the resolver code
//     (DNS/NIS+...) is misconfigured. This happens more than
//     programmers realize. It is better to find out by asking the
//     kernel for local address assignments. This API is similar to
//     what netstat -ni or ifconfig -a produces on UNIX or ipconfig on
//     Windows NT. In fact, it was by reverse engineering these tools
//     that this api was created.
//
// = AUTHOR
//    Michael R. MacFaden <mrm@cisco.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/INET_Addr.h"

int
main (int, char *[])
{
  ACE_START_TEST ("Enum_Interfaces_Test");

  ACE_INET_Addr *the_addr_array;
  size_t how_many = 0;

  int rc = ACE::get_ip_interfaces (how_many, the_addr_array);

  if (rc != 0)
    ACE_ERROR ((LM_ERROR,
		"%p\n",
		"ACE::get_ip_interfaces failed"));
  else if (how_many == 0)
    ACE_ERROR ((LM_ERROR,
		"No interfaces presently configured in the kernel\n"));
  else
    {
      ACE_DEBUG ((LM_DEBUG, "there are %d interfaces\n", how_many));

      for (size_t i = 0; i < how_many; i++)
	ACE_DEBUG ((LM_DEBUG, "\t%s\n", the_addr_array[i].get_host_addr ()));

      delete [] the_addr_array;
    }

  ACE_END_TEST;
  return rc != 0;	// return 1 if get_ip_interfaces failed
}

