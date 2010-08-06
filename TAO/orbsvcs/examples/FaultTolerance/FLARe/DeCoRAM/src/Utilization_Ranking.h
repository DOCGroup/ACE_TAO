// -*- C++ -*-

//=============================================================================
/**
 *  @file    Utilization_Ranking.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef UTILIZATION_RANKING_H_
#define UTILIZATION_RANKING_H_

#include "Schedule.h"
#include "CTT_Enhanced.h"

class Utilization_Ranking : public Ranking_Algorithm
{
 public:
  virtual ~Utilization_Ranking ();

  virtual unsigned long operator () (SCHEDULE_RESULT_LIST & result_list,
                                     const SCHEDULE & schedule);
private:
  CTT_Enhanced ctt_;
};

#endif /* UTILIZATION_RANKING_H_ */
