// -*- C++ -*-

//$Id$

ACE_INLINE
TAO_GIOP_Message_Acceptors::
  ~TAO_GIOP_Message_Acceptors (void)
{
  // Explicitly call the destructor of the output CDR first. They need
  // the allocators during destruction.
  delete this->output_;

  // Then call the destructor of our allocators
  if (this->cdr_dblock_alloc_ != 0)
    this->cdr_dblock_alloc_->remove ();
  delete this->cdr_dblock_alloc_;

  if (this->cdr_buffer_alloc_ != 0)
    this->cdr_buffer_alloc_->remove ();
  delete this->cdr_buffer_alloc_;
}


ACE_INLINE void
TAO_GIOP_Message_Acceptors::
set_state (CORBA::Octet def_major,
           CORBA::Octet def_minor)
{
  // @@Bala Need to find a better way
  // @@ Bala: what about a table:
  // Accept_State implementations[] = {
  //   Version_10,
  //   Version_11
  // };
  // this->accept_state_ = &implementations[def_minor];
  // @@ of course it requires range checking and the array must be in
  // some place where it is not detroyed too soon...
  // @@ I'm not sure if this implementations should be dynamically
  // loaded.
  // @@ BTW, this function is (IMHO) too big to be inlined, but that
  // is a matter of personal opinion, and what your experience with
  // busted compilers is...
  //
  switch (def_major)
    {
    case 1:
      switch (def_minor)
        {
        case 0:
          this->accept_state_ =
            &this->implementations_.version_10;
          break;
        case 1:
          this->accept_state_ =
            &this->implementations_.version_11;
          break;
        case 2:
          this->accept_state_ =
            &this->implementations_.version_12;
          break;
        default:
          break;
        }
      break;
    default:
      break;
    }
}
