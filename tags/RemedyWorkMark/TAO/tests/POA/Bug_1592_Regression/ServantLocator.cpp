// $Id$

#include "ServantLocator.h"
#include "test_i.h"

extern CORBA::Boolean receive_request_service_contexts_called;
extern CORBA::Boolean ending_interception_point_called;

ServantLocator::ServantLocator (CORBA::ORB_ptr orb)
  : error_status_ (false),
    orb_ (CORBA::ORB::_duplicate (orb)),
    servant_ ()
{
}

ServantLocator::~ServantLocator (void)
{
}

PortableServer::Servant
ServantLocator::preinvoke (
    const PortableServer::ObjectId & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    const char * operation,
    PortableServer::ServantLocator::Cookie & /* the_cookie */)
{
  if (ACE_OS::strcmp (operation, "shutdown") != 0)
    {
      if (receive_request_service_contexts_called == 0)
        {
          error_status_ = true;
          ACE_ERROR ((LM_ERROR,
                      "PortableInterceptor::ServerRequestInterceptor:: "
                      "receive_request_service_contexts() not called\n"
                      "prior to "
                      "PortableServer::ServantLocator::preinvoke().\n"));

          throw CORBA::INTERNAL ();
        }

      if (this->servant_.in () == 0)
        {
          test_i * servant;

          ACE_NEW_THROW_EX (servant,
                            test_i (this->orb_.in ()),
                            CORBA::NO_MEMORY ());

          this->servant_ = servant;
        }
    }

  return this->servant_.in ();
}


void
ServantLocator::postinvoke (
    const PortableServer::ObjectId & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    const char * operation,
    PortableServer::ServantLocator::Cookie /* the_cookie */,
    PortableServer::Servant /* the_servant */)
{
  if (ACE_OS::strcmp (operation, "shutdown") != 0)
    {
      // Ending interception points should be called after postinvoke().
      if (::ending_interception_point_called != 0)
        {
          error_status_ = true;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: PortableInterceptor::ServerRequestInterceptor"
                      "\n"
                      "ERROR: ending interception point incorrectly "
                      "called prior to\n"
                      "ERROR: "
                      "PortableServer::ServantLocator::postinvoke().\n"));

          throw CORBA::INTERNAL ();
        }
    }
}
