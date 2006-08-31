
#ifndef _CUTS_ACTIVATION_RECORD_H_
#define _CUTS_ACTIVATION_RECORD_H_

#include "cuts/Time_Measurement.h"

#include <list>
#include <map>
#include <string>

//=============================================================================
/**
 * @class CUTS_Activation_Record
 *
 * @brief Isolated in-memory storage for performance metrics.
 *
 * Activation records do not require any locking mechnisms when
 * logging the performance because each event in the system is allocated
 * is own record. This is possible since we are assuming the number of
 * active events in determistics and of a small amount.
 *
 * Actication records work in conjection with worker actions. While
 * a record is active/open, worker actions can be exectued in its context.
 * Also, depending on the method of execution, the performance of the action
 * is logged in a log format. This allows external clients to iterate over
 * the log to see the performance of each individual action, if needed.
 */
//=============================================================================

class CUTS_Export CUTS_Activation_Record
{
public:
  struct Entry
  {
    //
    // Entry
    //
    Entry (size_t reps, long worker_id, long action_id,
      ACE_Time_Value & start, ACE_Time_Value &stop)
      : reps_ (reps),
        worker_id_ (worker_id),
        action_id_ (action_id),
        start_time_ (start),
        stop_time_ (stop)
    {

    }

    //
    // operator =
    //
    const Entry & operator = (const Entry & entry)
    {
      this->reps_ = entry.reps_;
      this->worker_id_ = entry.worker_id_;
      this->action_id_ = entry.action_id_;
      this->start_time_ = entry.start_time_;
      this->stop_time_ = entry.stop_time_;
      return *this;
    }

    /// Number of repetitions.
    size_t reps_;

    /// ID is the worker performing the action.
    long worker_id_;

    /// ID of the action for the <worker_id_>.
    long action_id_;

    /// Start time for the entry.
    ACE_Time_Value start_time_;

    /// Stop time for the entry.
    ACE_Time_Value stop_time_;
  };

  /// Type definition for entries contained in the record.
  typedef std::list <Entry> Entries;

  /// Type definition for mapping exit points to time.
  typedef std::map <std::string, ACE_Time_Value> Exit_Points;

  /// Constructor.
  CUTS_Activation_Record (void);

  /// Destructor.
  virtual ~CUTS_Activation_Record (void);

  /// Get the start time for the activation record.
  const ACE_Time_Value & start_time (void) const;

  /// Get the start time for the activation record.
  const ACE_Time_Value & stop_time (void) const;

  /// Rest the activation record.
  void reset (void);

  /// Activate the activation record.
  virtual void activate (long owner = -1);

  /// Close the activation record.
  virtual void close (void);

  /// Determine if the record is active.
  bool is_active (void);

  /// Get the entries for the record.
  const Entries & entries (void) const;

  /// Get the exit points for the record.
  const Exit_Points & exit_points (void) const;

  /// Get the owner of the record.
  long owner (void) const;

  /// Perform the specified action without any logging.
  template <typename ACTION>
  void perform_action_no_logging (const ACTION & action);

  /// Perform the specified action without any logging.
  template <typename ACTION>
  void perform_action_no_logging (size_t repetitions, const ACTION & action);

  /// Perform the specified action and log its performance.
  template <typename ACTION>
  void perform_action (const ACTION & action);

  /// Perform the specified action and log its performance.
  template <typename ACTION>
  void perform_action (size_t repetitions, const ACTION & action);

  template <typename ACTION>
  void record_exit_point (const std::string & uuid, const ACTION & action);

  const ACE_Time_Value & transit_time (void) const;

  void transit_time (const ACE_Time_Value & transit_time);

  void queue_time (const ACE_Time_Value & queue_time);

  const ACE_Time_Value & queue_time (void) const;

private:
  /// Log the timing measurement.
  void log_time_measurement (size_t reps, long worker_id, long action_id);

  /// Active state of the record.
  bool active_;

  /// Owner of the record.
  long owner_;

  /// The start for the activation record.
  ACE_Time_Value start_time_;

  /// The stop time for the activation record.
  ACE_Time_Value stop_time_;

  /// Start time of the operation.
  ACE_Time_Value action_state_time_;

  /// Stop time of the operation.
  ACE_Time_Value action_stop_time_;

  /// Transit time for the event cause the record.
  ACE_Time_Value transit_time_;

  /// Queuing time for the event relating to this record.
  ACE_Time_Value queue_time_;

  /// Entries in the activation record.
  Entries entries_;

  /// Collection of exit points
  Exit_Points exit_points_;

  /// Disallowed operation.
  CUTS_Activation_Record (const CUTS_Activation_Record &);

  /// Disallowed operation.
  const CUTS_Activation_Record & operator = (const CUTS_Activation_Record &);
};

//=============================================================================
/**
 * @class CUTS_Cached_Activation_Record_T
 *
 * @brief Implementation of a CUTS_Activation_Record to be used with the
 * ACE_Free_List class.
 *
 * This class is intended to be used w/ the ACE_Free_List class. Do so
 * simplies the implementation of a free list becuase it does not have
 * be implemented using a heavyweight STL class. In order to use this
 * class within the ACE_Free_List, it defines and implements the set_next ()
 * and get_next () methods. Since this class is a subclass of a
 * CUTS_Activation_Record, clients will notice any difference.
 */
//=============================================================================

class CUTS_Export CUTS_Cached_Activation_Record :
  public CUTS_Activation_Record
{
public:
  /// Constructor.
  CUTS_Cached_Activation_Record (void);

  /// Destructor.
  virtual ~CUTS_Cached_Activation_Record (void);

  /// Set the next cached activation record.
  void set_next (CUTS_Cached_Activation_Record * next);

  /// Get the next cached activation record.
  CUTS_Cached_Activation_Record * get_next (void) const;

private:
  /// Pointer to the next cached record.
  CUTS_Cached_Activation_Record * next_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/ActivationRecord.inl"
#include "cuts/Activation_Record_T.inl"
#endif

#include "cuts/Activation_Record_T.cpp"

#endif  // !defined _CUTS_ACTIVATION_RECORD_H_
