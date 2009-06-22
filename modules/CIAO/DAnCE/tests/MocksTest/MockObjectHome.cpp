// $Id$
#include "MockObjectHome.h"

::Components::CCMObject_ptr
MockObjectHome::create_component_with_config_values (
    const ::Components::ConfigValues & config
)
  {
    return MockHome_Impl::create_component_with_config_values (config);
  }

extern "C" MocksTest_Export MockHome_Impl *create_MockHome (void);

MockHome_Impl*create_MockHome (void)
  {
    MockHome_Impl* home = 0;
    ACE_NEW_RETURN (home, MockObjectHome (), 0);
    return home;
  }
