// -*- C++ -*-

//=============================================================================
/**
 * @file    WML_Macros.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_WML_MACROS_H_
#define _CUTS_WML_MACROS_H_

#define CUTS_DECLARE_ACTION_FACTORY(worker_type, action) \
  public: \
    static CUTS_Action * create_action (worker_type & worker) \
    { \
      return new action (worker); \
    }

#define CUTS_ACTION_TABLE_DECLARE() \
  public: \
    virtual CUTS_Action * create_action (const char * action_name); \
    virtual size_t action_count (void) const; \
    virtual CUTS_Action_Iterator * begin (void); \
    static void init_action_table (void)

#define CUTS_ACTION_TABLE_BEGIN(worker_type, base_class) \
  CUTS_Action * worker_type::create_action (const char * action_name) \
  { CUTS_Action * action  = \
    CUTS_Worker_T <worker_type>::create_action (action_name); \
    return action != 0 ? action : base_class::create_action (action_name); \
  } \
  void worker_type::init_action_table (void) \
  { \
    base_class::init_action_table (); \
    if (action_table_.current_size () == 0) \
    {

#define CUTS_ACTION_TABLE_END(worker_type) \
    } \
  } \
  size_t worker_type::action_count (void) const \
  { return CUTS_Worker_T <worker_type>::action_count (); } \
  CUTS_Action_Iterator * worker_type::begin (void) \
  { return CUTS_Worker_T <worker_type>::begin (); }

#define CUTS_ACTION_TABLE_ENTRY(name, action) \
  action_table_.bind (name, &action::create_action)

//=============================================================================
/**
 * @def CUTS_PARAMETER_LIST_BEGIN
 *
 * @brief Defines the beginning of the parameter declaration
 * for an action.
 */
//=============================================================================

#define CUTS_ACTION_PARAMETER_LIST_BEGIN private:

//=============================================================================
/**
 * @def CUTS_ACTION_PARAMETER_LIST_END
 *
 * @brief Defines the end of the parameter declaration for
 * an action.
 */
//=============================================================================

#define CUTS_ACTION_DECLARE(parent, action) \
  class CUTS_Export action : \
    public CUTS_Action, \
    public CUTS_Action_T <parent>

#define CUTS_ACTION_DESTRUCTOR(action) \
  CUTS_INLINE virtual ~action (void)

#define CUTS_ACTION_PARAMETER_LIST_END

#define CUTS_ACTION_PARAMETER_DECLARE(type, decl) \
  type decl

#define CUTS_ACTION_DEFAULT_CONSTRUCTOR(parent, action) \
  CUTS_INLINE action (parent & worker) \
  : CUTS_Action_T <parent> (worker)

#define CUTS_ACTION_PARAMETER_INIT(decl, value) \
  , decl (value)

#define CUTS_ACTION_INIT_CONSTRUCTOR_BEGIN(parent, action) \
  CUTS_INLINE action (parent & worker

#define CUTS_ACTION_INIT_CONSTRUCTOR_END(parent) \
  ) : CUTS_Action_T <parent> (worker)

#define CUTS_ACTION_ARGUMENT(type, decl) \
  , type decl

#define CUTS_ACTION_ARGUMENT_IN_SINGLE(arg) \
  arg

#define CUTS_ACTION_EXECUTE(method, args) \
  CUTS_INLINE void execute (void) const \
  { \
    this->parent_.method (args); \
  }

#define CUTS_ACTION_EXECUTE_NO_ARGS(method) \
  CUTS_INLINE void execute (void) const \
  { \
    this->parent_.method (); \
  }

#endif  // !defined _CUTS_WML_MACROS_H_
