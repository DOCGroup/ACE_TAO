// $Id$
#include "ServantList_T.h"
#include "AppHelper.h"
#include "TestInf/TestAppExceptionC.h"


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
                                    const ACE_TCHAR*        ior_fname_prefix)
{
  for (unsigned i = 0; i < num_servants; i++)
    {
      ACE_TCHAR buf[32];
      ACE_OS::sprintf(buf, ACE_TEXT("%02d"), i + 1);
      ACE_TString filename = ACE_TString(ior_fname_prefix) + ACE_TEXT("_") + buf + ACE_TEXT(".ior");
      ServantRecord record;
      record.servant_ = new T();
      record.safe_servant_ = record.servant_;

      CORBA::Object_var obj
        = AppHelper::activate_servant(poa,
                                      record.safe_servant_.in());

      AppHelper::ref_to_file(orb, obj.in(), filename.c_str());

      record.obj_ = T_stub::_narrow(obj.in());

      if (CORBA::is_nil(record.obj_.in()))
        {
          throw TestAppException();
        }

      this->servant_records_.push_back(record);
    }
}


template <typename T>
void
ServantList<T>::create_and_activate(unsigned                num_servants,
                                    PortableServer::POA_ptr poa)
{
  for (unsigned i = 0; i < num_servants; i++)
    {
      ServantRecord record;
      record.servant_ = new T();
      record.safe_servant_ = record.servant_;

      CORBA::Object_var obj
        = AppHelper::activate_servant(poa,
                                      record.safe_servant_.in());

      record.obj_ = T_stub::_narrow(obj.in());

      if (CORBA::is_nil(record.obj_.in()))
        {
          throw TestAppException();
        }

      this->servant_records_.push_back(record);
    }
}


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
