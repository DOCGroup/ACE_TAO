// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

namespace A
{
  namespace C
  {
    class AbstractBase;
    template <typename T>
    class abstractbase_reference;
    template <typename T>
    struct abstractbase_traits
    {
      typedef abstractbase_reference<T> ref_type;
      static ref_type narrow(abstractbase_reference<C::AbstractBase>);
    };
  };
};

namespace A
{
  namespace C
  {
    class AbstractBase
    {
    protected:
      template<typename T>
      friend typename C::abstractbase_traits<T>::ref_type
      C::abstractbase_traits<T>::narrow(
      C::abstractbase_reference<C::AbstractBase>);
      AbstractBase() = default;
    };
  };
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_31_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++11 support ok\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_31_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
