// $Id$

#include "JAWS/HTTP_10.h"

JAWS_HTTP_IO_Handler::JAWS_HTTP_IO_Handler (JAWS_IO *io)
  : io_(io)
{
}

JAWS_HTTP_IO_Handler::~JAWS_HTTP_IO_Handler (void)
{
}

void
JAWS_HTTP_IO_Handler::accept_complete (void)
{
}

void
JAWS_HTTP_IO_Handler::accept_error (void)
{
}

void
JAWS_HTTP_IO_Handler::read_complete (ACE_Message_Block &data)
{
}

void
JAWS_HTTP_IO_Handler::read_error (void)
{
}

void
JAWS_HTTP_IO_Handler::transmit_file_complete (void)
{
}

void
JAWS_HTTP_IO_Handler::transmit_file_error (int result)
{
}

void
JAWS_HTTP_IO_Handler::receive_file_complete (void)
{
}

void
JAWS_HTTP_IO_Handler::receive_file_error (int result)
{
}

void
JAWS_HTTP_IO_Handler::write_error (void)
{
}

void
JAWS_HTTP_IO_Handler::confirmation_message_complete (void)
{
}

void
JAWS_HTTP_IO_Handler::error_message_complete (void)
{
}

