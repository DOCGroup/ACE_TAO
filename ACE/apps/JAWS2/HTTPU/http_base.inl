// -*- c++ -*-
#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_Base::HTTP_Base ()
  : status_ (200),
    line_ (0),
    deliver_state_ (0),
    no_headers_ (0),
    iter_ (*(this->info_.table ())),
    mb_ (0)
{
}

ACE_INLINE
HTTP_Base::~HTTP_Base ()
{
  if (this->line_)
    ACE_OS::free (this->line_);
  if (this->mb_)
    delete this->mb_;
  this->line_ = 0;
  this->mb_ = 0;
}

ACE_INLINE int
HTTP_Base::status () const
{
  return this->status_;
}

ACE_INLINE const char *
HTTP_Base::line () const
{
  return this->line_;
}

ACE_INLINE HTTP_Headers *
HTTP_Base::http_headers ()
{
  return &(this->info_);
}

ACE_INLINE JAWS_Headers *
HTTP_Base::headers ()
{
  return this->info_.table ();
}
