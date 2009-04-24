// $Id$
#ifndef MOCKOBJECTHOME_H_
#define MOCKOBJECTHOME_H_

#include "MocksTest_Export.h"
#include "Mocks/MockHome_Impl.h"

class MocksTest_Export MockObjectHome : public MockHome_Impl
  {
  public:
    MockObjectHome()
      {};
    virtual ~MockObjectHome()
      {};

    virtual ::Components::CCMObject_ptr create_component_with_config_values (
        const ::Components::ConfigValues & config
    );

  protected:
  private:
  };

#endif /*MOCKOBJECTHOME_H_*/
