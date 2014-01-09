// -*- C++ -*-

// Driver program for the gperf hash function generator.

/**
 * $Id$
 *
 * Copyright (C) 1989 Free Software Foundation, Inc.
 * written by Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *
 * This file is part of GNU GPERF.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "Gen_Perf.h"
#include "Options.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Sets the Options.
  if (option.parse_args (argc, argv) == -1)
    return 1;

  time_t clock;
  ACE_OS::time (&clock);
  struct tm *tm = ACE_OS::localtime (&clock);

  if (option[DEBUGGING])
    ACE_OS::printf ("/* starting time is %d:%02d:%02d */\n",
                    tm->tm_hour,
                    tm->tm_min,
                    tm->tm_sec);

  // Initializes the key word list.
  Gen_Perf gperf;

  // Generates and prints the gperf hash table.  Don't use exit here,
  // it skips the destructors.
  int const status = gperf.run ();

  ACE_OS::time (&clock);
  tm = ACE_OS::localtime (&clock);

  if (option[DEBUGGING])
    ACE_OS::printf ("/* ending time is %d:%02d:%02d */\n",
                    tm->tm_hour,
                    tm->tm_min,
                    tm->tm_sec);
  return status;
}
