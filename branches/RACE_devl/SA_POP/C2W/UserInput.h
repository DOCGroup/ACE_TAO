// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  UserInput.h
 *
 * This file contains the UserInput class definition for the input adapter
 * that provides a user interface for testing SA-POP.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_USER_INPUT_H_
#define SA_POP_USER_INPUT_H_

#include "SA_POP_Types.h"
#include "UserInput.h"
#include "Builder.h"
#include "Planner.h"


namespace SA_POP {

  /**
   * @class UserInput
   *
   * @brief Input adapter that provides a user interface for testing SA-POP.
   */
  class UserInput {
  public:
    /// Constructor.
    UserInput (void);

    /// Destructor.
    virtual ~UserInput (void);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_USER_INPUT_H_ */
