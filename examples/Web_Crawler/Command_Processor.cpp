 // $Id$

#include "URL.h"
#include "HTTP_URL.h"
#include "Options.h"
#include "Command_Processor.h"
#include "URL_Visitor.h"

ACE_RCSID(HTTP_1.1_Client, Command_Processor, "$Id$")

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

  ACE_CString check_string (this->url_->url_addr ().get_path_name ());
  if (check_string.find ("news:") >= 0)
    return 0;

  if (check_string.find (".cgi") >= 0)
     return 0;

  if (check_string.find ("mailto") >= 0)
    return 0;

  if (check_string.find (".gif") >= 0)
    return 0;
  
  if (check_string.find (".pdf") >= 0)
    return 0;

  if (check_string.find (".map") >= 0)
    return 0;

  if (check_string.find (".bmp") >= 0)
    return 0;
  
  if (check_string.find (".jpg") >= 0)
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
      URL_Command *url_command = ACE_dynamic_cast (URL_Command *,
                                                   command);
      Auto_Destroyer<URL_Command> url_command_ptr (url_command);
      if (url_command_ptr->execute () != 0)
        {
        ACE_DEBUG ((LM_DEBUG,
                    "Couldnt execute command"));
        return -1;
        }
    }
  return 0;
}

int
Command_Processor::insert (Command *command)
{
  // According to the order specified the commands are removed from the queue. 
  if (this->url_queue_.is_full() != 1)
    {
      if (ACE_OS::strcmp (OPTIONS::instance ()->order (),"FIFO") == 0)
        {
          if (this->url_queue_.enqueue_tail (command) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n", "enqueue_tail"),
                              - 1);
        } 
      if (ACE_OS::strcmp (OPTIONS::instance ()->order (),"LIFO") == 0)
        {
          if (this->url_queue_.enqueue_head (command) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n", "enqueue_head"),
                              - 1);
        } 
    }
  return 0; 
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Options, ACE_Null_Mutex>;
template class ACE_Unbounded_Queue<Command *>;
template class ACE_Node<Command *>;
template class ACE_Unbounded_Queue_Iterator<Command *>;
template class ACE_Unbounded_Queue<Command *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#pragma instantiate ACE_Unbounded_Queue<Command *>
#pragma instantiate ACE_Node<Command *> 
#pragma instantiate ACE_Unbounded_Queue_Iterator<Command *>
#pragma instantiate ACE_Unbounded_Queue<Command *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
