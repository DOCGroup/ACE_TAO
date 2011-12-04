// $Id$

#include "tao/Protocols_Hooks.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IIOP_Protocol_Properties::TAO_IIOP_Protocol_Properties (void)
  : send_buffer_size_ (0),
    recv_buffer_size_ (0),
    keep_alive_ (0),
    dont_route_ (0),
    no_delay_ (0),
    enable_network_priority_ (0),
    hop_limit_ (-1)
{
}

TAO_UIOP_Protocol_Properties::TAO_UIOP_Protocol_Properties (void)
  : send_buffer_size_ (0),
    recv_buffer_size_ (0)
{
}

TAO_SHMIOP_Protocol_Properties::TAO_SHMIOP_Protocol_Properties (void)
  : send_buffer_size_ (0),
    recv_buffer_size_ (0),
    keep_alive_ (0),
    dont_route_ (0),
    no_delay_ (0),
    preallocate_buffer_size_ (0),
    mmap_filename_ (),
    mmap_lockname_ ()
{
}

TAO_DIOP_Protocol_Properties::TAO_DIOP_Protocol_Properties (void)
  : send_buffer_size_ (0),
    recv_buffer_size_ (0),
    enable_network_priority_ (0),
    hop_limit_ (-1),
    enable_multicast_loop_ (1)
{
}

TAO_SCIOP_Protocol_Properties::TAO_SCIOP_Protocol_Properties (void)
  : send_buffer_size_ (0),
    recv_buffer_size_ (0),
    keep_alive_ (0),
    dont_route_ (0),
    no_delay_ (0),
    enable_network_priority_ (0),
    hop_limit_ (-1)
{
}

TAO_Protocols_Hooks::~TAO_Protocols_Hooks (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
