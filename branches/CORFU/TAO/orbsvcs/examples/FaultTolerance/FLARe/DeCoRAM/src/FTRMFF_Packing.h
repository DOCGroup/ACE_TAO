// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Packing.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_PACKING_ALGORITHM_H_
#define FTRMFF_PACKING_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Packing : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Packing ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

typedef std::vector<TASK_LIST> TASK_LISTS;

class FTRMFF_Packing_Algorithm : public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Packing_Algorithm (const PROCESSOR_LIST & processors,
                            unsigned int consistency_level);

  virtual ~FTRMFF_Packing_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  void copy_tasks (unsigned long primary_end,
                   unsigned long number);

  void merge_processors (unsigned long primary_processors);

  bool merge_check (const TASK_LISTS & lists);

  TASK_LIST merge_lists (const TASK_LISTS & lists);

private:
  SCHEDULE schedule_;
};

#endif /* FTRMFF_PACKING_ALGORITHM_H_ */
