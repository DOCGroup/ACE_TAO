//=============================================================================
/**
 *  @file    GPlot_File.h
 *
 *  $Id$
 *
 *  @author Tim Harrison
 */
//=============================================================================


#ifndef ACE_GPlot_File_H
#define ACE_GPlot_File_H
#include "ace/pre.h"

#include "ace/Map_Manager.h"
#include "old_event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"

/**
 * @class ACE_GPlot_File
 *
 * @brief Reads and writes files in GPlot format.
 *
 * Gplot formats are as follows:
 * entry   value
 * entry   value
 * entry   value
 * ...
 * They represent x,y pairs to be graphed by GPlot.  entry's are
 * type long.  value's are type long or float.
 */
class TAO_RTOLDEvent_Export ACE_GPlot_File
{
public:
  /// Construction.
  ACE_GPlot_File (void);

  /// Destruction.  Calls this->close.
  ~ACE_GPlot_File (void);

  /**
   * If the file does not exist, create it. If the file exists open
   * the file and read all the entries into map_.  Returns 0 on
   * success, -1 on failure.
   */
  int open (const char *filename);

  /// Close the file and sync all the contents.
  void close (void);

  /// Get the entry at this value.  Returns 0 if a value was found.
  /// Returns -1 if no value has been set for <entry>.
  int get (long entry, long &value);

  /// Get the entry at this value.  Returns 0 if a value was found.
  /// Returns -1 if no value has been set for <entry>.
  int get (long entry, float &value);

  /// Set the entry at this value.
  void set (long entry, long value);

  /// Set the entry at this value.
  void set (long entry, float value);

  /// Compare <value> with the value at <entry>.  Store the largest.
  void set_greatest (long entry, long value);

  /// Compare <value> with the value at <entry>.  Store the largest.
  void set_greatest (long entry, float value);

  /// Compare <value> with the value at <entry>.  Store the smallest.
  void set_least (long entry, long value);

  /// Compare <value> with the value at <entry>.  Store the smallest.
  void set_least (long entry, float value);

  /// Dump state of the object.
  void dump (void);

private:
  // = map_ stores all values.  It is sync'ed to file when this->close
  // is called.
  typedef ACE_Map_Entry <long, char *> GPLOT_ENTRY;
  typedef ACE_Map_Manager<long, char *, ACE_Null_Mutex> GPLOT_MAP;
  typedef ACE_Map_Iterator <long, char *, ACE_Null_Mutex> GPLOT_ITERATOR;

  GPLOT_MAP map_;

  char filename_[BUFSIZ];
  FILE *write_file_;

  /// Only close once.
  int closed_;
};

#if defined (__ACE_INLINE__)
#include "GPlot_File.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* ACE_GPlot_File_H */
