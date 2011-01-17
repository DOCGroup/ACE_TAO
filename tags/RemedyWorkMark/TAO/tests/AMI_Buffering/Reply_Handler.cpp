//
// $Id$
//
#include "Reply_Handler.h"

Reply_Handler::Reply_Handler (void)
{
}

void
Reply_Handler::receive_data (void)
{
}

void
Reply_Handler::receive_data_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("receive_data: ");
    }
}

void
Reply_Handler::sync (void)
{
}

void
Reply_Handler::sync_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("sync: ");
    }
}

void
Reply_Handler::shutdown (void)
{
}

void
Reply_Handler::shutdown_excep (::Messaging::ExceptionHolder *holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("shutdown: ");
    }
}
