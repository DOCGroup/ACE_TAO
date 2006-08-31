// -*- C++ -*-

//=============================================================================
/**
 * @file    Port_Measurement.h
 *
 * $Id$
 *
 * This file contains the classes measuring performance of port operations
 * of a component.
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_PORT_MEASUREMENT_H_
#define _CUTS_PORT_MEASUREMENT_H_

#include "cuts/Worker_Measurement.h"
#include <map>
#include <string>

//=============================================================================
/**
 * @class CUTS_Port_Measurement
 *
 * @brief Performance measurements for a port operation of a component.
 */
//=============================================================================

class CUTS_Export CUTS_Port_Measurement
{
public:
  /// Type definition for making operation ids to measurement data.
  typedef std::map <long, CUTS_Worker_Measurement> Worker_Map;

  /// Type definition for mapping exit points to timing data.
  typedef std::map <std::string, CUTS_Time_Measurement> Exit_Points;

  /// Constructor.
  CUTS_Port_Measurement (void);

  /// Destructor.
  ~CUTS_Port_Measurement (void);

  /// Update the time data for processing an event.
  void process_time (const ACE_Time_Value &tv);

  /// Update the time data for receiving a transmitted event.
  void transit_time (const ACE_Time_Value &tv);

  /// Get the time data for processing an event.
  const CUTS_Time_Measurement & process_time (void) const;

  /// Get the time data for receiving a transitted event.
  const CUTS_Time_Measurement & transit_time (void) const;

  /// Record an entry into the port measurements
  void record_entry (size_t reps, long worker, long opid,
                     const ACE_Time_Value & tm);

  /// Record an exit point entry.
  void record_exit_point_time (const std::string & uuid,
                               const ACE_Time_Value & tv);

  /// Reset all the collected data.
  void reset (void);

  /// Get the collection of worker measurements.
  const Worker_Map & worker_measurements (void) const;

  /// Get the collection of worker measurements.
  Worker_Map & worker_measurements (void);

  /// Get the collection of exit points.
  Exit_Points & exit_points (void);

  /// Get the collection of exit points.
  const Exit_Points & exit_points (void) const;

private:
  /// Time taken to complete the port operation.
  CUTS_Time_Measurement process_time_;

  /// Time taken to begin processing a port operation.
  CUTS_Time_Measurement transit_time_;

  /// Collection of operation measurement data.
  Worker_Map workers_;

  /// Collection of exit times.
  Exit_Points exit_points_;
};

#if defined (__CUTS_INLINE__)
# include "cuts/Port_Measurement.inl"
#endif

#endif  // !defined _CUTS_PORT_MEASUREMENT_H_
