// $Id$

#include "Acceptor_Service.h"
#include "ace/Dynamic_Service.h"

#include "client_handler.h"

/* A server has to listen for clients at a known TCP/IP port.  The
   default ACE port is 10002 (at least on my system) and that's good
   enough for what we want to do here.  Obviously, a more robust
   application would take a command line parameter or read from a
   configuration file or do some other clever thing.  Just like the
   signal handler above, though, that's not what we want to focus on,
   so we're taking the easy way out.  */

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

/* As in all our simple tutorials, our contructor also does nothing */
Acceptor_Service::Acceptor_Service (void)
{
  // Constructor
}

/* Same in the destructor case */
Acceptor_Service::~Acceptor_Service (void)
{
  // Constructor
}

/* This is the virtual method inherited from ACE_Service_Object. This
   method is called to initialize the service. In a generic sense, we
   initialize everything that is needed to initialize our service
   here. Ofcourse we need to do that only if there are not already
   initialized. One important point to note here is that we have to
   make sure that everything that is initialized here is actually
   removed when the service is shutdown */
int
Acceptor_Service::init (int argc, char *argv[])
{
  /* As you will see, we will not be using argc and argv here and
     hence declare them to be unused variables. This helps us from the
     complaints from the picky compilers about unused variables. */
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  /* Lets have a debug statement so that we can know that our
     Acceptor_Service will be initialized soon */
  ACE_DEBUG ((LM_DEBUG,
              "Starting the Acceptor_Service\n"));

  /* Create an ACE_INET_Addr that represents our endpoint of a
    connection. We then open our acceptor object with that Addr.
    Doing so tells the acceptor where to listen for connections.
    Servers generally listen at "well known" addresses.  If not, there
    must be some mechanism by which the client is informed of the
    server's address. */
  if (this->open (ACE_INET_Addr (PORT),
                  ACE_Reactor::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  return 0;
}

/* This virtual method will be invoked when we pass a directive to the
   service configurator framework to remove our service. Remember the
   threads and anything else that are initialized in the init
   method and remove each of them. If we leave anything that we
   initialized still running after this method is invoked ...well .. you
   know what happens :-) */
int
Acceptor_Service::fini (void)
{
  /* Lets have another debug statement to inform us the state of the
     service. */
   ACE_DEBUG ((LM_DEBUG,
              "Closing down the Acceptor_Service\n"));

  /* Now, lets see what did we start or initialize during the
     initialization process. The only thing that we did was opening
     our Acceptor to start listening for requests. So, lets close it
     down. */
  this->close ();

  return 0;
}

/* Now, lets see how we can suspend the service that we initialized
   and is running. By suspension, we mean that the Reactor still knows
   about the service and receives the requests. But, it just keeps
   quite even if there are any requests. It actually queues the
   requests and sends them to the service once it is resumed. */
int
Acceptor_Service::suspend (void)
{
/* You must be wondering, as I did, how we can simply suspend this
   service without any complex method invocations. Thanks to our
   ACE_Reactor class, we can actually suspend the service by just
   invoking the following method and passing a pointer to ourself.
   This method actually takes care of all the particulars for
   suspending the services and keeps us happy. */
  ACE_Reactor::instance ()->suspend_handler (this);
  return 0;
}

int
Acceptor_Service::resume (void)
{
  /* I had the same question again ... how do I do this ?? As before,
     our ACE_Reactor class came to a help with this method. ACE
     classes do make life simpler .. don't they !! */
  ACE_Reactor::instance ()->resume_handler (this);

  return 0;
}

/* The macro to be used to define the factory method and destructor
   for our dynamically loadable service. */
ACE_SVC_FACTORY_DEFINE (Acceptor_Service)

/* This macro helps to register a statically linked service into the
   service configurator. It is defined in ace/OS.h. All the parameters
   needed by the service configurator to build and control the
   statically linked servuce are configured in a single structure. An
   instance of this structure is statically initialized using this
   macro. The First parameter is SERVICE_CLASS i.e. the name of the
   class that implements the service. As we did implicitly, this class
   must derive from ACE_Service_Configurator. The second parameter is
   the NAME of the service. This name is used by the service
   configurator to match the configuration options provided in the
   svc.conf file. The third parameter is the type of the object which
   could be either streams or service objects. The next parameter is
   the name of the factory function which we defined in our header
   file and above using the macros ACE_FACTORY_DECLARE and
   ACE_FACTORY_DEFINE. The fifth parameter are a set of options or
   flags which are used to control the ownership and life cycle of the
   object. The final argument helps us to choose if we want a new
   thread for this service. If the argument is not 0, a thread will be
   dedicated to this service. .. lots of parameters .. Huh !! */  
ACE_STATIC_SVC_DEFINE (Acceptor_Service,
                       ACE_TEXT ("Acceptor_Service"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Acceptor_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
