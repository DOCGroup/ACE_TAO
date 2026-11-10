#include "receiver.h"

#include "ace/Get_Opt.h"

int
Receiver_StreamEndPoint::get_callback (char const *,
                                       TAO_AV_Callback *&callback)
{
  callback = &this->callback_;
  return 0;
}

int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                  TAO_AV_frame_info *,
                                  ACE_Addr const &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame for frame %d\n",
              ++this->frame_count_));
  return RECEIVER::instance ()->receive_frame (frame);
}

int
Receiver_Callback::handle_destroy ()
{
  try
    {
      TAO_AV_CORE::instance ()->orb ()->shutdown (false);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Receiver_Callback::handle_destroy");
      return -1;
    }
  return 0;
}

int
Receiver::parse_args (int argc,
                      ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:"));
  int option {};
  while ((option = opts ()) != -1)
    {
      if (option == 'f')
        this->options_.filename = opts.opt_arg ();
      else
        ACE_ERROR_RETURN ((LM_ERROR, "Usage: receiver -f filename\n"), -1);
    }
  return 0;
}

int
Receiver::init (int argc,
                ACE_TCHAR *argv[])
{
  if (this->parse_args (argc, argv) != 0)
    return -1;

  this->output_file_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "wb"));
  if (!this->output_file_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file %s\n"),
                       this->options_.filename.c_str ()),
                      -1);

  if (this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                     TAO_AV_CORE::instance ()->poa ()) != 0)
    return -1;

  TAO_MMDevice * const mmdevice = new (std::nothrow) TAO_MMDevice (&this->reactive_strategy_);
  if (mmdevice == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate receiver MMDevice\n"), -1);

  PortableServer::ServantBase_var safe_mmdevice = mmdevice;
  CORBA::Object_var mmdevice_object = mmdevice->_this ();

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("Receiver");

  if (this->naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to initialize the TAO_Naming_Client\n"),
                      -1);

  this->naming_client_->rebind (name, mmdevice_object.in ());
  return 0;
}

int
Receiver::receive_frame (ACE_Message_Block *frame)
{
  for (ACE_Message_Block *block = frame; block != nullptr; block = block->cont ())
    {
      size_t const frame_length = block->length ();
      size_t const bytes_written = ACE_OS::fwrite (block->rd_ptr (),
                                                   1,
                                                   frame_length,
                                                   this->output_file_.get ());
      if (bytes_written != frame_length)
        ACE_ERROR_RETURN ((LM_ERROR, "Receiver_Callback::fwrite failed\n"), -1);
    }

  if (ACE_OS::fflush (this->output_file_.get ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Flushing received frame failed\n"), -1);
  return 0;
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  int result = -1;
  try
    {
      orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();
      manager->activate ();

      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());
      result = RECEIVER::instance ()->init (argc, argv);
      if (result == 0)
        orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("receiver");
      result = -1;
    }

  RECEIVER::close ();
  if (!CORBA::is_nil (orb.in ()))
    orb->destroy ();
  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
