// -*- C++ -*-

//=============================================================================
/**
 * @file Hasher_i.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_HASHER_I_H
#define TAO_HASHER_I_H

#include "HasherS.h"

/**
 * @class Hasher_i
 *
 * @brief Simple implementation of the "Hasher" interface.
 *
 * This implementation simply invokes the hash utility functions in
 * the ACE class (i.e. those in `ace/ACE.h').
 */
class Hasher_i
  : public virtual POA_Hasher
{
public:

  /// Perform a simple hashing operation on the input string and
  /// return the computed hash.
  virtual CORBA::ULong hash_string (const char * str,
                                    CORBA::Environment &ACE_TRY_ENV =
                                      TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

//   /// Shutdown the ORB.
//   virtual void shutdown (CORBA::Environment &ACE_TRY_ENV =
//                            TAO_default_environment ())
//     ACE_THROW_SPEC ((CORBA::SystemException));

};

#endif  /* TAO_HASHER_I_H */
