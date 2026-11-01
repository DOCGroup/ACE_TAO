#include "ace/OS_NS_string.h"
#include "URL.h"
#include "HTTP_URL.h"
#include "Options.h"
#include "Command_Processor.h"
#include "URL_Visitor.h"


Command::~Command () = default;

URL_Command::URL_Command (URL *url)
  : url_ (url)
{
}

int
URL_Command::execute ()
{
  ACE_CString check_string
    (ACE_TEXT_ALWAYS_CHAR (this->url_->url_addr ().get_path_name ()));
  if (check_string.find ("news:") != ACE_CString::npos)
    return 0;

  if (check_string.find (".cgi") != ACE_CString::npos)
     return 0;

  if (check_string.find ("mailto") != ACE_CString::npos)
    return 0;

  if (check_string.find (".gif") != ACE_CString::npos)
    return 0;

  if (check_string.find (".pdf") != ACE_CString::npos)
    return 0;

  if (check_string.find (".map") != ACE_CString::npos)
    return 0;

  if (check_string.find (".bmp") != ACE_CString::npos)
    return 0;

  if (check_string.find (".jpg") != ACE_CString::npos)
    return 0;

  if (this->url_->accept (OPTIONS::instance ()->visitor ()) !=0)
    {
      ACE_DEBUG ((LM_DEBUG,
                "Coudnt accept url\n"));
      return -1;
    }
  return 0;
}

int
URL_Command::destroy ()
{
  delete this;
  return 0;
}
Command_Processor::Command_Processor () = default;

Command_Processor::~Command_Processor () = default;

int
Command_Processor::destroy ()
{
  delete this;
  return 0;
}

int
Command_Processor::execute ()
{
  Command *command;
  while (!this->url_queue_.is_empty ())
    {
      if (this->url_queue_.dequeue_head (command) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n", "dequeue_head"),
                          -1);
      Auto_Destroyer<Command> command_ptr (command);
      if (command_ptr->execute () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n", "Couldnt execute command"),
                          -1);
    }
  return 0;
}

int
Command_Processor::insert (Command *command)
{
  // According to the order specified the commands are removed from the queue.
  if (this->url_queue_.is_full ())
    ACE_ERROR_RETURN ((LM_ERROR, "command queue is full\n"), -1);

  const ACE_TCHAR *order = OPTIONS::instance ()->order ();
  if (ACE_OS::strcmp (order, ACE_TEXT ("FIFO")) == 0)
    {
      if (this->url_queue_.enqueue_tail (command) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"), ACE_TEXT ("enqueue_tail")),
                          -1);
    }
  else if (ACE_OS::strcmp (order, ACE_TEXT ("LIFO")) == 0)
    {
      if (this->url_queue_.enqueue_head (command) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"), ACE_TEXT ("enqueue_head")),
                          -1);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("invalid command queue order\n")), -1);

  return 0;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, Options, ACE_Null_Mutex);
