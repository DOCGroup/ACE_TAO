// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  client.h
 *
 * This file contains the UIOption base class (and derived classes) definition
 * for objects that encode command options available to the user in client
 * interface.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_UI_OPTION_H_
#define SA_POP_UI_OPTION_H_

#include <string>
#include "SA_POP_Types.h"
#include "Driver.h"

namespace SA_POP {


  /**
   * @class UIOption
   *
   * @brief Abstract base class for objects that encode client command options.
   */
  class UIOption {
  public:
    /// Constructor.
    /**
     * @param descrip  Description of option for display in UI.
     *
     * @param undo_descrip  Description of undo option for display in UI.
     *
     * @param driver  Pointer to SA-POP driver to use during option invocation.
     */
    UIOption (std::string descrip, std::string undo_descrip,
      ::CIAO::RACE::SA_POP::Driver_ptr driver);

    /// Destructor.
    virtual ~UIOption (void);



    /// Get description of option for display in UI.
    /**
     * @return  Description of option.
     */
    virtual std::string get_descrip (void);

    /// Get whether option is active.
    /**
     * @return  True if option is active; false otherwise.
     */
    virtual bool is_active (void);

    /// Get whether option is invoked.
    /**
     * @return  True if option is invoked; false otherwise.
     */
    virtual bool is_invoked (void);

    /// Do option action (if not invoked, invoke; otherwise, undo).
    /**
     * @return  True if action succeeded; false if action failed.
     */
    virtual bool do_action (void);

  protected:
    /// Description of option (for display in UI).
    std::string descrip_;

    /// Description of undo option (for display in UI).
    std::string undo_descrip_;

    /// Flag indicating whether option is currently invoked.
    bool is_invoked_;

    /// Flag indicating whether option is active/available.
    bool is_active_;

    /// Pointer to SA-POP driver to use during invocation of option.
    ::CIAO::RACE::SA_POP::Driver_var driver_;

    /// Invoke option action.
    /**
     * @return  True if invocation succeeded; otherwise false.
     */
    virtual bool invoke (void) = 0;

    /// Undo option action.
    /**
     * @return  True if undo succeeded; otherwise false.
     */
    virtual bool undo (void) = 0;

  };

  /**
   * @class GoalOption
   *
   * @brief Class for objects that encode client command option to plan and
   *        deploy an opstring for a goal.
   */
  class GoalOption : public UIOption {
  public:
    /// Constructor.
    /**
     * @param descrip  Description of option for display in UI.
     *
     * @param undo_descrip  Description of undo option for display in UI.
     *
     * @param driver  Pointer to SA-POP driver to use during option invocation.
     *
     * @param goal  Goal to plan and deploy opstring when invoked.
     */
    GoalOption (std::string descrip, std::string undo_descrip,
      ::CIAO::RACE::SA_POP::Driver_ptr driver,
      ::CIAO::RACE::GoalStructure goal);

    /// Constructor for a goal with only goal conditions.
    /**
     * @param descrip  Description of option for display in UI.
     *
     * @param undo_descrip  Description of undo option for display in UI.
     *
     * @param driver  Pointer to SA-POP driver to use during option invocation.
     *
     * @param goal_conds  Map from goal conditions to utilities.
     */
    GoalOption (std::string descrip, std::string undo_descrip,
      ::CIAO::RACE::SA_POP::Driver_ptr driver,
      ::SA_POP::GoalMap goal_conds);

    /// Destructor.
    virtual ~GoalOption (void);



  protected:
    /// Goal structure for this option.
    ::CIAO::RACE::GoalStructure goal_;

    /// Invoke option action (plan and deploy opstring for goal).
    /**
     * @return  True if invocation succeeded; otherwise false.
     */
    virtual bool invoke (void);

    /// Undo option action (remove deployment plan).
    /**
     * @return  True if undo succeeded; otherwise false.
     */
    virtual bool undo (void);

    /// Create and set internal goal using specified goal conditions & defaults.
    /**
     * @param goal_conds  Map from goal condition IDs to utilities.
     */
    virtual void create_def_goal (const ::SA_POP::GoalMap goal_conds);
  };

  /**
   * @class DeployOption
   *
   * @brief Class for objects that encode client command option to deploy
   *        a static deployment plan.
   */
  class DeployOption : public UIOption {
  public:
    /// Constructor.
    /**
     * @param descrip  Description of option for display in UI.
     *
     * @param undo_descrip  Description of undo option for display in UI.
     *
     * @param driver  Pointer to SA-POP driver to use during option invocation.
     *
     * @param dp_uri  URI of deployment plan.
     */
    DeployOption (std::string descrip, std::string undo_descrip,
      ::CIAO::RACE::SA_POP::Driver_ptr driver,
      std::string dp_uri);

    /// Destructor.
    virtual ~DeployOption (void);

  protected:
    /// URI of deployment plan for this option.
    std::string dp_uri_;

    /// Invoke option action (plan and deploy opstring for goal).
    /**
     * @return  True if invocation succeeded; otherwise false.
     */
    virtual bool invoke (void);

    /// Undo option action (remove deployment plan).
    /**
     * @return  True if undo succeeded; otherwise false.
     */
    virtual bool undo (void);
  };


};  /* SA_POP namespace */

#endif /* SA_POP_UI_OPTION_H_ */
