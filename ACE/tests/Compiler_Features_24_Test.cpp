// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <type_traits>
#include <memory>

template <typename T>
class o_r;

template <typename T>
struct o_t
{
  typedef o_r<T>       ref_type;
};
class T_base {};

template<typename T,
         typename = typename std::enable_if<
           std::is_base_of<T_base, T>::value>::type, typename ...Args>
o_r<T> make_f(Args&& ...args);

template <typename T>
class o_r final
{
public:
  template <typename _Tp1, typename, typename ...Args>
  friend o_r<_Tp1> make_f(Args&& ...args);
protected:
  typedef std::shared_ptr<T>    shared_ptr_type;
  template<typename _Tp1, typename = typename
    std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
  explicit o_r (_Tp1*)
    : stub_ ()
  {}
private:
  shared_ptr_type stub_;
};

#if defined (__GNUC__)
// g++ allows a redeclaration of the default
template<typename T,
         typename = typename std::enable_if<
           std::is_base_of<T_base, T>::value>::type, typename ...Args>
#else
template<typename T, typename, typename ...Args>
#endif
inline o_r<T> make_f(Args&& ...args)
{
  return o_r<T> (new T (std::forward<Args> (args)...));
}

class A : public T_base
{
protected:
  A () = default;
  template <typename _Tp1, typename, typename ...Args>
  friend o_r<_Tp1> make_reference(Args&& ...args);
};

o_t<A>::ref_type create ()
{
  return make_f<A>();
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_24_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Compiler Feature 24 Test does compile and run.\n")));

  o_r<A> l = create();

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_24_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
