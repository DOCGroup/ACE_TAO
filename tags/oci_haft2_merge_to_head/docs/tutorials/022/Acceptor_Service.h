//$Id$

#ifndef ACCEPTOR_SERVICE_H
#define ACCEPTOR_SERVICE_H
#include /**/ "ace/pre.h"

/* The ACE_Acceptor<> template lives in the ace/Acceptor.h header
   file. */
#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* Since we want to work with sockets, we'll need a SOCK_Acceptor to
   allow the clients to connect to us.  */
#include "ace/SOCK_Acceptor.h"

/* Pull in definition for ACE_Svc_Export.  On Win32 platforms
   this macro is used to export symbols from libraries. */
#include "ace/svc_export.h"

/* The Client_Handler object we develop will be used to handle clients
   once they're connected.  The ACE_Acceptor<> template's first
   parameter requires such an object.  In some cases, you can get by
   with just a forward declaration on the class, in others you have to
   have the whole thing.  */
#include "client_handler.h"

/* In our original simple server, we instantiated a
   ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR> object. We can
   make it much simpler and efficient by inheriting our
   Acceptor_Service from ACE_Acceptor itself.

   Our Acceptor_Service class also needs to inherit from
   ACE_Service_Object. ACE_Service_Object is an abstract class which
   includes methods called by the Service Configurator framework to
   start, remove, suspend or resume our service.

   You might have noticed that we didnt explicitly inherit from
   ACE_Service_Object here. That is because, ACE_Acceptor derives from
   ACE_Service_Object and hence there is no need to specify it again. */

/* ACE_Svc_Export is a macro which on Win32 platforms exports from the
   generated library the symbols for the following class.  This allows
   you to link to the library, and get the symbol definitions for the class.
*/


class ACE_Svc_Export Acceptor_Service : public ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR>
{
 public:
  // Constructor
  Acceptor_Service (void);

  // Destructor
  ~Acceptor_Service (void);

  /* This method is the one which is called by the Service
     Configurator Framework to initialize or start the service. */
  virtual int init (int argc, char *argv[]);

  /* Called by the Service Configurator framework to remove this
     Service. */
  virtual int fini (void);

  /* You could easily guess that this method is called to suspend the
     service by the same Service Configurator Framework. When in the
     suspend mode, the service is not removed completely and is *still
     there*. The difference is that the service is not in a active
     state and doesnot accept requests.*/
  virtual int suspend (void);

  /* And your guess that this method is called to resume the service
     is also not wrong. This call brings the service back to the
     active state and the service is all ready to accept requests */
  virtual int resume (void);

};

/* The following macros and similar macros which we will use in the
   implementation file later are used to define helper functions for
   the Service Configurator. As we can easily guess, these macros are
   intended to dynamically load ancd configure services using the
   svc.conf file. These macros will also help to dynamically configure
   even the statically linked services. */

/* This macro is used to declare a data structure required to register a
   statically linked service into the service configurator. As you can
   see it has only one argument which is the name of the class that
   implements this service... so Acceptor_Service in our case. */
ACE_STATIC_SVC_DECLARE (Acceptor_Service)

/*  Once the service implementation is dynamically loaded, the Service
    Configurator uses a factory method to create the object. This
    macro declares such a factory function with the proper interface
    and export macros. */
ACE_SVC_FACTORY_DECLARE (Acceptor_Service)

#include /**/ "ace/post.h"
#endif /* ACCEPTOR_SERVICE_H */
