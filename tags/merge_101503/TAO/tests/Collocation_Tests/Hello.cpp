//
// $Id$
//
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"

ACE_RCSID(Hello, Hello, "$Id$")

  Hello::Hello (CORBA::ORB_ptr orb,
              ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb))
    , thr_id_ (thrid)
{
}

char *
Hello::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Upcall in process ..\n"));

  if (ACE_Thread::self () == this->thr_id_)
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
          TAO_ORB_Table *orb_table =
            TAO_ORB_Table::instance ();

          if (orb_table->find ("server_orb") == 0)
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
Hello::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
