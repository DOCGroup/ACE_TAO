//
// $Id$
//

#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual POA_Test::AMH_Roundtrip
  , public virtual PortableServer::RefCountServantBase
{
 public:
    /// Constructor
    Roundtrip (CORBA::ORB_ptr orb);
    
    // = The skeleton methods
    void test_octet_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::octet_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));


    void test_long_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::long_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    void test_short_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::short_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));


    void test_char_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::char_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    void test_longlong_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::longlong_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));


    void test_double_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
		                    const Test::double_load& ol,
                            Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));


    
    void shutdown (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                   ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
    
 private:
    /// Use an ORB reference to conver strings to objects and shutdown
    /// the application.
    CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
