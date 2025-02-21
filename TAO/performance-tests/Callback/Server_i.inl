ACE_INLINE
Server_i::Server_i ()
  : done_ (0)
{
}

ACE_INLINE int
Server_i::done ()
{
  return this->done_;
}
