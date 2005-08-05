// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Location.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_SERVANT_LOCATION_H
#define TAO_SERVANT_LOCATION_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

enum TAO_SERVANT_LOCATION
{
  TAO_SERVANT_FOUND,
  TAO_DEFAULT_SERVANT,
  TAO_SERVANT_MANAGER,
  TAO_SERVANT_NOT_FOUND
};

#include /**/ "ace/post.h"

#endif /* TAO_SERVANT_LOCATION_H */
