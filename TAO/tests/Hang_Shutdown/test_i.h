#ifndef TAO_HANG_TEST_I_H
#define TAO_HANG_TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#include "ace/OS_NS_unistd.h"

namespace Test
{
  class test_i
    : public virtual POA_Test::Hang
  {
  public:
    test_i () = default;
    test_i (test_i const &) = delete;
    test_i (test_i &&) = delete;
    test_i &operator= (test_i const &) = delete;
    test_i &operator= (test_i &&) = delete;

    // = The skeleton methods
    void send_stuff (char const *str,
                     CORBA::Boolean flag) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - [%C]\n", str));

      if (flag)
        {
          ACE_OS::sleep (10);

          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Returning from send_stuff\n", str));
        }

    }
  };
}

#include /**/ "ace/post.h"
#endif /* TAO_HANG_TEST_I_H */
