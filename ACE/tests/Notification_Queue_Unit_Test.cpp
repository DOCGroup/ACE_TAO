/**
 * @file Notification_Queue_Unit_Test.cpp
 *
 * $Id$
 *
 * A unit test for the ACE_Notification_Queue class.
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */

#include "test_config.h"
#include "ace/Notification_Queue.h"

ACE_RCSID(tests,
          Notification_Queue_Unit_Test, "$Id$")

#define TEST_LIST \
  ACTION(null_test) \
  ACTION(pop_returns_element_pushed) \
  ACTION(purge_empty_queue) \
  ACTION(purge_with_no_matches) \
  ACTION(purge_with_single_match) \
  ACTION(purge_with_multiple_matches) \

// Declare all the tests
#define ACTION(TEST_NAME) void TEST_NAME (char const * test_name);
TEST_LIST
#undef ACTION

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Notification_Queue_Unit_Test"));

  // Call all the tests
#define ACTION(TEST_NAME) TEST_NAME (#TEST_NAME);
TEST_LIST
#undef ACTION

  ACE_END_TEST;

  return 0;
}

// There are far more elegant ways to do this.  Ideally one would use
// an existing framework (Boost.Test, TUT, CppTest).  But this will
// do for our purposes
#define TEST_INTEGER_EQUAL(X, Y, MSG) \
  do { \
    if ((X) == (Y)) break; \
    ACE_ERROR ((LM_ERROR, \
		ACE_TEXT("%C in (%C %N:%l) %C (%d) != %C (%d)\n"), \
		ACE_TEXT(MSG), test_name, \
		ACE_TEXT(#X), (X), ACE_TEXT(#Y), (Y) )); \
  } while(0)
#define TEST_INTEGER_NOT_EQUAL(X, Y, MSG) \
  do { \
    if ((X) != (Y)) break; \
    ACE_ERROR ((LM_ERROR, \
		ACE_TEXT("%C in (%C %N:%l) %C (%d) == %C (%d)\n"), \
		ACE_TEXT(MSG), test_name, \
		ACE_TEXT(#X), (X), ACE_TEXT(#Y), (Y) )); \
  } while(0)
#define TEST_ASSERT(PREDICATE, MESSAGE) \
  do { \
    if ((PREDICATE)) break; \
    ACE_ERROR ((LM_ERROR, \
		ACE_TEXT("Assertion failure in (%C %N:%l) %C %C\n"), \
		test_name, ACE_TEXT(#PREDICATE), MESSAGE )); \
  } while(0)


void null_test(char const * test_name)
{
  ACE_Notification_Queue queue;

  TEST_INTEGER_EQUAL(0, 0, "Test framework failure");
  TEST_INTEGER_NOT_EQUAL(1, 0, "Test framework failure");
  TEST_ASSERT(true, ACE_TEXT("True is still true"));
}

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler(int event_handler_id)
    : ACE_Event_Handler()
    , id (event_handler_id)
  {
  }

  int id;
};

void pop_returns_element_pushed(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);
  Event_Handler eh3(2);

  int result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::READ_MASK));
  TEST_ASSERT(result == 1, "push[1] should return 1");

  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));
  TEST_ASSERT(result == 0, "push[2] should return 0");

  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh3,
			      ACE_Event_Handler::READ_MASK
			      |ACE_Event_Handler::WRITE_MASK));
  TEST_ASSERT(result == 0, "push[3] should return 0");

  ACE_Notification_Buffer current;
  bool more_messages_queued;
  ACE_Notification_Buffer next;

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[0] should return 1");
  TEST_ASSERT(more_messages_queued, "pop[0] should have more messages");

  TEST_INTEGER_EQUAL(current.eh_, &eh1, "Wrong handler extracted");
  TEST_INTEGER_EQUAL(current.mask_, ACE_Event_Handler::READ_MASK,
		     "Wrong mask extracted");

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[1] should return 1");
  TEST_ASSERT(more_messages_queued, "pop[1] should have more messages");

  TEST_INTEGER_EQUAL(current.eh_, &eh2, "Wrong handler extracted");
  TEST_INTEGER_EQUAL(current.mask_, ACE_Event_Handler::WRITE_MASK,
		     "Wrong mask extracted");

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[2] should return 1");
  TEST_ASSERT(!more_messages_queued, "pop[2] should not have more messages");

  TEST_INTEGER_EQUAL(current.eh_, &eh3, "Wrong handler extracted");
  TEST_INTEGER_EQUAL(current.mask_, ACE_Event_Handler::READ_MASK
		     |ACE_Event_Handler::WRITE_MASK,
		     "Wrong mask extracted");

  more_messages_queued = true;
  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 0, "pop[3] should return 0");
  TEST_ASSERT(!more_messages_queued, "pop[3] should not have more messages");
}

void purge_empty_queue(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);

  int result = queue.purge_pending_notifications(&eh1,
						 ACE_Event_Handler::READ_MASK);
  TEST_ASSERT(result == 0, "purge of empty queue should return 0");
}

void purge_with_no_matches(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);

  int result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::READ_MASK));

  result = queue.purge_pending_notifications(&eh2,
					     ACE_Event_Handler::READ_MASK);
  TEST_ASSERT(result == 0, "purge of eh2 should return 0");

  result = queue.purge_pending_notifications(&eh1,
					     ACE_Event_Handler::WRITE_MASK);
  TEST_ASSERT(result == 0, "purge of eh1/WRITE should return 0");
}

void purge_with_single_match(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);

  int result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::READ_MASK
			      |ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::READ_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));

  result = queue.purge_pending_notifications(&eh2,
					     ACE_Event_Handler::READ_MASK);
  TEST_INTEGER_EQUAL(result, 1, "purge of eh2/READ should return 1");

  result = queue.purge_pending_notifications(&eh1,
					     ACE_Event_Handler::READ_MASK);
  TEST_INTEGER_EQUAL(result, 0, "purge of eh1/READ should return 0");
}

void purge_with_multiple_matches(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);

  int result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::READ_MASK
			      |ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::READ_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));
  result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
			      ACE_Event_Handler::WRITE_MASK));

  result = queue.purge_pending_notifications(&eh2,
					     ACE_Event_Handler::WRITE_MASK);
  TEST_INTEGER_EQUAL(result, 3, "purge of eh2/WRITE should return 3");

  result = queue.purge_pending_notifications(&eh1,
					     ACE_Event_Handler::WRITE_MASK);
  TEST_INTEGER_EQUAL(result, 1, "purge of eh1/WRITE should return 1");
}

