// $Id$
#ifndef SERVANTLIST_T_H
#define SERVANTLIST_T_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/Vector_T.h"


// The T type is a concrete servant type.
template <class T>
class ServantList
{
  public:

    typedef typename T::_stub_type     T_stub;
    typedef typename T::_stub_ptr_type T_stub_ptr;
    typedef typename T::_stub_var_type T_stub_var;

    ServantList();
    ~ServantList();

    /// Activate servant and output ior to a file.
    void create_and_activate(unsigned                num_servants,
                             CORBA::ORB_ptr          orb,
                             PortableServer::POA_ptr poa,
                             const ACE_TCHAR*        ior_fname_prefix);

    /// Activate servant and not output ior to a file.
    void create_and_activate(unsigned                num_servants,
                             PortableServer::POA_ptr poa);

    // Code not used and not working right now - possible future addition.
    //void deactivate(PortableServer::POA_ptr poa);

    /// Get a (copy) of one of the object references (for a specific servant).
    T_stub_ptr objref(unsigned index);
    /// This doesn't return a copy.
    T* servant(unsigned index);


  private:

    struct ServantRecord
    {
      T*                              servant_;
      PortableServer::ServantBase_var safe_servant_;
      T_stub_var                      obj_;
    };

    typedef ACE_Vector<ServantRecord> ServantRecordVector;

    ServantRecordVector servant_records_;
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ServantList_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ServantList_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* SERVANTLIST_T_H */

