// $Id$

//=============================================================================
/**
 * @file sum_server_i.h
 *
 * $Id$
 *
 * Implementation header for the "server" IDL interface for the
 * ORT example.
 *
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef SUM_SERVER_I_H
#define SUM_SERVER_I_H

#include "sum_serverS.h"

// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"

class sum_server_i : public virtual POA_ORT::sum_server
{
 public:

  /// Constructor
  sum_server_i ();

  /// add variables method
  CORBA::Long add_variables (CORBA::Long a,
                             CORBA::Long b
                             TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#endif /* SUM_SERVER_I_H */
