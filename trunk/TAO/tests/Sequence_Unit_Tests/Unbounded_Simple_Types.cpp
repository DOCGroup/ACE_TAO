/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 * for primitive and other self-managed types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "sequence.hpp"

struct Foo
{
  int x;
};

int main(int,char*[])
{
  typedef TAO::unbounded_sequence<int> int_sequence;

  int_sequence a;
  int_sequence b(23);

  a = b;

  typedef TAO::unbounded_sequence<Foo> Foo_sequence;

  Foo_sequence c;
  Foo_sequence d(32);
  d = c;

  return 0;
}

// We use explicit template instantiation to force the instantiation
// of all member function and thus tests the full class.  This should
// work across all platforms, even on platforms that do not *require*
// explicit instantiation of templates.
template class TAO::unbounded_sequence<int>;
template class TAO::unbounded_sequence<Foo>;
