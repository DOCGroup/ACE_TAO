// -*- C++ -*-

//=============================================================================
/**
 * @file TX_Object_i.h
 *
 * $Id$
 *
 * Header for the SSLIOP Big_Request test server implementation.
 *
 * @author Andrew Finnell
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TX_OBJECT_I_H
#define TX_OBJECT_I_H

#include "TX_ObjectS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  TX_Object_i : public virtual POA_TX_Object
{
public:

  // Constructor
  TX_Object_i (CORBA::ORB_ptr orb);

  // Destructor
  virtual ~TX_Object_i (void);

  virtual void send (const DataSeq & data,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void recv (DataSeq_out data,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;

  DataSeq data_;

};


#endif  /* TX_OBJECT_I_H */
