//$Id$

#include "Connection_Manager.h"
#include "tao/debug.h"

Connection_Manager::Connection_Manager (void)
{
}

Connection_Manager::~Connection_Manager (void)
{
}

int
Connection_Manager::init (CORBA::ORB_ptr orb)
{
  // Initialize the naming service
  if (this->naming_client_.init (orb) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client.\n"),
                      -1);
  return 0;
}

void
Connection_Manager::bind_to_receivers (const ACE_CString &sender_name,
                                       AVStreams::MMDevice_ptr sender)
{
  this->sender_name_ =
    sender_name;

  this->sender_ =
    AVStreams::MMDevice::_duplicate (sender);

  CosNaming::Name name (1);
  name.length (1);

  try
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      this->sender_context_ =
        this->naming_client_->bind_new_context (name);

      //
      // We reach here if there was no exception raised in
      // <bind_new_context>.  We then create a receiver context.
      //

      // Create the context for storing the receivers
      name [0].id =
        CORBA::string_dup ("Receivers");

      // Try binding the receivers context under the sender context.
      this->receiver_context_ =
        this->sender_context_->bind_new_context (name);
    }
  catch (const CosNaming::NamingContext::AlreadyBound&)
    {
      //
      // The sender context already exists, probably created by the
      // receiver(s).
      //

      // Get the sender context.
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      CORBA::Object_var object =
        this->naming_client_->resolve (name);

      this->sender_context_ =
        CosNaming::NamingContext::_narrow (object.in ());

      // Find the Receiver context.
      name [0].id =
        CORBA::string_dup ("Receivers");

      object =
        this->sender_context_->resolve (name);

      this->receiver_context_ =
        CosNaming::NamingContext::_narrow (object.in ());

      this->find_receivers ();
    }

  name [0].id =
    CORBA::string_dup (this->sender_name_.c_str ());

  // Register the sender object with the sender context.
  this->sender_context_->rebind (name,
                                 sender);
}

void
Connection_Manager::find_receivers (void)
{
  CosNaming::BindingIterator_var iterator;
  CosNaming::BindingList_var binding_list;
  const CORBA::ULong chunk = 100;

  // Get the list of receivers registered for this sender.
  this->receiver_context_->list (chunk,
                                 binding_list,
                                 iterator);

  // Add the receivers found in the bindinglist to the <receivers>.
  this->add_to_receivers (binding_list);

  if (!CORBA::is_nil (iterator.in ()))
    {
      CORBA::Boolean more = 1;

      // Check to see if there are more receivers listed.
      while (more)
        {
          more = iterator->next_n (chunk,
                                   binding_list);

          this->add_to_receivers (binding_list);
        }
    }
}

void
Connection_Manager::add_to_receivers (CosNaming::BindingList &binding_list)
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
        this->receiver_context_->resolve (name);

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
Connection_Manager::connect_to_receivers (void)
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

      // Create the forward flow specification to describe the flow.
      TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
                                               "IN",
                                               "USER_DEFINED",
                                               "",
                                               "UDP",
                                               0);

      // Set the flow specification for the stream between receiver
      // and distributer
      AVStreams::flowSpec flow_spec (1);
      flow_spec.length (1);
      flow_spec [0] =
        CORBA::string_dup (sender_entry.entry_to_string ());

      // Create the stream control for this stream.
      TAO_StreamCtrl *streamctrl;
      ACE_NEW (streamctrl,
               TAO_StreamCtrl);

      // Servant Reference Counting to manage lifetime
      PortableServer::ServantBase_var safe_streamctrl =
        streamctrl;

      // Register streamctrl.
      AVStreams::StreamCtrl_var streamctrl_object =
        streamctrl->_this ();

      // Bind the flowname and the corresponding stream controller to
      // the stream controller map
      this->streamctrls_.bind (flowname,
                               streamctrl_object);

      // Bind the sender and receiver MMDevices.
      (void) streamctrl->bind_devs (this->sender_.in (),
                                    (*iterator).int_id_.in (),
                                    the_qos.inout (),
                                    flow_spec);
    }
}

void
Connection_Manager::bind_to_sender (const ACE_CString &sender_name,
                                    const ACE_CString &receiver_name,
                                    AVStreams::MMDevice_ptr receiver)
{
  this->sender_name_ =
    sender_name;

  this->receiver_name_ =
    receiver_name;

  this->receiver_ =
    AVStreams::MMDevice::_duplicate (receiver);

  this->sender_ = 0 ;

  CosNaming::Name name (1);
  name.length (1);

  int sender_context_exists = 0;

  try
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      CORBA::Object_var object =
        this->naming_client_->resolve (name);

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
        this->sender_context_->resolve (name);

      this->receiver_context_ =
        CosNaming::NamingContext::_narrow (object.in ());
    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      name [0].id =
        CORBA::string_dup (this->sender_name_.c_str ());

      // Create the sender context
      this->sender_context_ =
        this->naming_client_->bind_new_context (name);

      name [0].id =
        CORBA::string_dup ("Receivers");

      // Create the receivers context under the sender's context
      this->receiver_context_ =
        this->sender_context_->bind_new_context (name);
    }

  //
  // At this point we either have resolved the receiver context or we
  // have created a new one.
  //
  name [0].id =
    CORBA::string_dup (this->receiver_name_.c_str ());

  // Register this receiver object under the receiver context.
  this->receiver_context_->rebind (name,
                                   receiver);

  //
  // Check if the sender was registered.  Note that if we created the
  // sender context, there is no point in checking for the sender.
  //
  if (sender_context_exists)
    {
      try
        {
          // Try binding the sender under the sender context
          name [0].id =
            CORBA::string_dup (this->sender_name_.c_str ());

          CORBA::Object_var object =
            this->sender_context_->resolve (name);

          this->sender_ =
            AVStreams::MMDevice::_narrow (object.in ());
        }
      catch (const CosNaming::NamingContext::NotFound&)
        {
          // No problem if the sender was not there.
        }
    }
}

void
Connection_Manager::connect_to_sender (void)
{
  if (CORBA::is_nil (this->sender_.in ()))
    return;

  ACE_CString flowname =
    this->sender_name_ +
    "_" +
    this->receiver_name_;

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
                                           "IN",
                                           "USER_DEFINED",
                                           "",
                                           "UDP",
                                           0);

  // Set the flow specification for the stream between sender and
  // receiver.
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] =
    CORBA::string_dup (sender_entry.entry_to_string ());

  // Create the stream control for this stream
  TAO_StreamCtrl* streamctrl;
  ACE_NEW (streamctrl,
           TAO_StreamCtrl);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_streamctrl =
    streamctrl;

  // Register streamctrl.
  AVStreams::StreamCtrl_var streamctrl_object =
    streamctrl->_this ();

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
                           flow_spec);

  if (result == 0)
    ACE_ERROR ((LM_ERROR,
                "Streamctrl::bind_devs failed\n"));

  // Start the data sending.
  AVStreams::flowSpec start_spec;
  streamctrl->start (start_spec);
}

void
Connection_Manager::add_streamctrl (const ACE_CString &flowname,
                                    TAO_StreamEndPoint *endpoint)
{
  // Get the stream controller for this endpoint.
  CORBA::Any_var streamctrl_any =
    endpoint->get_property_value ("Related_StreamCtrl");

  AVStreams::StreamCtrl_ptr streamctrl;

  if( streamctrl_any.in() >>= streamctrl )
  {
    // the CORBA::Any_var owns the pointer, so we should
    // _duplicate it before passing it around
    AVStreams::StreamCtrl::_duplicate( streamctrl );
    this->streamctrls_.unbind(flowname);
    this->streamctrls_.bind (flowname,
                             streamctrl);
  }
}


void
Connection_Manager::destroy (const ACE_CString &flowname)
{
  this->protocol_objects_.unbind (flowname);
  this->receivers_.unbind (flowname);

  this->streamctrls_.unbind(flowname);
}

void
Connection_Manager::unbind_sender (const ACE_CString &sender_name,
                                   AVStreams::MMDevice_ptr sender_mmdevice_obj)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Connection_Manager::unbind_sender\n"));

  CosNaming::Name name (1);
  name.length (1);

  // Try binding the sender context in the NS
  name [0].id =
    CORBA::string_dup (sender_name.c_str ());

  try
    {
      CORBA::Object_var object =
        this->naming_client_->resolve (name);

      CosNaming::NamingContext_var sender_context =
        CosNaming::NamingContext::_narrow (object.in ());

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Sender Context Found\n"));

      try
        {
          object =
            sender_context->resolve (name);

          AVStreams::MMDevice_var mmdevice =
            AVStreams::MMDevice::_narrow (object.in ());

          if (mmdevice->_is_equivalent (sender_mmdevice_obj))
          {
            sender_context->unbind (name);

            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "Sender Unbound\n"));
          }
        }
      catch (const CosNaming::NamingContext::NotFound&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Sender object not found\n"));
          // Do not have to unbind.
        }

    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Sender Context Not Found\n"));
      // Do not have to unbind.
    }

}

void
Connection_Manager::unbind_receiver (const ACE_CString &sender_name,
                                     const ACE_CString &receiver_name,
                                     AVStreams::MMDevice_ptr receiver_mmdevice)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Connection_Manager::unbind_receiver\n"));

  CosNaming::Name name (1);
  name.length (1);

  // Try binding the sender context in the NS
  name [0].id =
    CORBA::string_dup (sender_name.c_str ());

  try
    {
      CORBA::Object_var object =
        this->naming_client_->resolve (name);

      CosNaming::NamingContext_var sender_context =
        CosNaming::NamingContext::_narrow (object.in ());

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Sender Context Found\n"));

      try
        {
          name [0].id =
            CORBA::string_dup ("Receivers");

          object =
            sender_context->resolve (name);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Receivers Context Found\n"));

          CosNaming::NamingContext_var receivers_context =
            CosNaming::NamingContext::_narrow (object.in ());

          name [0].id =
            CORBA::string_dup (receiver_name.c_str ());

          object =
            receivers_context->resolve (name);

          AVStreams::MMDevice_var  mmdevice =
            AVStreams::MMDevice::_narrow (object.in ());

          if (mmdevice->_is_equivalent (receiver_mmdevice))
          {
            receivers_context->unbind (name);

            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "Receiver Unbound\n"));
          }
        }
      catch (const CosNaming::NamingContext::NotFound&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Receiver Not found\n"));
          // Do not have to unbind.
        }

    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Sender Context Not Found\n"));
      // Do not have to unbind.
    }

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
