/* -*- C++ -*- */
// $Id$
//
// ==================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Stream
//
// = DESCRIPTION
//    This class is to help write the generated code to files.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ==================================================================

#ifndef TAO_PSDL_STREAM_H
#define TAO_PSDL_STREAM_H

#include "ace/pre.h"

#include "PSDL_Scope.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Stream
{
 public:

  TAO_PSDL_Stream (void);

  ~TAO_PSDL_Stream (void);

  /// Open the file name to be able to write to it.
  int open (const char *fname);

  /// Return the underlying lowlevel file pointer.
  FILE *file (void);

  /// increment the indentation level and by default actually indent the output
  /// accordingly
  int incr_indent (unsigned short flag=1);

  /// decrease the indentation level and by default actually indent the output
  /// accordingly
  int decr_indent (unsigned short flag=1);

  /// reset indentation level to 0
  int reset (void);

  /// indent starting next line
  int indent (void);

  /// put a newline and indent on the next line
  int nl (void);

  /// "printf" style variable argument print
  int print (const char *format, ...);

  // =overloaded operators

  // output the char string and return a reference to ourselves
  TAO_PSDL_Stream &operator<< (const char *str);

  TAO_PSDL_Stream &operator<< (ACE_CString str);

  TAO_PSDL_Stream &operator<< (const unsigned long num);
  // output the integer and return a reference to ourselves

  TAO_PSDL_Stream &operator<< (const long num);
  // output the integer and return a reference to ourselves

private:

  // the underlying low-level I/O handle
  FILE *fp_;

  // indnetation level
  int indent_level_;
};


#include "ace/post.h"

#endif /* TAO_PSDL_STREAM_H */
