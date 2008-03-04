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
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::Components::CreateFailure
        ));

  protected:
  private:
  };

#endif /*MOCKOBJECTHOME_H_*/
