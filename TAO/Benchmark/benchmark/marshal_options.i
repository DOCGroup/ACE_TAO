// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark/Marshal
// 
// = FILENAME
//    marshal_options.i
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

// constructor
ACE_INLINE
CORBA_Marshal_Options::CORBA_Marshal_Options (void)
  : object_name_ (0),
    data_type_ (CORBA_Marshal_Options::NO_DATATYPE),
    proxy_type_ (CORBA_Marshal_Options::NO_PROXY),
    policy_ (CORBA_Marshal_Options::NO_POLICY),
    role_ (CORBA_Marshal_Options::NO_ROLE),
    rcv_socket_queue_size_ (-1),
    snd_socket_queue_size_ (-1),
    disable_nagle_ (0),
    debug_ (0),
    hostname_ (0),
    port_ (-1),
    iterations_ (0)
{
}

    
// destructor
ACE_INLINE
CORBA_Marshal_Options::~CORBA_Marshal_Options (void)
{
}

// set the data type to be transferred
ACE_INLINE void
CORBA_Marshal_Options::data_type (CORBA_Marshal_Options::DATATYPE dt)
{
  data_type_ = dt;
}

// get the data type to be transferred
ACE_INLINE CORBA_Marshal_Options::DATATYPE
CORBA_Marshal_Options::data_type (void)
{
  return data_type_;
}

// set the proxy type to be transferred
ACE_INLINE void
CORBA_Marshal_Options::proxy_type (CORBA_Marshal_Options::PROXY_TYPE pt)
{
  proxy_type_ = pt;
}

// get the data type to be transferred
ACE_INLINE CORBA_Marshal_Options::PROXY_TYPE
CORBA_Marshal_Options::proxy_type (void)
{
  return proxy_type_;
}

// set the policy type to be transferred
ACE_INLINE void
CORBA_Marshal_Options::policy (CORBA_Marshal_Options::POLICY inv)
{
  policy_ = inv;
}

// get the policy type to be transferred
ACE_INLINE CORBA_Marshal_Options::POLICY
CORBA_Marshal_Options::policy (void)
{
  return policy_;
}

// set the role type to be transferred
ACE_INLINE void
CORBA_Marshal_Options::role (CORBA_Marshal_Options::ROLE my_role)
{
  role_ = my_role;
}

// get the role type to be transferred
ACE_INLINE CORBA_Marshal_Options::ROLE
CORBA_Marshal_Options::role (void)
{
  return role_;
}

// set the hostname
ACE_INLINE void
CORBA_Marshal_Options::hostname (const char *hname)
{
  hostname_ = hname;
}

// get the hostname
ACE_INLINE const char*
CORBA_Marshal_Options::hostname (void)
{
  return hostname_;
}

// set the post
ACE_INLINE void
CORBA_Marshal_Options::port (short portnum)
{
  port_ = portnum;
}

// get the port
ACE_INLINE short
CORBA_Marshal_Options::port (void)
{
  return port_;
}

// set the object name
ACE_INLINE void
CORBA_Marshal_Options::object_name (const char *objname)
{
  object_name_ = objname;
}

// get the object name
ACE_INLINE const char *
CORBA_Marshal_Options::object_name (void)
{
  return object_name_;
}

// set the service name
ACE_INLINE void
CORBA_Marshal_Options::service_name (const char *srvname)
{
  service_name_ = srvname;
}

// get the service name
ACE_INLINE const char *
CORBA_Marshal_Options::service_name (void)
{
  return service_name_;
}

// set the receive sock size
ACE_INLINE void
CORBA_Marshal_Options::rcv_socket_queue_size (int socksize)
{
  rcv_socket_queue_size_ = socksize;
}

// get the receive sock size
ACE_INLINE int
CORBA_Marshal_Options::rcv_socket_queue_size (void)
{
  return rcv_socket_queue_size_;
}

// set the sender sock size
ACE_INLINE void
CORBA_Marshal_Options::snd_socket_queue_size (int socksize)
{
  snd_socket_queue_size_ = socksize;
}

// get the snd sock size
ACE_INLINE int
CORBA_Marshal_Options::snd_socket_queue_size (void)
{
  return snd_socket_queue_size_;
}

// set whether Nagle algo is to be disabled
ACE_INLINE void
CORBA_Marshal_Options::disable_nagle (unsigned short val)
{
  disable_nagle_ = (val ? 1 : 0);
}

// get whether Nagle algo is disabled
ACE_INLINE unsigned short
CORBA_Marshal_Options::disable_nagle (void)
{
  return disable_nagle_;
}

// set whether debug is to be disabled
ACE_INLINE void
CORBA_Marshal_Options::debug (unsigned short val)
{
  debug_ = (val ? 1 : 0);
}

// get whether debug is disabled
ACE_INLINE unsigned short
CORBA_Marshal_Options::debug (void)
{
  return debug_;
}

// set the filename
ACE_INLINE void
CORBA_Marshal_Options::filename (const char *fname)
{
  filename_ = fname;
}

// get the filename
ACE_INLINE const char*
CORBA_Marshal_Options::filename (void)
{
  return filename_;
}

// set the iterations
ACE_INLINE void
CORBA_Marshal_Options::iterations (int iter)
{
  iterations_ = iter;
}

// get the iterations
ACE_INLINE int
CORBA_Marshal_Options::iterations (void)
{
  return iterations_;
}
