// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Protocol_Factory.h
//
// = AUTHOR
//   Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PROTOCOL_FACTORY_H
#define TAO_PROTOCOL_FACTORY_H

#include "ace/Service_Object.h"
#include "ace/Strategies_T.h"
#include "ace/SString.h"
#include "tao/Pluggable.h"
#include "tao/GIOP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Export TAO_Protocol_Factory : public ACE_Service_Object
{
public:
  TAO_Protocol_Factory (void);
  virtual ~TAO_Protocol_Factory (void);

  virtual int init (int argc, char *argv[]);
  // Initialization hook.

  virtual int match_prefix (const ACE_CString &prefix);
  // verify prefix is a match

  // Factory methods 
  virtual TAO_Acceptor  *make_acceptor (void);
  // create an acceptor

  virtual TAO_Connector *make_connector  (void);
  // create a connector

};

#endif /* TAO_PROTOCOL_FACTORY_H */
