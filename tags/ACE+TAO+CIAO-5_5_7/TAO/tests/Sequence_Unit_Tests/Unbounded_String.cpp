/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 *        for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"

typedef TAO::unbounded_basic_string_sequence<char> s_sequence;

int main(int,char*[])
{
  s_sequence a;
  s_sequence b(23);

  s_sequence c(32, 0, s_sequence::allocbuf(32), true);
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

  b.replace(64, 0, s_sequence::allocbuf(64), true);

  char const * const * x = d.get_buffer();
  if (x != 0)
  {
    s_sequence::freebuf(a.get_buffer(true));
  }
  x = b.get_buffer();

  if (d.length())
  {
    s_sequence::freebuf(s_sequence::allocbuf(64));
  }

  s_sequence e(c);

  CORBA::String_var w(const_cast<char const*>("World"));
  e[0] = w;

  return 0;
}
