#include "ServantLocator.h"
#include "test_i.h"

#include <cstring>

extern CORBA::Boolean receive_request_service_contexts_called;
extern CORBA::Boolean ending_interception_point_called;

ServantLocator::ServantLocator (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
ServantLocator::preinvoke (
    PortableServer::ObjectId const & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    char const *operation,
    PortableServer::ServantLocator::Cookie & /* the_cookie */)
{
  if (std::strcmp (operation, "shutdown") != 0)
    {
      if (!receive_request_service_contexts_called)
        {
          error_status_ = true;
          ACE_ERROR ((LM_ERROR,
                      "PortableInterceptor::ServerRequestInterceptor:: "
                      "receive_request_service_contexts() not called\n"
                      "prior to "
                      "PortableServer::ServantLocator::preinvoke().\n"));

          throw CORBA::INTERNAL ();
        }

      if (this->servant_.in () == nullptr)
        {
          test_i *servant {};

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
    PortableServer::ObjectId const & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    char const *operation,
    PortableServer::ServantLocator::Cookie /* the_cookie */,
    PortableServer::Servant /* the_servant */)
{
  if (std::strcmp (operation, "shutdown") != 0)
    {
      // Ending interception points should be called after postinvoke().
      if (::ending_interception_point_called)
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
