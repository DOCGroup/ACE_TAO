/**
 * @file Auto_Functor_Test.cpp
 */

#include "test_config.h"
#include "ace/Auto_Functor.h"

class Test_Base
{
public:
  virtual ~Test_Base () = default;
};

class Test_Derived : public Test_Base
{
};

struct Counting_Deleter
{
  int *count {};

  void operator() (Test_Base *value) const
  {
    ++*this->count;
    delete value;
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Auto_Functor_Test"));
  int first_count {};
  int second_count {};
  {
    ACE_Utils::Auto_Functor<Test_Derived, Counting_Deleter> source (
      new Test_Derived, Counting_Deleter {&first_count});
    ACE_Utils::Auto_Functor<Test_Base, Counting_Deleter> target (source);
    if (source.get () != nullptr || target.get () == nullptr)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Converting construction failed\n")), -1);
      }
    ACE_Utils::Auto_Functor<Test_Derived, Counting_Deleter> replacement (
      new Test_Derived, Counting_Deleter {&second_count});
    target = replacement;
    if (replacement.get () != nullptr || target.get () == nullptr
        || first_count != 1 || second_count != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Converting assignment failed\n")), -1);
      }
  }
  if (first_count != 1 || second_count != 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect deleter state after transfer\n")), -1);
    }
  {
    ACE_Utils::Auto_Functor<Test_Derived, Counting_Deleter> source (
      new Test_Derived, Counting_Deleter {&first_count});
    ACE_Utils::Auto_Functor<Test_Derived, Counting_Deleter> moved (std::move (source));
    ACE_Utils::Auto_Functor<Test_Base, Counting_Deleter> target (std::move (moved));
    ACE_Utils::Auto_Functor<Test_Derived, Counting_Deleter> replacement (
      new Test_Derived, Counting_Deleter {&second_count});
    target = std::move (replacement);
    if (source.get () != nullptr || moved.get () != nullptr
        || replacement.get () != nullptr || target.get () == nullptr
        || first_count != 2 || second_count != 1)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Move transfer failed\n")), -1);
      }
    ACE_Utils::Auto_Functor<Test_Base, Counting_Deleter> final_target;
    final_target = std::move (target);
    if (target.get () != nullptr || final_target.get () == nullptr)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Move assignment failed\n")), -1);
      }
  }
  if (first_count != 2 || second_count != 2)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect deleter state after move\n")), -1);
    }
  ACE_END_TEST;
  return 0;
}
