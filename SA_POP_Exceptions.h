// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_POP_Exceptions.h
 *
 * This file contains the class definitions for SA-POP exceptions.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_EXCEPTIONS_H_
#define SA_POP_EXCEPTIONS_H_

#include <stdexcept>
#include <string>
#include "SA_POP_Types.h"

namespace SA_POP {

  /**
   * @class InvalidTask
   *
   * @brief Class for invalid task id argument exceptions.
   */
  class InvalidTask : public std::invalid_argument {
  public:
    /// Default constructor.
    InvalidTask (void);

    /// Constructor with task id info for exception message.
    /**
     * @param task_id  Invalid task id.
     */
    InvalidTask (TaskID task_id);

    /// Destructor.
    virtual ~InvalidTask (void) throw ();

    /// Create exception message string.
    /**
     * @param task_id  Invalid task id.
     *
     * @return  Exception message string.
     */
    std::string create_msg (TaskID task_id);
  };

  /**
   * @class DuplicateTask
   *
   * @brief Class for duplicate task id argument exceptions.
   */
  class DuplicateTask : public std::invalid_argument {
  public:
    /// Default constructor.
    DuplicateTask (void);

    /// Constructor with task id info for exception message.
    /**
     * @param task_id  Duplicate task id.
     */
    DuplicateTask (TaskID task_id);

    /// Destructor.
    virtual ~DuplicateTask (void) throw ();

    /// Create exception message string.
    /**
     * @param task_id  Duplicate task id.
     *
     * @return  Exception message string.
     */
    std::string create_msg (TaskID task_id);
  };

  /**
   * @class DuplicateCond
   *
   * @brief Class for duplicate condition id argument exceptions.
   */
  class DuplicateCond : public std::invalid_argument {
  public:
    /// Default constructor.
    DuplicateCond (void);

    /// Constructor with condition id info for exception message.
    /**
     * @param cond_id  Duplicate condition id.
     */
    DuplicateCond (CondID cond_id);

    /// Destructor.
    virtual ~DuplicateCond (void) throw ();

    /// Create exception message string.
    /**
     * @param cond_id  Duplicate condition id.
     *
     * @return  Exception message string.
     */
    std::string create_msg (CondID cond_id);
  };

  /**
   * @class InvalidCond
   *
   * @brief Class for invalid condition id argument exceptions.
   */
  class InvalidCond : public std::invalid_argument {
  public:
    /// Default constructor.
    InvalidCond (void);

    /// Constructor with condition id info for exception message.
    /**
     * @param cond_id  Invalid condition id.
     */
    InvalidCond (CondID cond_id);

    /// Destructor.
    virtual ~InvalidCond (void) throw ();

    /// Create exception message string.
    /**
     * @param cond_id  Duplicate condition id.
     *
     * @return  Exception message string.
     */
    std::string create_msg (CondID cond_id);
  };

  /**
   * @class Uninitialized
   *
   * @brief Class for exceptions an object being used before being initialized.
   */
  class Uninitialized : public std::logic_error {
  public:
    /// Default constructor.
    Uninitialized (void);

    /// Constructor with exception message.
    /**
     * @param msg  Exception message.
     */
    Uninitialized (std::string msg);

    /// Destructor.
    virtual ~Uninitialized (void) throw ();
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXCEPTIONS_H_ */
