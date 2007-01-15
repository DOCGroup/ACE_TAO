/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 *        for octets.
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#include "tao/Basic_Types.h"
#include "tao/Unbounded_Octet_Sequence_T.h"

typedef TAO::unbounded_value_sequence <CORBA::Octet> s_sequence;

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
  a[0] = 'a';
  b[0] = a[0];

  s_sequence const & d = a;
  c[0] = d[0];

  b.replace(64, 0, s_sequence::allocbuf(64), true);

  CORBA::Octet const * x = d.get_buffer();
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

  return 0;
}
