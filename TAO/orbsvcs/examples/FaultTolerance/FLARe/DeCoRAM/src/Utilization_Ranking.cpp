// -*- C++ -*-

//=============================================================================
/**
 *  @file    Utilization_Ranking.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "Utilization_Ranking.h"

class WCRTOrdering : std::binary_function <ScheduleResult,
                                           ScheduleResult,
                                           bool>
{
public:
  bool operator () (const ScheduleResult & result1,
                    const ScheduleResult & result2)
  {
    return result1.wcrt < result2.wcrt;
  }
};

Utilization_Ranking::~Utilization_Ranking ()
{
}

unsigned long
Utilization_Ranking::operator () (SCHEDULE_RESULT_LIST & result_list,
                                  const SCHEDULE & schedule)
{
  unsigned long scheduled_replicas;

  for (SCHEDULE_RESULT_LIST::iterator it = result_list.begin ();
       it != result_list.end ();
       ++it)
    {
      // if task was unschedulable, don't add it
      if (it->wcrt <= .0)
        continue;

      // calculate worst case response time of each processor in
      // failure free situation and overwrite the entries value
      SCHEDULE::const_iterator proc_it = 
        schedule.find (it->processor);

      if (proc_it == schedule.end ())
        {
          std::cerr << "OOOPS" << std::endl;
          continue;
        }

      it->wcrt = ctt_ (proc_it->second);

      // count how many replicas were scheduled
      ++scheduled_replicas;
    }

  // sort the entries according to their wcrt value
  std::sort (result_list.begin (),
             result_list.end (),
             WCRTOrdering ());
  
  // make the first entry a primary and the rest backups with
  // ascending rank

  int rank = 0;
  for (SCHEDULE_RESULT_LIST::iterator r_it = result_list.begin ();
       r_it != result_list.end ();
       ++r_it)
    {
      Task t = r_it->task;
      
      // make the first entry the primary
      if (r_it == result_list.begin ())
        {
          t.rank = 0;
          t.role = PRIMARY;
        }
      else
        {
          t.rank = ++rank;
          std::stringstream ss;
          ss << t.name << "_" << t.rank;
          t.name = ss.str ();
          t.role = BACKUP;
        }
      r_it->task = t;
    }  

  return scheduled_replicas;
}
