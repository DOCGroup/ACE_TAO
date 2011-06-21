/* -*- C++ -*- */
/**
 *  @file   EC_Thread_Flags.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland <cleeland@ociweb.com>
 *
 *  Encapsulate flags that can be used for creating threads.
 */

#ifndef TAO_EC_THREAD_FLAGS_H
#define TAO_EC_THREAD_FLAGS_H
#include /**/ "ace/pre.h"

#include /**/ "tao/orbconf.h"
#include /**/ "tao/Versioned_Namespace.h"
#include /**/ "orbsvcs/Event/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*!
  \class TAO_EC_Thread_Flags

  \brief Parse thread flags from string to a long.

  Encapsulate flags used for creating threads with \a
  ACE_OS::thr_create and \a ACE_Task::activate.

  \note On platforms that do not support the thread schedulers, the
  ACE OS adaptation layer preserves the symbols for THR_SCHED_*, but
  defines them as zero.  Thus, on such platforms, setting a scheduler
  in the flags, then inquiring for the scheduler type will yield the
  same result as an unknown or unset scheduler.

  \bugs This should really be part of ACE or ACE_OS, and maybe someday
  it will, but right now it's not.

*/
class TAO_RTEvent_Serv_Export TAO_EC_Thread_Flags
{
public:
  TAO_EC_Thread_Flags () { }
  /*! \brief See \a operator=() */
  TAO_EC_Thread_Flags (const char* symbolic_flags)
    { this->parse_symbols(symbolic_flags); }
  ~TAO_EC_Thread_Flags ();

  // Default Copy CTOR is fine.

  /*!
    \brief Assign a new set of symbolic flags, setting \a flags, \a scope, and \a sched as possible.

    The flags can be symbolic, separated by the vertical bar ('|').
    In case a platform supports a creation flag not available
    symbolically, the user can specify a numeric value any place a
    symbol could be used.

    \sa TAO_EC_Thread_Flags::supported_flags

    \note The \a sched value only gets set if the scheduler is specified using symbols.
  */
  const TAO_EC_Thread_Flags& operator= (const char* symbolic_flags)
    { this->parse_symbols(symbolic_flags); return *this; }

  /*!
    \brief Returns numeric equivalent of the thread flags suitable for passing to ACE_Task::activate.
  */
  long flags() const { return this->flags_; }

  /*!
    \brief Returns value of THR_SCOPE_* used, or zero if unknown.
  */
  long scope() const { return this->scope_; }

  /*!
    \brief Returns value of THR_SCHED_* used, or zero if unknown.
  */
  long sched() const { return this->sched_; }

  /// Synonym for flags(), i.e., syntactic sugar.
  operator long () const { return this->flags(); }

  /*!
    \brief Return an acceptable default priority for the scheduler returned by sched().
    \return the priority, or ACE_DEFAULT_THREAD_PRIORITY if the scheduler is unknown.
  */
  long default_priority () const;



  struct Supported_Flag
  {
    const char* n; /// Flag name, e.g., THR_NEW_LWP
    long        v; /// Flag value, i.e., actual symbol
  };

  // TETFSF == TAO_EC_THREAD_FLAGS_SUPPORTED_FLAGS
  static Supported_Flag supported_flags_[];

protected:
  long flags_; /// Value of all flags OR'd together
  long scope_; /// Value of THR_SCOPE_*
  long sched_; /// Value of THR_SCHED_*

  void parse_symbols (const char* syms);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_THREAD_FLAGS_H */
