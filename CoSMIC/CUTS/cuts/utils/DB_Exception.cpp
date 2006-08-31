// $Id$

#include "DB_Exception.h"
#include "ace/Log_Msg.h"

#if !defined (__CUTS_INLINE__)
#include "DB_Exception.inl"
#endif

//
// CUTS_DB_Exception
//
CUTS_DB_Exception::CUTS_DB_Exception (void)
: native_ (0)
{

}

//
// CUTS_DB_Exception
//
CUTS_DB_Exception::CUTS_DB_Exception (const CUTS_DB_Exception & ex)
: native_ (ex.native_),
  state_ (ex.state_),
  message_ (ex.message_)
{

}

//
// CUTS_DB_Exception
//
CUTS_DB_Exception::CUTS_DB_Exception (const char * message)
: native_ (0),
  message_ (message)
{

}

//
// CUTS_DB_Exception
//
CUTS_DB_Exception::~CUTS_DB_Exception (void)
{

}

//
// print
//
void CUTS_DB_Exception::print (void) const
{
  ACE_ERROR ((LM_ERROR,
              "exception (%d | %s): %s\n",
              this->native_,
              this->state_.c_str (),
              this->message_.c_str ()));
}
