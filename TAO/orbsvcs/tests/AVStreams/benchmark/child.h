// $Id$

#if !defined (TAO_AV_BENCH_CHILD_H)
#define TAO_AV_BENCH_CHILD_H

#include "ace/Get_Opt.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"


class Bench_Server_StreamEndPoint :
  public virtual TAO_Server_StreamEndPoint
{
public:
  virtual int handle_open (void) ;
  // called when streamendpoint is instantiated

  virtual int handle_close (void) ;
  // called when streamendpoint is being destructed

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env) ;
  // Application needs to define this
  
  virtual int handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env) ;
  // Application needs to define this

  
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env) ;
  // Application needs to define this

  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec,  
                                                      CORBA::Environment &env) ;
};


#endif /* TAO_AV_BENCH_CHILD_H */
