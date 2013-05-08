//$Id$

#include "tao/Remote_Invocation.h"
#include "tao/Profile.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Stub.h"
#include "tao/Connection_Handler.h"
#include "tao/operation_details.h"
#include "tao/ORB_Core.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Network_Priority_Protocols_Hooks.h"
#include "tao/debug.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Remote_Invocation::Remote_Invocation (
    CORBA::Object_ptr otarget,
    Profile_Transport_Resolver &resolver,
    TAO_Operation_Details &detail,
    bool response_expected)
    : Invocation_Base (otarget,
                       resolver.object (),
                       resolver.stub (),
                       detail,
                       response_expected,
                       true /* request_is_remote */ )
    , resolver_ (resolver)
  {
  }

  void
  Remote_Invocation::init_target_spec (TAO_Target_Specification &target_spec,
                                       TAO_OutputCDR &output)
  {
    // Generate all service contexts
    this->resolver_.stub ()->orb_core ()->service_context_registry ().
      generate_service_context (
        this->resolver_.stub (),
        *this->resolver_.transport (),
        this->details_,
        target_spec,
        output);

    TAO_Profile *pfile = this->resolver_.profile ();

    // Set the target specification mode
    switch (pfile->addressing_mode ())
      {
      case TAO_Target_Specification::Key_Addr:
        target_spec.target_specifier (pfile->object_key ());
        break;
      case TAO_Target_Specification::Profile_Addr:
        {
          IOP::TaggedProfile *tp = pfile->create_tagged_profile ();

          if (tp)
            {
              target_spec.target_specifier (*tp);
            }
        }
        break;

    case TAO_Target_Specification::Reference_Addr:
      // We need to call the method seperately. If there is no
      // IOP::IOR info, the call would create the info and return the
      // index that we need.
      CORBA::ULong index = 0;
      IOP::IOR *ior_info = 0;

      if (this->resolver_.stub ()->create_ior_info (ior_info, index) == -1)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("Remote_Invocation::init_target_spec, ")
                          ACE_TEXT ("Error in finding index for ")
                          ACE_TEXT ("IOP::IOR\n")));
            }

          return;
        }

      target_spec.target_specifier (*ior_info, index);
      break;
    }
  }

  void
  Remote_Invocation::write_header (TAO_OutputCDR &out_stream)
  {
    this->resolver_.transport ()->clear_translators (0, &out_stream);

    TAO_Target_Specification spec;
    this->init_target_spec (spec, out_stream);

    // Send the request for the header
    if (this->resolver_.transport ()->generate_request_header (this->details_,
                                                               spec,
                                                               out_stream) == -1)
      {
        throw ::CORBA::MARSHAL ();
      }

    this->resolver_.transport ()->assign_translators (0, &out_stream);
  }

  void
  Remote_Invocation::marshal_data (TAO_OutputCDR &out_stream)
  {
    // Marshal application data
    if (this->details_.marshal_args (out_stream) == false)
      {
        throw ::CORBA::MARSHAL ();
      }
  }

  Invocation_Status
  Remote_Invocation::send_message (TAO_OutputCDR &cdr,
                                   TAO_Message_Semantics message_semantics,
                                   ACE_Time_Value *max_wait_time)
  {
    TAO_Protocols_Hooks *tph =
      this->resolver_.stub ()->orb_core ()->get_protocols_hooks ();

    TAO_Network_Priority_Protocols_Hooks *nph =
      this->resolver_.stub ()->orb_core ()->
        get_network_priority_protocols_hooks ();

    TAO_Connection_Handler *connection_handler =
      this->resolver_.transport ()->connection_handler ();

    if (nph != 0)
      {
        // nph = 0, means DiffServ library is not used
        // nph = 0, means DiffServ library is used, and
        // request DSCP and reply DSCP are set.
        // Note that the application could still be using
        // RTCORBA, but still setting DIffServ codepoints
        // using the DiffServ library takes precedence.
        //
        CORBA::Long const dscp = nph->get_dscp_codepoint (this->resolver_.stub (),
          this->resolver_.object ());
        connection_handler->set_dscp_codepoint (dscp);
      }
    else if (tph != 0)
      {
        // If we execute this code, DiffServ library is not used,
        // but RTCORBA could be used.
        // Which means that using the enable_network_priority flag,
        // the application might want to set DiffServ codepoints.
        // Check if that is the case.
        //
        CORBA::Boolean const set_client_network_priority =
          tph->set_client_network_priority (
            this->resolver_.transport ()->tag (),
            this->resolver_.stub ());
        connection_handler->set_dscp_codepoint (set_client_network_priority);
      }

    // Note that if noth nph and tph are 0, then we do not make any
    // virtual calls any more, because we have removed the default
    // implementations.

    if (! this->resolver_.transport ()->is_connected()) {
      throw ::CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

    int const retval =
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
            // We sent a message already and we haven't gotten a
            // reply.  Just throw TIMEOUT with *COMPLETED_MAYBE*.
            throw ::CORBA::TIMEOUT (
              CORBA::SystemException::_tao_minor_code (
                TAO_TIMEOUT_SEND_MINOR_CODE,
                errno),
              CORBA::COMPLETED_MAYBE);
          }

        if (TAO_debug_level > 2)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("Remote_Invocation::send_message, ")
                        ACE_TEXT ("failure while sending message\n")));
          }

        // Close the transport and all the associated stuff along with
        // it.
        this->resolver_.transport ()->close_connection ();
        this->resolver_.stub ()->reset_profiles ();
        return TAO_INVOKE_RESTART;
      }

    this->resolver_.stub ()->set_valid_profile ();
    return TAO_INVOKE_SUCCESS;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
