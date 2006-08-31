// $Id$

//
// is_connected
//
CUTS_INLINE
bool CUTS_DB_Connection::is_connected (void) const
{
  return this->connected_;
}

//
// disconnect
//
CUTS_INLINE
void CUTS_DB_Connection::disconnect (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->connected_ = false;
}

//
// connect
//
CUTS_INLINE
void CUTS_DB_Connection::connect (const char * username,
                                  const char * password,
                                  const char * server,
                                  int port)
                                  ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->connected_ = true;
}
