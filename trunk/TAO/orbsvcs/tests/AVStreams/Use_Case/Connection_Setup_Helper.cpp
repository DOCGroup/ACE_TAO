//$Id$
#include "Connection_Setup_Helper.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"

Connection_Setup_Helper::Connection_Setup_Helper (void)
{
}

Connection_Setup_Helper::~Connection_Setup_Helper (void)
{
}

int 
Connection_Setup_Helper::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  this->orb_ = orb;
  this->poa_ = poa;

  // Initialize the naming services
  if (this->my_naming_client_.init (this->orb_.in ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize "
		       "the TAO_Naming_Client. \n"),
		      -1);
  return 0;
}


int
Connection_Setup_Helper::create_recv_obj_ref_list (CosNaming::BindingList &bl,
                                                   ACE_CString *sender_device_name,
                                                   Recv_Obj_Ref_Set &recv_obj_ref_set,
                                                   CORBA::Environment& ACE_TRY_ENV)
{
  for (CORBA::ULong i = 0; i < bl.length (); i++)
    {
      Recv_Obj_Ref_Item *item;
      
      // Create the recv_onj_ref_item
      ACE_NEW_RETURN (item,
                      Recv_Obj_Ref_Item,
                      -1);
      
      // get the device name from the binding list
      item->name = bl [i].binding_name [0].id.in ();


      // Create the full name of the receiver for resolving from NS
      // correctly
      CosNaming::Name mmdevice_name (1);
      mmdevice_name.length (1);
      mmdevice_name [0].id = CORBA::string_dup (sender_device_name->c_str ());
      
      mmdevice_name.length (2);
      mmdevice_name [1].id = CORBA::string_dup ("Receivers");
      
      mmdevice_name.length (3);
      mmdevice_name [2].id = CORBA::string_dup (item->name.c_str ());
      
      // Resolve the reference of the receiver from the NS.
      CORBA::Object_var obj = my_naming_client_->resolve (mmdevice_name,
                                                          ACE_TRY_ENV);
      
      // Store the reference in the item
      item->ref = AVStreams::MMDevice::_narrow (obj.in ());
      
      // Add the object reference item corresponding to this receuver
      // in the set
      recv_obj_ref_set.insert (item);
    }
  return 0;
}


int
Connection_Setup_Helper::bind_to_receivers (ACE_CString *sender_device_name,
                                            AVStreams::MMDevice_ptr sender_mmdevice_obj,
                                            Recv_Obj_Ref_Set &recv_obj_ref_set,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  
  // Create name with the name of the sender
  CosNaming::Name sender_mmdevice_name (1);
  sender_mmdevice_name.length (1);
  sender_mmdevice_name [0].id = CORBA::string_dup (sender_device_name->c_str ());

      
  ACE_TRY_EX (bind_naming_context)
    {
      // Try binding the sender context to the NS
      this->my_naming_client_->bind_new_context (sender_mmdevice_name,
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (bind_naming_context);
  
      // Sender context does not exist

      // Create the name for storing the receivers
      sender_mmdevice_name.length (2);
      sender_mmdevice_name [1].id = CORBA::string_dup ("Receivers");
  
      // Try binding the receivers context under the sender
      this->my_naming_client_->bind_new_context (sender_mmdevice_name,
                                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
  
      
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, al_ex)
    {
      
      // The sender is already bound, probably by the receiver.

      sender_mmdevice_name.length (2);
      sender_mmdevice_name [1].id = CORBA::string_dup ("Receivers");

      // Try to resolev the receivers
      CORBA::Object_var rc_obj = this->my_naming_client_->resolve (sender_mmdevice_name,
                                                                   ACE_TRY_ENV);
      
      CosNaming::NamingContext_var rc = CosNaming::NamingContext::_narrow (rc_obj.in ());

 
      CosNaming::BindingIterator_var it;
      CosNaming::BindingList_var bl;
      const CORBA::ULong CHUNK = 100;
 
      // Get the list of receivers registered for this server
      rc->list (CHUNK, 
                bl, 
                it);

      // Add the receivers found in the bindinglist to the recv_obj_ref_set
      create_recv_obj_ref_list (bl, 
                                sender_device_name,
                                recv_obj_ref_set,
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      
      if (!CORBA::is_nil (it.in ()))
        {
          CORBA::Boolean more = 1;
          // Check to see if there are more receivers listed.
          while (more)
            {
              more = it->next_n (CHUNK, bl);
              
              create_recv_obj_ref_list (bl, 
                                        sender_device_name,
                                        recv_obj_ref_set,
                                        ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
            }
        }
    }
  ACE_ENDTRY;
  
  
  sender_mmdevice_name.length (2);
  sender_mmdevice_name [1].id = CORBA::string_dup (sender_device_name->c_str ());
  
  // Register the sender object with the naming server.
  this->my_naming_client_->rebind (sender_mmdevice_name,
                                   sender_mmdevice_obj,
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  

  
  return 0;
}

int
Connection_Setup_Helper::connect_to_receivers (ACE_CString *sender_device_name,
                                               ACE_CString *flow_protocol_str,
                                               ACE_CString *protocol,
                                               ACE_CString *host_address,
                                               AVStreams::MMDevice_ptr sender_mmdevice_obj,
                                               Recv_Obj_Ref_Set *recv_obj_ref_set,
                                               StreamCtrl_Map &streamctrl_map,
                                               CORBA::Environment& ACE_TRY_ENV)
{
  
  if (recv_obj_ref_set->size () == 0)
    return 0;
  
  ACE_CString address (host_address->c_str ());

  // Set the address of the of the distributer receiver endpoint.
  ACE_INET_Addr sender_addr (address.c_str ());
  
  char hostname [BUFSIZ];

  int port = sender_addr.get_port_number ();
  sender_addr.get_host_name (hostname,
                             BUFSIZ);

  ACE_DEBUG ((LM_DEBUG,
              "Hostname:Port is %s:%d\n",
              hostname,
              port));

  // Connect to all the receivers bound to
  Recv_Obj_Ref_Set_Itor end = recv_obj_ref_set->end ();
  for (Recv_Obj_Ref_Set_Itor begin = recv_obj_ref_set->begin ();
           begin != end; ++begin)
    {
      // Initialize the  QoS
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      
      char buf [BUFSIZ];
      
      address = hostname;
      address += ":";
      address += ACE_OS::itoa (port++, buf,10);

      sender_addr.set (address.c_str ());

      ACE_CString flowname (sender_device_name->c_str ());
      flowname += "_";
      flowname += (*begin)->name;
      
      // Create the forward flow specification to describe the flow.
      TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
                                               "IN",
                                               "USER_DEFINED",
                                               flow_protocol_str->c_str (),
                                               protocol->c_str (),
                                               &sender_addr);
      
      // Set the flow specification for the stream between receiver and distributer 
      AVStreams::flowSpec flow_spec (1);
      flow_spec.length (1);
      flow_spec [0] = CORBA::string_dup (sender_entry.entry_to_string ());
      
      StreamCtrl_Item *item;
      ACE_NEW_RETURN (item,
                      StreamCtrl_Item,
                      -1);
      
      // Create the stream control for this stream.
      TAO_StreamCtrl* streamctrl;
      ACE_NEW_RETURN (streamctrl,
                      TAO_StreamCtrl,
                      -1);
      
      // Servant Reference Counting to manage lifetime
      PortableServer::ServantBase_var safe_streamctrl = streamctrl;
        
      
      item->streamctrl = streamctrl;
      item->flowname = flowname;
      
      TAO_String_Hash_Key flowname_key (CORBA::string_dup (flowname.c_str ()));
      

      // Bind/Connect  the sender and receiver MMDevices.
      int result = streamctrl->bind_devs (sender_mmdevice_obj,
                                          (*begin)->ref.in (),
                                          the_qos.inout (),
                                          flow_spec,
                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in binding sender and receiver devices\n"),
                          -1);

      // Bind the flowname and the corresponding stream controller to the stream controller map
      if (streamctrl_map.bind (flowname_key, item) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Bind to Stream Ctrl Map failed\n"),
                            -1);
        }
      
    }
  return 0;
}

int
Connection_Setup_Helper::bind_to_sender (ACE_CString *sender_device_name,
                                         ACE_CString *receiver_device_name,
                                         AVStreams::MMDevice_ptr &sender_mmdevice_obj,
                                         AVStreams::MMDevice_ptr receiver_mmdevice_obj, 
                                         CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name mmdevice_naming_name (1);

  ACE_TRY_EX (resolve_sender)
    {

      // Create name with the name of the sender
      mmdevice_naming_name.length (1);
      mmdevice_naming_name [0].id = CORBA::string_dup (sender_device_name->c_str ());
      
      mmdevice_naming_name.length (2);
      mmdevice_naming_name [1].id = CORBA::string_dup (sender_device_name->c_str ());
      
      // Resolve the sender mmdevice object reference from the Naming Service
      CORBA::Object_var mmdevice_obj =
        this->my_naming_client_->resolve (mmdevice_naming_name,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK_EX (resolve_sender);
      
      sender_mmdevice_obj =
        AVStreams::MMDevice::_narrow (mmdevice_obj.in (),
                                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      
      if (CORBA::is_nil (sender_mmdevice_obj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve MMdevice in Naming service \n"),
                          -1);
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, al_ex)
    {
      // Sender does not exist
      ACE_TRY_EX (bind_sender_context)
        {
          mmdevice_naming_name.length (1);
          mmdevice_naming_name [0].id = CORBA::string_dup (sender_device_name->c_str ());

          // Create the sender context
          this->my_naming_client_->bind_new_context (mmdevice_naming_name,
                                                     ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (bind_sender_context);

          mmdevice_naming_name.length (2);
          mmdevice_naming_name [1].id = CORBA::string_dup ("Receivers");

          // Craete the receivers context under the sender's context
          this->my_naming_client_->bind_new_context (mmdevice_naming_name,
                                                     ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          
        }
      ACE_CATCH (CosNaming::NamingContext::AlreadyBound, al_ex)
        {
        }
      ACE_ENDTRY;
    }
  ACE_ENDTRY;

  mmdevice_naming_name.length (2);
  mmdevice_naming_name [1].id = CORBA::string_dup ("Receivers");
  
  mmdevice_naming_name.length (3);
  mmdevice_naming_name [2].id = CORBA::string_dup (receiver_device_name->c_str ());

  
  // Register the receiver object under the sender context with the naming server.
  this->my_naming_client_->rebind (mmdevice_naming_name,
                                   receiver_mmdevice_obj,
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
   return 0;
}


int
Connection_Setup_Helper::connect_to_sender (ACE_CString *sender_device_name,
                                            ACE_CString *receiver_device_name,
                                            ACE_CString *flow_protocol_str,
                                            ACE_CString *protocol,
                                            ACE_CString *host_address,
                                            AVStreams::MMDevice_ptr &sender_mmdevice_obj,
                                            AVStreams::MMDevice_ptr receiver_mmdevice_obj,
                                            StreamCtrl_Map &streamctrl_map,
                                            CORBA::Environment& ACE_TRY_ENV)
{

  if (CORBA::is_nil (sender_mmdevice_obj))
    return 0;


  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  // Set the address of the sender.
  ACE_INET_Addr sender_addr (host_address->c_str ());

  ACE_CString flowname  (sender_device_name->c_str ());
  flowname += "_";
  flowname += receiver_device_name->c_str ();
    
  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry sender_entry (flowname.c_str (),
					   "IN",
					   "USER_DEFINED",
                                           flow_protocol_str->c_str (),
                                           protocol->c_str (),
                                           &sender_addr);
                                           
  
  // Set the flow specification for the stream between sender and distributer
  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] = CORBA::string_dup (sender_entry.entry_to_string ());

  StreamCtrl_Item *item;
  ACE_NEW_RETURN (item,
                  StreamCtrl_Item,
                  -1);

  // Create teh stream control for the stream
  TAO_StreamCtrl* streamctrl;
  ACE_NEW_RETURN (streamctrl,
                  TAO_StreamCtrl,
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_streamctrl = streamctrl;
    
  item->streamctrl = streamctrl;
  item->flowname = flowname;
  
  TAO_String_Hash_Key flowname_key (CORBA::string_dup (flowname.c_str ()));
  
  
  // Bind/Connect  the sender and receiver MMDevices.
  CORBA::Boolean res =
    streamctrl->bind_devs (sender_mmdevice_obj,
                           receiver_mmdevice_obj,  
                           the_qos.inout (),
                           flow_spec,
                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  
  if (res == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Streamctrl::bind_devs failed\n"),-1);

  // Bind the streamcontroller and stream name to the Stream Controller hash map.
  if (streamctrl_map.bind (flowname_key, item) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Bind to Stream Ctrl Map failed\n"),
                        -1);
    }
  
  // Start the data sending
  AVStreams::flowSpec start_spec;
  streamctrl->start (start_spec,ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
return 0;
}

int 
Connection_Setup_Helper::disconnect (AVStreams::flowSpec &flowspec,
                                     StreamEndpoint_Set *endpoint_set,
                                     StreamCtrl_Map *streamctrl_map,
                                     CORBA::Environment& ACE_TRY_ENV)
{

  AVStreams::flowSpec stop_spec;

  for (unsigned int i = 0; i < flowspec.length (); i++)
    {
      TAO_Forward_FlowSpec_Entry entry;
      entry.parse (flowspec[i].in ());
      
      ACE_CString flowname (entry.flowname ());

      ACE_DEBUG ((LM_DEBUG,
                  "Disconnect %s\n",
                  flowname.c_str ()));
      
      TAO_String_Hash_Key flow_name_key (flowname.c_str ());
      StreamCtrl_Map_Entry *streamctrl_entry = 0;
      // Check if this flow has been initiated by this device. If so then
      if (streamctrl_map->find (flow_name_key,
                                streamctrl_entry) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Found flow in the streamctrl\n"));

          // The flow has been initiated by this device and hence
          // call destroy on the stream controller.
          streamctrl_entry->int_id_->streamctrl->destroy (stop_spec,
                                                          ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          if (streamctrl_map->unbind (streamctrl_entry) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unbinding %s flow srom StreamCtrl Hash Map failed\n",
                               flowname.c_str ()),
                              -1);

          // Disconnect every stream whose endpoints are in the endpoint_set.
          StreamEndpoint_Set_Itor end = endpoint_set->end ();
          for (StreamEndpoint_Set_Itor begin = endpoint_set->begin ();
               begin != end; ++begin)
            {
              // The flow was initiated by the peer device.
              if (ACE_OS::strcmp (flowname.c_str (), (*begin)->flowname.c_str ()) == 0)
                {
                  endpoint_set->remove ((*begin));
                  
                  ACE_DEBUG ((LM_DEBUG,
                              "After calling remove\n"));
                  break;
                }

            }
      
          return 0;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Did not find flow in the streamctrl\n"));
      
      // Disconnect every stream whose endpoints are in the endpoint_set.
      StreamEndpoint_Set_Itor end = endpoint_set->end ();
      for (StreamEndpoint_Set_Itor begin = endpoint_set->begin ();
           begin != end; ++begin)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Found flow in the Endpoint\n"));
          
          // The flow was initiated by the peer device.
          if (ACE_OS::strcmp (flowname.c_str (), (*begin)->flowname.c_str ()) == 0)
            {
              
              // Get the stream controller for this stream. This is the stream controller of the peer device
              CORBA::Any_ptr streamctrl_any = (*begin)->endpoint->get_property_value ("Related_StreamCtrl",
                                                                                      ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
       
              ACE_DEBUG ((LM_DEBUG,
                          "AFter getting StreamCtrl\n"));

              AVStreams::StreamCtrl_ptr streamctrl;
              
              *streamctrl_any >>= streamctrl;
              
              //Destroy the stream
              streamctrl->destroy (stop_spec,
                                   ACE_TRY_ENV);
              ACE_CHECK_RETURN (-1);
              
              ACE_DEBUG ((LM_DEBUG,
                          "After calling destroy\n"));
                          
              endpoint_set->remove ((*begin));
              
              ACE_DEBUG ((LM_DEBUG,
                          "After calling remove\n"));

              end = endpoint_set->end ();
              ACE_DEBUG ((LM_DEBUG,
                          "After resetting end\n"));
                                        
              return 0;
            }
        }
    }  
  return 0;
  
}

int 
Connection_Setup_Helper::disconnect (StreamEndpoint_Set *endpoint_set,
                                     StreamCtrl_Map *streamctrl_map,
                                     CORBA::Environment& ACE_TRY_ENV)
{
  AVStreams::flowSpec stop_spec;
  
  // Disconnect every stream whose endpoints are in the endpoint_set.
  StreamEndpoint_Set_Itor end = endpoint_set->end ();
  for (StreamEndpoint_Set_Itor begin = endpoint_set->begin ();
       begin != end; ++begin)
    {
      ACE_CString flowname = (*begin)->flowname;
      TAO_String_Hash_Key flow_name_key (flowname.c_str ());
      StreamCtrl_Map_Entry *streamctrl_entry = 0;
      // Check if this flow has been initiated by this device. If so then
      if (streamctrl_map->find (flow_name_key,
                                streamctrl_entry) == 0)
        {
          // The flow has been initiated by this device and hence
          // call destroy on the stream controller.
          streamctrl_entry->int_id_->streamctrl->destroy (stop_spec,
                                                          ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
        }
      else
        {
          // The flow was initiated by the peer device.

          // Get the stream controller for this stream. This is the stream controller of the peer device
          CORBA::Any_ptr streamctrl_any = (*begin)->endpoint->get_property_value ("Related_StreamCtrl",
                                                                                  ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          
          AVStreams::StreamCtrl_ptr streamctrl;
          
          *streamctrl_any >>= streamctrl;
          
          //Destroy the stream
          streamctrl->destroy (stop_spec,
                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
        }
    }
  
  return 0;
  
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash<TAO_String_Hash_Key>;
template class ACE_Equal_To<TAO_String_Hash_Key>;

template class ACE_Hash_Map_Entry<TAO_String_Hash_Key, StreamCtrl_Item*>;
template class ACE_Hash_Map_Manager<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Iterator<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

template class ACE_Node <Recv_Obj_Ref_Item*>;
template class ACE_Unbounded_Set <Recv_Obj_Ref_Item*>;
template class ACE_Unbounded_Set_Iterator <Recv_Obj_Ref_Item*>;

template class ACE_Node <Protocol_Object_Item*>;
template class ACE_Unbounded_Set <Protocol_Object_Item*>;
template class ACE_Unbounded_Set_Iterator <Protocol_Object_Item*>;

template class ACE_Node <StreamEndpoint_Item*>;
template class ACE_Unbounded_Set <StreamEndpoint_Item*>;
template class ACE_Unbounded_Set_Iterator <StreamEndpoint_Item*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiation ACE_Hash<TAO_String_Hash_Key>;
#pragma instantiation ACE_Equal_To<TAO_String_Hash_Key>;

#pragma instantiation ACE_Hash_Map_Entry<TAO_String_Hash_Key, StreamCtrl_Item*>;
#pragma instantiation ACE_Hash_Map_Manager<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
#pragma instantiation ACE_Hash_Map_Manager_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

#pragma instantiation ACE_Hash_Map_Iterator<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
#pragma instantiation ACE_Hash_Map_Iterator_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
#pragma instantiation ACE_Hash_Map_Iterator_Base_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;
#pragma instantiation ACE_Hash_Map_Reverse_Iterator<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex>;
#pragma instantiation ACE_Hash_Map_Reverse_Iterator_Ex<TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Hash<TAO_String_Hash_Key>, ACE_Equal_To<TAO_String_Hash_Key>, ACE_Null_Mutex>;

#pragma instantiation ACE_Node <Recv_Obj_Ref_Item*>;
#pragma instantiation ACE_Unbounded_Set <Recv_Obj_Ref_Item*>;
#pragma instantiation ACE_Unbounded_Set_Iterator <Recv_Obj_Ref_Item*>;

#pragma instantiation ACE_Node <Protocol_Object_Item*>;
#pragma instantiation ACE_Unbounded_Set <Protocol_Object_Item*>;
#pragma instantiation ACE_Unbounded_Set_Iterator <Protocol_Object_Item*>;

#pragma instantiation ACE_Node <StreamEndpoint_Item*>;
#pragma instantiation ACE_Unbounded_Set <StreamEndpoint_Item*>;
#pragma instantiation ACE_Unbounded_Set_Iterator <StreamEndpoint_Item*>;


#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
