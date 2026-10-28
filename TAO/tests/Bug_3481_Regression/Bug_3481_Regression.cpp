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

  Foo_i (Foo_i const &) = delete;
  Foo_i (Foo_i &&) = delete;
  Foo_i &operator= (Foo_i const &) = delete;
  Foo_i &operator= (Foo_i &&) = delete;
};

size_t Foo_i::s_count_ = 0;

int
ACE_TMAIN (int, ACE_TCHAR * [])
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
