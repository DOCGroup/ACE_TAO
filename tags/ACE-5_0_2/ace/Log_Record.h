/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Log_Record.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

// These need to go outside of the #ifdef to avoid problems with
// circular dependencies...
#include "ace/ACE.h"

#include "ace/Log_Priority.h"

#ifndef ACE_LOG_RECORD_H
#define ACE_LOG_RECORD_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Log_Record
{
public:
  // = TITLE
  //     Defines the structure of an ACE logging record.

  enum
  {
    MAXLOGMSGLEN = ACE_MAXLOGMSGLEN,
    // Maximum size of a logging message.

    ALIGN_WORDB  = 8,
    // Most restrictive alignment.

    VERBOSE_LEN = 128,
    // Size used by verbose mode.
    // 20 (date) + 15 (host_name) + 10 (pid) + 10 (type) + 4 (@) ... +
    // ? (progname)

    MAXVERBOSELOGMSGLEN = VERBOSE_LEN + MAXLOGMSGLEN
    // Maximum size of a logging message with the verbose headers
  };

  // = Initialization
  ACE_Log_Record (void);
  ACE_Log_Record (ACE_Log_Priority lp,
                  long time_stamp,
                  long pid);
  // Create a <Log_Record> and set its priority, time stamp, and
  // process id.
  ACE_Log_Record (ACE_Log_Priority lp,
                  const ACE_Time_Value &time_stamp,
                  long pid);
  // Create a <Log_Record> and set its priority, time stamp, and
  // process id.

  ~ACE_Log_Record (void);
  // Default dtor.


  int print (const ASYS_TCHAR host_name[],
             u_long verbose_flag,
#if !defined (ACE_HAS_WINCE)
             FILE *fp = stderr);
#else
             FILE *fp);
#endif /* ACE_HAS_WINCE */
  // Write the contents of the logging record to the appropriate
  // <FILE>.

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  int print (const ASYS_TCHAR host_name[],
             u_long verbose_flag,
             ostream &stream);
  // Write the contents of the logging record to the appropriate
  // <ostream>.
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

  int format_msg (const ASYS_TCHAR host_name[],
                  u_long verbose_flag,
                  ASYS_TCHAR *verbose_msg);

#if defined (ACE_HAS_WINCE)
  int print (const ASYS_TCHAR host_name[],
             u_long verbose_flag,
             ACE_CE_Bridge *log_ = 0);
  // For Windows CE, the default is to log messages to a preset
  // window.
#endif /* defined (ACE_HAS_WINCE) */

  static const ASYS_TCHAR *priority_name (ACE_Log_Priority p);
  // Returns a character array with the string form of the
  // <ACE_Log_Priority> parameter.  This is used for the verbose
  // printing format.

  // = Marshall/demarshall
  void encode (void);
  // Encode the <Log_Record> for transmission on the network.

  void decode (void);
  // Decode the <Log_Record> received from the network.

  // = Set/get methods

  long type (void) const;
  // Get the type of the <Log_Record>.

  void type (long);
  // Set the type of the <Log_Record>.

  u_long priority (void) const;
  // Get the priority of the <Log_Record> <type_>.  This is computed
  // as the base 2 logarithm of <type_> (which must be a power of 2,
  // as defined by the enums in <ACE_Log_Priority>).

  void priority (u_long num);
  // Set the priority of the <Log_Record> <type_> (which must be a
  // power of 2, as defined by the enums in <ACE_Log_Priority>).

  long length (void) const;
  // Get the length of the <Log_Record>.

  void length (long);
  // Set the length of the <Log_Record>.

  const ACE_Time_Value &time_stamp (void) const;
  // Get the time stamp of the <Log_Record>.

  void time_stamp (const ACE_Time_Value &);
  // Set the time stamp of the <Log_Record>.

  long pid (void) const;
  // Get the process id of the <Log_Record>.

  void pid (long);
  // Set the process id of the <Log_Record>.

  ASYS_TCHAR *msg_data (void);
  // Get the message data of the <Log_Record>.

  void msg_data (const ASYS_TCHAR *data);
  // Set the message data of the <Log_Record>.

  void msg_data_len (size_t len);
  // Set the size of the message data of the <Log_Record>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void round_up (void);
  // Round up to the alignment restrictions.

  ACE_INT32 length_;
  // Total length of the logging record in bytes.  This field *must*
  // come first in order for various IPC framing mechanisms to work
  // correctly.  In addition, the field must be an ACE_INT32 in order
  // to be passed portably across platforms.

  long type_;
  // Type of logging record.

  ACE_Time_Value time_stamp_;
  // Time that the logging record was generated.

  long pid_;
  // Id of process that generated the logging record.

  ASYS_TCHAR msg_data_[MAXLOGMSGLEN];
  // Logging record data

  static const ASYS_TCHAR *priority_names_[];
  // Symbolic names for the <ACE_Log_Priority> enums.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Log_Record.i"
#endif

#endif /* ACE_LOG_RECORD_H */
