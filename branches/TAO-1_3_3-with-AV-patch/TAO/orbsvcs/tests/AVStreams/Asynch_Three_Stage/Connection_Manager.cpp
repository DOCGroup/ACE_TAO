//$Id$

#include "Connection_Manager.h"

Connection_Manager::Connection_Manager (void)
{
}

Connection_Manager::~Connection_Manager (void)
{
}

void
Connection_Manager::load_ep_addr (const char* file_name)
{
  FILE* addr_file = ACE_OS::fopen (file_name, "r");
  
  if (addr_file == 0)
    {
      ACE_ERROR ((LM_DEBUG,
		  "Cannot open addr file %s\n",
		  file_name));
      return;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                "Addr file opened successfully\n"));

  while (1)
    {
      char buf [BUFSIZ];
      
      // Read from the file into a buffer

      
      /*
      int n = ACE_OS::fread (buf,
			     1,
			     BUFSIZ,
			     addr_file);
      */

      if ((ACE_OS::fgets (buf,BUFSIZ,addr_file)) == NULL)
	{
	  // At end of file break the loop and end the sender.
	  if (TAO_debug_level > 0)
	    ACE_DEBUG ((LM_DEBUG,"End of Addr file\n"));
	  break;
	}
	

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "%s\n",
		    buf));

      Endpoint_Addresses* addr;
      ACE_NEW (addr,
	       Endpoint_Addresses);

      TAO_Tokenizer addr_tokenizer (buf,'/');

      ACE_CString flowname;

      if (addr_tokenizer [0] == 0)
	{
	  ACE_ERROR ((LM_ERROR,
		      "Corresponding flow name not specified for endpoint addresses\n"));
	  return;
	}
      else
	flowname += addr_tokenizer [0];
      
      if (addr_tokenizer [1] != 0)
	{
	  ACE_CString token (addr_tokenizer [1]);

	  int pos = token.find ('\r');
	  if (pos != ACE_CString::npos)
	    {
	      addr->sender_addr = CORBA::string_dup ((token.substr (0, pos)).c_str ());
	    }
	  else addr->sender_addr = CORBA::string_dup (token.c_str());

	  pos = addr->sender_addr.find ('\n');
	  if (pos != ACE_CString::npos)
	    {
	      addr->sender_addr = (addr->sender_addr.substr (0, pos)).c_str ();
	    }
	}

      if (addr_tokenizer [2] != 0)
	{
	  ACE_CString token (addr_tokenizer [2]);

	  int pos = token.find ('\r');
	  if (pos != ACE_CString::npos)
	    {
	      addr->receiver_addr = CORBA::string_dup ((token.substr (0, pos)).c_str ());
	    }
	  else addr->receiver_addr = CORBA::string_dup (token.c_str());

	  pos = addr->receiver_addr.find ('\n');
	  if (pos != ACE_CString::npos)
	    {
	      addr->sender_addr = (addr->receiver_addr.substr (0, pos)).c_str ();
	    }
	}
      
      int result = ep_addr_.bind (flowname,
				  addr);
      if (result == 0)
	{
	if (TAO_debug_level > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "Flowname %s Bound Successfully\n",
		      flowname.c_str ()));
	}
      else if (result == 1)
	ACE_DEBUG ((LM_DEBUG,
		    "Flowname %s already exists\n",
		    flowname.c_str ()));
      else ACE_DEBUG ((LM_DEBUG,
		       "Flowname %s Bound Failed\n",
		       flowname.c_str ()));

      
    }

}

int
Connection_Manager::init (CORBA::ORB_ptr orb)
{
  // Initialize the naming service
  if (this->naming_client_.init (orb) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);
  return 0;
}

void
Connection_Manager::bind_to_receivers (const ACE_CString &sender_name,
                                       AVStreams::MMDevice_ptr sender
                                       ACE_ENV_ARG_DECL)
{
  this->sender_name_ =
    sender_name;

  /* 
  this->sender_ =
    AVStreams::MMDevice::_duplicate (sender);
  */
 
  CosNaming::Name name (1);
  name.length (1);

  ACE_TRY
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      this->sender_context_ =
        this->naming_client_->bind_new_context (name
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // We reach here if there was no exception raised in
      // <bind_new_context>.  We then create a receiver context.
      //

      // Create the context for storing the receivers
      name [0].id =
        CORBA::string_dup ("Receivers");

      // Try binding the receivers context under the sender context.
      this->receiver_context_ =
        this->sender_context_->bind_new_context (name
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, al_ex)
    {
      //
      // The sender context already exists, probably created by the
      // receiver(s).
      //

      // Get the sender context.
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      CORBA::Object_var object =
        this->naming_client_->resolve (name
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->sender_context_ =
        CosNaming::NamingContext::_narrow (object.in ());

      // Find the Receiver context.
      name [0].id =
        CORBA::string_dup ("Receivers");

      object =
        this->sender_context_->resolve (name
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->receiver_context_ =
        CosNaming::NamingContext::_narrow (object.in ());

      this->find_receivers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  name [0].id =
    CORBA::string_dup (this->sender_name_.c_str ());

  // Register the sender object with the sender context.
  this->sender_context_->rebind (name,
                                 sender
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Connection_Manager::find_receivers (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNaming::BindingIterator_var iterator;
  CosNaming::BindingList_var binding_list;
  const CORBA::ULong chunk = 100;

  // Get the list of receivers registered for this sender.
  this->receiver_context_->list (chunk,
                                 binding_list,
                                 iterator
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Add the receivers found in the bindinglist to the <receivers>.
  this->add_to_receivers (binding_list
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (iterator.in ()))
    {
      CORBA::Boolean more = 1;

      // Check to see if there are more receivers listed.
      while (more)
        {
          more = iterator->next_n (chunk,
                                   binding_list
                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          this->add_to_receivers (binding_list
                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
Connection_Manager::add_to_receivers (CosNaming::BindingList &binding_list
                                      ACE_ENV_ARG_DECL)
{
  for (CORBA::ULong i = 0;
       i < binding_list.length ();
       i++)
    {
      // Get the receiver name from the binding list.
      ACE_CString receiver_name =
        binding_list [i].binding_name [0].id.in ();

      CosNaming::Name name (1);
      name.length (1);
      name [0].id =
        CORBA::string_dup (receiver_name.c_str ());

      // Resolve the reference of the receiver from the receiver
      // context.
      CORBA::Object_var obj =
        this->receiver_context_->resolve (name
                                          ACE_ENV_ARG_PARAMETER);

      AVStreams::MMDevice_var receiver_device =
        AVStreams::MMDevice::_narrow (obj.in ());

      // Add this receiver to the receiver map.
      ACE_CString flowname =
        this->sender_name_ +
        "_" +
        receiver_name;
      this->receivers_.bind (flowname,
                             receiver_device);
    }
}

void
Connection_Manager::connect_to_receivers (AVStreams::MMDevice_ptr sender
					  ACE_ENV_ARG_DECL)
{
  // Connect to all receivers that we know about.
  for (Receivers::iterator iterator = this->receivers_.begin ();
       iterator != this->receivers_.end ();
       ++iterator)
    {
      // Initialize the QoS
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

      ACE_CString flowname =
        (*iterator).ext_id_;


      Endpoint_Addresses* addr = 0;
      ep_addr_.find (flowname,
		     addr);

      ACE_CString sender_addr_str;
      ACE_CString receiver_addr_str;

      if (addr != 0)
	{
	  sender_addr_str = addr->sender_addr;
	  receiver_addr_str = addr->receiver_addr;
	  ACE_DEBUG ((LM_DEBUG,
		      "Address Strings %s %s\n",
		      sender_addr_str.c_str (),
		      receiver_addr_str.c_str ()));
	  
	}
      else ACE_DEBUG ((LM_DEBUG,
		       "No endpoint address for flowname %s\n",
		       flowname.c_str ()));
      
      ACE_INET_Addr receiver_addr (receiver_addr_str.c_str ());      
      ACE_INET_Addr sender_addr (sender_addr_str.c_str ());

      // Create the forward flow specification to describe the flow.
      TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
                                               "IN",
                                               "USER_DEFINED",
                                               "",
                                               "UDP",
                                               &sender_addr);

      sender_entry.set_peer_addr (&receiver_addr);

      // Set the flow specification for the stream between receiver
      // and distributer
      AVStreams::flowSpec flow_spec (1);
      flow_spec.length (1);
      flow_spec [0] =
        CORBA::string_dup (sender_entry.entry_to_string ());

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
		    "Connection_Manager::connect_to_receivers Flow Spec Entry %s\n",
		    sender_entry.entry_to_string ()));

      // Create the stream control for this stream.
      TAO_StreamCtrl *streamctrl;
      ACE_NEW (streamctrl,
               TAO_StreamCtrl);

      // Servant Reference Counting to manage lifetime
      PortableServer::ServantBase_var safe_streamctrl =
        streamctrl;

      // Register streamctrl.
      AVStreams::StreamCtrl_var streamctrl_object =
        streamctrl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Bind the flowname and the corresponding stream controller to
      // the stream controller map
      this->streamctrls_.bind (flowname,
                               streamctrl_object);

      // Bind the sender and receiver MMDevices.
      (void) streamctrl->bind_devs (sender,
                                    (*iterator).int_id_.in (),
                                    the_qos.inout (),
                                    flow_spec
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Connection_Manager::bind_to_sender (const ACE_CString &sender_name,
                                    const ACE_CString &receiver_name,
                                    AVStreams::MMDevice_ptr receiver
                                    ACE_ENV_ARG_DECL)
{
  this->sender_name_ =
    sender_name;

  this->receiver_name_ =
    receiver_name;

  this->receiver_ =
    AVStreams::MMDevice::_duplicate (receiver);

  CosNaming::Name name (1);
  name.length (1);

  int sender_context_exists = 0;

  ACE_TRY
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      CORBA::Object_var object =
        this->naming_client_->resolve (name
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // We reach here if there was no exception raised in <resolve>.
      // Therefore, there must be a valid sender context available.
      //
      sender_context_exists = 1;

      this->sender_context_ =
        CosNaming::NamingContext::_narrow (object.in ());

      name [0].id =
        CORBA::string_dup ("Receivers");

      // Find the receivers context under the sender's context
      object =
        this->sender_context_->resolve (name
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->receiver_context_ =
        CosNaming::NamingContext::_narrow (object.in ());
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, al_ex)
    {
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      // Create the sender context
      this->sender_context_ =
        this->naming_client_->bind_new_context (name
                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      name [0].id =
        CORBA::string_dup ("Receivers");

      // Create the receivers context under the sender's context
      this->receiver_context_ =
        this->sender_context_->bind_new_context (name
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  //
  // At this point we either have resolved the receiver context or we
  // have created a new one.
  //
  name [0].id =
    CORBA::string_dup (this->receiver_name_.c_str ());

  // Register this receiver object under the receiver context.
  this->receiver_context_->rebind (name,
                                   receiver
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //
  // Check if the sender was registered.  Note that if we created the
  // sender context, there is no point in checking for the sender.
  //
  if (sender_context_exists)
    {
      ACE_TRY_EX(SENDER_CONTEXT_EXISTS)
        {
          // Try binding the sender under the sender context
          name [0].id =
            CORBA::string_dup (this->sender_name_.c_str ());

          CORBA::Object_var object =
            this->sender_context_->resolve (name
                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX(SENDER_CONTEXT_EXISTS);

          this->sender_ =
            AVStreams::MMDevice::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX(SENDER_CONTEXT_EXISTS);
        }
      ACE_CATCH (CosNaming::NamingContext::NotFound, al_ex)
        {
          // No problem if the sender was not there.
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

void
Connection_Manager::connect_to_sender (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->sender_.in ()))
    return;

  ACE_CString flowname =
    this->sender_name_ +
    "_" +
    this->receiver_name_;


  Endpoint_Addresses* addr = 0;
  ep_addr_.find (flowname,
		 addr);
  
  ACE_CString sender_addr_str;
  ACE_CString receiver_addr_str;
  
  if (addr != 0)
    {
      sender_addr_str = addr->sender_addr;
      receiver_addr_str = addr->receiver_addr;

      ACE_DEBUG ((LM_DEBUG,
		  "Address Strings %s %s\n",
		  sender_addr_str.c_str (),
		  receiver_addr_str.c_str ()));
    }

  ACE_INET_Addr receiver_addr (receiver_addr_str.c_str ());  
  ACE_INET_Addr sender_addr (sender_addr_str.c_str ());
  
  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
                                           "IN",
                                           "USER_DEFINED",
                                           "",
                                           "UDP",
                                           &sender_addr);

  sender_entry.set_peer_addr (&receiver_addr);

  // Set the flow specification for the stream between sender and
  // receiver.
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] =
    CORBA::string_dup (sender_entry.entry_to_string ());
  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Connection_Manager::connect_to_sender Flow Spec Entry %s\n",
		sender_entry.entry_to_string ()));
  
  // Create the stream control for this stream
  TAO_StreamCtrl* streamctrl;
  ACE_NEW (streamctrl,
           TAO_StreamCtrl);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_streamctrl =
    streamctrl;

  // Register streamctrl.
  AVStreams::StreamCtrl_var streamctrl_object =
    streamctrl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  //
  // Since senders terminate the streams, we don't need the streamctrl
  // for these.
  //
  // this->streamctrls_.bind (flowname,
  //                          streamctrl_object);

  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  // Connect the sender and receiver devices.
  CORBA::Boolean result =
    streamctrl->bind_devs (this->sender_.in (),
                           this->receiver_.in (),
                           the_qos.inout (),
                           flow_spec
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (result == 0)
    ACE_ERROR ((LM_ERROR,
                "Streamctrl::bind_devs failed\n"));

  // Start the data sending.
  AVStreams::flowSpec start_spec;
  streamctrl->start (start_spec
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Connection_Manager::add_streamctrl (const ACE_CString &flowname,
                                    TAO_StreamEndPoint *endpoint
                                    ACE_ENV_ARG_DECL)
{
  // Get the stream controller for this endpoint.
  CORBA::Any_var streamctrl_any =
    endpoint->get_property_value ("Related_StreamCtrl"
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  AVStreams::StreamCtrl_ptr streamctrl;

  if( streamctrl_any.in() >>= streamctrl )
  {
     // Any still owns the pointer, so we duplicate it 
     AVStreams::StreamCtrl::_duplicate( streamctrl );
     this->streamctrls_.bind (flowname,
                             streamctrl);
  }
}

void
Connection_Manager::destroy (const ACE_CString &flowname
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  this->protocol_objects_.unbind (flowname);
  this->receivers_.unbind (flowname);

  this->streamctrls_.unbind (flowname );

}

Connection_Manager::Receivers &
Connection_Manager::receivers (void)
{
  return this->receivers_;
}

Connection_Manager::Protocol_Objects &
Connection_Manager::protocol_objects (void)
{
  return this->protocol_objects_;
}

Connection_Manager::StreamCtrls &
Connection_Manager::streamctrls (void)
{
  return this->streamctrls_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, AVStreams::MMDevice_var>;
template class ACE_Hash_Map_Manager<ACE_CString, AVStreams::MMDevice_var, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString, TAO_AV_Protocol_Object *>;
template class ACE_Hash_Map_Manager<ACE_CString, TAO_AV_Protocol_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString, AVStreams::StreamCtrl_var>;
template class ACE_Hash_Map_Manager<ACE_CString, AVStreams::StreamCtrl_var, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString, Endpoint_Addresses*>;
template class ACE_Hash_Map_Manager<ACE_CString, Endpoint_Addresses*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, AVStreams::MMDevice_var>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, AVStreams::MMDevice_var, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::MMDevice_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, TAO_AV_Protocol_Object *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, TAO_AV_Protocol_Object *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_AV_Protocol_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, AVStreams::StreamCtrl_var>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, AVStreams::StreamCtrl_var, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, AVStreams::StreamCtrl_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, Endpoint_Addresses*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, Endpoint_Addresses*, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Endpoint_Addresses*, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
