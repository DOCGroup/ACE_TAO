// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Basic_Algorithm.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_BASIC_ALGORITHM_H_
#define FTRMFF_BASIC_ALGORITHM_H_

#include "Schedule.h"

typedef std::auto_ptr <CTT_Algorithm> CTT_PTR;

class FTRMFF_Basic : public FTRMFF_Algorithm
{
public:
  FTRMFF_Basic (CTT_Algorithm & ctt);

  virtual ~FTRMFF_Basic ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
private:
  CTT_Algorithm & ctt_;
};

class FTRMFF_Basic_Algorithm : 
  public std::unary_function <TASK_LIST,
                              SCHEDULING_MAP>
{
public:
  FTRMFF_Basic_Algorithm (const PROCESSOR_LIST & processors,
                          unsigned int consistency_level,
                          CTT_Algorithm & ctt);

  virtual ~FTRMFF_Basic_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  SCHEDULE_PROGRESS_LIST get_unschedulable ();

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  unsigned int consistency_level_;

  CTT_Algorithm & ctt_;
};

#endif /* FTRMFF_BASIC_ALGORITHM_H_ */
