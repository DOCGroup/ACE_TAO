//
// $Id$
//
#include "Reply_Handler.h"

ACE_RCSID(AMI_Buffering, Reply_Handler, "$Id$")

Reply_Handler::Reply_Handler (void)
{
}

void
Reply_Handler::receive_data (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::receive_data_excep (::Messaging::ExceptionHolder *holder)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::sync_excep (::Messaging::ExceptionHolder *holder)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::shutdown_excep (::Messaging::ExceptionHolder *holder)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
