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

#define TEST_LIST \
  ACTION(null_test) \
  ACTION(pop_returns_element_pushed) \
  ACTION(purge_empty_queue) \
  ACTION(purge_with_no_matches) \
  ACTION(purge_with_single_match) \
  ACTION(purge_with_multiple_matches) \
  ACTION(reset_empty_queue) \
  ACTION(reset_non_empty_queue) \

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
void test_equal(int x, int y, char const * x_msg, char const * y_msg,
                char const * error_message,
                char const * test_name, char const * filename, int lineno);
void test_equal(void * x, void * y, char const * x_msg, char const * y_msg,
                char const * error_message,
                char const * test_name, char const * filename, int lineno);
void test_not_equal(int x, int y, char const * x_msg, char const * y_msg,
                    char const * error_message,
                    char const * test_name, char const * filename, int lineno);
void test_assert(bool predicate, char const * predicate_msg,
                 char const * error_message,
                 char const * test_name, char const * filename, int lineno);

#define TEST_EQUAL(X, Y, MSG) \
  test_equal((X), (Y), #X, #Y, MSG, test_name, __FILE__, __LINE__)
#define TEST_NOT_EQUAL(X, Y, MSG) \
  test_not_equal((X), (Y), #X, #Y, MSG, test_name, __FILE__, __LINE__)
#define TEST_ASSERT(PREDICATE, MESSAGE) \
  test_assert((PREDICATE), #PREDICATE, MESSAGE, test_name, __FILE__, __LINE__)

void null_test(char const * test_name)
{
  ACE_Notification_Queue queue;

  TEST_EQUAL(0, 0, "Test framework failure");
  TEST_NOT_EQUAL(1, 0, "Test framework failure");
  TEST_ASSERT(true, "True is still true");
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
                              ACE_Event_Handler::READ_MASK |
                              ACE_Event_Handler::WRITE_MASK));
  TEST_ASSERT(result == 0, "push[3] should return 0");

  ACE_Notification_Buffer current;
  bool more_messages_queued;
  ACE_Notification_Buffer next;

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[0] should return 1");
  TEST_ASSERT(more_messages_queued, "pop[0] should have more messages");

  TEST_EQUAL(current.eh_, &eh1, "Wrong handler extracted");
  TEST_EQUAL(current.mask_, ACE_Event_Handler::READ_MASK,
             "Wrong mask extracted");

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[1] should return 1");
  TEST_ASSERT(more_messages_queued, "pop[1] should have more messages");

  TEST_EQUAL(current.eh_, &eh2, "Wrong handler extracted");
  TEST_EQUAL(current.mask_, ACE_Event_Handler::WRITE_MASK,
             "Wrong mask extracted");

  result = queue.pop_next_notification(current, more_messages_queued, next);
  TEST_ASSERT(result == 1, "pop[2] should return 1");
  TEST_ASSERT(!more_messages_queued, "pop[2] should not have more messages");

  TEST_EQUAL(current.eh_, &eh3, "Wrong handler extracted");
  TEST_EQUAL(current.mask_, ACE_Event_Handler::READ_MASK |
                            ACE_Event_Handler::WRITE_MASK,
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
                              ACE_Event_Handler::READ_MASK |
                              ACE_Event_Handler::WRITE_MASK));
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
  TEST_EQUAL(result, 1, "purge of eh2/READ should return 1");

  result = queue.purge_pending_notifications(&eh1,
                                             ACE_Event_Handler::READ_MASK);
  TEST_EQUAL(result, 0, "purge of eh1/READ should return 0");
}

void purge_with_multiple_matches(char const * test_name)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);

  int result = queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
                              ACE_Event_Handler::READ_MASK |
                              ACE_Event_Handler::WRITE_MASK));
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
  TEST_EQUAL(result, 3, "purge of eh2/WRITE should return 3");

  result = queue.purge_pending_notifications(&eh1,
                                             ACE_Event_Handler::WRITE_MASK);
  TEST_EQUAL(result, 1, "purge of eh1/WRITE should return 1");
}

void reset_empty_queue(char const * /* test_name */)
{
  ACE_Notification_Queue queue;

  queue.reset();
}

void reset_non_empty_queue(char const * /* test_name */)
{
  ACE_Notification_Queue queue;

  Event_Handler eh1(1);
  Event_Handler eh2(2);

  queue.push_new_notification(
      ACE_Notification_Buffer(0,
                              ACE_Event_Handler::READ_MASK));
  queue.push_new_notification(
      ACE_Notification_Buffer(&eh1,
                              ACE_Event_Handler::READ_MASK));
  queue.push_new_notification(
      ACE_Notification_Buffer(&eh2,
                              ACE_Event_Handler::WRITE_MASK));
  queue.push_new_notification(
      ACE_Notification_Buffer(0,
                              ACE_Event_Handler::WRITE_MASK));

  queue.reset();
}

void test_equal(int x, int y, char const * x_msg, char const * y_msg,
                char const * error_message,
                char const * test_name, char const * filename, int lineno)
{
  if (x == y) return;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("%C in (%C %C:%d) %C (%d) != %C (%d)\n"),
              error_message,
              test_name, filename, lineno,
              x_msg, x, y_msg, y));
}

void test_equal(void * x, void * y, char const * x_msg, char const * y_msg,
                char const * error_message,
                char const * test_name, char const * filename, int lineno)
{
  if (x == y) return;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("%C in (%C %C:%d) %C (%@) != %C (%@)\n"),
              error_message,
              test_name, filename, lineno,
              x_msg, x, y_msg, y));
}

void test_not_equal(int x, int y, char const * x_msg, char const * y_msg,
                    char const * error_message,
                    char const * test_name, char const * filename, int lineno)
{
  if (x != y) return;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("%C in (%C %C:%d) %C (%d) != %C (%d)\n"),
              error_message,
              test_name, filename, lineno,
              x_msg, x, y_msg, y));
}

void test_assert(bool predicate, char const * predicate_msg,
                 char const * error_message,
                 char const * test_name, char const * filename, int lineno)
{
  if (predicate) return;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("Assertion in (%C %C:%d) %C %C\n"),
              test_name, filename, lineno,
              predicate_msg, error_message));
}
