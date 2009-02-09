// $Id$

#include "UDPTest_i.h"
#include <ace/Log_Msg.h>

unsigned char buf[1000] = {0} ;

UDPTest_i::UDPTest_i () : received_send_ (false)
{
}

UDPTest_i::~UDPTest_i ()
{
}

void UDPTest_i::send( const ::UDPTestI::BufferT Msg )
{
    memcpy( buf, Msg, 1000 ) ;
    memset( buf, 0, 1000 ) ;
    received_send_ = true;
}
