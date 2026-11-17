#include "Kokyu.h"

#include <memory>

#include "Default_Dispatcher_Impl.h"

namespace Kokyu
{
Dispatcher::Dispatcher () = default;
Dispatcher::~Dispatcher () = default;

int Dispatcher::dispatch (Dispatch_Command const *cmd, QoSDescriptor const &qos)
{
  return dispatcher_impl_->dispatch (cmd, qos);
}

int Dispatcher::shutdown ()
{
  return dispatcher_impl_->shutdown ();
}

int Dispatcher::activate ()
{
  return dispatcher_impl_->activate ();
}

void Dispatcher::implementation (Dispatcher_Impl *impl)
{
  this->dispatcher_impl_.reset (impl);
}

Dispatcher*
Dispatcher_Factory::
create_dispatcher (Dispatcher_Attributes const &attrs)
{
  Dispatcher_Impl *raw_impl {};
  ACE_NEW_RETURN (raw_impl, Default_Dispatcher_Impl, nullptr);
  std::unique_ptr<Dispatcher_Impl> impl (raw_impl);

  Dispatcher *raw_dispatcher {};
  ACE_NEW_RETURN (raw_dispatcher, Dispatcher, nullptr);
  std::unique_ptr<Dispatcher> dispatcher (raw_dispatcher);

  dispatcher->implementation (impl.release ());
  raw_impl->init (attrs);
  return dispatcher.release ();
}

}
