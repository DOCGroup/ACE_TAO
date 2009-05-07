//
// $Id$
//
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/ORB_Core_Auto_Ptr.h"

ACE_RCSID(Collocation_Oneway_Tests, Hello, "$Id$")

  Hello::Hello (CORBA::ORB_ptr orb,
              ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb))
    , thr_id_ (thrid)
{
}

void
Hello::system_exception_test (void)
{
  throw CORBA::INTERNAL ();
}

void
Hello::user_exception_expected (void)
{
  throw ::Test::Hello::A ();
}

void
Hello::user_exception_not_expected (void)
{
  this->throw_internal_b ();
}

void
Hello::throw_internal_b (void)
{
  throw ::Test::Hello::B ();
}

char *
Hello::get_string (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Upcall in process ..\n"));

  // Use portable thread IDs
  ACE_Thread_ID self_ID;
  ACE_Thread_ID this_ID;
  this_ID.id(this->thr_id_);

  if (self_ID == this_ID)
    {
      if (this->orb_->orb_core ()->optimize_collocation_objects () &&
          this->orb_->orb_core ()->use_global_collocation ())
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: A remote call has been made "
                      " exiting ..\n"));
          ACE_OS::abort ();
        }
      else if (this->orb_->orb_core ()->optimize_collocation_objects () &&
               this->orb_->orb_core ()->use_global_collocation () == 0)
        {
          TAO::ORB_Table * const orb_table =
            TAO::ORB_Table::instance ();

          TAO_ORB_Core_Auto_Ptr tmp (orb_table->find ("server_orb"));
          if (tmp.get () == 0)
            {
              // We are running on a single ORB and this is an error.
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) ERROR: A remote call has been made "
                          " with a single ORB "
                          " exiting ..\n"));
              ACE_OS::abort ();
            }
        }
    }

  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
