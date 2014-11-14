// $Id$

#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Adapter_Registry.h"
#include "tao/Adapter.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/TAO_Server_Request.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter_Registry::TAO_Adapter_Registry (TAO_ORB_Core *)
  : adapters_capacity_ (16), // @@ Make it configurable
    adapters_count_ (0),
    adapters_ (0)
{
  ACE_NEW (this->adapters_,
           TAO_Adapter*[this->adapters_capacity_]);
}

TAO_Adapter_Registry::~TAO_Adapter_Registry (void)
{
  for (size_t i = 0; i != this->adapters_count_; ++i)
    delete this->adapters_[i];

  delete[] this->adapters_;
}

void
TAO_Adapter_Registry::close (int wait_for_completion)
{
  try
    {
      for (size_t i = 0; i != this->adapters_count_; ++i)
        {
          this->adapters_[i]->close (wait_for_completion);
        }
    }
  catch (const::CORBA::Exception &ex)
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
  for (size_t i = 0; i != this->adapters_count_; ++i)
    {
      this->adapters_[i]->check_close (wait_for_completion);
    }
}

void
TAO_Adapter_Registry::insert (TAO_Adapter *adapter)
{
  if (this->adapters_capacity_ == this->adapters_count_)
    {
      this->adapters_capacity_ *= 2;
      TAO_Adapter **tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        TAO_Adapter*[this->adapters_capacity_],
                        CORBA::NO_MEMORY ());

      for (size_t i = 0; i != this->adapters_count_; ++i)
        tmp[i] = this->adapters_[i];
      delete[] this->adapters_;
      this->adapters_ = tmp;
    }

  int const priority = adapter->priority ();
  for (size_t i = 0; i != this->adapters_count_; ++i)
    {
      if (this->adapters_[i]->priority () >= priority)
        {
          for (size_t j = this->adapters_count_ + 1;
               j > i;
               --j)
            {
              this->adapters_[j] = this->adapters_[j - 1];
            }
          this->adapters_[i] = adapter;
          ++this->adapters_count_;
          return;
        }
    }
  this->adapters_[this->adapters_count_++] = adapter;
}

void
TAO_Adapter_Registry::dispatch (TAO::ObjectKey &key,
                                TAO_ServerRequest &request,
                                CORBA::Object_out forward_to)
{
  for (size_t i = 0; i != this->adapters_count_; ++i)
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
                                                const TAO_MProfile &mprofile)
{
  for (size_t i = 0; i != this->adapters_count_; ++i)
    {
      CORBA::Object_ptr x =
        this->adapters_[i]->create_collocated_object (stub, mprofile);
      if (x != 0)
        {
          if (!stub->collocated_servant ())
            {
              // This adapter created an object but it was not able to locate
              // a servant so we need to give the rest of the adapters a chance to
              // initialise the stub and find a servant or forward us or whatever.
              for (CORBA::Long go_on = 1; go_on && i != this->adapters_count_;
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
  return 0;
}

CORBA::Long
TAO_Adapter_Registry::initialize_collocated_object (TAO_Stub *stub)
{
  for (size_t i = 0; i != this->adapters_count_; ++i)
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
TAO_Adapter_Registry::find_adapter (const char *name) const
{
  for (TAO_Adapter **i = this->adapters_;
       i != this->adapters_ + this->adapters_count_;
       ++i)
    if (ACE_OS::strcmp ((*i)->name (), name) == 0)
      return *i;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
