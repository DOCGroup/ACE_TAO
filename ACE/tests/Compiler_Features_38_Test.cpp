/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing with TAO
 */

#include "test_config.h"

// g++ 4.1 can't handle this construct
#if defined (__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 1))
# define ACE_DISABLE_TEST
#endif

// Visual Studio 7.1 can't handle this construct
#if (_MSC_VER == 1310)
# define ACE_DISABLE_TEST
#endif

// Sun Studio 5.13 and older can't handle this construct
#if defined (__SUNPRO_CC) && (__SUNPRO_CC <= 0x5130)
# define ACE_DISABLE_TEST
#endif

#if !defined (ACE_DISABLE_TEST)
template <typename T>
class Foo_T
{
public:
  Foo_T () : member() {};
  typedef T& T_elem;
  T_elem operator[] (long) { return member; }
  operator T *& () { return & member; };
private:
  T member;
};

struct Foo2
{
  long i;
};
typedef Foo_T<Foo2> Foo2Seq;
#endif /* !ACE_DISABLE_TEST */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_38_Test"));

#if !defined (ACE_DISABLE_TEST)
  Foo2Seq f2;
  long const j = f2[0].i;
  ACE_UNUSED_ARG(j);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++ support ok\n")));
#endif /* !ACE_DISABLE_TEST */

  ACE_END_TEST;

  return 0;
}

