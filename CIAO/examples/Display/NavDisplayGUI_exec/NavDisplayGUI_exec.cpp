// $Id$

#include "NavDisplayGUI_exec.h"
#include <QtGui/qapplication.h>
#include <QtGui/qevent.h>

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
  : number_of_GPS_(1), loc_(10, 10)
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
  ACE_DEBUG ((LM_DEBUG, "ENTER: MyImpl::NavDisplayGUI_exec_impl::push_Refresh().\n"));
  ACE_DEBUG ((LM_DEBUG, "NavDisplay receives Refresh event from a GPS supplier.\n"));

  // Refresh position
  ACE_DEBUG ((LM_DEBUG, "NavDisplay reads coordinates via GPS receptacles.\n"));

  ::HUDisplay::NavDisplay::GPSLocationConnections_var locs =
      this->context_->get_connections_GPSLocation ();

  for(CORBA::ULong i = 0; i < locs->length(); ++i)
    {
      HUDisplay::position_var loc = locs[i].objref;

      //only if GPS is started, update position
      if (loc->started ())
        {
          HUDisplay::GPS_position posxy = loc->posxy();
          loc_.x_ = posxy.pos_x;
          loc_.y_ = posxy.pos_y;

          navunitarr[i]->setLocation (loc_);
          RootPanel *root_pane = worker_->getMainWindow ();

          if (root_pane != 0)
            {
              UpdatePositionCmd *cmd = UpdatePositionCmd::create (
                 root_pane, navunitarr[i]);
              NavEvent *evt = new NavEvent (cmd);
              QApplication::postEvent (root_pane, evt);
            }
        }
    }
  ACE_DEBUG ((LM_DEBUG,
              "LEAVE: MyImpl::NavDisplayGUI_exec_impl::push_Refresh()\n"));
}

CORBA::Long
MyImpl::NavDisplayGUI_exec_impl::number_of_GPS ()
{
  return this->number_of_GPS_;
}

void
MyImpl::NavDisplayGUI_exec_impl::number_of_GPS (CORBA::Long number_of_GPS)
{
  this->number_of_GPS_ = number_of_GPS;
}

// Operations from Components::SessionComponent
void
MyImpl::NavDisplayGUI_exec_impl::set_session_context (
                                        Components::SessionContext_ptr ctx)
{
  this->context_ =
    HUDisplay::CCM_NavDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  // Urm, we actually discard exceptions thrown from this operation.
}

void
MyImpl::NavDisplayGUI_exec_impl::configuration_complete (void)
{
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_activate (void)
{
  ACE_DEBUG((LM_DEBUG,"MyImpl::NavDisplayGUI_exec_impl::ccm_activate \n"));
  worker_ = new Worker (sizeof (argv) / sizeof (argv[0]),
                        const_cast<char **> (argv));

  if (worker_->activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    {
      ACE_DEBUG((LM_ERROR,
                 "Cannot activate client threads\n"));
      throw Components::CCMException ();
    }
  worker_->waitUntillInitialized ();

  NavEvent::set_type(QEvent::User);

  //make for all GPS devices a unit with initial different locations.
  for (int i = 0; i < number_of_GPS(); i ++)
    {
      NavUnit * unit;
      unit = new NavUnit((i+ 1));
      navunitarr.push_back(unit);
      navunitarr[i]->setLocation (UnitLocation(10.0 + (10*i), 20.0 + (10*i)));

      AddNavUnitCmd *cmd = AddNavUnitCmd::create (
         worker_->getMainWindow(),
        (navunitarr[i]));
      NavEvent *evt = new NavEvent (cmd);
      QApplication::postEvent (worker_->getMainWindow (), evt);
   }
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_passivate (void)
{
  RootPanel *target = worker_->getMainWindow ();

  if (target != 0)
    {
      QuitCmd *cmd = QuitCmd::create( target);
      NavEvent *evt = new NavEvent (cmd);
      QApplication::postEvent (target, evt);
      worker_->thr_mgr ()->wait ();
    }

  delete worker_;
}

void
MyImpl::NavDisplayGUI_exec_impl::ccm_remove (void)
{
  for  (std::vector<NavUnit*>::iterator unitObj = navunitarr.begin();
        unitObj != navunitarr.end(); ++unitObj)
    {
      delete *unitObj;
    }
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
