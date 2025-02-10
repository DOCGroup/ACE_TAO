/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

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
      using ref_type = abstractbase_reference<T>;
      static ref_type narrow(abstractbase_reference<C::AbstractBase>);
      static ref_type narrow2(abstractbase_reference<C::AbstractBase>);
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
      template<typename T>
      friend auto
        C::abstractbase_traits<T>::narrow2(
        C::abstractbase_reference<C::AbstractBase>) -> ref_type;
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
