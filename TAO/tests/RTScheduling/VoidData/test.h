#include "ace/Log_Msg.h"

class test_impl : public test
{
 public:
  test_impl () = default;
  test_impl (test_impl const &) = delete;
  test_impl (test_impl &&) = delete;
  test_impl & operator= (test_impl const &) = delete;
  test_impl & operator= (test_impl &&) = delete;

  void bar (CORBA::VoidData something) override
  {
    int const * const value = static_cast<int const *> (something);
    ACE_DEBUG ((LM_DEBUG,
                "Void Data Passed to method %d\n",
                *value));
  }
};

