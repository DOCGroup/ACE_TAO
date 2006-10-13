// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SANet_Exceptions.h
 *
 * This file contains the exception definitions for spreading activation
 * networks.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SANET_EXCEPTIONS_H_
#define SANET_EXCEPTIONS_H_

#include <stdexcept>

namespace SANet {
  /**
   * @class Invalid_Step
   *
   * @brief Exception class for invalid step arguments.
   */
  class Invalid_Step : public std::invalid_argument {
  public:
    /// Constructor.
    Invalid_Step (void);
  };

  /**
   * @class Update_Error
   *
   * @brief Exception class for unrecoverable update errors.
   */
  class Update_Error : public std::logic_error {
  public:
    /// Constructor.
    Update_Error (void);
  };

  /**
   * @class Duplicate_ID
   *
   * @brief Exception class for duplicate ID value during node creation.
   */
  class Duplicate_ID : public std::invalid_argument {
  public:
    /// Constructor.
    Duplicate_ID (void);
  };

  /**
   * @class Insertion_Error
   *
   * @brief Exception class for unknown errors during map/set insertion.
   */
  class Insertion_Error : public std::logic_error {
  public:
    /// Constructor.
    Insertion_Error (void);
  };

  /**
   * @class UnknownNode
   *
   * @brief Exception class for unknown node error when looking up a node.
   */
  class UnknownNode : public std::invalid_argument {
  public:
    /// Constructor.
    UnknownNode (void);
  };

};  /* SANet namespace */


#endif /* SANET_EXCEPTIONS_H_ */
