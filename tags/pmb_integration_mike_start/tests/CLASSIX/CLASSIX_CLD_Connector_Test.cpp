/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    CLD_Connector.cpp
//
// = DESCRIPTION
//     Based on $ACE_ROOT/tests/MT_SOCK.cpp
//     
//     This is a multi-threaded torture test of the ACE_CLASSIX_CLD_Connector
//     class.
//
//     The test spawns a server and multiple clients allowing clients to 
//     exchange data with the server.
//
//     This example demonstrates the following use cases
//     - server is a subclass of ACE_Svc_Handler,
//     but it does not  involve an acceptor class.
//     - server multicasts message to the clients
//     - client uses ACE_CLASSIX_Connector to "connect" to the server
//     
// ============================================================================

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"
#include "ace/Svc_Handler.h"

#include "ace/CLASSIX/CLASSIX_Select_Reactor.h"
#include "ace/CLASSIX/CLASSIX_Stream.h"
#include "ace/CLASSIX/CLASSIX_Dgram_Mcast.h"
#include "ace/CLASSIX/CLASSIX_Group_Stamp.h"
#include "ace/CLASSIX/CLASSIX_CLD_Connector.h"

#include "CLASSIX_test_config.h"

#define MAX_TEST_CLIENTS 30
#define TEST_STAMP 300
#define ACE_CLASSIX_MCAST_DGRAM ACE_CLASSIX_Dgram_Mcast, ACE_CLASSIX_Group

static ACE_Atomic_Op<ACE_Thread_Mutex, u_int> client_id = 0;


struct client_arg
{
    ACE_Barrier           *wait;
    ACE_CLASSIX_Port_Core *server;
};

struct client_data
{
    u_int me;
    char c;
};

class server_handler : public ACE_Svc_Handler<ACE_CLASSIX_MCAST_DGRAM,
		       ACE_MT_SYNCH>
{
public:
    server_handler(int /* stamp */, const ACE_CLASSIX_Port_Core&);
    // port that the server uses to receive data from all sorts of clients
    virtual int open (void * = 0 /* args */);
    virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
    virtual int svc (void);
	
    // = Demultiplexing hooks for Reactor
    virtual int handle_input (ACE_HANDLE);
    // The first int of each message identifies the sender

private:
    ACE_Atomic_Op<ACE_Thread_Mutex, u_int> in_svc_;
    // 1 if svc() is running
};

server_handler::server_handler(int theStamp,
			       const ACE_CLASSIX_Port_Core & thePort)
    : ACE_Svc_Handler<ACE_CLASSIX_MCAST_DGRAM, ACE_MT_SYNCH> (),
      in_svc_ (0)
{
    if (this->peer().set_saps(theStamp, thePort) != 0)
	ACE_DEBUG((LM_DEBUG, "failed to set up IO stream \n"));
}

int
server_handler::open (void *)
{
    ACE_DEBUG((LM_DEBUG, "server_handler::open()\n"));
    if (reactor ()->register_handler (this, READ_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%t)server_handler:: cannot register handler\n"),
			  -1);

    if (this->peer().selectable() != 0)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) failed to make Server's port %d selectable\n", 
		get_handle ()));
    else
	ACE_DEBUG ((LM_DEBUG, "(%t) created svc_handler for handle %d\n", 
		    get_handle ()));

    if (this->peer().control(K_BROADMODE) != 0)
	ACE_DEBUG((LM_DEBUG, "(%t) %p\n"
		   "server cannot send data to destination\n"));

    return this->activate (THR_BOUND);
}

int
server_handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
    while (this->in_svc_ == 1)
    {
	ACE_DEBUG ((LM_DEBUG,  "(%t) server is closing down\n"));

	// Use a blank message to tell the thread to stop 
	ACE_Message_Block *mb = new ACE_Message_Block((size_t) 0);
	ACE_Time_Value timeout(0, 10*1000); // wait for at most 10 usec
	this->putq(mb, &timeout);
	ACE_OS::thr_yield();
    }

    this->destroy();
}

int
server_handler::handle_input(ACE_HANDLE)
{
    // Get input as fast as it can to free the reactor to handle other work.
    size_t n = 0;
    if (ACE_Reactor::instance()->
	current_info(this->get_handle(),n) == -1)
	ACE_ERROR_RETURN((LM_ERROR, "???(%t) failed to get input size\n"), -1);

    ACE_Message_Block *msg;
    ACE_NEW_RETURN(msg, ACE_Message_Block(n == 0 ? 1 : n), -1);
    if (this->peer().recv(msg->wr_ptr(), n) == -1)
	ACE_ERROR_RETURN((LM_ERROR, "???(%t) %p\n", "get_data()"), -1);
    msg->wr_ptr(n);


    if (this->putq(msg) == -1)
	ACE_ERROR_RETURN((LM_ERROR, "???(%t) failed to enqueue message\n"), 
			 -1);
    return 0;
}

int
server_handler::svc(void)
{
    this->peer().open_writer();
    this->in_svc_ = 1;

    ACE_Message_Block *mb = 0;
    int result = 0;
    char *storage[MAX_TEST_CLIENTS];

    // initialize the expected result per client
    for (int i = 0; i < MAX_TEST_CLIENTS; i++)
	storage[i] = ACE_ALPHABET;

    int len = sizeof (client_data);
    int total_clients = MAX_TEST_CLIENTS;

    ACE_DEBUG((LM_DEBUG, "(%t)server is waiting for clients\n"));
    // read input
    for (;;)
    {
	// wait until a message has arrived
	result = this->getq (mb);

	if (result == -1)
	{
	    ACE_ERROR_RETURN((LM_ERROR, 
			      "???(%t), error while waiting for a message "
			      "on the queue\n"), 0);
	}

	int length = mb->length ();

	u_int client = 0;
	if (length != 0 && length  == len)
	{
	    client = *((u_int*) mb->rd_ptr ());
	    mb->rd_ptr(sizeof (u_int));
	    char* data = storage[client];
	    // Check if the client has done
	    if (*(mb->rd_ptr()) == '\0')
	    {		
		ACE_DEBUG((LM_DEBUG, "handshake with client %d\n", client));
		client_data response;
		response.me = client;
		int r = this->peer().send_n(&response, len);
		if (r != len)
		    ACE_ERROR((LM_ERROR, 
			       "(%t):%d %p\n",r,
			       "server faided to send handshake msg"));
		total_clients--;
		if (total_clients == 0)
		{
		    mb->release();
		    ACE_DEBUG((LM_DEBUG, "(%t) end event loop \n"));
		    ACE_Reactor::end_event_loop();
		    this->in_svc_ = 0;
		    break;
		}
	    }
	    if (*data != *(mb->rd_ptr()))
		ACE_ERROR((LM_ERROR, "???(%t), invalid input\n"));
	    storage[client] = ++data;
	}
	else if (length > 0)
	{
	    ACE_ERROR((LM_ERROR, "???(%t), invalid input length(%d)\n",
		       length));
	}
	mb->release();

    }

    return 0;
}


static void *
client (void *arg)
{  
    client_data info;
    info.me = client_id++;
    int info_len = sizeof (client_data);
    
    client_arg *data = (client_arg*) arg;
    ACE_CLASSIX_Port_Core *server_port = data->server;
    ACE_CLASSIX_Port server_addr(*server_port);
    ACE_Barrier *barrier = data->wait;
    //===================================================================
    // Stream & Connector
    // 
//    ACE_CLASSIX_Stream cli_stream(*server_port);
    ACE_CLASSIX_Stream cli_stream;
    // create a stream where the local SAP uses the actor's default port.

    ACE_CLASSIX_CLD_Connector con;
    // create a connector for the stream

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Connecting local and peer SAPs\n"));
    // Connect local and peer SAPs.

    barrier->wait();
    //===================================================================
    // Attempt a connect to the server...
    // A local port will be created as a local SAP
    if (con.connect (cli_stream, server_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p\n",
                         "connection failed"),
                        0);
    }
    ACE_CLASSIX_Port client_addr;
    if (cli_stream.local_sap().get_addr (client_addr) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "get_local_addr"), 0);
    else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected client at %d\n",
		    client_addr.get_handle ()));

    
    //===================================================================
    // Insert the local SAP to the test group
    ACE_CLASSIX_Group_Stamp  group(TEST_STAMP);
    // group that the client's port is in
    if (group.insert(&client_addr) == -1)
	ACE_ERROR_RETURN((LM_ERROR, 
			  "Failed to insert local SAP of client %d in to the"
			  "group \n"), -1);

    //===================================================================
    // Do not use Reactor, so disable  local port from being monitored
    int result = cli_stream.unselectable();
    if (result != 0)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "(%P|%t) failed to disable local port(%d)\n", 
			   result), -1);

    //===================================================================
    // Send data to server (correctly handles "incomplete writes").
    char *c = ACE_ALPHABET;

    do
    {
	ACE_OS::thr_yield();
	info.c = *c;
	if (cli_stream.send_n (&info, info_len) != info_len)
	    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));
    }while(*c++ != '\0');

    //===================================================================
    // Close writer
    // ACE_DEBUG ((LM_DEBUG, "(%P|%t) closing writer\n"));
    // Explicitly close the writer-side of the connection.
    //if (cli_stream.close_writer () == -1)
    //	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close_writer"));

    // Wait for handshake with server.
    client_data response;
    do
    {
	if (cli_stream.ipcRecv_n (&response, info_len) != info_len)
	    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "recv_n"));
    }
    while (response.me != info.me);

    ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) received handshake from server\n"));

    // Close the connection completely.
    if (cli_stream.close () == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));

    return 0;
}


static void
spawn (ACE_CLASSIX_Port_Core* theServer)
{
    // create a port for the server
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
		theServer->get_handle ()));

    // activate server
    server_handler handler(TEST_STAMP, *theServer);
    handler.open();		// make the server active

    // activate clients
    // make sure
    // - we don't let client send messages before the event loop is running
    ACE_Barrier wait(MAX_TEST_CLIENTS);
    client_arg  data;
    data.server = theServer;
    data.wait   = &wait;

    ACE_DEBUG ((LM_DEBUG, "(%t) starting clients\n"));
    if (ACE_Thread_Manager::instance ()->spawn_n
	(MAX_TEST_CLIENTS,
	 ACE_THR_FUNC (client),
	 (void *) &data,
	 THR_BOUND | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "spawn failed"));

    // handle event
    ACE_DEBUG((LM_DEBUG, "(%t)run event loop\n"));
    ACE_Reactor::run_event_loop();

    // wait until all server has completed
    ACE_Thread_Manager::instance()->wait_task(&handler);
}

int
main (int, char *[])
{
    ACE_START_TEST ("CLD_Connector_Test");
    
    // initialize classix environment, such as reactor
    ACE_CLASSIX_OS classix;

    // running server and clients
    ACE_CLASSIX_Port_Core server_port;
    spawn(&server_port);


    // Wait all the threads to exit.
    ACE_Thread_Manager::instance ()->wait ();
    
    ACE_END_TEST;
    return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, u_int>;
template class ACE_Svc_Handler<ACE_CLASSIX_MCAST_DGRAM, ACE_MT_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, u_int>
#pragma instantiate ACE_Svc_Handler<ACE_CLASSIX_MCAST_DGRAM, ACE_MT_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


