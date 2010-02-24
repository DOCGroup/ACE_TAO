//$Id$
#include "Big_Reply_i.h"

Big_Reply_i::Big_Reply_i (CORBA::ORB_ptr orb,
                          CORBA::ULong len)
  : orb_ (orb),
    length_ (len)
{
}

Big_Reply_i::~Big_Reply_i (void)
{
  // no-op
}

Test::Octet_Seq *
Big_Reply_i::get_big_reply ( /**/)
{
  Test::Octet_Seq_var reply_mesg =
    new Test::Octet_Seq (this->length_);

  reply_mesg->length (this->length_);

  return reply_mesg._retn ();
}

void
Big_Reply_i::ping ( /**/)
{
}

void
Big_Reply_i::shutdown ( /**/)
{
  this->orb_->shutdown ();
}
