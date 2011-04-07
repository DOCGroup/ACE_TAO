// -*- C++ -*-

//=============================================================================
/**
 *  @file    Trading_Service.h
 *
 *  $Id$
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef _TRADING_SERVICE_H
#define _TRADING_SERVICE_H

#include "ace/Auto_Ptr.h"
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
  Trading_Shutdown (Trading_Service& trader);

  /// Signal handler.
  virtual int handle_signal (int,
                             siginfo_t *,
                             ucontext_t *);

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

  /// Default constructor.
  Trading_Service (void);

  /// Destructor
  ~Trading_Service (void);

  /// Initialize the Trading Service with arguments.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the Trading Service.
  int run (void);

  int shutdown (void);

protected:

  /// Pointer to the linked trader.
  auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader_;

  /// Instance of TAO_Trading_Laoder
  TAO_Trading_Loader trading_loader_;
};

#endif /* _TRADING_SERVICE_H */
