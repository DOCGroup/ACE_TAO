//
// $Id$
//
#include "Empty.h"

ACE_RCSID(Multiple_Deactivations, Empty, "$Id$")

CORBA::ULong Empty::in_request_count_ = 0;

Empty::Empty (void)
  : refcnt_ (1)
{
}

Empty::~Empty (void)
{
  if (this->refcnt_ != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Empty::~Empty ref count is not 0\n"));
    }
  if (Empty::in_request_count_ != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Empty::~Empty invoked while in a request\n"));
    }
}

void
Empty::_add_ref (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "Empty::_add_ref\n"));
  this->refcnt_++;
}

void
Empty::_remove_ref (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "Empty::_remove_ref\n"));
  if (this->refcnt_ == 0)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: ref count is 0\n"));
      return;
    }

  --this->refcnt_;

  if (this->refcnt_ == 0)
    delete this;
}

void
Empty::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Empty::_destroy\n"));
  Empty::in_request_count_++;
  ACE_TRY
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Empty::_destroy, deactivate[1]\n"));
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Empty::_destroy, deactivate[2]\n"));
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Empty::_destroy, deactivate[3]\n"));
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Empty::destroy - exception caught:");
    }
  ACE_ENDTRY;
  Empty::in_request_count_--;
}
