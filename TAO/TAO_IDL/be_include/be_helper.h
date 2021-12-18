/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_helper.h
 *
 *  Defines the abstract class for outputting the C++ mapping. This is a
 *  helper class to the singleton TAO_CodeGen class
 *
 *  @author Aniruddha Gokhale Improvements by Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_BE_OUTSTRM_H
#define TAO_BE_OUTSTRM_H

#include "ace/CDR_Base.h"
#include "ace/SString.h"

#include "ace/OS_NS_stdio.h"
#include <type_traits>

class Identifier;
class UTL_IdList;
class AST_Expression;

// A dummy structure to inform TAO_OutStream's << operator to  put a newline
// and use the current indentation for the succeeding line
struct TAO_NL
{
  TAO_NL ();
};

struct TAO_NL_2
{
  TAO_NL_2 ();
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
extern const TAO_NL_2 be_nl_2;
extern const TAO_INDENT be_idt;
extern const TAO_INDENT be_idt_nl;
extern const TAO_UNINDENT be_uidt;
extern const TAO_UNINDENT be_uidt_nl;

// A structure to inform TAO_OutStream's << operator to
// conditionally output a newline and some form of "ACE_CHECK".
struct TAO_ACE_CHECK
{
  TAO_ACE_CHECK (const char *retval = 0,
                 bool do_return = false);

  const char *retval_;
  bool do_return_;
};

/**
 * @class TAO_OutStream
 *
 * Defines an interface by which the backend code generator can
 * print its output to the underlying I/O handle. This is a
 * helper class that will be used by the TAO_CodeGen
 * class. However, this is an abstract class and classes that
 * understand specific front ends must derive from this class.
 */
class TAO_OutStream
{
public:

  /// Enumerated type to indicate the stream type
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
      TAO_GPERF_INPUT,
      CIAO_SVNT_HDR,
      CIAO_SVNT_IMPL,
      CIAO_SVNT_T_HDR,
      CIAO_SVNT_T_IMPL,
      CIAO_EXEC_HDR,
      CIAO_EXEC_IMPL,
      CIAO_EXEC_IDL,
      CIAO_EXEC_SVNT,
      CIAO_CONN_HDR,
      CIAO_CONN_IMPL,
      CIAO_AMI4CCM_CONN_IDL,
      CIAO_AMI_RH_IMPL_HDR,
      CIAO_AMI_RH_IMPL_SRC
    };

  TAO_OutStream ();

  virtual ~TAO_OutStream ();

  /// Open the underlying low-level handle for output.
  int open (const char *fname,
            TAO_OutStream::STREAM_TYPE st = TAO_OutStream::TAO_CLI_HDR);

  /// Set the stream type
  void stream_type (TAO_OutStream::STREAM_TYPE);

  /// Return the stream type
  TAO_OutStream::STREAM_TYPE stream_type ();

  /// Return the underlying lowlevel file pointer.
  FILE *&file ();

  /// Increment the indentation level and by default actually indent the output
  /// accordingly
  int incr_indent (unsigned short flag = 1);

  /// Decrease the indentation level and by default actually indent the output
  /// accordingly
  int decr_indent (unsigned short flag = 1);

  /// Reset indentation level to 0
  int reset ();

  /// Indent starting next line
  int indent ();

  /// Put a newline and indent on the next line
  int nl ();

  /// "printf" style variable argument print
  int print (const char *format, ...)
    ACE_GCC_FORMAT_ATTRIBUTE (printf, 2, 3);

  /// Generate a #if !defined, #defined macro
  int gen_ifdef_macro (const char *flat_name,
                       const char *suffix = 0,
                       bool add_stream_type_suffix = true);

  /// Generate an endif statement
  int gen_endif ();

  // =overloaded operators
  // Avoid duplication for the underlying type of size_t
  template <typename Dummy = TAO_OutStream &>
  typename std::enable_if<std::is_same<Dummy, TAO_OutStream &>::value &&
                          !std::is_same<ACE_CDR::ULongLong, size_t>::value &&
                          !std::is_same<ACE_CDR::ULong, size_t>::value,
                          TAO_OutStream &>::type
  operator << (const size_t num)
  {
    ACE_OS::fprintf (this->fp_,
                     ACE_SIZE_T_FORMAT_SPECIFIER_ASCII,
                     num);

    return *this;
  }

  TAO_OutStream &operator<< (const char *str);
  TAO_OutStream &operator<< (char ch);
  TAO_OutStream &operator<< (const ACE_CString &str);
  TAO_OutStream &operator<< (const ACE_CDR::UShort num);
  TAO_OutStream &operator<< (const ACE_CDR::Short num);
  TAO_OutStream &operator<< (const ACE_CDR::ULong num);
  TAO_OutStream &operator<< (const ACE_CDR::Long num);
  TAO_OutStream &operator<< (const ACE_CDR::ULongLong num);
  TAO_OutStream &operator<< (const ACE_CDR::LongLong num);

  // = MANIPULATORS

  TAO_OutStream &operator<< (const TAO_NL& nl);
  TAO_OutStream &operator<< (const TAO_NL_2& nl_twice);
  TAO_OutStream &operator<< (const TAO_INDENT& i);
  TAO_OutStream &operator<< (const TAO_UNINDENT& i);

  // The following will be provided by specialized classes

  /// Output an Identifier node
  TAO_OutStream &operator<< (Identifier *id);

  /// Output a scoped name
  TAO_OutStream &operator<< (UTL_IdList *idl);

  /// Output an AST_Expression node
  TAO_OutStream &operator<< (AST_Expression *expr);

  TAO_OutStream &print (Identifier *id);

  TAO_OutStream &print (UTL_IdList *idl);

  TAO_OutStream &print (AST_Expression *idl);

  void insert_comment (const char *file, int line);
#define TAO_INSERT_COMMENT(STRM) (STRM)->insert_comment (__FILE__, __LINE__)

protected:
  /// The underlying low-level I/O handle
  FILE *fp_;

  /// Stream type
  TAO_OutStream::STREAM_TYPE st_;

  /// Indentation level
  int indent_level_;

  /// Used to set tab spaces.
  ACE_CString tab_unit_str_;
};

#endif // if !defined
