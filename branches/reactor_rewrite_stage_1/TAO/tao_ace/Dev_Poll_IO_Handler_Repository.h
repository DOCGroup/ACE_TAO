/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Select_IO_Handler_Repository.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_DEV_POLL_IO_HANDLER_REPOSITORY_H
#define TAO_ACE_DEV_POLL_IO_HANDLER_REPOSITORY_H
#include "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Dev_Poll_IO_Handler_State: public TAO_ACE_IO_Handler_Repository::IO_Handler_State
{
public:

  Dev_Poll_IO_Handler_State (void);

  /// Need to add mask and the suspend status...
};

/**
 * @class TAO_ACE_Select_IO_Handler_Repository
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_Dev_Poll_IO_Handler_Repository : public TAO_ACE_IO_Handler_Repository
{
public:

  /// Ctor
  TAO_ACE_Dev_Poll_IO_Handler_Repository (void);

private:


};

#endif /*TAO_ACE_DEV_POLL_IO_HANDLER_REPOSITORY_H*/
