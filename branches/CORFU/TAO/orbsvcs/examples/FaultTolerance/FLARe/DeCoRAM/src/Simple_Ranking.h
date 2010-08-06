// -*- C++ -*-

//=============================================================================
/**
 *  @file    Simple_Ranking.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef SIMPLE_RANKING_H_
#define SIMPLE_RANKING_H_

#include "Schedule.h"

class Simple_Ranking : public Ranking_Algorithm
{
 public:
  virtual ~Simple_Ranking ();

  virtual unsigned long operator () (SCHEDULE_RESULT_LIST & result_list,
                                     const SCHEDULE & schedule);
};

#endif /* SIMPLE_RANKING_H_ */
