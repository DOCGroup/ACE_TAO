// -*- C++ -*-
//
// $Id$

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE ::TAO_Root_POA &
    Non_Servant_Upcall::poa (void) const
    {
      return this->poa_;
    }
  }
}

