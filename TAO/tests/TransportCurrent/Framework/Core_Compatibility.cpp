/**
 * @file Core_Compatibility.cpp
 *
 * Verify copy behavior of TAO transport-related extension interfaces.
 */

#include "tao/Acceptor_Filter.h"
#include "tao/Connection_Purging_Strategy.h"
#include "tao/PolicyFactory_Registry_Adapter.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/Service_Context_Handler.h"

#include <type_traits>

namespace
{
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
}
