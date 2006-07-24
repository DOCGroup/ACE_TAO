//$Id$
#ifndef Thread_Action_H
#define Thread_Action_H

#include "tao/RTScheduling/RTScheduler.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"

class Data
{
 public:
  ACE_CString data;
  RTScheduling::Current_var current;
};

class Test_Thread_Action:
public RTScheduling::ThreadAction
{
 public:
  Test_Thread_Action (void);

  virtual void _cxx_do (CORBA::VoidData data
			ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /*Thread_Action_H*/


Test_Thread_Action::Test_Thread_Action (void)
{
}

void
Test_Thread_Action::_cxx_do (CORBA::VoidData data
			     ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException))
{

  Data* tmp = (Data*) data;
  ACE_DEBUG ((LM_DEBUG,
	      "Test_Thread_Action::do %s\n",
	      tmp->data.c_str ()));

  size_t count = 0;
  ACE_OS::memcpy (&count,
		  tmp->current->id ()->get_buffer (),
		  tmp->current->id ()->length ());

  ACE_DEBUG ((LM_DEBUG,
	      "The spawned DT Guid is %d\n",
	      count));

  RTScheduling::Current::NameList_var segment_name_list =
    tmp->current->current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
	      "Segment Names for DT %d:\n",
	      count));

  for (unsigned int i = 0; i < segment_name_list->length (); i ++)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "%s\n",
      static_cast<char const*>((*segment_name_list) [i])));
    }
}
