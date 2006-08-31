#ifndef IDLSTREAM_H
#define IDLSTREAM_H

#include <ostream>

// A dummy structure to inform TAO_OutStream's << operator to  put a newline
// and use the current indentation for the succeeding line
struct NL
{
  NL (void);
};

struct INDENT
{
  //   Increase the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  INDENT (int do_now = 0);

  const int do_now_;
};

struct UNINDENT
{
  //   Decrease the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  UNINDENT (int do_now = 0);

  const int do_now_;
};

extern const NL nl;
extern const INDENT idt;
extern const INDENT idt_nl;
extern const UNINDENT uidt;
extern const UNINDENT uidt_nl;

class IDLStream
{
public:
  IDLStream (std::ostream& strm);
  // constructor.

  virtual ~IDLStream (void);
  // destructor.

  void incr_indent (unsigned short flag = 1);
  // increment the indentation level and by default actually indent the output
  // accordingly

  void decr_indent (unsigned short flag = 1);
  // decrease the indentation level and by default actually indent the output
  // accordingly

  void reset (void);
  // reset indentation level to 0

  void indent (void);
  // indent starting next line

  void nl (void);
  // put a newline and indent on the next line

  void gen_ifdef_macro (const std::string &);
  void gen_ifdef_macro (const char *);
  // generate a #if !defined, #defined macro

  void gen_endif (const std::string &);
  void gen_endif (const char *);
  // generate an endif statement

  IDLStream &operator<< (const std::string &);
  IDLStream &operator<< (const char *);
  IDLStream &operator<< (const long &);
  IDLStream &operator<< (const unsigned long &);
  IDLStream &operator<< (const double &);
  IDLStream &operator<< (const char &);

  // = MANIPULATORS

  IDLStream &operator<< (const NL &);
  IDLStream &operator<< (const INDENT &);
  IDLStream &operator<< (const UNINDENT &);

private:
  std::ostream& strm_;
  int indent_level_;
  
private:
  void upcase (const char *);
};

#endif // IDLSTREAM_H