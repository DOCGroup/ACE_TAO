/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing with ACE
 */

#include "test_config.h"
#include <utility>

template <typename key, typename value>
class my_map
{
public:
  my_map () : capacity_ (0), nodes_(0) {}
  ~my_map ();
  using key_type = key;
  using data_type = value;
  using value_type = std::pair<key, value>;
  size_t capacity_;
  value_type* nodes_;
};

template <typename key, typename value>
my_map<key, value>::~my_map ()
{
 for (size_t i = 0; i != capacity_; ++i)
 {
   (nodes_ + i)->~value_type ();
 }
}

class Bar
{
public:
 Bar () {}
 ~Bar () {}
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_37_Test"));

  my_map <size_t, Bar*> foo;
  ACE_UNUSED_ARG(foo);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("C++ support ok\n")));

  ACE_END_TEST;

  return 0;
}

