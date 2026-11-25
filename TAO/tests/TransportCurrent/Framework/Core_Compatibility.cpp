/**
 * @file Core_Compatibility.cpp
 *
 * Verify source compatibility of TAO transport-related interfaces.
 */

#include "tao/Acceptor_Filter.h"
#include "tao/Adapter.h"
#include "tao/Connection_Purging_Strategy.h"
#include "tao/IIOP_Connector.h"
#include "tao/PolicyFactory_Registry_Adapter.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/Protocol_Factory.h"
#include "tao/Resource_Factory.h"
#include "tao/Service_Context_Handler.h"
#include "tao/Transport_Acceptor.h"

#include <type_traits>
#include <utility>

namespace
{
  struct Connector_Probe : TAO_IIOP_Connector
  {
    using TAO_Connector::active_connect_strategy_;
  };

  struct Object_Adapter : TAO_Adapter
  {
    void open () override;
    void close (int) override;
    void check_close (int) override;
    int priority () const override;
    int dispatch (TAO::ObjectKey &, TAO_ServerRequest &, CORBA::Object_out) override;
    char const *name () const override;
    CORBA::Object_ptr root () override;
    CORBA::Object_ptr create_collocated_object (TAO_Stub *, TAO_MProfile const &) override;
    CORBA::Long initialize_collocated_object (TAO_Stub *) override;
  };

  struct Handler : TAO_Service_Context_Handler
  {
    int process_service_context (TAO_Transport &, IOP::ServiceContext const &,
                                 TAO_ServerRequest *) override
    {
      return 0;
    }
    int generate_service_context (TAO_Stub *, TAO_Transport &,
                                  TAO_Operation_Details &,
                                  TAO_Target_Specification &,
                                  TAO_OutputCDR &) override
    {
      return 0;
    }
  };

  struct Adapter : TAO::PolicyFactory_Registry_Adapter
  {
    void register_policy_factory (CORBA::PolicyType,
                                  PortableInterceptor::PolicyFactory_ptr) override
    {
    }
    CORBA::Policy_ptr create_policy (CORBA::PolicyType,
                                     CORBA::Any const &) override
    {
      return nullptr;
    }
    CORBA::Policy_ptr _create_policy (CORBA::PolicyType) override
    {
      return nullptr;
    }
    bool factory_exists (CORBA::PolicyType) const override
    {
      return false;
    }
  };

  struct Strategy : TAO_Connection_Purging_Strategy
  {
    Strategy () : TAO_Connection_Purging_Strategy (10)
    {
    }
    void update_item (TAO_Transport &) override
    {
    }
  };

  static_assert (std::is_copy_constructible<Handler>::value);
  static_assert (std::is_move_assignable<Handler>::value);
  static_assert (std::is_copy_constructible<Adapter>::value);
  static_assert (std::is_move_assignable<Adapter>::value);
  static_assert (std::is_copy_constructible<Strategy>::value);
  static_assert (std::is_move_constructible<Strategy>::value);
  static_assert (!std::is_copy_assignable<Strategy>::value);
  static_assert (std::is_copy_constructible<TAO_Default_Acceptor_Filter>::value);
  static_assert (std::is_move_assignable<TAO_Default_Acceptor_Filter>::value);
  static_assert (std::is_copy_constructible<Object_Adapter>::value);
  static_assert (std::is_move_assignable<Object_Adapter>::value);
  static_assert (std::is_same<decltype (std::declval<TAO_Acceptor &> ().is_collocated (nullptr)),
                              bool>::value);
  using Match_Prefix = int (TAO_Protocol_Factory::*) (std::string const &);
  static_assert (std::is_same<decltype (&TAO_Protocol_Factory::match_prefix),
                              Match_Prefix>::value);
  static_assert (std::is_constructible<TAO_Protocol_Item, std::string const &>::value);
  static_assert (std::is_same<decltype (std::declval<TAO_Protocol_Item &> ().protocol_name ()),
                              std::string const &>::value);
  static_assert (std::is_same<decltype (Connector_Probe::active_connect_strategy_),
                              std::unique_ptr<TAO_Connect_Strategy>>::value);
}
