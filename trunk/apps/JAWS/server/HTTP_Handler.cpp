// HTTP_Service.cpp -- simple implementation of the HTTP protocol

#include "ace/Message_Block.h"

#include "JAWS/server/HTTP_Handler.h"
#include "JAWS/server/HTTP_Helpers.h"
#include "JAWS/server/IO.h"

/* static */
// const char *HTTP_Handler::HTTP_HEADER = "HTTP/1.0 200 Document follows\r\n" "Server: JAWS\r\n\r\n";
const char *HTTP_Handler::HTTP_HEADER = "";
const char *HTTP_Handler::HTTP_TRAILER = "";
const int HTTP_Handler::HTTP_HEADER_LENGTH  = ACE_OS::strlen (HTTP_Handler::HTTP_HEADER);
const int HTTP_Handler::HTTP_TRAILER_LENGTH = ACE_OS::strlen (HTTP_Handler::HTTP_TRAILER);

HTTP_Handler::HTTP_Handler (JAWS_IO &io,
			    HTTP_Handler_Factory &factory)
  : factory_ (factory),
    request_data_ (0),
    handle_ (ACE_INVALID_HANDLE),
    io_ (io)
{
  this->io_.handler (this);
}

HTTP_Handler::~HTTP_Handler (void)
{
  this->request_data_->release ();
  this->request_data_ = 0;
}

void 
HTTP_Handler::open (ACE_HANDLE handle, 
		    ACE_Message_Block &initial_data)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) New connection \n"));
  
  this->handle_ = handle;
  this->io_.handle (this->handle_);
  
  this->request_data_ = initial_data.duplicate ();
  this->client_data (initial_data);
}

void 
HTTP_Handler::client_data (ACE_Message_Block &message_block)
{
  message_block.rd_ptr ()[message_block.length ()] = '\0';

  if (this->enough_data ())
    {
      this->parse_request ();
      return;
    }
  
  int next_read_size = HTTP_Handler::MAX_REQUEST_SIZE - this->request_data_->length ();
  if (next_read_size == 0)
    {
      this->request_too_long ();
      return;
    }
  
  this->io_.read (*this->request_data_, next_read_size);
}

int 
HTTP_Handler::enough_data (void)
{
  char *data = this->request_data_->rd_ptr ();
  if (ACE_OS::strstr (data, "\r\n\r\n") != NULL ||
      ACE_OS::strstr (data, "\n\n") != NULL)
    return 1;
  else
    return 0;
}

void
HTTP_Handler::parse_request (void)
{
  request_.init (request_data_->rd_ptr (), request_data_->length ());

  if (request_.status () != HTTP_Request::OK)
    this->invalid_request (HTTP_Status_Code::STATUS_BAD_REQUEST);
  else
    {
      switch (request_.type ())
	{
	case HTTP_Request::GET :
	  this->io_.transmit_file (request_.filename (), 
				   HTTP_HEADER, 
				   HTTP_HEADER_LENGTH,
				   HTTP_TRAILER, 
				   HTTP_TRAILER_LENGTH);
	  break;

	case HTTP_Request::PUT :
	  this->io_.receive_file (request_.filename (),
				  request_.data (),
				  request_.data_length (),
				  request_.content_length ());
	  break;

	default :
	  this->invalid_request (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED);
	}
    }
}

void
HTTP_Handler::serve_error (int status_code)
{
  static char const errormessage[] =
    "HTTP/1.0 %d %s\r\n"
    "Content-type: text/html\r\n"
    "\r\n"
    "<html>\n"
    "<head><title>Error message</title></head>\n"
    "<body>\n"
    "<h1>Error %d: %s</h1>"
    "Could not access file: %s.\n"
    "</body>\n"
    "</html>\n"
    ;

  char buffer[2 * MAXPATHLEN];
  int length = sprintf (buffer, 
			errormessage,
			status_code, HTTP_Status_Code::instance()[status_code],
			status_code, HTTP_Status_Code::instance()[status_code],
			request_.filename ());
  
  this->io_.send_error_message (buffer, length);
}

void
HTTP_Handler::serve_directory (void)
{
  // We'll just forbid it for now.
  this->serve_error(403);
}

void 
HTTP_Handler::receive_file_complete (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s received successfully\n", request_.filename ()));
  char buffer[BUFSIZ];
  ACE_OS::sprintf (buffer, 
		   "%s %d %s",
		   "HTTP/1.0",
		   HTTP_Status_Code::STATUS_OK,
		   "Successful");  
  this->io_.send_confirmation_message (buffer, ACE_OS::strlen (buffer));
}

void 
HTTP_Handler::receive_file_error (int result)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s error in receiving file\n", request_.filename ()));
  char buffer[BUFSIZ];
  ACE_OS::sprintf (buffer, 
		   "%s %d %s",
		   "HTTP/1.0",
		   result,
		   "Failed");  
  this->io_.send_confirmation_message (buffer, ACE_OS::strlen (buffer));
}

void 
HTTP_Handler::confirmation_message_complete (void)
{
  this->done ();  
}

void 
HTTP_Handler::error_message_complete (void)
{
  this->done ();  
}

void 
HTTP_Handler::transmit_file_complete (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s transmitted successfully\n", request_.filename ()));
  this->done ();
}

void 
HTTP_Handler::transmit_file_error (int result)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s error in transmitting file\n", request_.filename ()));
  this->serve_error (result);
}

void 
HTTP_Handler::read_error (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) error in reading request\n"));
  this->done ();
}

void
HTTP_Handler::write_error (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s error in writing response\n", request_.filename ()));
  this->done ();
}

void 
HTTP_Handler::timeout (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s error in reading request\n", request_.filename ()));
  this->serve_error (HTTP_Status_Code::STATUS_INTERNAL_SERVER_ERROR);
}

void 
HTTP_Handler::request_too_long (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) request too long\n"));
  this->serve_error (HTTP_Status_Code::STATUS_BAD_REQUEST);
}

void 
HTTP_Handler::invalid_request (int error)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) invalid request\n"));
  this->serve_error (error);
}

void 
HTTP_Handler::done (void)
{
  this->factory_.destroy_http_handler (*this, this->io_);
}

HTTP_Handler_Factory::~HTTP_Handler_Factory (void)
{
}

HTTP_Handler *
Synch_HTTP_Handler_Factory::create_http_handler (void)
{
  JAWS_Synch_IO *io;
  ACE_NEW_RETURN (io, JAWS_Synch_IO, 0);
  HTTP_Handler *handler;
  ACE_NEW_RETURN (handler, HTTP_Handler (*io, *this), 0);
  return handler;
}

void
Synch_HTTP_Handler_Factory::destroy_http_handler (HTTP_Handler &handler,
						  JAWS_IO &io)
{
  delete &handler;
  delete &io;
}


// This only works on Win32
#if defined (ACE_WIN32)
void
Asynch_HTTP_Handler_Factory::open (ACE_HANDLE handle,
				   ACE_Message_Block &mb)
{
  JAWS_Asynch_IO *io;
  ACE_NEW (io, JAWS_Asynch_IO);
  HTTP_Handler *handler;
  ACE_NEW (handler, HTTP_Handler (*io, *this));
  handler->open (handle, mb);
}

void
Asynch_HTTP_Handler_Factory::destroy_http_handler (HTTP_Handler &handler,
						   JAWS_IO &io)
{
  delete &handler;
  delete &io;
  delete this;
}

HTTP_Handler *
Asynch_HTTP_Handler_Factory::create_http_handler (void)
{
  return 0;
}

#endif /* ACE_WIN32 */
