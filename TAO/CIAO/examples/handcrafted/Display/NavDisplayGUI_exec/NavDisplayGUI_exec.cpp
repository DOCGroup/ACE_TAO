// $Id$

#include "NavDisplayGUI_exec.h"
#include "CIAO/CIAO_common.h"
#include <qapplication.h>
#include <qevent.h>

#include "UpdatePositionCmd.h"
#include "QuitCmd.h"
#include "AddNavUnitCmd.h"
#include "Worker.h"

static char *argv[] =
{
  "NavDisplayGUI"
};

/// Default constructor.
MyImpl::NavDisplayGUI_exec_impl::NavDisplayGUI_exec_impl ()
: unit_(1, "Model T3+"), loc_(50, 20, 0)
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::NavDisplayGUI_exec_impl ()\n"));
}

/// Default destructor.
MyImpl::NavDisplayGUI_exec_impl::~NavDisplayGUI_exec_impl ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::~NavDisplayGUI_exec_impl ()\n"));
}

// Operations from HUDisplay::NavDisplay

void
MyImpl::NavDisplayGUI_exec_impl::push_Refresh (HUDisplay::tick_ptr ev
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //ACE_DEBUG ((LM_DEBUG, "ENTER: MyImpl::NavDisplayGUI_exec_impl::push_Refresh()\n"));

  // Refresh position
  HUDisplay::position_var loc
    = this->context_->get_connection_GPSLocation (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long lx = loc->posx (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Long ly = loc->posy (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  mutex_.acquire();

  loc_.x_ = lx % 500;
  loc_.y_ = ly % 300;

  this->unit_.setLocation(loc_);

  mutex_.release();

  //ACE_DEBUG ((LM_DEBUG, "DISPLAY: Current Location is: %d %d\n", int(attrs.x_), int(attrs.y_)));

  RootPanel *root_pane = worker_->getMainWindow();
  if(root_pane)
  {
    UpdatePositionCmd *cmd = UpdatePositionCmd::create(
      root_pane, &(this->unit_));
    QCustomEvent *evt = new QCustomEvent(QEvent::User, cmd);
    QApplication::postEvent(root_pane, evt);
  }

  //ACE_DEBUG ((LM_DEBUG, "LEAVE: MyImpl::NavDisplayGUI_exec_impl::push_Refresh()\n"));
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplayGUI_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::set_session_context\n"));

  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "ENTER: MyImpl::NavDisplayGUI_exec_impl::ccm_activate\n"));

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), HUDisplay::tick_init,
                               HUDisplay::tick);

  worker_ = new Worker(sizeof(argv)/sizeof(argv[0]), argv);

  if (worker_->activate(THR_NEW_LWP | THR_JOINABLE, 1) != 0)
  {
    ACE_DEBUG((LM_ERROR,
               "Cannot activate client threads\n"));
    ACE_THROW(Components::CCMException());
  }

  worker_->waitUntillInitialized();

  AddNavUnitCmd *cmd = AddNavUnitCmd::create(
    worker_->getMainWindow(), &(this->unit_));
  QCustomEvent *evt = new QCustomEvent(QEvent::User, cmd);
  QApplication::postEvent(worker_->getMainWindow(), evt);


  ACE_DEBUG ((LM_DEBUG, "LEAVE: MyImpl::NavDisplayGUI_exec_impl::ccm_activate\n"));
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::ccm_passivate\n"));

  RootPanel *target = worker_->getMainWindow();
  if(target)
  {
    QuitCmd *cmd = QuitCmd::create(target);
    QCustomEvent *evt = new QCustomEvent(QEvent::User, cmd);
    QApplication::postEvent(target, evt);
    worker_->thr_mgr()->wait();
  }

  delete worker_;
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::ccm_remove\n"));
}

/// Default ctor.
MyImpl::NavDisplayGUIHome_exec_impl::NavDisplayGUIHome_exec_impl ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::NavDisplayGUIHome_exec_impl ()\n"));
}

/// Default dtor.
MyImpl::NavDisplayGUIHome_exec_impl::~NavDisplayGUIHome_exec_impl ()
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUI_exec_impl::~NavDisplayGUIHome_exec_impl ()\n"));
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::NavDisplayGUIHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::NavDisplayGUIHome_exec_impl::create()\n"));
  return new MyImpl::NavDisplayGUI_exec_impl;
}


extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "createNavDisplayHome_Impl()\n"));
  return new MyImpl::NavDisplayGUIHome_exec_impl;
}
