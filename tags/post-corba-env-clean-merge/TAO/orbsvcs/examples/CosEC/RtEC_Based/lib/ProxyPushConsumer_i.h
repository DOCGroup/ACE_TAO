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

#ifndef _PROXYPUSHCONSUMER_I_H
#define _PROXYPUSHCONSUMER_I_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "rtec_cosec_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEC_PushSupplierWrapper;

class TAO_RTEC_COSEC_Export TAO_CosEC_ProxyPushConsumer_i :
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

  virtual void push (const CORBA::Any &data
                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Suppliers call this method to pass data to connected consumers.

  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Disconnects the supplier from the event communication.

  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier
                                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
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

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* _PROXYPUSHCONSUMER_I_H */
