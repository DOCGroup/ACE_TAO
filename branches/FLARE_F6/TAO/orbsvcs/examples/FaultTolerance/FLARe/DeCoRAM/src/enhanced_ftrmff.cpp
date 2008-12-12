// -*- C++ -*-

//=============================================================================
/**
 *  @file    enhanced_ftrmff.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include <ace/Get_Opt.h>
#include "FTRMFF_Enhanced.h"

std::string filename = "test.sd"; // filename of task list input
unsigned int m = 4; // number of processors
unsigned int c = 2; // consitency level

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)
    filename = argv[1];

  if (argc > 2)
    m = atoi (argv[2]);

  if (argc > 3)
    c = atoi (argv[3]);

  TASK_LIST tasks;
  PROCESSOR_LIST procs;
  for (unsigned int i = 1; i <= m; ++i)
    {
      std::stringstream ss;
      ss << "P";
      if (i < 10)
        ss << "00";
      else if (i < 100)
        ss << "0";
      ss << i;
      procs.push_back (ss.str ());
    }

  std::ifstream ifile;
  ifile.open (filename.c_str ());

  std::transform (
    std::istream_iterator <std::string> (ifile),
    std::istream_iterator <std::string> (),
    std::inserter <TASK_LIST> (tasks,
                               tasks.begin ()),
    string_to_task ());
  
  ifile.close ();

  FTRMFF_Enhanced ftrmff;

  FTRMFF_Input input;
  input.tasks = tasks;
  input.processors = procs;
  input.backup_count = c;
  FTRMFF_Output result = ftrmff (input);

#if 0
  for (SCHEDULING_MAP::iterator it = result.schedule.begin ();
       it != result.schedule.end ();
       ++it)
    {
      std::cout << it->first << " -> " 
                << it->second.processor 
                << " (" << it->second.priority << ")" 
                << std::endl;
    }
#endif

  if (result.unscheduled_tasks.size () > 0)
    {
      std::cout << "Could not schedule :"
                << std::endl
                << result.unscheduled_tasks
                << std::endl;

      return 1;
    }

  return 0;
}

