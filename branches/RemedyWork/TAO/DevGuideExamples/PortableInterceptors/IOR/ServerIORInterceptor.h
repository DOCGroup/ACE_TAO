// $Id$

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/IORInterceptor/IORInterceptor.h"

class ServerIORInterceptor :
public virtual PortableInterceptor::IORInterceptor_3_0,
public virtual ::CORBA::LocalObject
{
 public:
  virtual char * name ();

  virtual void destroy ();

  virtual void establish_components (PortableInterceptor::IORInfo_ptr info);

  virtual void components_established (PortableInterceptor::IORInfo_ptr info);

  virtual void adapter_manager_state_changed (
    const char* id,
    PortableInterceptor::AdapterState state);

  virtual void adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq& templates,
    PortableInterceptor::AdapterState state);
};
