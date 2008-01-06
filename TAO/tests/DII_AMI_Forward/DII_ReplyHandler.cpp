/* -*- C++ -*- */

//
// $Id$
//

#include "tao/DynamicInterface/Request.h"  /* This must come first for
                                              G++ 3.4 or better */
#include "DII_ReplyHandler.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl.h"

DII_ReplyHandler::DII_ReplyHandler (bool &reply_notifier)
  :got_reply_(reply_notifier)
{
}

DII_ReplyHandler::~DII_ReplyHandler ()
{
}


void
DII_ReplyHandler::handle_response (TAO_InputCDR &incoming)
{
  this->got_reply_ = true;
  CORBA::String_var result;
  try
    {
      if (incoming >> result.inout())
        ACE_DEBUG ((LM_DEBUG,"Got response: %s\n",result.in()));
      else
        ACE_ERROR ((LM_ERROR,"ERROR: Response is not a string!\n"));
    }
  catch (CORBA::SystemException &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: DII_ReplyHandler::handle_response caught %s\n",
                  ex._name()));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "DII_Reply_Handler::handle_response Unknown Exception\n"));
    }

}

void
DII_ReplyHandler::handle_excep (TAO_InputCDR &incoming,
                                CORBA::ULong reply_status)
{
  TAO_InputCDR for_reading (incoming);
  CORBA::String_var id;
  if ((for_reading >> id.inout()) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "DII_ReplyHandler::handle_excep could not extract id\n"));
      return;
    }

  if (reply_status == TAO_AMI_REPLY_USER_EXCEPTION)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "DII_ReplyHandler::handle_excep got user ex = %s\n",
                  id.in()));
    }
  else if (reply_status == TAO_AMI_REPLY_SYSTEM_EXCEPTION)
    {
      CORBA::ULong minor = 0;
      CORBA::ULong completion = 0;

      if ((for_reading >> minor) == 0
          || (for_reading >> completion) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "DII_ReplyHandler::handle_excep could not "
                      "extract system ex minor code or completion\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Got system exception: %s, minor = %d, completed = %d\n",
                      id.in(), minor, completion));
        }

    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Got an exception that is neither user nor system. "
                  "reply_status = %d\n",reply_status));
    }
}

void
DII_ReplyHandler::handle_location_forward (TAO_InputCDR &incoming,
                                           CORBA::ULong reply_status)
{
  TAO_InputCDR for_reading (incoming);
  CORBA::Object_var fwd;
  if ((for_reading >> fwd) == 0)
    {
      return;
    }

  int argc = 0;
  char **argv = 0;
  CORBA::ORB_var  orb = CORBA::ORB_init(argc,argv);

  CORBA::String_var iorstr = orb->object_to_string(fwd.in ());

  bool is_perm = reply_status == TAO_AMI_REPLY_LOCATION_FORWARD_PERM;
  ACE_DEBUG ((LM_DEBUG,
              "DII_ReplyHandler::handle_location_forward"
              " got %s, is_perm = %b\n",
              iorstr.in() ,is_perm));

  CORBA::Request_var req = fwd->_request ("do_forward");
  CORBA::String_var test_string = CORBA::string_dup ("123 look at me");
  req->add_in_arg ("text") <<= CORBA::Any::from_string(test_string.in(),30);
  req->sendc(this);



}
