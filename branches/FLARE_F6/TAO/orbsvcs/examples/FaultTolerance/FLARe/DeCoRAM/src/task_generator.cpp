// -*- C++ -*-

//=============================================================================
/**
 *  @file    task_generator.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include "Algorithms.h"
#include <ace/Get_Opt.h>

std::string filename = "test.sd"; // filename of task list output
unsigned long task_number = 10;
unsigned long load_interval_number = 5;
double minimum_load = 0.0;
double maximum_load = 0.50;
double statesync_lower_bound = 0.10; // proportional to the worst case
double statesync_upper_bound = 0.15; // execution time
double period_lower_bound = 1.0; // 1 ms
double period_upper_bound = 1000.0; // 1 s

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:n:i:m:o:p:l:s:z:h"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'f':
      filename = get_opt.opt_arg ();
      break;
    case 'n':
      task_number = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'i':
      load_interval_number = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'o':
      minimum_load = atof (get_opt.opt_arg ());
      break;
    case 'm':
      maximum_load = atof (get_opt.opt_arg ());
      break;
    case 'p':
      period_upper_bound = atof (get_opt.opt_arg ());
      break;
    case 'l':
      period_lower_bound = atof (get_opt.opt_arg ());
      break;
    case 's':
      statesync_upper_bound = atof (get_opt.opt_arg ());
      break;
    case 'z':
      statesync_lower_bound = atof (get_opt.opt_arg ());
      break;
    case 'h':
    default:
      std::cerr << "usage: tgen "
        //                << "\t-f <filename>" << std::endl
                << "\t-n <task_number>" << std::endl
                << "\t-i <load_interval_number>" << std::endl
                << "\t-o <minimum_load>" << std::endl
                << "\t-m <maximum_load>" << std::endl
                << "\t-p <period_upper_bound>" << std::endl
                << "\t-l <period_lower_bound>" << std::endl
                << "\t-s <statesync_upper_bound>" << std::endl
                << "\t-z <statesync_lower_bound>" << std::endl
                << std::endl;
      return 1;
  }

  return 0;
}

double random_value (double lower_bound, double upper_bound)
{
  return lower_bound + 
    (rand()/(double)RAND_MAX) * (upper_bound - lower_bound);
}

int main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  // initialize random number generator
  srand ((long) time (0));

  TASK_LIST tasks;
  
  unsigned long interval_size = task_number / load_interval_number;
  for (unsigned long i = 0; i < load_interval_number; ++i)
    for (unsigned long j = 0; j < interval_size; ++j)
      {
        std::stringstream name;
        name << "T" << i * interval_size + j;

        double period = 0;
        if (i == 0)
          {
            period = random_value (period_lower_bound,
                                   period_upper_bound);
          }
        else
          {
            period = tasks[j].period;
          }
        
        double execution_time = 
          period * random_value (minimum_load, 
                                 ((i+1) * maximum_load / 
                                  (double) load_interval_number));
        
        double state_sync_time =
          execution_time * random_value (statesync_lower_bound,
                                         statesync_upper_bound);

        Task task = {name.str (),
                     execution_time,
                     period,
                     state_sync_time,
                     PRIMARY,
                     0};
                        
        tasks.push_back (task);
      } // end for each interval

  for (TASK_LIST::const_iterator it = tasks.begin ();
       it != tasks.end ();
       ++it)
    {
      if (it != tasks.begin ())
        std::cout << std::endl;
      std::cout << *it;
    }

  return 0;
}
