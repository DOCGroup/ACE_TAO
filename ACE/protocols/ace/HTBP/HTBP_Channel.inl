// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE::HTBP::Session *
ACE::HTBP::Channel::session () const
{
  return this->session_;
}

// session assignment only occurs when the stream is transferred to another
// session. Do *not* delete the old session object.
ACE_INLINE
void
ACE::HTBP::Channel::session (ACE::HTBP::Session *s)
{
  this->session_ = s;
}

// I'm not sure of the proper use case for getting the filter reference. The
// problem is that the filter is not reference counted, so the returned value
// should not be stored.
ACE_INLINE
ACE::HTBP::Filter *
ACE::HTBP::Channel::filter () const
{
  return this->filter_;
}

// Filters are owned by the channel. If a new filter is applied, the old one
// must be deleted.
ACE_INLINE
void
ACE::HTBP::Channel::filter (ACE::HTBP::Filter *f)
{
  delete this->filter_;
  this->filter_ = f;
}

ACE_INLINE
int
ACE::HTBP::Channel::close_reader ()
{
  return this->ace_stream_.close_reader();
}

ACE_INLINE
int
ACE::HTBP::Channel::close_writer ()
{
  return this->ace_stream_.close_writer();
}

ACE_INLINE
int
ACE::HTBP::Channel::close ()
{
  return this->ace_stream_.close();
}

ACE_INLINE
ACE::HTBP::Channel::State
ACE::HTBP::Channel::state () const
{
  return this->state_;
}

ACE_INLINE
ACE_Message_Block &
ACE::HTBP::Channel::leftovers ()
{
  return this->leftovers_;
}

ACE_INLINE
size_t
ACE::HTBP::Channel::data_len() const
{
  return this->data_len_;
}

ACE_INLINE
void
ACE::HTBP::Channel::data_len(size_t n)
{
  this->data_len_ = n;
}

ACE_INLINE
size_t
ACE::HTBP::Channel::data_consumed() const
{
  return this->data_consumed_;
}


ACE_INLINE
const ACE_SOCK_Stream&
ACE::HTBP::Channel::ace_stream () const
{
  return this->ace_stream_;
}

ACE_INLINE
ACE_SOCK_Stream&
ACE::HTBP::Channel::ace_stream ()
{
  return this->ace_stream_;
}

ACE_INLINE
void
ACE::HTBP::Channel::set_handle (ACE_HANDLE h)
{
  if (h == 0)
    return;
  this->ace_stream_.set_handle (h);
}

ACE_END_VERSIONED_NAMESPACE_DECL
