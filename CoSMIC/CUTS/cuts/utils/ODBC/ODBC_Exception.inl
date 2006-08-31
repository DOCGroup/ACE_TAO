// $Id$

//
// message
//
CUTS_INLINE
const ACE_CString & ODBC_Exception::message (void) const
{
  return this->message_;
}

//
// state
//
CUTS_INLINE
const ACE_CString & ODBC_Exception::state (void) const
{
  return this->state_;
}

//
// native
//
CUTS_INLINE
long ODBC_Exception::native (void) const
{
  return this->native_;
}
