/* -*- C++ -*- */
// $Id$

// ================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   ProxyPushConsumer_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the
//   CosEventChannelAdmin::ProxyPushConsumer interface.
//
// ================================================================

#if !defined (_PROXYPUSHCONSUMER_I_H)
#define _PROXYPUSHCONSUMER_I_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEC_PushSupplierWrapper;

class TAO_ORBSVCS_Export TAO_CosEC_ProxyPushConsumer_i :
  public virtual POA_CosEventChannelAdmin::ProxyPushConsumer,
  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   class TAO_CosEC_ProxyPushConsumer_i implements the ProxyPushConsumer
  //   interface.
  //
  // = DESCRIPTION
  //   This implementation of the ProxyPushConsumer uses the
  //   RtecEventChannelAdmin::ProxyPushConsumer.
  //
  //   NOTE:  RtecEventChannelAdmin::ProxyPushConsumer::push method is
  //   passed a RtecEventComm::EventSet.The <EventHeader> field in that is
  //   initialized using the  the 1st <publications> from the <SupplierQOS>.
  //   so we assume that publications[0] is initialized.
  //
public:
  // = Initialization and termination methods.
  TAO_CosEC_ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos,
                                 RtecEventChannelAdmin::ProxyPushConsumer_ptr proxypushconsumer);
  // Constructor.

  ~TAO_CosEC_ProxyPushConsumer_i (void);
  // Destructor.

  virtual void push (const CORBA::Any &data,
                     CORBA::Environment &ACE_TRY_ENV);
  // Suppliers call this method to pass data to connected consumers.

  virtual void disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV);
  // Disconnects the supplier from the event communication.

  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier,
                                     CORBA::Environment &ACE_TRY_ENV);
  // Connects a push supplier.

private:
  int connected (void);
  // Returns 0 if a push_supplier is already connected to this
  // ProxyPushConsumer, otherwise it returns a 1.

  const RtecEventChannelAdmin::SupplierQOS &qos_;
  // The SupplierQOS specified by the user of this class.

  RtecEventChannelAdmin::ProxyPushConsumer_var proxypushconsumer_;
  // The Rtec ProxyPushConsumer specified by the user of this class.

  TAO_CosEC_PushSupplierWrapper *wrapper_;
  // The Rtec PushSupplier wrapper used by the Rtec ProxyPushConsumer.
};

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

#endif /* _PROXYPUSHCONSUMER_I_H */
