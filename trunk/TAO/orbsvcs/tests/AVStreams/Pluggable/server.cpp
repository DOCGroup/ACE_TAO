

// $Id$

#include "server.h"
#include "ace/Get_Opt.h"

static FILE *output_file = 0;
// File into which the received data is written.

static const char *output_file_name = "output";
// File handle of the file into which data is written.

int
FTP_Server_StreamEndPoint::get_callback (const char *,
                                         TAO_AV_Callback *&callback)
{
  // Return the server application callback to the AVStreams for further upcalls,
  callback = &this->callback_;
  return 0;
}

int
FTP_Server_Callback::receive_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info *,
                                    const ACE_Addr &)
{
  // Upcall from the AVStreams when there is data to be received from the
  // ftp client.

  ACE_DEBUG ((LM_DEBUG,
              "FTP_Server_Callback::receive_frame\n"));

  while (frame != 0)
    {
      // Write the received data to the file.
      unsigned int result = ACE_OS::fwrite (frame->rd_ptr (),
                                            frame->length (),
                                            1,
                                            output_file);

      if (result == frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "FTP_Server_Flow_Handler::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }
  return 0;
}

int
FTP_Server_Callback::handle_destroy (void)
{
  // Called when the ftp client requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "FTP_Server_Callback::end_stream\n"));
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

Server::Server (void)
  : mmdevice_ (0)
{
}

Server::~Server (void)
{
  delete this->mmdevice_;
}

int
Server::init (int,
              char **
              TAO_ENV_ARG_DECL)
{
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Register the server mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Register the mmdevice with the naming service.
  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice");

  CORBA::Object_var mmdevice =
    this->mmdevice_->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  // Initialize the naming services
  if (this->my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to initialize "
                       "the TAO_Naming_Client\n"),
                      -1);

  // Register the server object with the naming server.
  this->my_naming_client_->rebind (server_mmdevice_name,
                                   mmdevice.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
parse_args (int argc,
            char **argv)
{
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          output_file_name = opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: server -f filename"),
                            -1);
        }
    }

  return 0;
}

int
main (int argc,
      char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc,
                    argv);

      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      output_file = ACE_OS::fopen (output_file_name,
                                   "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           output_file_name),
                          -1);

      else ACE_DEBUG ((LM_DEBUG,
                       "File Opened Successfull\n"));

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Server server;
      result =
        server.init (argc,
                     argv
                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_OS::fclose (output_file);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy_B
<FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy
<FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B
<FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy
<FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
