#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Adapter_Registry.h"
#include "tao/Adapter.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/TAO_Server_Request.h"

#include <cstring>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter_Registry::TAO_Adapter_Registry (TAO_ORB_Core *)
{
}

TAO_Adapter_Registry::~TAO_Adapter_Registry ()
{
  for (TAO_Adapter *adapter : this->adapters_)
    delete adapter;
}

void
TAO_Adapter_Registry::close (int wait_for_completion)
{
  try
    {
      for (size_t i = 0; i != this->adapters_.size (); ++i)
        {
          this->adapters_[i]->close (wait_for_completion);
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

  return;
}

void
TAO_Adapter_Registry::check_close (int wait_for_completion)
{
  for (size_t i = 0; i != this->adapters_.size (); ++i)
    {
      this->adapters_[i]->check_close (wait_for_completion);
    }
}

void
TAO_Adapter_Registry::insert (TAO_Adapter *adapter)
{
  int const priority = adapter->priority ();
  size_t i = 0;
  for (; i != this->adapters_.size (); ++i)
    {
      if (this->adapters_[i]->priority () >= priority)
        {
          break;
        }
    }

  try
    {
      this->adapters_.insert (
        this->adapters_.begin () +
          static_cast<std::vector<TAO_Adapter *>::difference_type> (i),
        adapter);
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
  for (size_t i = 0; i != this->adapters_.size (); ++i)
    {
      int const r = this->adapters_[i]->dispatch (key, request, forward_to);

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
  for (size_t i = 0; i != this->adapters_.size (); ++i)
    {
      CORBA::Object_ptr x =
        this->adapters_[i]->create_collocated_object (stub, mprofile);
      if (x != nullptr)
        {
          if (!stub->collocated_servant ())
            {
              // This adapter created an object but it was not able to locate
              // a servant so we need to give the rest of the adapters a chance to
              // initialise the stub and find a servant or forward us or whatever.
              for (CORBA::Long go_on = 1; go_on && i != this->adapters_.size ();
                   ++i)
                {
                  // initialize_collocated_object only returns 0 if it has completely
                  // initialised the object.
                  go_on = this->adapters_[i]->initialize_collocated_object (
                    stub);
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
  for (size_t i = 0; i != this->adapters_.size (); ++i)
    {
      int const retval =
        this->adapters_[i]->initialize_collocated_object (stub);
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
  for (TAO_Adapter *adapter : this->adapters_)
    if (std::strcmp (adapter->name (), name) == 0)
      return adapter;

  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
