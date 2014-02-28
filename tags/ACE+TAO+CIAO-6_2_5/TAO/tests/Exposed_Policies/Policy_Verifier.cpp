// $Id$
#include "Policy_Verifier.h"
#include "ace/OS_NS_string.h"

Policy_Verifier::Policy_Verifier (void)
  : rt_object_properties_ (0)
  , rt_poa_properties_ (0)
  , priority_bands_ (0)
{
  ACE_OS::strcpy (this->base_object_ref_,
                  ACE_TEXT("file://default.ior"));
  ACE_OS::strcpy (this->overridden_object_ref_,
                  ACE_TEXT("file://overridden.ior"));
}

Policy_Verifier::~Policy_Verifier (void)
{
  delete rt_object_properties_;
  delete rt_poa_properties_;
}

bool
Policy_Verifier::init (int argc, ACE_TCHAR *argv[])
{
  this->orb_ =
    CORBA::ORB_init (argc, argv);

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = 0;
      // IOR File Name Option.
      if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-POAConfigFile"))))
        {
          this->rt_poa_properties_ =
            RT_Properties::read_from (arg);
          this->priority_bands_ =
            this->rt_poa_properties_->priority_bands ().length ();
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-ObjectConfigFile"))))
        {
          this->rt_object_properties_ =
            RT_Properties::read_from (arg);
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-BaseObjectIOR"))))
        {
          if (this->rt_poa_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_poa_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO::VMCID,
                                                  CORBA::COMPLETED_NO));
            }
          this->rt_poa_properties_->ior_source (arg);
          ACE_OS::strcpy (this->base_object_ref_, ACE_TEXT("file://"));
          ACE_OS::strcat (this->base_object_ref_,
                          this->rt_poa_properties_->ior_source ());
        }
      else if (0 != (arg = arg_shifter.get_the_parameter (ACE_TEXT("-OverriddenIOR"))))
        {
          if (this->rt_object_properties_ == 0)
            {
              ACE_NEW_THROW_EX (this->rt_object_properties_,
                                RT_Properties,
                                CORBA::NO_MEMORY (TAO::VMCID,
                                                  CORBA::COMPLETED_NO));
            }
          this->rt_object_properties_->ior_source (arg);
          ACE_OS::strcpy (this->overridden_object_ref_, ACE_TEXT("file://"));
          ACE_OS::strcat (this->overridden_object_ref_,
                          this->rt_object_properties_->ior_source ());
        }
      else
        {
          arg_shifter.consume_arg ();
        }
    }

  if ((this->rt_poa_properties_ == 0) || (this->rt_object_properties_ == 0))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Configuration file missing!\n")));
      return false;
    }

  // Get the Object references.
  CORBA::Object_var object = this->orb_->string_to_object (this->base_object_ref_);

  if (!Policy_Verifier::check_reference (object.in (), "Invalid IOR file!\n"))
    return false;

  this->base_object_ = Counter::_narrow (object.in ());

  if (!Policy_Verifier::check_reference (this->base_object_.in (),
                                         "Unable to convert the IOR to the proper object reference.\n"))
    return false;

  object = this->orb_->string_to_object (this->overridden_object_ref_);

  if (!Policy_Verifier::check_reference (object.in (), "Invalid IOR file!\n"))
    return false;

  this->overridden_object_ = Counter::_narrow (object.in ());

  if (!Policy_Verifier::check_reference (this->overridden_object_.in (),
                                         "Unable to convert the IOR to the proper object reference.\n"))
    return false;

  return true;
}

void
Policy_Verifier::run ( )
{
  this->verify_reference (this->base_object_.in (),
                          this->rt_poa_properties_);

  this->verify_reference (this->overridden_object_.in (),
                          this->rt_object_properties_);
}

void
Policy_Verifier::verify_reference (Counter_ptr object,
                                   RT_Properties *rt_properties)
{

  try
    {
      CORBA::Policy_var policy_var =
        object->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

      if (Policy_Verifier::check_reference (policy_var.in (), "Unable to get Priority Policy.\n"))
        {
          RTCORBA::PriorityModelPolicy_var priority_policy =
            RTCORBA::PriorityModelPolicy::_narrow (policy_var.in ());

          RTCORBA::PriorityModel priority_model =
            priority_policy->priority_model ();

          RTCORBA::Priority priority =
            priority_policy->server_priority ();

          if (priority_model == RTCORBA::SERVER_DECLARED)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nPriority Model: RTCORBA::SERVER_DECLARED\n")
                        ));

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Priority Model: %d\nCORBA Priority: %d\n\n"),
                      priority_model,
                      priority
                      ));


          if (priority != rt_properties->priority ())
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Priority Value Mismatch.\n")));
        }

      policy_var = object->_get_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE);

      if (Policy_Verifier::check_reference (policy_var.in (),
                                          "Unable to get Priority Banded Policy\n"))
        {

          RTCORBA::PriorityBandedConnectionPolicy_var priority_banded_policy =
            RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy_var.in ());


          if (Policy_Verifier::check_reference (priority_banded_policy.in (),
                                              "Unable to get Priority Banded Policy\n"))
            {

              // Here we have a priority banded connection policy.

              RTCORBA::PriorityBands_var pb =
                priority_banded_policy->priority_bands ();
              unsigned int band_num = pb->length ();
              if (band_num != this->priority_bands_)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Mismatching Number of Priority Bands!\n")));

              for (unsigned int i = 0; i < band_num; ++i)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("Priority Band <%d>: (%d, %d)\n"),
                              i,
                              pb[i].low,
                              pb[i].high
                              ));


                  if ((band_num == rt_properties->priority_bands ().length ()) &&
                      ((pb[i].low !=  rt_properties->priority_bands ()[i].low) ||
                       (pb[i].high !=  rt_properties->priority_bands ()[i].high)))
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("Mismatching Priority Band Range!\n")));

                }
            }
        }
      policy_var = object->_get_policy (RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE);

      if (Policy_Verifier::check_reference (policy_var.in (),
                                            "Unable to get Client Protocol Policy\n"))
        {
          RTCORBA::ClientProtocolPolicy_var client_protocol_policy =
            RTCORBA::ClientProtocolPolicy::_narrow (policy_var.in ());

          RTCORBA::ProtocolList_var protocol_list =
            client_protocol_policy->protocols ();

          for (unsigned int  i = 0; i < protocol_list->length (); i++)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\nThe Client Protocol Type: %d\n"),
                        protocol_list[i].protocol_type));

        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CORBA Exception Raised");
    }

}

CORBA::Boolean
Policy_Verifier::check_reference (CORBA::Object_ptr object,
                                  const char *msg)
{
  if (CORBA::is_nil (object))
    {
      ACE_DEBUG ((LM_DEBUG, msg));
      return 0;
    }
  return 1;
}
