// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_Util.h
 *
 *  $Id$
 *
 *  Utility class used by the SSLIOP pluggable protocol.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_UTIL_H
#define TAO_SSLIOP_UTIL_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Current.h"

class TAO_ORB_Core;
class TAO_IIOP_Properties;

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class Util
     *
     * @brief Class that provides utility/helper methods for several
     *        classes in the SSLIOP pluggable protocol.
     *
     * Methods useful to many classes in the SSLIOP pluggable protocol
     * are centrally located in this uility class.
     */
    class Util
    {
    public:

      /// Access Current.
      static TAO::SSLIOP::Current_ptr current (
        TAO_ORB_Core *orb_core);

    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.


#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_UTIL_H */
