// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  Reply_Guard::Reply_Guard (Invocation_Base *b,
                            Invocation_Status s)
    : invocation_ (b)
    , status_ (s)
  {
  }

  ACE_INLINE
  Reply_Guard::~Reply_Guard (void)
  {
    this->invocation_->reply_received (this->status_);
  }

  ACE_INLINE void
  Reply_Guard::set_status (Invocation_Status s)
  {
    this->status_ = s;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
