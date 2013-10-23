// $Id$

#include "ace/OS_NS_string.h"
#include "URL.h"
#include "HTTP_URL.h"
#include "Options.h"
#include "Command_Processor.h"
#include "URL_Visitor.h"



Command::~Command (void)
{
}

URL_Command::URL_Command (URL *url)
  : url_ (url)
{
}

int
URL_Command::execute (void)
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
URL_Command::destroy (void)
{
  delete this;
  return 0;
}
Command_Processor::Command_Processor (void)
{
}

Command_Processor::~Command_Processor (void)
{
}

int
Command_Processor::destroy (void)
{
  delete this;
  return 0;
}

int
Command_Processor::execute (void)
{
  Command *command;
  while (this->url_queue_.is_empty () != 1)
    {
      if (this->url_queue_.dequeue_head (command) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n", "dequeue_head"),
                          -1);
      URL_Command *url_command = dynamic_cast<URL_Command *> (command);
      Auto_Destroyer<URL_Command> url_command_ptr (url_command);
      if (url_command_ptr->execute () != 0)
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
  if (this->url_queue_.is_full() != 1)
    {
      if (ACE_OS::strcmp (OPTIONS::instance ()->order (), ACE_TEXT ("FIFO")) == 0)
        {
          if (this->url_queue_.enqueue_tail (command) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"), ACE_TEXT ("enqueue_tail")),
                              - 1);
        }
      if (ACE_OS::strcmp (OPTIONS::instance ()->order (), ACE_TEXT ("LIFO")) == 0)
        {
          if (this->url_queue_.enqueue_head (command) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"), ACE_TEXT ("enqueue_head")),
                              - 1);
        }
    }
  return 0;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, Options, ACE_Null_Mutex);
