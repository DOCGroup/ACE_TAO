// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Chat_i.cpp
//
// = DESCRIPTION
//    Implementation of the Chat_i class.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Chat_i.h"
#include "ace/Read_Buffer.h"
#include "tao/ORB.h"

Chat_i::Chat_i (char *ior_file_name, char *nick) :
  ior_file_name_(ior_file_name), nick_(nick)
{
}

Chat_i::~Chat_i (void)
{
}

int
Chat_i::init (int argc, char *argv[])
{
  TAO_TRY
  {
    // Retrieve the ORB.
    this->orb_ = CORBA::ORB_init (argc,
                                  argv,
                                  0,
                                  TAO_TRY_ENV);
    TAO_CHECK_ENV;

     if (this->read_ior(this->ior_file_name_) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "could not read the ior from the file: <%s>\n",
                         this->ior_file_name_),
                         -1);
    }

    CORBA::Object_var server_object =
                 this->orb_->string_to_object (this->ior_,
                                                  TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (CORBA::is_nil (server_object.in ()))
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                           -1);
    }

    this->server_ = Broadcaster::_narrow (server_object.in (),
					 TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Chat_i::init\n");
    return -1;
  }
  TAO_ENDTRY;

  // Register our <Input_Handler> to handle STDIN events, which will
  // trigger the handle_input() method to process these events.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler
      (ACE_STDIN,
       this,
       ACE_Event_Handler::READ_MASK) == -1)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_handler"),
		      -1);
  }

  return 0;
}

int
Chat_i::run (void)
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      this->receiver_var_ = this->receiver_i_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Register ourselves with the server.
      if (server_->_cxx_register (this->receiver_var_.in (), nick_,
		      TAO_TRY_ENV) != 0)
	{
          ACE_ERROR_RETURN((LM_ERROR,
			    "failed to register the client\n" ), -1);
	}

      TAO_CHECK_ENV;

      // Run the ORB
      this->orb_->run ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::run ()");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Chat_i::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  if (ACE_OS::fgets (buf, BUFSIZ, stdin) == 0)
  {
    return 0;
  }

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      char *message = CORBA::string_dup (buf);

      this->server_->say (message, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Input_Handler::init");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

int
Chat_i::read_ior (const char *filename)
{
  // Open the file for reading.

  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
  {
      ACE_ERROR_RETURN ((LM_ERROR,
			 "Unable to open %s for writing: %p\n",
			 filename,
			 "invalid handle"),
			-1);
  }

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

 if (data == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
		       "Unable to read ior: %p\n"),
			-1);
  }

  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  if (this->ior_ == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                        "failed to read ior from file\n",
                         ""),
                        -1);
  }

  return 0;
}
