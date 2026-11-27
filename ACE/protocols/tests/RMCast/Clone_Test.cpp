#include "ace/RMCast/Protocol.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

namespace
{
  int clones {};
  int destructions {};

  class Derived_From : public ACE_RMCast::From
  {
  public:
    explicit Derived_From (ACE_RMCast::Address const &address)
      : ACE_RMCast::From (address)
    {
    }

    ~Derived_From () override
    {
      ++destructions;
    }

  private:
    Derived_From (Derived_From const &rhs)
      : ACE_RMCast::From (rhs)
    {
    }

    ACE_RMCast::Profile_ptr clone_ () override
    {
      ++clones;
      return ACE_RMCast::Profile_ptr (new Derived_From (*this));
    }
  };
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_UINT32 const loopback {0x7f000001};
  ACE_RMCast::Address const address (1234, loopback);
  Derived_From source (address);
  {
    ACE_RMCast::From_ptr copy = source.clone ();
    if (clones != 1 || copy.null () || copy->address () != address)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Typed profile clone failed\n")), 1);
      }

    ACE_RMCast::Profile_ptr const base (copy);
    copy.reset ();
    if (destructions != 0 || base.null () || base->id () != ACE_RMCast::From::id)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Profile clone ownership failed\n")), 1);
      }
  }
  if (destructions != 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Profile clone was not destroyed once\n")), 1);
    }
  return 0;
}
