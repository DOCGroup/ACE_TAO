// $Id$

#include "Exception.h"

#if !defined (__ADBC_INLINE__)
#include "Exception.inl"
#endif

namespace ADBC
{
//
// Exception
//
Exception::Exception (void)
: native_ (0)
{

}

//
// Exception
//
Exception::
Exception (long native, const char * state, const char * msg)
: native_ (native),
  state_ (state),
  message_ (msg)
{

}

//
// Exception
//
Exception::Exception (const Exception & ex)
: native_ (ex.native_),
  state_ (ex.state_),
  message_ (ex.message_)
{

}

//
// Exception
//
Exception::Exception (const char * message)
: native_ (0),
  message_ (message)
{

}

//
// Exception
//
Exception::~Exception (void)
{

}
}
