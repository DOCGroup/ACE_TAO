// $Id$

//
// message
//
CUTS_INLINE
const ACE_CString & CUTS_DB_Exception::message (void) const
{
  return this->message_;
}

//
// native
//
CUTS_INLINE
long CUTS_DB_Exception::native (void) const
{
  return this->native_;
}

//
// state
//
CUTS_INLINE
const ACE_CString & CUTS_DB_Exception::state (void) const
{
  return this->state_;
}
