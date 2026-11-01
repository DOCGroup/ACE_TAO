#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/IORInterceptor/IORInterceptor.h"

class ServerIORInterceptor :
public virtual PortableInterceptor::IORInterceptor_3_0,
public virtual ::CORBA::LocalObject
{
 public:
  ServerIORInterceptor () = default;
  ServerIORInterceptor (ServerIORInterceptor const &) = delete;
  ServerIORInterceptor (ServerIORInterceptor &&) = delete;
  ServerIORInterceptor &operator= (ServerIORInterceptor const &) = delete;
  ServerIORInterceptor &operator= (ServerIORInterceptor &&) = delete;

  char * name () override;

  void destroy () override;

  void establish_components (PortableInterceptor::IORInfo_ptr info) override;

  void components_established (PortableInterceptor::IORInfo_ptr info) override;

  void adapter_manager_state_changed (
    char const *id,
    PortableInterceptor::AdapterState state) override;

  void adapter_state_changed (
    PortableInterceptor::ObjectReferenceTemplateSeq const& templates,
    PortableInterceptor::AdapterState state) override;
};
