/* -*- c++ -*- */
// $Id$

/**
 * @file be_helper.h
 * @author Aniruddha Gokhale
 * @author Carlos O'Ryan
 *
 * Defines the abstract class for outputting the C++ mapping. This is a
 * helper class to the singleton TAO_CodeGen class
 */

#ifndef TAO_BE_OUTSTRM_H
#define TAO_BE_OUTSTRM_H

#include "ace/CDR_Base.h"

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

/**
 * Operates like a manipulator, increasing the indentation level.
 *
 * Increase the indentation level, if the "do_now" parameter is
 * not zero then the <indent> method is called on the stream.
 */
struct TAO_INDENT
{
  TAO_INDENT (int do_now = 0);

  const int do_now_;
};

/**
 * Operates like a manipulator, decreasing the indentation level.
 *
 * Decrease the indentation level, if the "do_now" parameter is
 * not zero then the <indent> method is called on the stream.
 */
struct TAO_UNINDENT
{
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
  TAO_OutStream (void);
  // constructor.

  virtual ~TAO_OutStream (void);
  // destructor.

  int open (const char *fname);
  // open the underlying low-level handle for output.


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

  void gen_ifndef_string (const char *fname,
                          const char *prefix,
                          const char *suffix);

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

#if defined (ACE_WIN64)
  TAO_OutStream &operator<< (const ACE_CDR::ULongLong num);
  // output the integer and return a reference to ourselves

  TAO_OutStream &operator<< (const ACE_CDR::LongLong num);
  // output the integer and return a reference to ourselves
#endif /* ACE_WIN64 */

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

  int indent_level_;
  // indentation level
};

#endif // if !defined
