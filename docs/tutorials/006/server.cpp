// $Id$
  
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Thread.h"
#include "ace/Singleton.h"
#include "ace/Message_Queue.h"

static sig_atomic_t finished = 0;
static ACE_Thread_Manager thr_mgr;
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

extern "C" void handler (int) { finished = 1; }

typedef ACE_Message_Queue<ACE_MT_SYNCH> Message_Queue;
typedef ACE_Singleton<Message_Queue,ACE_Mutex> Safe_Queue;
typedef ACE_Singleton<ACE_Reactor,ACE_Mutex>g_reactor;


#define MAX_HANDLERS 10

class Logging_Handler;

// Used to hold our array of service handler pointers so we can pass the proper
// service handler object to our notify call when we want to alert a reactor 

Logging_Handler *map[MAX_HANDLERS];

// Used to hold the next non-used position in our service handler array
int nextFree = -1;

// Used to denote which service handler thread we are referring to
int whichOne = 0;

// Creates a critical section in the svc() function of the active objects
// so that only one at a time can add itself to the map.
ACE_Mutex addlock_;

class Queue_Derived : public ACE_Message_Queue<ACE_MT_SYNCH>
{
  public :
  Queue_Derived() : ()
  {
    reactor_ = NULL; 
  }
	
  // Sets the reactor for this queue - so we can notify the proper reactor
  // when we need to inform the reactor of an addition to the queue	
  void set_reactor(ACE_Reactor *reactor)
  {
    this->reactor_ = reactor;
  }
	
  // This is the method called by the queue when item added
  virtual int notify(void)
  {
    // Notify the reactor that a message is on our queue ready to be read. Note the 
    // use of the WRITE_MASK flag which tells our reactor to call the handle_output
    // function of our intended service handler(which is the first parameter in argument)

    this->reactor_->notify((ACE_Event_Handler *)map[whichOne],ACE_Event_Handler::WRITE_MASK);

  }

  private :
  ACE_Reactor *reactor_;

};

class Reactor_Derived : public ACE_Reactor
{	
public:
  Reactor_Derived() : ()
  {
    cout << "*****Calling the reactor constructor*****" << (void *)this << endl;
    counter = 0;
  }
	
  virtual ~Reactor_Derived()
  {
    cout << "*****Calling the reactor destructor*****" << (void *)this << endl;
  }

private:
  friend class Logging_Handler;
	
  // counter is used to keep track of the number of service handlers
  // registered with this reactor (Surely theres a better way ;-)
  int counter;
};

class Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{

public:
  //Our queue which is going to hold any messages for this logging handler
  Queue_Derived* local_queue;  
  
  // The reference number of this logging handler object - used when we are deleting
  // this logging handler object from our global list - so we dont try and write
  // to a queue that is non-existent
  int refno = 0;


 
  // Return the pointer to the queue associated with this logging handler object
  Queue_Derived* get_queue(void)
  {
    return(this->local_queue);
  }

  // Basic constructor.  This happens when we are built by the acceptor.  Since
  // that will cause us to activate ourselves, we will have to provide the svc()
  // function and create another of us to handle the actuall connection.
  //

  Logging_Handler (void) 
  {
    cerr << "Constructing logging handler " << (void *)this << endl;
    local_queue = NULL;
    this->reactor(NULL);
  }

  // Provide a copy constructor.  This will allow us to assume control of
  // a connection which was being handled by a brother object.
  //
  Logging_Handler ( Logging_Handler * foo )
  {
    cerr << "Constructing (via pointer) logging handler " << (void *)this << endl;
    this->set_handle( foo->get_handle() );
    local_queue = new Queue_Derived;
  }

  virtual ~Logging_Handler(void)
  {
    cerr << "Destructing logging handler " << (void *)this << endl;
  }

  virtual void destroy(void)
  {
    cerr << "Logging handler destroy " << (void *)this << endl;

    // If we're an active object, we don't have (or need) a pointer
    // to the reactor.  We're stuck inside of our svc() function and
    // never get to do anything else :-(
    //
    // On the other hand, if we're actually registered with a reactor
    // and are doing some work, then we *will* have a pointer to the
    // reactor we're registred with and we'll need to use it when we
    // get destroyed.
    //
    if(reactor())
      {
        // Since we dont want to try and write to a queue that is non-existent, 
        // we have to delete our Logging_Handler reference in our global list
        // of Logging_Handlers...By setting to null we indicate that the Logging Handler 
        // is no longer existent  

        map[this->refno] = NULL; 
	
        // Delete our reference to the queue associated with this Logging Handler
        delete local_queue;
     	
        // Remove the handler from the local reactors table of registered handlers
        if (this->reactor()->remove_handler(this,
                                            ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL)== -1) 
          ACE_ERROR_RETURN ((LM_ERROR, "can'(%P|%t) t remove service from reactor\n"), -1);

        // Decrement the handler tracking variable in the reactor to
        // indicate this service handler has terminated
        --((Reactor_Derived*)reactor())->counter;
      } 

    this->peer ().close ();
    cout << "About to delete" << (void *)this << endl;
    delete this;
  }
  
  virtual int open(void *)
  {
    int foo = 1;

    // Take an arbitrary number 'bar' and activate up to that many
    // objects to provide reactor loops.  On and after the 'bar+1'
    // connection attempt, register all new connections with the
    // same reactor that is handling the bar-th connection.
    // Note that what we have in the map is an array of service
    // handlers that are registered with reactors.  We DO NOT HAVE
    // an array of pointers to the active objects which provide the
    // reactor loop.  A better approach would be to have an array
    // of pointers to reactors.  We could then keep statistics info
    // in the reactor object and access that from here to determine
    // which reactor should have the new connection.

    if( nextFree >= MAX_HANDLERS )
      {
        return -1;
      }
    else
      if ((nextFree <foo) || ! map[foo] )
        {
          this->activate(THR_NEW_LWP);
        }
      else
        { 
	   
          // If we're not going to activate ourselves and we haven't already
          // done so, now is the time to create a local queue.
          if( ! local_queue ) local_queue = new Queue_Derived;

          // Likewise, if we don't have a pointer to a reactor, we should get one.
          if( ! reactor() )reactor( map[foo]->reactor());
		
          if (map[foo]->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,"can'(%P|%t) t register with reactor\n"), -1);
          ++ ((Reactor_Derived*)(map[foo]->reactor()))->counter;
        }
    return 0;	
  }
	

  int svc(void)
  {
    int result;
    Reactor_Derived thread_reactor;     

    cerr << "Thread reactor is " << (void *)&thread_reactor << endl;
          	
    // Create the object we will actually register with the reactor.
    // It will be our duty to provide the typical endless-reactor-loop
    // while we let somebody else have all the fun of handling the connection
    // we were created to deal with.
    //
    Logging_Handler * trueHandler = new Logging_Handler(this);

    // Enter a block of code which adds a reference to this Logging Handler object 
    // into our global table of service handlers
    { 

      // Use the ACE_Gurad class to lock the critical code until the end  bracket
      ACE_Guard <ACE_Mutex> lock (addlock_);	
	   
      //Set our reference to this Logging Handler object in the global array
      map[++nextFree] = trueHandler;	

      trueHandler->refno = nextFree;
    }  
	
    trueHandler->reactor(&thread_reactor);
    trueHandler->local_queue->set_reactor(trueHandler->reactor());

    this->reactor(0);
    this->set_handle( ACE_INVALID_HANDLE );

    // Increment our handler counter to account for this trueHandler
    ++thread_reactor.counter;

	
    if (thread_reactor.register_handler(trueHandler, ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,"can'(%P|%t) t register with reactor\n"), -1);
	 
    while( thread_reactor.counter > 0 )
      {

        // If thread_reactor.counter = 0 then we have no more service
        // handlers connected to the reactor. We set a timeout value
        // of 1 second so that the handle_events loop break out every
        // second to check on the count ( because of it blocking 
        // even when there are no connections we need to do this)
        thread_reactor.handle_events(ACE_Time_Value(1,0));
      }

    cerr << "svc exiting scope " << (void *)this <<endl;
  } 


  virtual int close (u_long)
  {
    this->destroy ();
    return 0;
  }

protected:

  virtual int handle_output(ACE_HANDLE)
  {
				
    ACE_Time_Value timeout(ACE_OS::time(0)+1,0);
    ACE_Message_Block *data_read;
		
    // Read from our queue - waiting for a timeout of 4 seconds(specified in var. timeout)
    if (this->local_queue->dequeue_head(data_read,&timeout) != -1)
      {
        cout << " Received some data! : "  << (void *)data_read->base() << " " << data_read->base() << endl;
        cout << " This data was recevied from queue number : " << this->refno << endl;
        //delete data_read; 
      }
    else
      cout << "No data to dequeue " <<endl; 


             
    return 0;
  }
  
  virtual int handle_input (ACE_HANDLE)
  {
    char buf[128];
    memset(buf,0,sizeof(buf));

    switch( this->peer().recv(buf,sizeof buf) )
      {
      case -1:
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p bad read\n", "client logger"), -1);
      case 0:
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) closing log daemon (fd = %d)\n", this->get_handle ()), -1);
      default:
        cout << "Received  from client : " << buf;
      }
          
	  
    return 0;
  }

};


typedef ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR> Logging_Acceptor;


int main (int argc, char *argv[])
{
  char data_write[60] = "This is a message from the main thread.....!!";

  // Acceptor factory.
  Logging_Acceptor peer_acceptor;

  if (peer_acceptor.open (ACE_INET_Addr ( argc > 1 ? atoi(argv[1]) : PORT)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  else if (g_reactor::instance()->register_handler (&peer_acceptor, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  // Run forever, performing logging service.

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting up server logging daemon\n"));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  
  while ( !finished )
  {
	ACE_Message_Block *data_send = new  ACE_Message_Block(sizeof(data_write),ACE_Message_Block::MB_DATA,0,data_write);

	g_reactor::instance()->handle_events (new ACE_Time_Value(4,0));
	
	
	// Loop through all of the Logging Handler objects in our global array
        // The end is indicated by the variable nextFree

	for (int i=0;i<=nextFree;++i)
	{
	  // If map[i] is equal to NULL then we know that that service handler has
	  // deceased, so we do not want to try and queue a message on its queue 
	  if (map[i] != NULL)
          {		
                // whichOne is a global variable used to determine which service handler
		// to notify of a queue addition, since each service handler will have
                // a seperate queue
		whichOne = i;	

	        // Place our message on the service handler's queue
                if (map[i]->local_queue->enqueue_tail(data_send) == -1)
			cout << "**Error occured while writing to the queue**" << endl;
 	  	else
			cout << "\n Wrote to queue number : !!"<<i<<endl;
          }

	}


  }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down server logging daemon\n"));

  return 0;
}
