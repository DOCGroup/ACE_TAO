/**
 * @file Loopback.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback.h"
#include "ECFL_Configuration.h"

ACE_RCSID(EC_Federated_Latency, Loopback, "$Id$")

ECFL_Loopback::ECFL_Loopback (void)
{
}

void
ECFL_Loopback::init (CORBA::Long experiment_id,
                     RtecEventChannelAdmin::EventChannel_ptr ec
                     ACE_ENV_ARG_DECL)
{
  this->supplier_ =
    Servant_var<ECFL_Loopback_Supplier> (
           new ECFL_Loopback_Supplier (experiment_id)
           );
  this->supplier_->connect (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_ =
    Servant_var<ECFL_Loopback_Consumer> (
            new ECFL_Loopback_Consumer (experiment_id,
                                        this->supplier_.in ())
            );
  this->consumer_->connect (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ECFL_Loopback::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->consumer_->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->supplier_->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<ECFL_Loopback_Consumer>;
template class Servant_var<ECFL_Loopback_Supplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<ECFL_Loopback_Consumer>
#pragma instantiate Servant_var<ECFL_Loopback_Supplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
