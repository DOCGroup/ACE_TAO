// -*- C++ -*-

//=============================================================================
/**
 *  @file     FTRMFF_Ranked.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_RANKED_ALGORITHM_H_
#define FTRMFF_RANKED_ALGORITHM_H_

#include "Schedule.h"
#include <memory>

class FTRMFF_Ranked : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Ranked ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Ranked_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Ranked_Algorithm (const PROCESSOR_LIST & processors,
                           unsigned int consistency_level,
                           const std::string & ranking_type = "simple");

  virtual ~FTRMFF_Ranked_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

  const SCHEDULE & schedule () const;

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;
  std::auto_ptr<Ranking_Algorithm> ranking_algorithm_;
};

#endif /* FTRMFF_RANKED_ALGORITHM_H_ */
