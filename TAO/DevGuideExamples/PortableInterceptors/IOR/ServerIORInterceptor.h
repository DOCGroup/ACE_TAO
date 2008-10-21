#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/IORInterceptor_Adapter.h"
#include "tao/IORInterceptor/IORInterceptor.h"

class ServerIORInterceptor :
public virtual PortableInterceptor::IORInterceptor,
public virtual TAO_Local_RefCounted_Object
{
 public:
  virtual char * name ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void establish_components (
                                     PortableInterceptor::IORInfo_ptr info
                                     )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void components_established (
    PortableInterceptor::IORInfo_ptr /*info*/
    )
    ACE_THROW_SPEC ((
    CORBA::SystemException
    ))
  {
  }

  virtual void adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId /*id*/,
    PortableInterceptor::AdapterState /*state*/
    )
    ACE_THROW_SPEC ((
    CORBA::SystemException
    ))
  {
  }

  virtual void adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq& /*templates*/,
    PortableInterceptor::AdapterState /*state*/
    )
    ACE_THROW_SPEC ((
    CORBA::SystemException
    ))
  {
  }
};
