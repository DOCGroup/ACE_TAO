// -*- C++ -*-

//=============================================================================
/**
 *  @file    Synch_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_SYNCH_INVOCATION_H
#define TAO_SYNCH_INVOCATION_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace  CORBA
{
  class Object;
}

namespace TAO
{
  /**
   * @class Synch_Invocation
   *
   * @brief Base class for Twoway_Invocation and Oneway_Invocation.
   *
   */
  class TAO_Export Synch_Invocation
  {
  public:
    Synch_Invocation ();
  };

  class TAO_Export Synch_Twoway_Invocation: public Synch_Invocation
  {
  public:

  };
}

#endif /*TAO_SYNCH_INVOCATION_H*/
