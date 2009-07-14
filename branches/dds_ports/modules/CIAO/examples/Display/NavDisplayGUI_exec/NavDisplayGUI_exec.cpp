// $Id$

#include "NavDisplayGUI_exec.h"
#include <Qt/qapplication.h>
#include <Qt/qevent.h>

#include "UpdatePositionCmd.h"
#include "QuitCmd.h"
#include "AddNavUnitCmd.h"
#include "Worker.h"

static const char *argv[] =
{
  "NavDisplayGUI"
};

/// Default constructor.
MyImpl::NavDisplayGUI_exec_impl::NavDisplayGUI_exec_impl (void)
  : unit_(1, "Model T3+"), loc_(50, 20, 0)
{
}

/// Default destructor.
MyImpl::NavDisplayGUI_exec_impl::~NavDisplayGUI_exec_impl (void)
{
}

// Operations from HUDisplay::NavDisplay

void
MyImpl::NavDisplayGUI_exec_impl::push_Refresh (
  HUDisplay::tick * /* ev */)
{
  //ACE_DEBUG ((LM_DEBUG, "ENTER: MyImpl::NavDisplayGUI_exec_impl::push_Refresh()\n"));

  // Refresh position
  HUDisplay::position_var loc =
    this->context_->get_connection_GPSLocation ();

  CORBA::Long lx = loc->posx ();

  CORBA::Long ly = loc->posy ();

  mutex_.acquire ();

  loc_.x_ = lx % 500;
  loc_.y_ = ly % 300;

  this->unit_.setLocation (loc_);

  mutex_.release ();

  //ACE_DEBUG ((LM_DEBUG, "DISPLAY: Current Location is: %d %d\n", int(attrs.x_), int(attrs.y_)));

  RootPanel *root_pane = worker_->getMainWindow ();

  if (root_pane != 0)
    {
      UpdatePositionCmd *cmd = UpdatePositionCmd::create (
        root_pane,
        &(this->unit_));
      QCustomEvent *evt = new QCustomEvent (QEvent::User, cmd);
      QApplication::postEvent (root_pane, evt);
    }

  //ACE_DEBUG ((LM_DEBUG, "LEAVE: MyImpl::NavDisplayGUI_exec_impl::push_Refresh()\n"));
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplayGUI_exec_impl::set_session_context (Components::SessionContext_ptr ctx)
{
  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::NavDisplayGUI_exec_impl::configuration_complete (void)
{
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_activate (void)
{
  worker_ = new Worker (sizeof (argv) / sizeof (argv[0]),
                        const_cast<char **> (argv));

  if (worker_->activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    {
      ACE_DEBUG((LM_ERROR,
                 "Cannot activate client threads\n"));
      throw Components::CCMException ();
    }

  worker_->waitUntillInitialized ();

  AddNavUnitCmd *cmd = AddNavUnitCmd::create (
    worker_->getMainWindow(),
    &(this->unit_));
  QCustomEvent *evt = new QCustomEvent (QEvent::User, cmd);
  QApplication::postEvent (worker_->getMainWindow (), evt);
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_passivate (void)
{
  RootPanel *target = worker_->getMainWindow ();

  if (target != 0)
    {
      QuitCmd *cmd = QuitCmd::create( target);
      QCustomEvent *evt = new QCustomEvent (QEvent::User, cmd);
      QApplication::postEvent (target, evt);
      worker_->thr_mgr ()->wait ();
    }

  delete worker_;
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_remove (void)
{
}

/// Default ctor.
MyImpl::NavDisplayGUIHome_exec_impl::NavDisplayGUIHome_exec_impl (void)
{
}

/// Default dtor.
MyImpl::NavDisplayGUIHome_exec_impl::~NavDisplayGUIHome_exec_impl (void)
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::NavDisplayGUIHome_exec_impl::create (void)
{
  return new MyImpl::NavDisplayGUI_exec_impl;
}


extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void)
{
  return new MyImpl::NavDisplayGUIHome_exec_impl;
}
