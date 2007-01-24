// $Id$
#include "ServantList_T.h"
#include "AppHelper.h"
#include "TestAppExceptionC.h"


template <typename T>
ServantList<T>::ServantList()
{
}


template <typename T>
ServantList<T>::~ServantList()
{
}


template <typename T>
void
ServantList<T>::create_and_activate(unsigned                num_servants,
                                    CORBA::ORB_ptr          orb,
                                    PortableServer::POA_ptr poa,
                                    const char*             ior_fname_prefix
                                    ACE_ENV_ARG_DECL)
{
  for (unsigned i = 0; i < num_servants; i++)
    {
      char buf[32];
      ACE_OS::sprintf(buf, "%02d", i + 1);
      ACE_CString filename = ACE_CString(ior_fname_prefix) + "_" + buf + ".ior";
      ServantRecord record;
      record.servant_ = new T();
      record.safe_servant_ = record.servant_;

      CORBA::Object_var obj
        = AppHelper::activate_servant(poa,
                                      record.safe_servant_.in()
                                      ACE_ENV_ARG_PARAMETER);

      AppHelper::ref_to_file(orb, obj.in(), filename.c_str() ACE_ENV_ARG_PARAMETER);

      record.obj_ = T_stub::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil(record.obj_.in()))
        {
          ACE_THROW (TestAppException());
        }

      this->servant_records_.push_back(record);
    }
}


template <typename T>
void
ServantList<T>::create_and_activate(unsigned                num_servants,
                                    PortableServer::POA_ptr poa
                                    ACE_ENV_ARG_DECL)
{
  for (unsigned i = 0; i < num_servants; i++)
    {
      ServantRecord record;
      record.servant_ = new T();
      record.safe_servant_ = record.servant_;

      CORBA::Object_var obj
        = AppHelper::activate_servant(poa,
                                      record.safe_servant_.in()
                                      ACE_ENV_ARG_PARAMETER);

      record.obj_ = T_stub::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil(record.obj_.in()))
        {
          ACE_THROW (TestAppException());
        }

      this->servant_records_.push_back(record);
    }
}


// Code for reference - doesn't work right now
#if 0
template <typename T>
void
ServantList<T>::deactivate(PortableServer::POA_ptr poa)
{
  ServantRecord record;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    PortableServer::ObjectId_var id =
      poa->servant_to_id(record.safe_servant_.in() ACE_ENV_ARG_PARAMETER);

    poa->deactivate_object(id.in() ACE_ENV_ARG_PARAMETER);
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "ServantList_T::deactivate_servant ()");
  }
  ACE_CATCHALL
  {
    ACE_ERROR((LM_ERROR, "(%P|%t)ServantList_T::deactivate_servant "
    "Caught unknown exception \n"));
  }
  ACE_ENDTRY;
}
#endif


template <typename T>
typename ServantList<T>::T_stub_ptr
ServantList<T>::objref(unsigned index)
{
  return T_stub::_duplicate(this->servant_records_[index].obj_.in());
}


template <typename T>
T*
ServantList<T>::servant(unsigned index)
{
  return this->servant_records_[index].servant_;
}
