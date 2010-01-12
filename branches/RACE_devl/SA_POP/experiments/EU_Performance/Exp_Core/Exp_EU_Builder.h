// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_Builder.h
 *
 * This file contains the definition of the Exp_EU_Builder concrete class,
 * which implements a Builder creating Exp_EU_Planner and associated
 * objects for planning with spreading activation networks and scheduling
 * with the "roadblock" scheduler (Exp_EU_SchedStrategy).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_EXP_EU_BUILDER_H_
#define SA_POP_EXP_EU_BUILDER_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SA_POP_Types.h"

//#include "Builder.h"
#include "SA_Builder.h"

//#include "Planner.h"
#include "Exp_EU_Planner.h"

#include "SANet/SANet.h"
#include "SA_PlanHeuristics.h"
#include "SA_PlanStrategy.h"

//#include "SA_SchedStrategy.h"
#include "Exp_EU_SchedStrategy.h"

#include "SA_WorkingPlan.h"
#include "TaskMap.h"

//#include "SA_Builder_Export.h"

namespace SA_POP {

  /**
   * @class Exp_EU_Builder
   *
   * @brief Builder concrete class for creating Exp_EU_Planner and
   *        associated objects for planning with spreading activation networks
   *        and scheduling with the "roadblock" scheduler (Exp_EU_SchedStrategy).
   */
  class Exp_EU_Builder : public SA_Builder {
  public:
    /// Constructor.
    Exp_EU_Builder (void);

    /// Destructor.
    virtual ~Exp_EU_Builder (void);

    /// Get Exp_EU_Planner object.
    /**
     * @return  Pointer to the configured planner object.
     */
    virtual Exp_EU_Planner *get_exp_eu_planner (void);

  protected:
    /// Planner object to centralize/mediate planning (as Exp_EU_Planner pointer).
    Exp_EU_Planner *exp_eu_planner_;

    /// Create SA-POP objects.
    virtual void init (void);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_BUILDER_H_ */
