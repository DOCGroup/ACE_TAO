// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO::Upcall_Wrapper::Upcall_Wrapper (TAO::Argument * args[],
                                     size_t nargs,
                                     TAO_ServerRequest & server_request,
                                     TAO::Upcall_Command & command,
                                     void * servant_upcall,
                                     PortableServer::ServantBase * servant,
                                     CORBA::TypeCode_ptr * exceptions,
                                     size_t nexceptions)
  : args_ (args)
  , nargs_ (nargs)
  , server_request_ (server_request)
  , upcall_command_ (command)
  , request_info_ (args,
                   nargs,
                   server_request,
                   servant_upcall,
                   servant,
                   exceptions,
                   nexceptions)
  , interceptor_adapter_ (server_request)
{
}
