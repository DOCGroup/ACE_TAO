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


class TAO_ORB_Core;
class TAO_IIOP_Properties;

namespace TAO
{
  namespace SSLIOP
  {
    class Connection_Handler_State;

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

      /// Setup the state that will be passed to the connection handler
      /// when the handler is constructed.
      static int setup_handler_state (
        TAO_ORB_Core *orb_core,
        TAO_IIOP_Properties *tcp_properties,
        Connection_Handler_State &state);

    };

  }  // End SSLIOP namespace.
}  // End TAO namespace.


#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_UTIL_H */
