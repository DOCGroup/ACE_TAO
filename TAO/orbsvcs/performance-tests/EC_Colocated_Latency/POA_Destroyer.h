/**
 * @file POA_Destroyer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef CS_POA_DESTROYER_H
#define CS_POA_DESTROYER_H

#include "tao/PortableServer/PortableServer.h"

class POA_Destroyer
{
public:
  /// Constructor
  ACE_EXPLICIT POA_Destroyer (PortableServer::POA_ptr poa);

  /// Destructor
  ~POA_Destroyer ();

  /// Release the POA and do not destroy it.
  void release ();

private:
  /// The owned object
  PortableServer::POA_var poa_;
};

#if defined(__ACE_INLINE__)
#include "POA_Destroyer.inl"
#endif /* __ACE_INLINE__ */

#endif /* CS_POA_DESTROYER_H */
