// $Id$

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include        "be_sunsoft.h"

ACE_RCSID(be, be_factory, "$Id$")

// constructor
TAO_OutStream_Factory::TAO_OutStream_Factory (void)
  : strm_type_ (TAO_OutStream_Factory::TAO_SUNSOFT)
{
}

// destructor
TAO_OutStream_Factory::~TAO_OutStream_Factory (void)
{
}

// set the type of specialized o/p stream we want
int
TAO_OutStream_Factory::set_stream_type
(TAO_OutStream_Factory::TAO_OutStream_Type t)
{
  this->strm_type_ = t;
  return 0;
}

// factory method
TAO_OutStream *
TAO_OutStream_Factory::make_outstream (void)
{
  switch (this->strm_type_)
    {
    case TAO_OutStream_Factory::TAO_SUNSOFT:
      return new TAO_SunSoft_OutStream ();
    case TAO_OutStream_Factory::TAO_FLICK:
      return (TAO_OutStream *)0;  // not implemented as yet
    default:
      return (TAO_OutStream *)0;
    }
}
