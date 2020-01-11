/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing with TAO
 */

#include "test_config.h"
#include "ace/CDR_Base.h"

template <typename T>
class Foo_T
{
public:
  Foo_T () : member() {};
  typedef T& T_elem;
  T_elem operator[] (ACE_CDR::ULong) { return member; }
private:
  T member;
};

struct Foo2
{
  long i;
};
typedef Foo_T<Foo2> Foo2Seq;

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_38_Test"));

  Foo2Seq f2;
  long const j = f2[0].i;
  ACE_UNUSED_ARG(j);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++ support ok\n")));

  ACE_END_TEST;

  return 0;
}

