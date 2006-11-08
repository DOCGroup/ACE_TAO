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

#include "event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"  // for ACE_Lock

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

#if 0
  /*!
    \brief Indicate whether a priority is valid for the scheduler returned by sched().

    \return non-zero if the priority is valid.
    
    \note Always returns non-zero if sched == 0, i.e., the scheduler is unknown.
  */
  int is_valid_priority (long priority) const;

  /*!
    \brief Convert THR_SCHED_* values to ACE_SCHED_* values.
    
    For some wacked out reason the THR_SCHED_* values are different from
    the ACE_SCHED_* values, so we provide a mapping function.
  */
  long ace_sched_from_thr_sched (long thr_sched) const;
  long ace_scope_from_thr_scope (long thr_scope) const;
  long thr_sched_from_ace_sched (long ace_sched) const;
  long thr_scope_from_ace_scope (long ace_scope) const;
#endif

  /*!
    \brief Return an acceptable default priority for the scheduler returned by sched().
    \return the priority, or ACE_DEFAULT_THREAD_PRIORITY if the scheduler is unknown.
  */
  long default_priority () const;
  


#if 0
  /// Return a |-separated list of all flags that we can parse.
  const char* supported_flags () const;
#endif

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
  
#if 0
  ACE_Lock sf_lock_; // lock guarding access to supported_flags_
  const char* supported_flags_;
#endif
};

#if 0  && defined (__ACE_INLINE__)
#include "EC_Thread_Flags.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_THREAD_FLAGS_H */
