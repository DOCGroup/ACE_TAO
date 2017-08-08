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
  ~my_map ();
  typedef key key_type;
  typedef value data_type;
  typedef std::pair<key, value> value_type;
  size_t capacity_;
  value_type* nodes_;
};

template <typename key, typename value>
my_map<key, value>::~my_map ()
{
 for (size_t i = 0; i != capacity_; ++i)
 {
#if defined (ACE_HAS_BCC32)
   using std::pair;
   (nodes_ + i)->~pair<key, value>();
#else
   (nodes_ + i)->~value_type ();
#endif
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

