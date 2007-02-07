/**
 * @file
 *
 * @brief Smoke test (basically just compile) the bounded sequences
 *        for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"

CORBA::ULong const MAXIMUM = 42;
typedef TAO::bounded_basic_string_sequence<char, MAXIMUM> s_sequence;

int main(int,char*[])
{
  s_sequence a;
  s_sequence b(a);

  s_sequence c(0, s_sequence::allocbuf(), true);
  a = b;

  a.length(c.maximum());
  if (a.release())
  {
    b.length(a.length());
  }
  a[0] = const_cast<char const*>("Hello");
  b[0] = a[0];

  s_sequence const & d = a;
  c[0] = d[0];

  b.replace(0, s_sequence::allocbuf(), true);

  char const * const * x = d.get_buffer();
  if (x != 0)
  {
    s_sequence::freebuf(a.get_buffer(true));
  }
  x = b.get_buffer();

  if (d.length())
  {
    s_sequence::freebuf(s_sequence::allocbuf());
  }

  s_sequence e(c);

  CORBA::String_var w(const_cast<char const*>("World"));
  e[0] = w;

  return 0;
}
