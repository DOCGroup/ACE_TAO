/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features.
 *
 * This test validates whether a alias can be used to explicitly
 * call a destructor, which is related to DR244 (see
 * http://wg21.cmeerw.net/cwg/issue244)
 *
 * This is partly fixed May 2014 in clang, see
 * http://llvm.org/viewvc/llvm-project?view=revision&revision=209319
 *
 * Currently the using is required, see
 * https://bugs.llvm.org/show_bug.cgi?id=12350
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <string>

class A
{
public:
  A () = default;
  void clear ();
private:
  union u_type_
  {
    u_type_ ();
    ~u_type_ ();
    std::string string_member_;
    bool bool_member_;
  } u_;
};

A::u_type_::u_type_ ()
{
}

A::u_type_::~u_type_ ()
{
}

void A::clear ()
{
#if defined __clang__ && \
    (defined __apple_build_version__ && __apple_build_version__ < 9100000 \
     || __clang_major__ <= 9)
#define CLANG_WORKAROUND
#endif

#ifdef CLANG_WORKAROUND
  // clang requires one of two workarounds:
  // 1. the name after ~ must be in scope
  using std::string;
#endif
  this->u_.string_member_.std::string::~string ();
}

struct B {
  struct u_type_ {
    std::string m;
  } u_;
  void clear() {
#ifdef CLANG_WORKAROUND
    // 2. actual class name instead of typedef
    u_.m.std::string::~basic_string ();
#else
    u_.m.std::string::~string ();
#endif
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_32_Test"));

  A a_v;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++11 support ok\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_32_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
