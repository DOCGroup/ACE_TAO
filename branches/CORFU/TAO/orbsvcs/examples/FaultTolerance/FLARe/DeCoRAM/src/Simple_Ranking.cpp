// -*- C++ -*-

//=============================================================================
/**
 *  @file    Simple_Ranking.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "Simple_Ranking.h"
#include <algorithm>

Simple_Ranking::~Simple_Ranking ()
{
}

unsigned long 
Simple_Ranking::operator () (SCHEDULE_RESULT_LIST & result_list,
                             const SCHEDULE & /*schedule*/)
{
  std::sort (result_list.begin (),
             result_list.end (),
             WCRTComparison ());

  unsigned int rank = 0;
  unsigned int scheduled_backups = 0;

  for (SCHEDULE_RESULT_LIST::iterator r_it = result_list.begin ();
       r_it != result_list.end ();
       ++r_it)
    {
      // abort if a tasks happens to be not schedulable
      if (r_it->wcrt <= .0)
        continue;

      Task t = r_it->task;

      // make the first entry the primary
      if (scheduled_backups == 0)
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
      ++scheduled_backups;
    }  

  return scheduled_backups;
}

