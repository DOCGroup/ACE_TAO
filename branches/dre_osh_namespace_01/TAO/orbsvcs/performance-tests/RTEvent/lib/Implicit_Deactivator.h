/**
 * @file Implicit_Deactivator.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_IMPLICIT_DEACTIVATOR_H
#define TAO_PERF_RTEC_IMPLICIT_DEACTIVATOR_H

#include "rtec_perf_export.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class Implicit_Deactivator
 *
 * @brief Automatically deactivate an implicitly activated object.
 */
class TAO_RTEC_Perf_Export Implicit_Deactivator
{
public:
  /// Constructor
  /**
   * @param client The client
   */
  ACE_EXPLICIT Implicit_Deactivator (PortableServer::Servant servant = 0
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  Implicit_Deactivator (Implicit_Deactivator& rhs);
  Implicit_Deactivator& operator= (Implicit_Deactivator& rhs);

  /// Destructor
  ~Implicit_Deactivator (void);

  /// Assignment operator
  Implicit_Deactivator& operator= (PortableServer::Servant servant);

  /// Release the servant, i.e. the destructor does not deactivate anything
  void release (void);

private:
  /// The POA
  PortableServer::POA_var poa_;

  /// The object id
  PortableServer::ObjectId_var id_;
};

#if defined(__ACE_INLINE__)
#include "Implicit_Deactivator.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_IMPLICIT_DEACTIVATOR_H */
