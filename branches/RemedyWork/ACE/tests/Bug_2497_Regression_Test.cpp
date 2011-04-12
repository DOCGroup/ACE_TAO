/**
 * @file Bug_2497_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2497
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2497
 *
 * @author sergant128@mail.ru
 */

#include "test_config.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Stream.h"

class Test_Task : public ACE_Task<ACE_SYNCH>
{
public:
  Test_Task( void ) :
    _destructorCalled(0)
  {
  }

  virtual ~Test_Task( void )
  {
    ++_destructorCalled;
    if (_destructorCalled > 1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Test_Task::~Test_Task() should be called once!!!\n")));
  }

private:
  int _destructorCalled;
};


class Test_Module : public ACE_Module<ACE_SYNCH>
{
public:
  Test_Module( void )
    {
      this->open( ACE_TEXT("Test module"),
                  &_writerTask,
                  &_readerTask,
                  0,
                  M_DELETE_NONE );
    }

private:
  Test_Task _writerTask, _readerTask;
};


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2497_Regression_Test"));

  ACE_Stream<ACE_SYNCH> stream;

  if (stream.push(new Test_Module()) == -1)
    {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Error: push failed\n")));
    }

  ACE_END_TEST;

  return 0;
}
