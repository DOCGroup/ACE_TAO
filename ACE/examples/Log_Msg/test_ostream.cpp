
//=============================================================================
/**
 *  @file    test_ostream.cpp
 *
 *   This program tests the Log_Msg abstraction wrt writing to
 *   stderr and to a file.
 *
 *  @author Irfan Pyarali <irfan@cse.wustl.edu>
 */
//=============================================================================


#include "ace/OS_main.h"

#include "ace/Log_Msg.h"
#include <cstdio>
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
#include <fstream>
#include <iostream>
#endif


int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // This message should show up in stderr.
  ACE_DEBUG ((LM_DEBUG,
              "first message\n"));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

  // This message should not show up anywhere.
  ACE_DEBUG ((LM_DEBUG,
              "second message\n"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  // This message should not show up anywhere since no ostream has
  // been specified.
  ACE_DEBUG ((LM_DEBUG,
              "third message\n"));

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  // Create a persistent store.
  char const *const filename = "output";
  std::ofstream outfile (filename, std::ios::out | std::ios::trunc);

  // Check for errors.
  if (outfile.bad ())
    return 1;

  // Set the ostream.
  ACE_LOG_MSG->msg_ostream (&outfile);

  // This message should show up in the ostream.
  ACE_DEBUG ((LM_DEBUG,
              "fourth message\n"));
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

  // This message should show up in stderr and the ostream (without
  // ACE_LACKS_IOSTREAM_TOTALLY).
  ACE_DEBUG ((LM_DEBUG,
              "fifth message\n"));

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  std::ifstream infile (filename, std::ios::in);

  if (infile.bad ())
    return 1;

  // This loop should print out the contents of file "output", which should
  // have the strings "fourth\n" and "fifth\n" in them.

  char line[BUFSIZ] {};

  while (infile.getline (line, BUFSIZ, '\n'))
    std::cout << line << '\n';

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
  return 0;
}
