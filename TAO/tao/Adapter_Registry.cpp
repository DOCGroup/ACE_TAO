#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Adapter_Registry.h"
#include "tao/Adapter.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/TAO_Server_Request.h"

#include <cstring>
#include <new>
#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter_Registry::TAO_Adapter_Registry (TAO_ORB_Core *)
{
}

TAO_Adapter_Registry::~TAO_Adapter_Registry () = default;

void
TAO_Adapter_Registry::close (int wait_for_completion)
{
  try
    {
      for (adapter_type const &adapter : this->adapters_)
        {
          adapter->close (wait_for_completion);
        }
    }
  catch (::CORBA::Exception const &ex)
    {
      if (TAO_debug_level > 3)
        {
          ex._tao_print_exception (
            "Exception in TAO_Adapter_Registry::close ()");
        }
      return;
    }
}

void
TAO_Adapter_Registry::check_close (int wait_for_completion)
{
  for (adapter_type const &adapter : this->adapters_)
    {
      adapter->check_close (wait_for_completion);
    }
}

void
TAO_Adapter_Registry::insert (TAO_Adapter *adapter)
{
  int const priority = adapter->priority ();
  adapter_container_type::difference_type insertion_offset {};
  adapter_container_type::const_iterator position = this->adapters_.begin ();
  for (; position != this->adapters_.end (); ++position, ++insertion_offset)
    {
      if ((*position)->priority () >= priority)
        {
          break;
        }
    }

  try
    {
      // Reserve before taking ownership so a failed allocation leaves the
      // adapter with the caller, as expected by insert's transfer contract.
      this->adapters_.reserve (this->adapters_.size () + 1);
      adapter_type owned_adapter (adapter);
      this->adapters_.insert (this->adapters_.begin () + insertion_offset,
                              std::move (owned_adapter));
    }
  catch (std::bad_alloc const &)
    {
      throw CORBA::NO_MEMORY ();
    }
}

void
TAO_Adapter_Registry::dispatch (TAO::ObjectKey &key,
                                TAO_ServerRequest &request,
                                CORBA::Object_out forward_to)
{
  for (adapter_type const &adapter : this->adapters_)
    {
      int const r = adapter->dispatch (key, request, forward_to);

      if (r != TAO_Adapter::DS_MISMATCHED_KEY)
        {
          return;
        }
    }

  if (!request.is_forwarded ())
    {
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }
}

CORBA::Object_ptr
TAO_Adapter_Registry::create_collocated_object (TAO_Stub *stub,
                                                TAO_MProfile const &mprofile)
{
  for (adapter_container_type::iterator adapter = this->adapters_.begin ();
       adapter != this->adapters_.end ();
       ++adapter)
    {
      CORBA::Object_ptr x =
        (*adapter)->create_collocated_object (stub, mprofile);
      if (x != nullptr)
        {
          if (!stub->collocated_servant ())
            {
              // This adapter created an object but it was not able to locate
              // a servant so we need to give the rest of the adapters a chance to
              // initialise the stub and find a servant or forward us or whatever.
              for (CORBA::Long go_on = 1;
                   go_on && adapter != this->adapters_.end ();
                   ++adapter)
                {
                  // initialize_collocated_object only returns 0 if it has completely
                  // initialised the object.
                  go_on = (*adapter)->initialize_collocated_object (stub);
                }
            }
          return x;
        }
    }
  return nullptr;
}

CORBA::Long
TAO_Adapter_Registry::initialize_collocated_object (TAO_Stub *stub)
{
  for (adapter_type const &adapter : this->adapters_)
    {
      int const retval =
        adapter->initialize_collocated_object (stub);
      if (retval == 0)
        {
          // initialize_collocated_object only returns 0 if it has completely
          // initialised the object. We can return early.
          return retval;
        }
    }
  return 0;
}

TAO_Adapter *
TAO_Adapter_Registry::find_adapter (char const *name) const
{
  for (adapter_type const &adapter : this->adapters_)
    if (std::strcmp (adapter->name (), name) == 0)
      return adapter.get ();

  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
