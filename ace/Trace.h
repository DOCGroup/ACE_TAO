/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Trace.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TRACE_H
#define ACE_TRACE_H

class ACE_Export ACE_Trace
{
  // = TITLE
  //     A C++ trace facility that keeps track of which methods are
  //     entered and exited.
  //
  // = DESCRIPTION
  //     This class uses C++ constructors and destructors to automate
  //     the ACE_Trace nesting.  In addition, thread-specific storage
  //     is used to enable multiple threads to work correctly.
public:
  // = Initialization and termination methods.

  ACE_Trace (const ASYS_TCHAR *n, int line = 0,
             const ASYS_TCHAR *file = ASYS_TEXT (""));
  // Perform the first part of the trace, which prints out the string
  // N, the LINE, and the ACE_FILE as the function is entered.

  ~ACE_Trace (void);
  // Perform the second part of the trace, which prints out the NAME
  // as the function is exited.

  // = Control the tracing level.
  static int  is_tracing(void);
  // Determine if tracing is enabled (return == 1) or not (== 0)

  static void start_tracing (void);
  // Enable the tracing facility.

  static void stop_tracing (void);
  // Disable the tracing facility.

  static void set_nesting_indent (int indent);
  // Change the nesting indentation level.

  void dump (void) const;
  // Dump the state of an object.

private:
  // Keeps track of how deeply the call stack is nested (this is
  // maintained in thread-specific storage to ensure correctness in
  // multiple threads of control.

  const ASYS_TCHAR *name_;
  // Name of the method we are in.

  static int nesting_indent_;
  // Keeps track of how far to indent per trace call.

  static int enable_tracing_;
  // Is tracing enabled?

  // Default values.
  enum
  {
    DEFAULT_INDENT  = 3,
    DEFAULT_TRACING = 1
  };
};

#endif /* ACE_TRACE_H */
