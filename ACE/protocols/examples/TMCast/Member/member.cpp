// file      : TMCast/Member/member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_string.h"

#include "ace/TMCast/Group.hpp"

#include <cstdio>
#include <cstring>

class Args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};
  try
  {
    if (argc < 4)
      throw Args {};

    if (argv[1][0] != 'r' && argv[1][0] != 's')
      throw Args {};
    bool const receiver {argv[1][0] == 'r'};

    if (!receiver)
      ACE_OS::srand (static_cast<u_int> (ACE_OS::time ()));

    ACE_INET_Addr const address (argv[3]);

    ACE_TMCast::Group group (address, ACE_TEXT_ALWAYS_CHAR (argv[2]));

    if (receiver)
    {
      for (char buffer[256] {}; ;)
      {
        group.recv (buffer, sizeof (buffer));

        ACE_DEBUG ((LM_DEBUG, "%s\n", buffer));
      }
    }
    else
    {
      char buffer[256] {};
      ACE_UINT64 const usec_per_sec {1000000U};

      for (unsigned long i {}; i < 1000; ++i)
      {
        // Sleep some random time around 1 sec.

        unsigned long const t {
          static_cast<unsigned long> ((usec_per_sec * ACE_OS::rand ()) / RAND_MAX)};

        // ACE_DEBUG ((LM_DEBUG, "sleeping for %u\n", t));

        ACE_OS::sleep (ACE_Time_Value (0, t));

        std::snprintf (buffer, sizeof (buffer), "message # %lu", i);

        try
        {
          group.send (buffer, std::strlen (buffer) + 1);
        }
        catch (ACE_TMCast::Group::Aborted const&)
        {
          ACE_ERROR ((LM_ERROR, "%s has been aborted\n", buffer));
        }
      }
    }
  }
  catch (Args const&)
  {
    ACE_ERROR ((LM_ERROR,
                "Usage: member {r|s} <id> <IPv4 mcast address>:<port>\n"));
    ++status;
  }
  catch (ACE_TMCast::Group::Failed const&)
  {
    ACE_ERROR ((LM_ERROR,
                "Group failure. Perhaps I am alone in the group.\n"));
    ++status;
  }
  catch (ACE_TMCast::Group::InsufficienSpace const&)
  {
    ACE_ERROR ((LM_ERROR, "Insufficient space in receive buffer.\n"));
    ++status;
  }
  return status;
}
