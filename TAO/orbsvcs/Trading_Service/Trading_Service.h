// -*- C++ -*-

//=============================================================================
/**
 *  @file    Trading_Service.h
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef _TRADING_SERVICE_H
#define _TRADING_SERVICE_H

#include "ace/Sig_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"
#include "orbsvcs/Trader/Trading_Loader.h"

class Trading_Service;

/**
 * @class Trading_Shutdown
 *
 * @brief A class that shutsdown a Trading Service instance.
 */
class Trading_Shutdown : public ACE_Event_Handler
{
public:
  /// Constructor.
  Trading_Shutdown (Trading_Service & trader);

  Trading_Shutdown (Trading_Shutdown const &) = delete;
  Trading_Shutdown (Trading_Shutdown &&) = delete;
  Trading_Shutdown &operator= (Trading_Shutdown const &) = delete;
  Trading_Shutdown &operator= (Trading_Shutdown &&) = delete;

  /// Signal handler.
  int handle_signal (int,
                     siginfo_t *,
                     ucontext_t *) override;

protected:
  Trading_Service &trader_;
  ACE_Sig_Handler shutdown_;
};

/**
 * @class Trading_Service
 *
 * @brief A class that initializes a Trading Service instance.
 */
class Trading_Service
{
public:
  Trading_Service () = default;

  Trading_Service (Trading_Service const &) = delete;
  Trading_Service (Trading_Service &&) = delete;
  Trading_Service &operator= (Trading_Service const &) = delete;
  Trading_Service &operator= (Trading_Service &&) = delete;

  ~Trading_Service ();

  /// Initialize the Trading Service with arguments.
  int init (int argc,
            ACE_TCHAR * argv[]);

  /// Run the Trading Service.
  int run ();

  int shutdown ();

protected:
  /// Instance of TAO_Trading_Laoder
  TAO_Trading_Loader trading_loader_;
};

#endif /* _TRADING_SERVICE_H */
