// -*- C++ -*-
//

//=============================================================================
/**
 * @file  SchemaOut.h
 *
 * This file contains the Class Description for SchemaOut which is an output adapater
 * that creates the XML necessary for a Schema
 *
 * @author  Daniel L.C. Mack <daniel.l.mack@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SCHEMA_OUT_H_
#define SA_POP_SCHEMA_OUT_H_

#include <iostream>
#include "OutAdapter.h"
#include <vector>

namespace SA_POP {

  /**
   * @class LogGraphOut
   *
   * @brief Output adapter that creates a graph visualization of the plan 
   *			as they occur.
   */
  class SchemaOut : public OutAdapter {
  public:
    /// Constructor.
    /**
     * @param strm  Output stream to log to.
     */
	  SchemaOut (std::ostream &out, std::vector<SA_POP::CondID> ks);

    /// Destructor.
    virtual ~SchemaOut (void);

    /// Notify about task future expected utility changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_eu (SA_POP::Planner *planner);

    /// Notify about plan changes.
    /**
     * @param planner  Planner for which changes have occurred.
     */
    virtual void notify_plan (SA_POP::Planner *planner);

  protected:
    /// Output stream to log to.
    std::ostream &out_;
	int graphn;
	std::vector<SA_POP::CondID> knowns;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_SCHEMA_OUT_H_ */
