
// $Id$

#if !defined (TAO_AV_BENCH_CLIENT_H)
#define TAO_AV_BENCH_CLIENT_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/ARGV.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Connector.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/Synch.h"


class Client_StreamEndPoint
  : public virtual TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines a client StreamEndpoint class
  // = DESCRIPTION
  //    Makes use of a connected dgram to receive data from the server
  //    side stream endpoint.
public:
  Client_StreamEndPoint (void);

  virtual int handle_open (void);
  // called when streamendpoint is instantiated

  virtual int handle_close (void);
  // called when streamendpoint is being destructed

  virtual CORBA::Boolean handle_preconnect (AVStreams::flowSpec &the_spec);
  // called before connecting

  virtual CORBA::Boolean handle_postconnect (AVStreams::flowSpec &the_spec);
  // called after connecting
  
  virtual int handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env) ;

  virtual int handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env) ;
  
  virtual int handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);
  
};


class Client : public ACE_Task<ACE_SYNCH>
// one of these per client thread
{
public:
  Client (int argc, char **argv, ACE_Barrier *barrier);

  virtual int svc (void);

private:
  
  int bind_to_server (void);

  int establish_stream (void);


  TAO_ORB_Manager orb_manager_;

  TAO_AV_Endpoint_Reactive_Strategy_A<Client_StreamEndPoint, TAO_VDev,
    AV_Null_MediaCtrl> reactive_strategy_;

  AVStreams::MMDevice_var server_mmdevice_;

  TAO_MMDevice client_mmdevice_;

  TAO_StreamCtrl streamctrl_;
  // Video stream controller 

  int argc_;

  char **argv_;
  
  ACE_Barrier *barrier_;

};

#endif /* TAO_AV_BENCH_CLIENT_H */
