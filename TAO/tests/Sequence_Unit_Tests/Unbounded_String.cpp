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
#include "unbounded_string_sequence.hpp"

typedef TAO::unbounded_string_sequence s_sequence;

int main(int,char*[])
{
  s_sequence a;
  s_sequence b(23);

  s_sequence c(32);
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

  if (d.length())
  {
    s_sequence::freebuf(s_sequence::allocbuf(64));
  }

  s_sequence e(c);

  CORBA::String_var w(const_cast<char const*>("World"));
  e[0] = w;

  return 0;
}
