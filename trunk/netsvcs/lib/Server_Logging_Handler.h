#if !defined (ACE_SERVER_LOGGING_HANDLER_H)
#define ACE_SERVER_LOGGING_HANDLER_H

#include "Log_Message_Receiver.h"
#include "Server_Logging_Handler_T.h"

// Type based log message receivers.
typedef Static_Log_Message_Receiver<ACE_NULL_SYNCH> Null_Synch_Static_Receiver;
typedef Static_Log_Message_Receiver<ACE_LOGGER_SYNCH> Synch_Static_Receiver;

typedef ACE_Server_Logging_Handler<Null_Synch_Static_Receiver> Null_Synch_Logging_Handler_Static_Receiver;

typedef ACE_Server_Logging_Handler<Synch_Static_Receiver> Synch_Logging_Handler_Static_Receiver;
typedef ACE_Thr_Server_Logging_Handler<Synch_Static_Receiver> Synch_Thr_Logging_Handler_Static_Receiver;

typedef ACE_Server_Logging_Acceptor_T<Null_Synch_Logging_Handler_Static_Receiver, 
                                     Null_Synch_Static_Receiver, 
                                     ACE_Schedule_All_Reactive_Strategy<Null_Synch_Logging_Handler_Static_Receiver> > 
                  Null_Synch_Logging_Handler_Static_Receiver_Acceptor;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Static_Receiver, 
                                     Synch_Static_Receiver, 
                                     ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Static_Receiver> > 
                  Synch_Logging_Handler_Static_Receiver_Acceptor;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Static_Receiver, 
                                     Synch_Static_Receiver, 
                                     ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Static_Receiver> > 
                  Synch_Thr_Logging_Handler_Static_Receiver_Acceptor;                  

// Instance based log message receivers 
typedef Log_Message_Receiver<ACE_LOGGER_SYNCH> Synch_Receiver;

typedef ACE_Server_Logging_Handler<Synch_Receiver> Synch_Logging_Handler_Receiver;
typedef ACE_Thr_Server_Logging_Handler<Synch_Receiver> Synch_Thr_Logging_Handler_Receiver;
                                 
typedef ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Receiver, 
                                     Synch_Receiver, 
                                     ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Receiver> > 
                  Synch_Logging_Handler_Receiver_Acceptor;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Receiver, 
                                     Synch_Receiver,
                                     ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Receiver> > 
                  Synch_Thr_Logging_Handler_Receiver_Acceptor;                  

// Define external acceptors
typedef  Null_Synch_Logging_Handler_Static_Receiver_Acceptor ACE_Server_Logging_Acceptor;
typedef  Synch_Thr_Logging_Handler_Static_Receiver_Acceptor ACE_Thr_Server_Logging_Acceptor;

//typedef  Synch_Logging_Handler_Receiver_Acceptor ACE_Server_Logging_Acceptor;
//typedef  Synch_Thr_Logging_Handler_Receiver_Acceptor ACE_Thr_Server_Logging_Acceptor;


ACE_SVC_FACTORY_DECLARE (ACE_Server_Logging_Acceptor)
ACE_SVC_FACTORY_DECLARE (ACE_Thr_Server_Logging_Acceptor)

#endif
