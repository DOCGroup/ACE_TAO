//$Id$
#include "Remote_Invocation.h"
#include "Profile.h"
#include "Profile_Transport_Resolver.h"
#include "target_specification.h"
#include "IOP_IORC.h"
#include "Stub.h"
#include "Transport.h"
#include "operation_details.h"

ACE_RCSID (tao,
           Remote_Invocation,
           "$Id$")

namespace TAO
{
  Remote_Invocation::Remote_Invocation (Profile_Transport_Resolver &resolver,
                                        TAO_Operation_Details &detail)
    : Invocation_Base (resolver.stub (), detail)
    , resolver_ (resolver)
  {
  }

  void
  Remote_Invocation::init_target_spec (TAO_Target_Specification &target_spec
                                       ACE_ENV_ARG_DECL)
  {
    TAO_Profile *pfile =
      this->resolver_.profile ();

    // Set the target specification mode
    switch (pfile->addressing_mode ())
      {
      case TAO_Target_Specification::Key_Addr:
        target_spec.target_specifier (pfile->object_key ());
        break;
      case TAO_Target_Specification::Profile_Addr:
        {
          IOP::TaggedProfile *tp =
            pfile->create_tagged_profile ();

        if (tp)
          target_spec.target_specifier (*tp);
        }
        break;

    case TAO_Target_Specification::Reference_Addr:
      // We need to call the method seperately. If there is no
      // IOP::IOR info, the call would create the info and return the
      // index that we need.
      CORBA::ULong index = 0;

      IOP::IOR *ior_info = 0;
      int retval =
        this->resolver_.stub ()->create_ior_info (ior_info,
                                                  index
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (retval == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) Error in finding index for \n")
                          ACE_TEXT ("IOP::IOR \n")));
            }
          return;
        }

      target_spec.target_specifier (*ior_info,
                                    index);
      break;
    }


  }

  void
  Remote_Invocation::write_header (TAO_Target_Specification &spec,
                                   TAO_OutputCDR &out_stream
                                   ACE_ENV_ARG_DECL)
  {
    // Send the request for the header
    if (this->resolver_.transport ()->generate_request_header (this->details_,
                                                               spec,
                                                               out_stream)
        == -1)
      {
        ACE_THROW (CORBA::MARSHAL ());
      }

    this->resolver_.transport ()->assign_translators (0,
                                                      &out_stream);
  }

  void
  Remote_Invocation::marshal_data (TAO_OutputCDR &out_stream
                                   ACE_ENV_ARG_DECL)
  {
    if (this->details_.marshal_args (out_stream) == false)
      ACE_THROW (CORBA::MARSHAL ());

    return;
  }

  Invocation_Status
  Remote_Invocation::send_message (TAO_OutputCDR &cdr,
                                   short message_semantics,
                                   ACE_Time_Value *max_wait_time
                                   ACE_ENV_ARG_DECL)
  {
    int retval =
      this->resolver_.transport ()->send_request (
        this->resolver_.stub (),
        this->resolver_.stub ()->orb_core (),
        cdr,
        message_semantics,
        max_wait_time);

    if (retval == -1)
      {
        if (errno == ETIME)
        {
          ACE_THROW_RETURN (
              CORBA::TIMEOUT (
                  CORBA::SystemException::_tao_minor_code (
                      TAO_TIMEOUT_SEND_MINOR_CODE,
                      errno
                    ),
                  CORBA::COMPLETED_NO
                  ),
              TAO_INVOKE_FAILURE
              );
        }
        if (TAO_debug_level > 2)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Remote_Invocation::send_message - ")
                      ACE_TEXT ("failure while sending message \n")));

        // Close the transport and all the associated stuff along with
        // it.
        this->resolver_.transport ()->close_connection ();

        return TAO_INVOKE_RESTART;
      }

    this->resolver_.stub ()->set_valid_profile ();
    return TAO_INVOKE_SUCCESS;
  }
}
