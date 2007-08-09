// $Id$

ACE_INLINE
Server_i::Server_i (void)
  : done_ (0)
{
}

ACE_INLINE int
Server_i::done (void)
{
  return this->done_;
}
