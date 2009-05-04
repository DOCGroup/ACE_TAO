// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Binary_Search.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_BINARY_SEARCH_ALGORITHM_H_
#define FTRMFF_BINARY_SEARCH_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Binary_Search : public FTRMFF_Algorithm
{
public:
  FTRMFF_Binary_Search (const std::string & scheduling_algorithm);

  virtual ~FTRMFF_Binary_Search ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
private:
  std::string scheduling_algorithm_;
};

class FTRMFF_Binary_Search_Algorithm : 
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Binary_Search_Algorithm (const PROCESSOR_LIST & processors,
                                  unsigned int consistency_level,
                                  const std::string & scheduling_algorithm);

  virtual ~FTRMFF_Binary_Search_Algorithm (void);

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule (void) const;

private:
  // factory method that creates the appropriate algorithm to work
  // with during the search
  FTRMFF_Algorithm_Impl * create_scheduling_algorithm (
    const PROCESSOR_LIST & processors);

  const PROCESSOR_LIST & processors_;
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
  std::string scheduling_algorithm_;
};

#endif /* FTRMFF_BINARY_SEARCH_ALGORITHM_H_ */
