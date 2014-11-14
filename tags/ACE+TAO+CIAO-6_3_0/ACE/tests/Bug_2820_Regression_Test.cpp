/**
 * @file Bug_2820_Regression_Test.cpp
 *
 * $Id$
 *
 * Verify that the event handler reference counting works correctly
 * when the reactor is destroyed.
 *
 * Pushing a notification through the reactor increments the reference
 * count on the target event handler.  Both dispatching and purging
 * the notification decrement the reference count.  However,
 * destroying the reactor used to not decrement the reference count.
 * This test reproduces the problem and serves as a regression for it.
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */

#include "test_config.h"
#include "ace/Auto_Ptr.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"

/**
 * @class Simple_Handler
 *
 * @brief A simple event handler for the test
 *
 */
class Simple_Handler : public ACE_Event_Handler
{
public:
  /// Constructor
  Simple_Handler(ACE_Reactor * reactor);

  /// Destructor
  ~Simple_Handler();

  /// Receive (and ignore) the notifications
  virtual int handle_exception(ACE_HANDLE);
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2820_Regression_Test"));

  int result = 0;

  auto_ptr<ACE_Reactor> reactor(
      new ACE_Reactor(new ACE_Select_Reactor, 1));

#if defined ACE_HAS_CPP11
  ACE_Event_Handler_var v =
    ACE::make_event_handler<Simple_Handler> (reactor.get());
#else
  ACE_Event_Handler_var v(
      new Simple_Handler(reactor.get()));
#endif

  ACE_Event_Handler::Reference_Count pre_notify_count =
    v->add_reference();

  int const notify_count = 4;
  for(int i = 0; i != notify_count; ++i)
    {
      reactor->notify(v.handler());
    }

  ACE_Event_Handler::Reference_Count pos_notify_count =
    v->add_reference();

  if(pos_notify_count != pre_notify_count + notify_count + 1)
    {
      result = -1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Reference count should increase by %d.")
                 ACE_TEXT("  Initial count=%d, final count = %d\n"),
                 notify_count, pre_notify_count, pos_notify_count));
    }

  ACE_auto_ptr_reset(reactor, (ACE_Reactor*)0);

  // Reset the reactor in the event handler, since it is gone.p
  v->reactor(0);

  ACE_Event_Handler::Reference_Count pos_release_count =
    v->add_reference();

  // Only our explicit calls to add_reference() should be reflected in
  // the refence_count...
  if (pos_release_count != pre_notify_count + 2)
    {
      result = -1;
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("Reference count should have increased by 2.")
                 ACE_TEXT("  Initial count=%d, final count = %d\n"),
                 pre_notify_count, pos_release_count));
    }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT("Ref count results.  pre_notify refcount=%d,")
              ACE_TEXT(" pos_notify=%d, pos_delete=%d\n"),
              pre_notify_count, pos_notify_count, pos_release_count));

  // Remove a reference for each time we explicitly increased it.
  v->remove_reference();
  v->remove_reference();
  ACE_Event_Handler::Reference_Count pos_remove_count =
      v->remove_reference();

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT("Ref count results.  pre_notify refcount=%d,")
              ACE_TEXT(" pos_notify=%d, pos_delete=%d, pos_remove=%d\n"),
              pre_notify_count, pos_notify_count, pos_release_count,
              pos_remove_count));

  ACE_END_TEST;

  return result;
}

// ============================================

Simple_Handler::
Simple_Handler(
    ACE_Reactor * r)
  : ACE_Event_Handler(r)
{
  reference_counting_policy().value(
        ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

Simple_Handler::
~Simple_Handler()
{
}

int Simple_Handler::
handle_exception(ACE_HANDLE)
{
  return 0;
}
