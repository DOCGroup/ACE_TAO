#ifndef Thread_Action_H
#define Thread_Action_H

#include "tao/RTScheduling/RTScheduler.h"
#include "ace/Log_Msg.h"
#include <cstring>
#include <string>

class Data
{
 public:
  std::string data;
  RTScheduling::Current_var current;
};

class Test_Thread_Action:
public RTScheduling::ThreadAction
{
 public:
  Test_Thread_Action () = default;

  void _cxx_do (CORBA::VoidData data) override;
};

#endif /*Thread_Action_H*/

void
Test_Thread_Action::_cxx_do (CORBA::VoidData data)
{
  Data * const tmp = static_cast<Data *> (data);
  ACE_DEBUG ((LM_DEBUG,
              "Test_Thread_Action::do %C\n",
              tmp->data.c_str ()));

  size_t count {};
  RTScheduling::Current::IdType_var const id = tmp->current->id ();
  std::memcpy (&count,
               id->get_buffer (),
               id->length ());

  ACE_DEBUG ((LM_DEBUG,
              "The spawned DT Guid is %B\n",
              count));

  RTScheduling::Current::NameList_var segment_name_list =
    tmp->current->current_scheduling_segment_names ();

  ACE_DEBUG ((LM_DEBUG,
              "Segment Names for DT %B:\n",
              count));

  for (CORBA::ULong i = 0; i < segment_name_list->length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C\n",
                  (*segment_name_list)[i].in ()));
    }
}
