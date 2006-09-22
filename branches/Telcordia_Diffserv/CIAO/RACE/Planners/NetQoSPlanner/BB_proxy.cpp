// $Id$
#include "BB_proxy.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include <string>


FlowRequest::FlowRequest (AdmissionControl_ptr bb_ref,
                          const AdmissionControl::FlowInfo &f,
                          CommonDef::QOSRequired qos_req)
:   BB_ref_ (bb_ref),
    flowinfo_ (new AdmissionControl::FlowInfo (f)),
    qos_req_ (qos_req),
    sched_action_ (AdmissionControl::reserve)
{
}

FlowRequest::~FlowRequest ()  throw ()
{
}

AdmissionControl::AdmissionControlResult
FlowRequest::send_request ()
{
  this->adm_ctrl_result_ = this->BB_ref_->flowRequest (this->flowinfo_.in (),
                                                       this->qos_req_,
                                                       this->sched_action_,
                                                       this->flowtoken_,
                                                       this->dscp_);
  return this->adm_ctrl_result_;
}

int FlowRequest::commit ()
{
  int retval = -1;
  try
    {
      AdmissionControl::AdmissionControlResult result = this->BB_ref_->flowRequestCommit(this->flowtoken_);
      if (AdmissionControl::DECISION_ADMIT == result)
        retval = 0;
      else
        retval = -1;
    }
  catch (AdmissionControl::AdmissionControlException &adm_ctrl_ex)
    {
      ACE_DEBUG ((LM_ERROR,"In FlowRequest::commit: AdmissionControlException was raised.\n"));
      ACE_DEBUG ((LM_ERROR,"-- Reason: %s\n", adm_ctrl_ex.reason.in()));
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR,"In FlowRequest::commit: Unknown exception was raised.\n"));
    }

  return retval;
}

int FlowRequest::rollback ()
{
  int retval = -1;
  try
    {
      AdmissionControl::AdmissionControlResult result = this->BB_ref_->flowDelete(this->flowtoken_);
      if (AdmissionControl::DECISION_ADMIT == result)
        retval = 0;
      else
        retval = -1;
    }
  catch (AdmissionControl::AdmissionControlException &adm_ctrl_ex)
    {
      ACE_DEBUG ((LM_ERROR,"In FlowRequest::commit: AdmissionControlException was raised.\n"));
      ACE_DEBUG ((LM_ERROR,"-- Reason: %s\n", adm_ctrl_ex.reason.in()));
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR,"In FlowRequest::commit: Unknown exception was raised.\n"));
    }

  return retval;
}

long FlowRequest::get_dscp () const
{
  return this->dscp_;
}

BB_Proxy::BB_Proxy ()
: resolved_ (false),
  BB_iorfile_ ("BB.ior"),
  BB_nameserv_context_ ("AdmissionControlBackup")
{
}

BB_Proxy::~BB_Proxy ()  throw ()
{
  std::for_each (this->BB_commands_.begin (), this->BB_commands_.end (), BB_Proxy::del);
}

bool BB_Proxy::resolve (CORBA::ORB_ptr orb)
{
  if (!resolved_)
  {
        struct NameServResolutionFailed {};
        struct FilebasedResolutionFailed {};

        BandwidthBroker::AdmissionControl_var adm_ctrl;

        try
        {
          /// Try resolving BB using the nameing service
          CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
          CosNaming::NamingContext_var rootcontext = CosNaming::NamingContext::_narrow (obj.in());
          if (CORBA::is_nil(rootcontext.in()))
          {
            ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve (): Can't narrow to NamingContext.\n"));
            throw NameServResolutionFailed ();
          }

          try
            {
              CosNaming::Name name;
              this->populate_name (name);
              obj = rootcontext->resolve (name);
            }
          catch (const CosNaming::NamingContext::NotFound &)
            {
              ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): NamingContext::NotFound exception in nameserv resolution.\n"));
              throw NameServResolutionFailed ();
            }

          if (CORBA::is_nil (obj))
          {
            ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve (): Naming service reference is nil.\n"));
            throw NameServResolutionFailed ();
          }

          /// Downcast the object reference to a reference of type Time_Date.
          adm_ctrl = BandwidthBroker::AdmissionControl::_narrow (obj);
          if (CORBA::is_nil (adm_ctrl))
          {
            ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): The IOR obtained from \
                                   the NS is not an AdmissionControl reference.\n"));
            throw NameServResolutionFailed ();
          }
/*        {
          AdmissionControl::FlowInfo flowinfo;

          flowinfo.srcIP.dottedDecimal = CORBA::string_dup("129.59.129.82");
          flowinfo.srcIP.subnetMask = CORBA::string_dup("255.255.255.255");
          flowinfo.srcPort.low = -1;
          flowinfo.srcPort.high = -1;

          flowinfo.destIP.dottedDecimal = CORBA::string_dup("129.59.129.91");
          flowinfo.destIP.subnetMask = CORBA::string_dup("255.255.255.255");
          flowinfo.destPort.low = -1;
          flowinfo.destPort.high = -1;

          flowinfo.protocol = AdmissionControl::notSpecified;
          flowinfo.fwdRate.requiredBW = CORBA::Long (10);
          flowinfo.fwdRate.extraBW = CORBA::Long (0);
          flowinfo.revRate.requiredBW = CORBA::Long (0);
          flowinfo.revRate.extraBW = CORBA::Long (0);
          flowinfo.biDirectional = CORBA::Boolean (false);
          flowinfo.flowDuration = CORBA::Long (1000);

          ACE_DEBUG ((LM_DEBUG,"In BB_Proxy::resolve: Requesting flow.\n"));
          CommonDef::QOSRequired qos_req = CommonDef::highReliability;
          AdmissionControl::SchedulingAction sched_action = AdmissionControl::reserve;
          CORBA::Long dscp;
          CORBA::String_var flowtoken;
          try
          {
            adm_ctrl->flowRequest (flowinfo, qos_req , sched_action, flowtoken, dscp);
            ACE_DEBUG ((LM_ERROR,"In BB_Proxy::resolve: flowRequest successful\n"));
          }
          catch (CORBA::Exception &e)
            {
              ACE_PRINT_EXCEPTION (e, "In BB_Proxy::resolve: A CORBA exception was raised:\n");
            }
          catch (...)
            {
              ACE_DEBUG ((LM_ERROR,"In BB_Proxy::resolve: Unknown exception was raised.\n"));
            }
          }
*/
        }
        catch (NameServResolutionFailed &)
        {
          ACE_DEBUG ((LM_ERROR, "In NetQoSPlanner_exec_i::process_netqos_req(): NameService based resolution of BB failed,\
                                 trying filebased resolution %s.\n",this->BB_iorfile_.c_str()));
          try
          {
            /// Try to resolve using BB.ior file.
            std::string BB_ior_url = std::string ("file://") + this->BB_iorfile_;

            CORBA::Object_var obj = orb->string_to_object (BB_ior_url.c_str());

            if (CORBA::is_nil (obj))
            {
              ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): BandwidthBroker is a nil object reference.\n"));
              throw FilebasedResolutionFailed ();
            }

            /// Downcast the object reference to a reference of type AdmissionControl.
            adm_ctrl = BandwidthBroker::AdmissionControl::_narrow (obj);
            if (CORBA::is_nil (adm_ctrl))
            {
              ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): The IOR obtained from \
                                     the file is not an AdmissionControl reference.\n"));
              throw FilebasedResolutionFailed ();
            }
          }
          catch (FilebasedResolutionFailed &)
          {
              ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): Filebased IOR resolution of BB also failed.\n"));
              this->resolved_ = false;
              return this->resolved_;
          }
          catch (...)
          {
            ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): Unknown exception in file based resolution.\n"));
            this->resolved_ = false;
            return this->resolved_;
          }
        }
        catch (...)
        {
          ACE_DEBUG ((LM_ERROR, "In BB_Proxy::resolve(): Unknown exception in nameserv resolution.\n"));
          this->resolved_ = false;
          return this->resolved_;
        }

      ACE_DEBUG ((LM_DEBUG, "In BB_Proxy::resolve(): BandwidthBroker resolved successfully.\n"));
      this->BB_ref_ = adm_ctrl;
      this->resolved_ = true;
      return this->resolved_;
  }
  else
    return this->resolved_;
}

void BB_Proxy::populate_name (CosNaming::Name &name)
{
    name.length (6);
    name[0].id = CORBA::string_dup ("mil");
    name[1].id = CORBA::string_dup ("darpa");
    name[2].id = CORBA::string_dup ("arms");
    name[3].id = CORBA::string_dup ("mlrm");
    name[4].id = CORBA::string_dup ("BandwidthBroker");
    name[5].id = CORBA::string_dup (this->BB_nameserv_context_.c_str());
}

int BB_Proxy::flow_request (const AdmissionControl::FlowInfo &f, CommonDef::QOSRequired qos_req, long &dscp)
{
  int retval = -1;
  try
    {
      std::auto_ptr <FlowRequest> flow_request (new FlowRequest (this->BB_ref_.in(), f, qos_req));

      ACE_DEBUG ((LM_DEBUG,"In BB_Proxy::flow_request: Requesting flow.\n"));
      AdmissionControl::AdmissionControlResult adm_ctrl_result = flow_request->send_request ();
      if (AdmissionControl::DECISION_ADMIT == adm_ctrl_result)
        {
          ACE_DEBUG ((LM_DEBUG,"In BB_Proxy::flow_request: Flow Accepted.\n"));
          this->BB_commands_.push_back (flow_request.release ());
          dscp = flow_request->get_dscp ();
          retval = 0;
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,"In BB_Proxy::flow_request: Requested flow was not admitted.\n"));
          dscp = -1;
          retval = -1;
        }
    }
  catch (AdmissionControl::AdmissionControlException &adm_ctrl_ex)
    {
      ACE_DEBUG ((LM_ERROR,"In BB_Proxy::flow_request: AdmissionControlException was raised.\n"));
      ACE_DEBUG ((LM_ERROR,"-- Reason: %s\n", adm_ctrl_ex.reason.in()));
    }
  catch (CORBA::Exception &e)
    {
      ACE_DEBUG ((LM_ERROR,"In BB_Proxy::flow_request: A CORBA exception was raised: %s\n",e._name()));
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR,"In BB_Proxy::flow_request: Unknown exception was raised.\n"));
    }

  return retval;
}

int BB_Proxy::commit ()
{
  int result = 0;
  for (CommandList::iterator iter = this->BB_commands_.begin ();
       iter != this->BB_commands_.end ();
       ++iter)
    {
      result |= (*iter)->commit ();
    }

  return result;
}

int BB_Proxy::rollback ()
{
  int result = 0;
  for (CommandList::iterator iter = this->BB_commands_.begin ();
       iter != this->BB_commands_.end ();
       ++iter)
    {
      result |= (*iter)->rollback ();
    }

  return result;
}

void BB_Proxy::del (const FlowRequest *c) throw ()
{
  delete c;
}
