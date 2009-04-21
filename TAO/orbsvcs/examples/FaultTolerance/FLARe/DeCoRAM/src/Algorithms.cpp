// -*- C++ -*-

//=============================================================================
/**
 *  @file    Algorithms.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <iostream>
#include <sstream>
#include "Algorithms.h"

FTRMFF_Algorithm::~FTRMFF_Algorithm ()
{
}

CTT_Algorithm::~CTT_Algorithm ()
{
}

std::ostream & operator<< (std::ostream & ostr, const Task & t)
{
  ostr << "T["
       << t.name
       << "]("
       << t.execution_time
       << ","
       << t.period
       << ","
       << t.sync_time
//       << (t.role == PRIMARY ? ",p" : ",b")
       << ")";

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, const TASK_LIST & t)
{
  if (t.empty ())
    ostr << "empty";

  for (TASK_LIST::const_iterator it = t.begin ();
       it != t.end ();
       ++it)
    {
      ostr << *it << " ";
    }

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, const PROCESSOR_LIST & p)
{
  if (p.empty ())
    ostr << "empty";

  for (PROCESSOR_LIST::const_iterator it = p.begin ();
       it != p.end ();
       ++it)
    {
      ostr << *it << " ";
    }

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const SCHEDULE_PROGRESS_LIST & pl)
{
  for (SCHEDULE_PROGRESS_LIST::const_iterator it = 
         pl.begin ();
       it != pl.end ();
       ++it)
    {
      ostr << it->scheduled_replicas << "x" << it->task << " ";
    }

  return ostr;
}

Task
string_to_task::operator () (const std::string & s)
{
  // this method parses a string of the format:
  // T[<taskname](execution_time,period,synchronization_time)

  std::string::size_type first_comma = s.find_first_of (',');
  std::string::size_type second_comma = s.find (',', first_comma + 1);

  std::stringstream ss_ext (s.substr (s.find_first_of ('(') + 1, 
				      first_comma - 2));
  double execution_time = .0;
  ss_ext >> execution_time;

  std::stringstream ss_period (s.substr (first_comma + 1, 
                                         second_comma - 2));
  double period = .0;
  ss_period >> period;

  std::stringstream ss_synctime (s.substr (second_comma + 1,
                                           s.find_first_of (')') - 2));

  double synctime = .0;
  ss_synctime >> synctime;

  Task t;
  t.name = s.substr (s.find_first_of ('[') + 1, s.find_first_of (']') - 2);
  t.execution_time = execution_time;
  t.period = period;
  t.sync_time = synctime;
  t.role = PRIMARY;
  t.rank = 0;
  
  if (t.name.find ('_') != std::string::npos)
    {
      t.role = BACKUP;
      std::stringstream ss (t.name.substr (t.name.find ('_') + 1));
      ss >> t.rank;
    }

  return t;
}

bool
double_equal::operator () (double d1, double d2)
{
  if (d1 < d2)
    return (d2 - d1) < 0.00001;
  else
    return (d1 - d2) < 0.00001;
}

TaskNamePredicate::TaskNamePredicate (const std::string & task_name)
  : task_name_ (task_name)
{
}

bool
TaskNamePredicate::operator () (const Task & task)
{
  return task_name_.compare (task.name) == 0;
}
