#include "server.h"

#include "ace/OS_NS_stdio.h"

#include <cstddef>
#include <new>

int
FTP_Server_StreamEndPoint::get_callback (char const *,
                                         TAO_AV_Callback *&callback)
{
  callback = &this->callback_;
  return 0;
}

int
FTP_Server_Callback::receive_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info *,
                                    ACE_Addr const &)
{
  return FTP_SERVER::instance ()->write_frame (frame);
}

int
FTP_Server_Callback::handle_destroy ()
{
  FTP_SERVER::instance ()->stop ();
  return 0;
}

int
Server::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:"));
  int option {};
  while ((option = opts ()) != -1)
    {
      switch (option)
        {
        case 'f':
          this->options_.filename = opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR, "Usage: server -f filename\n"), -1);
        }
    }
  return 0;
}

int
Server::init (int argc,
              ACE_TCHAR *argv[])
{
  if (this->parse_args (argc, argv) != 0)
    return -1;

  this->output_file_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "wb"));
  if (!this->output_file_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file %s\n",
                       ACE_TEXT_ALWAYS_CHAR (this->options_.filename.c_str ())),
                      -1);

  if (this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ()) != 0)
    return -1;

  try
    {
      this->mmdevice_ = std::make_unique<TAO_MMDevice> (&this->reactive_strategy_);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }

  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name[0].id = CORBA::string_dup ("Server_MMDevice");

  CORBA::Object_var mmdevice = this->mmdevice_->_this ();
  if (this->my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to initialize the TAO_Naming_Client\n"),
                      -1);

  this->my_naming_client_->rebind (server_mmdevice_name, mmdevice.in ());
  return 0;
}

int
Server::write_frame (ACE_Message_Block *frame)
{
  while (frame != nullptr)
    {
      std::size_t const bytes_to_write = frame->length ();
      std::size_t const bytes_written = std::fwrite (frame->rd_ptr (),
                                                     1,
                                                     bytes_to_write,
                                                     this->output_file_.get ());
      if (bytes_written != bytes_to_write)
        ACE_ERROR_RETURN ((LM_ERROR, "Writing received frame failed\n"), -1);
      frame = frame->cont ();
    }
  return 0;
}

void
Server::stop ()
{
  this->done_ = true;
}

int
Server::run ()
{
  CORBA::ORB_ptr const orb = TAO_AV_CORE::instance ()->orb ();
  while (!this->done_)
    if (orb->work_pending ())
      orb->perform_work ();
  orb->shutdown (true);
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = -1;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
      mgr->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());
      Server * const server = FTP_SERVER::instance ();
      result = server->init (argc, argv);
      if (result == 0)
        result = server->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server");
      result = -1;
    }

  FTP_SERVER::close ();
  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
