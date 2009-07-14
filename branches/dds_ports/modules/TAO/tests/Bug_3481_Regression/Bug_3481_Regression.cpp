// $Id$

#include "FooC.h"

struct Foo_i : Foo
{
  static size_t s_count_;

  Foo_i ()
  {
    ++s_count_;
  }

  ~Foo_i ()
  {
    --s_count_;
  }

private:
  Foo_i (const Foo_i &);
  Foo_i operator= (const Foo_i &);
};

size_t Foo_i::s_count_ = 0;

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  {
    Foo_var f1 = new Foo_i;
    FooSeq fs;
    fs.length (1);
    fs[0] = f1;
    fs.length (0);
    fs.length (1);
    fs[0] = f1;
  }
  return Foo_i::s_count_; // must be zero to pass the test
}
