#if !defined (ACE_SERVER_LOGGING_HANDLER_H)
#define ACE_SERVER_LOGGING_HANDLER_H

#include "Server_Logging_Handler_T.h"
#include "Log_Message_Receiver.h"

// Type based log message receivers.
typedef Static_Log_Message_Receiver<ACE_NULL_SYNCH> 
        Null_Synch_Static_Receiver;
typedef Static_Log_Message_Receiver<ACE_LOGGER_SYNCH> 
        Synch_Static_Receiver;

typedef ACE_Server_Logging_Handler<Null_Synch_Static_Receiver> 
        Null_Synch_Logging_Handler_Static_Receiver;

typedef ACE_Server_Logging_Handler<Synch_Static_Receiver> 
        Synch_Logging_Handler_Static_Receiver;
typedef ACE_Thr_Server_Logging_Handler<Synch_Static_Receiver> 
        Synch_Thr_Logging_Handler_Static_Receiver;

typedef ACE_Server_Logging_Acceptor_T<Null_Synch_Logging_Handler_Static_Receiver, 
                                     Null_Synch_Static_Receiver, 
				     ACE_Schedule_All_Reactive_Strategy<Null_Synch_Logging_Handler_Static_Receiver> >
        Null_Synch_Logging_Handler_Static_Receiver_Acceptor_T;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Static_Receiver, 
                                     Synch_Static_Receiver, 
                                     ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Static_Receiver> >
        Synch_Logging_Handler_Static_Receiver_Acceptor_T;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Static_Receiver, 
                                     Synch_Static_Receiver, 
                                     ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Static_Receiver> >
        Synch_Thr_Logging_Handler_Static_Receiver_Acceptor_T;                  


// Instance based log message receivers 
typedef Log_Message_Receiver<ACE_LOGGER_SYNCH> 
        Synch_Receiver;

typedef ACE_Server_Logging_Handler<Synch_Receiver> 
        Synch_Logging_Handler_Receiver;
typedef ACE_Thr_Server_Logging_Handler<Synch_Receiver> 
        Synch_Thr_Logging_Handler_Receiver;
                                 
typedef ACE_Server_Logging_Acceptor_T<Synch_Logging_Handler_Receiver, 
                                     Synch_Receiver, 
                                     ACE_Schedule_All_Reactive_Strategy<Synch_Logging_Handler_Receiver> > 
        Synch_Logging_Handler_Receiver_Acceptor_T;                  

typedef ACE_Server_Logging_Acceptor_T<Synch_Thr_Logging_Handler_Receiver, 
                                      Synch_Receiver, 
                                      ACE_Schedule_All_Threaded_Strategy<Synch_Thr_Logging_Handler_Receiver> > 
        Synch_Thr_Logging_Handler_Receiver_Acceptor_T;                  

// Define external Acceptor_Ts
typedef Null_Synch_Logging_Handler_Static_Receiver_Acceptor_T 
        ACE_Server_Logging_Acceptor_T;
typedef Synch_Thr_Logging_Handler_Static_Receiver_Acceptor_T 
        ACE_Thr_Server_Logging_Acceptor_T;

//typedef  Synch_Logging_Handler_Receiver_Acceptor_T ACE_Server_Logging_Acceptor_T;
//typedef  Synch_Thr_Logging_Handler_Receiver_Acceptor_T ACE_Thr_Server_Logging_Acceptor_T;

ACE_SVC_FACTORY_DECLARE (ACE_Server_Logging_Acceptor_T)
ACE_SVC_FACTORY_DECLARE (ACE_Thr_Server_Logging_Acceptor_T)

#endif /* SERVER_LOGGING_HANDLER_H */
