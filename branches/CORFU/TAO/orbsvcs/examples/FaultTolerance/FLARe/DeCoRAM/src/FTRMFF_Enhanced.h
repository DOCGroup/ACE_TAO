// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Enhanced.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_ENHANCED_ALGORITHM_H_
#define FTRMFF_ENHANCED_ALGORITHM_H_

#include "Schedule.h"
#include <memory>

class FTRMFF_Enhanced : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Enhanced ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Enhanced_Algorithm : 
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Enhanced_Algorithm (const PROCESSOR_LIST & processors,
                             unsigned int consistency_level);

  virtual ~FTRMFF_Enhanced_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  SCHEDULE schedule_;
  std::auto_ptr<Ranking_Algorithm> ranking_algorithm_;
};

#endif /* FTRMFF_ENHANCED_ALGORITHM_H_ */
