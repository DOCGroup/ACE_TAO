// $Id$

#include "Upcall_Wrapper.h"

#ifndef __ACE_INLINE__
# include "tao/PortableServer/Upcall_Wrapper.inl"
#endif  /* __ACE_INLINE_*/


ACE_RCSID (PortableServer,
           Upcall_Wrapper,
           "$Id$")


bool
TAO::Upcall_Wrapper::pre_upcall (void)
{
  TAO_InputCDR & cdr = this->server_request_.incoming ();

  // Demarshal the operation arguments, if any.

  TAO::Argument ** const begin = this->args_;
  TAO::Argument ** const end   = begin + nargs_;

  for (TAO::Argument ** i = begin; i != end; ++i)
    {
      if (!(*i)->demarshal (cdr))
        return false;
    }

#if TAO_HAS_INTERCEPTORS == 1

  // Invoke intermediate server side interception points.

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO::PICurrent_Guard _tao_pi_guard (_tao_ri.server_request (),
                                          true  /* Copy TSC to RSC */);

      this->interceptor_adapter_.receive_request (this->request_info_
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

}

void
TAO::Upcall_Wrapper::post_upcall (void)
{

}
