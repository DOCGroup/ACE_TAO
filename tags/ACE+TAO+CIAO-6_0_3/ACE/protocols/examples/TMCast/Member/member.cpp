// file      : TMCast/Member/member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_string.h"

#include "ace/TMCast/Group.hpp"

class Args {};

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  try
  {
    if (argc < 4) throw Args ();

    bool receiver (true);

    if (argv[1][0] == 'r') receiver = true;
    else if (argv[1][0] == 's') receiver = false;
    else throw Args ();

    if (!receiver) ACE_OS::srand ((u_int) ACE_OS::time ());

    ACE_INET_Addr address (argv[3]);

    ACE_TMCast::Group group (address, ACE_TEXT_ALWAYS_CHAR (argv[2]));

    if (receiver)
    {
      for (char buffer[256];;)
      {
        group.recv (buffer, sizeof (buffer));

        ACE_DEBUG ((LM_DEBUG, "%s\n", buffer));
      }
    }
    else
    {
      char buffer[256];

      for (unsigned long i = 0; i < 1000; i++)
      {
        // Sleep some random time around 1 sec.

        ACE_UINT64 tmpl = 1000000U;
        unsigned long t =
          static_cast<unsigned long> (((tmpl * ACE_OS::rand ()) / RAND_MAX));

        // ACE_DEBUG ((LM_DEBUG, "sleeping for %u\n", t));

        ACE_OS::sleep (ACE_Time_Value (0, t));

        ACE_OS::sprintf (buffer, "message # %lu", i);

        try
        {
          group.send (buffer, ACE_OS::strlen (buffer) + 1);
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
    status++;
  }
  catch (ACE_TMCast::Group::Failed const&)
  {
    ACE_ERROR ((LM_ERROR,
                "Group failure. Perhaps I am alone in the group.\n"));
    status++;
  }
  catch (ACE_TMCast::Group::InsufficienSpace const&)
  {
    ACE_ERROR ((LM_ERROR, "Insufficient space in receive buffer.\n"));
    status++;
  }
  return status;
}
