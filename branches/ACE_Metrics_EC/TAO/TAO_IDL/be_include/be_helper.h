/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_helper.h
//
// = DESCRIPTION
//    Defines the abstract class for outputting the C++ mapping. This is a
//    helper class to the singleton TAO_CodeGen class
//
// = AUTHOR
//    Aniruddha Gokhale
//
//    Improvements by Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_BE_OUTSTRM_H
#define TAO_BE_OUTSTRM_H

#include "ace/CDR_Base.h"

#include <stdio.h>

class Identifier;
class UTL_IdList;
class AST_Expression;

// a dummy structure to inform TAO_OutStream's << operator to  put a newline
// and use the current indentation for the succeeding line
struct TAO_NL
{
public:
  TAO_NL (void);
};

struct TAO_INDENT
{
  // = TITLE
  //   Operates like a manipulator, increasing the indentation level.
  //
  // = DESCRIPTION
  //   Increase the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  TAO_INDENT (int do_now = 0);

  const int do_now_;
};

struct TAO_UNINDENT
{
  // = TITLE
  //   Operates like a manipulator, decreasing the indentation level.
  //
  // = DESCRIPTION
  //   Decrease the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  TAO_UNINDENT (int do_now = 0);

  const int do_now_;
};

extern const TAO_NL be_nl;
extern const TAO_INDENT be_idt;
extern const TAO_INDENT be_idt_nl;
extern const TAO_UNINDENT be_uidt;
extern const TAO_UNINDENT be_uidt_nl;

class TAO_OutStream
{
  // =TITLE
  //   TAO_OutStream
  //
  // =DESCRIPTION
  //    Defines an interface by which the backend code generator can
  //    print its output to the underlying I/O handle. This is a
  //    helper class that will be used by the TAO_CodeGen
  //    class. However, this is an abstract class and classes that
  //    understand specific front ends must derive from this class.
public:

  // Enumerated type to indicate the stream type
  enum STREAM_TYPE
    {
      TAO_CLI_HDR,
      TAO_CLI_INL,
      TAO_CLI_IMPL,
      TAO_SVR_HDR,
      TAO_IMPL_HDR,
      TAO_IMPL_SKEL,
      TAO_SVR_TMPL_HDR,
      TAO_SVR_INL,
      TAO_SVR_TMPL_INL,
      TAO_SVR_IMPL,
      TAO_SVR_TMPL_IMPL,
      TAO_GPERF_INPUT
    };

  TAO_OutStream (void);
  // constructor.

  virtual ~TAO_OutStream (void);
  // destructor.

  int open (const char *fname,
            TAO_OutStream::STREAM_TYPE st = TAO_OutStream::TAO_CLI_HDR);
  // open the underlying low-level handle for output.

  void stream_type (TAO_OutStream::STREAM_TYPE);
  // set the stream type

  TAO_OutStream::STREAM_TYPE stream_type (void);
  // return the stream type

  FILE *file (void);
  // Return the underlying lowlevel file pointer.

  int incr_indent (unsigned short flag = 1);
  // increment the indentation level and by default actually indent the output
  // accordingly

  int decr_indent (unsigned short flag = 1);
  // decrease the indentation level and by default actually indent the output
  // accordingly

  int reset (void);
  // reset indentation level to 0

  int indent (void);
  // indent starting next line

  int nl (void);
  // put a newline and indent on the next line

  int print (const char *format, ...);
  // "printf" style variable argument print

  int gen_ifdef_macro (const char *flat_name,
                       const char *suffix = 0);
  // generate a #if !defined, #defined macro

  int gen_endif (void);
  // generate an endif statement

  int gen_ifdef_AHETI (void);
  // generate the ACE_HAS_EXPLICIT... ifdef

  int gen_elif_AHETI (void);
  // generate the ACE_HAS_EXPLICIT... elif

  int gen_endif_AHETI (void);
  // generate the ACE_HAS_EXPLICIT... endif

  // =overloaded operators

  TAO_OutStream &operator<< (const char *str);
  // output the char string and return a reference to ourselves

  TAO_OutStream &operator<< (const ACE_CDR::UShort num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const ACE_CDR::Short num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const ACE_CDR::ULong num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const ACE_CDR::Long num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const unsigned long num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const long num);
  // output the integer and return a reference to ourselves

  // = MANIPULATORS

  TAO_OutStream &operator<< (const TAO_NL& nl);
  TAO_OutStream &operator<< (const TAO_INDENT& i);
  TAO_OutStream &operator<< (const TAO_UNINDENT& i);

  // The following will be provided by specialized classes

  TAO_OutStream &operator<< (Identifier *id);
  // output an Identifier node

  TAO_OutStream &operator<< (UTL_IdList *idl);
  // output a scoped name

  TAO_OutStream &operator<< (AST_Expression *expr);
  // output an AST_Expression node

  // provided by specialized classes
  virtual TAO_OutStream &print (Identifier *id) = 0;

  virtual TAO_OutStream &print (UTL_IdList *idl) = 0;

  virtual TAO_OutStream &print (AST_Expression *idl) = 0;

protected:
  FILE *fp_;
  // the underlying low-level I/O handle

  TAO_OutStream::STREAM_TYPE st_;
  // stream type

  int indent_level_;
  // indentation level
};

#endif // if !defined
