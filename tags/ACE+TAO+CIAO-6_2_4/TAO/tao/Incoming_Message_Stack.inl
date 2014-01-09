// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/************************************************************************/
// Methods  for TAO_Incoming_Message_Stack
/************************************************************************/
namespace TAO
{
ACE_INLINE
Incoming_Message_Stack::Incoming_Message_Stack()
: top_(0)
{
}

ACE_INLINE
Incoming_Message_Stack::~Incoming_Message_Stack()
{
  // Delete all the nodes left behind
  TAO_Queued_Data *del = 0;

  while (this->pop (del) != -1)
    {
      TAO_Queued_Data::release (del);
    }
}

ACE_INLINE void
Incoming_Message_Stack::push(TAO_Queued_Data *data)
{
  data->next (this->top_);
  this->top_ = data;
}


/* @return 0 for Ok, -1 for error */
ACE_INLINE int
Incoming_Message_Stack::pop (TAO_Queued_Data* &data)
{
  if (this->top_ == 0)
    return -1;

  data = this->top_;
  this->top_ = data->next ();

  return 0;
}

ACE_INLINE int
Incoming_Message_Stack::top (TAO_Queued_Data* &data) const
{
  if (this->top_ == 0)
    return -1;

  data = this->top_;

  return 0;
}

} /* namespace TAO */
TAO_END_VERSIONED_NAMESPACE_DECL
