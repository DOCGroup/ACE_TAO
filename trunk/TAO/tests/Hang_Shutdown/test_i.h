//
// $Id$
//
#ifndef TAO_HANG_TEST_I_H
#define TAO_HANG_TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
#  pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

#include "ace/OS_NS_unistd.h"

namespace TAO
{
  namespace Test
  {
    class test_i
      : public virtual POA_TAO::Test::Hang
      , public virtual PortableServer::RefCountServantBase
    {
    public:
      // = The skeleton methods
      virtual void send_stuff (const char* str,
                               bool flag)
        ACE_THROW ((CORBA::SystemException))
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
}
#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_HANG_TEST_I_H */
