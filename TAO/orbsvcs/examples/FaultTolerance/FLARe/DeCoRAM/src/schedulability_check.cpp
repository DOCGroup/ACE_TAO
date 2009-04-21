// -*- C++ -*-

//=============================================================================
/**
 *  @file    schedulability_check.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>
#include "Schedule.h"
#include "CTT_Enhanced.h"
#include "CTT_Basic.h"
#include <ace/Get_Opt.h>

std::string filename = "test.sd"; // filename of task list input
bool counting_mode = false;
bool average_mode = false;
bool check_overbooking = false;

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("acf:ho"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'f':
      filename = get_opt.opt_arg ();
      break;
    case 'c':
      counting_mode = true;
      break;
    case 'a':
      average_mode = true;
      break;
    case 'o':
      check_overbooking = true;
      break;
    case 'h':
    default:
      std::cerr << "usage: scheck "
                << "\t-a" << std::endl
                << "\t-c" << std::endl
                << "\t-f <filename>" << std::endl;
      return 1;
  }

  return 0;
}


int main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  std::ifstream ifile;
  ifile.open (filename.c_str ());

  SCHEDULE schedule = read_schedule (ifile);
  
  ifile.close ();

  if (counting_mode)
    {
      std::cout << processor_usage (schedule) << std::endl;
    }
  else if (average_mode)
    {
      for (SCHEDULE::iterator it = schedule.begin ();
           it != schedule.end ();
           ++it)
        {
          if (it->first.compare ("Unknown") != 0)
            std::cout << it->first << ":" << std::endl;

          std::list <double> utilization;

          for (TASK_LIST::iterator t_it = it->second.begin ();
               t_it != it->second.end ();
               ++t_it)
            {
              utilization.push_back (t_it->execution_time / t_it->period);
            }

          std::cout << "minimum utilization = " 
                    << *std::min_element (utilization.begin (),
                                          utilization.end ()) * 100 << " %"
                    << std::endl;

          std::cout << "maximum utilization = " 
                    << *std::max_element (utilization.begin (),
                                          utilization.end ()) * 100 << " %"
                    << std::endl;

          std::cout << "average utilization = " 
                    << (std::accumulate (utilization.begin (),
                                         utilization.end (),
                                         0.0) / utilization.size ()) * 100 << " %"
                    << std::endl;
          
        }
    }
  else
    {
      std::auto_ptr <CTT_Algorithm> ctt;
      if (check_overbooking)
        ctt.reset (new CTT_Basic ());
      else
        ctt.reset (new CTT_Enhanced ());

      int status = 0;
      double wcrt = 0;
      double period = 0;

      for (SCHEDULE::iterator it = schedule.begin ();
           it != schedule.end ();
           ++it)
        {
          std::cout << it->first << ": ";
          wcrt = (*ctt) (it->second);

          if (wcrt > 0)
            {
              period = it->second.back ().period;

              std::cout << wcrt << " / "
                        << period;
            }
          else
            {
              std::cout << "not schedulable";
            }
          std::cout << std::endl;

          if ((wcrt > period) || (wcrt == 0))
            ++status;
        }
      
        return status;
    } // end else counting_mode not true

  return 0;
}
