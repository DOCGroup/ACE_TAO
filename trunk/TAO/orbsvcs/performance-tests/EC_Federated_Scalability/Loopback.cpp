/**
 * @file Loopback.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback.h"
#include "ECFS_Configuration.h"

ACE_RCSID(EC_Federated_Scalability, Loopback, "$Id")

ECFS_Loopback::ECFS_Loopback (void)
{
}

void
ECFS_Loopback::init (CORBA::Long experiment_id,
                     RtecEventChannelAdmin::EventChannel_ptr ec,
                     CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_ =
    Servant_var<ECFS_Loopback_Supplier> (
           new ECFS_Loopback_Supplier (experiment_id)
           );
  this->supplier_->connect (ec, ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_ =
    Servant_var<ECFS_Loopback_Consumer> (
            new ECFS_Loopback_Consumer (experiment_id,
                                        this->supplier_.in ())
            );
  this->consumer_->connect (ec, ACE_TRY_ENV);
  ACE_CHECK;
}

void
ECFS_Loopback::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->consumer_->disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->supplier_->disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<ECFS_Loopback_Consumer>;
template class Servant_var<ECFS_Loopback_Supplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<ECFS_Loopback_Consumer>
#pragma instantiate Servant_var<ECFS_Loopback_Supplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
