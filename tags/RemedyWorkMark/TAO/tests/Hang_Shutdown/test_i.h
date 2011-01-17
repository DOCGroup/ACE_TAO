//
// $Id$
//
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
    // = The skeleton methods
    virtual void send_stuff (const char* str,
                             CORBA::Boolean flag)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - [%C]\n", str));

      if (flag)
        {
          ACE_OS::sleep (10);

          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Returning from send_stuff\n", str));
        }

      return;
    }
  private:
  };
}

#include /**/ "ace/post.h"
#endif /* TAO_HANG_TEST_I_H */
