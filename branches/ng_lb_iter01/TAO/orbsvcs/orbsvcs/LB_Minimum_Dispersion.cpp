// -*- C++ -*-

#include "LB_Minimum_Dispersion.h"
#include "LB_Location_Map.h"
#include "LB_ObjectGroup_Map.h"
//#include "ReplicaProxy.h"

ACE_RCSID (LoadBalancing,
           LB_Minimum_Dispersion,
           "$Id$")


TAO_LB_Minimum_Dispersion_Strategy::TAO_LB_Minimum_Dispersion_Strategy (void)
{
}

TAO_LB_Minimum_Dispersion_Strategy::~TAO_LB_Minimum_Dispersion_Strategy (void)
{
}

CORBA::Object_ptr
TAO_LB_Minimum_Dispersion_Strategy::replica (
    TAO_LB_ObjectGroup_Map_Entry *entry,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for ( ; ; )
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        entry->lock,
                        CORBA::Object::_nil ());

      if (entry->replica_infos.is_empty ())
        // @@ What do we do if the set is empty?
        ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                          CORBA::Object::_nil ());

      TAO_LB_ReplicaInfo_Set::iterator begin = entry->replica_infos.begin ();
      TAO_LB_ReplicaInfo_Set::iterator end = entry->replica_infos.end ();

      TAO_LB_ReplicaInfo_Set::iterator i = begin;
      TAO_LB_ReplicaInfo *replica_info = (*i);

      LoadBalancing::LoadList *d =
        replica_info->location_entry->load_list.ptr ();

      for (++i ; i != end; ++i)
        {
          LoadBalancing::LoadList *load =
            (*i)->location_entry->load_list.ptr ();

          // @@ Hardcode one load and don't bother checking the
          // LoadId, for now.  (just to get things going)
          if ((*d)[CORBA::ULong (0)].value > (*load)[CORBA::ULong (0)].value)
            {
              replica_info = *i;
              d = (*i)->location_entry->load_list.ptr ();
            }
        }

      // Before returning an object reference to the client
      // validate it first.
      CORBA::Object_ptr object = replica_info->replica.in ();

      {
        ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (entry->lock);

        ACE_GUARD_RETURN (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                          reverse_guard,
                          reverse_lock,
                          CORBA::Object::_nil ());

        // @@ Ossama: we should setup a timeout policy here...
        ACE_TRY
          {
            CORBA::Boolean non_existent =
              object->_non_existent (ACE_TRY_ENV);
            ACE_TRY_CHECK;
            if (!non_existent)
              {
                return CORBA::Object::_duplicate (object);
              }
          }
        ACE_CATCHANY
          {
            // @@ HACK!  Do the right thing!
            return CORBA::Object::_duplicate (object);
          }
        ACE_ENDTRY;
      }
    }
}

void
TAO_LB_Minimum_Dispersion_Strategy::analyze_loads (
  TAO_LB_Location_Map &location_map,
  CORBA::Environment &ACE_TRY_ENV)
{
  int send_load_advisory = 0;

  {
    ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                       guard,
                       this->lock_));


    TAO_LB_Location_Map::iterator begin =
      location_map.begin ();

    TAO_LB_Location_Map::iterator end =
      location_map.end ();

    float s = 0;
    CORBA::ULong n = 0;
    TAO_LB_Location_Map::iterator i = begin;
    for ( ; i != end; ++i)
      {
        s += (*i)->int_id_.load_list[0].value;  // @@ Hard coded to
                                                //    get things
                                                //    going.
        n++;
      }

    float avg = (n == 0 ? s : s / n);
    float cl = proxy->current_load ();

    if (avg == 0)
      return;

    float relative_load = cl / avg;

    // @@ Ossama: Make the 1.5 factor adjustable, it is how much
    // dispersion we tolerate before starting to send advisories.
    if (relative_load > 1 + 1.5F / n)
      {
        proxy->has_high_load_ = 1;
        send_load_advisory = 2;  // 2 == Send high load advisory
      }

    if (send_load_advisory == 1 && relative_load < 1 + 0.9F / n)
      {
        proxy->has_high_load_ = 0;
        send_load_advisory = 1;  // 1 == Send nominal load advisory
      }
  }

  // @@ Ossama: no debug messages in production code, my fault....
  // ACE_DEBUG ((LM_DEBUG, "Load[%x] %f %f %f\n",
  //             proxy, cl, avg, relative_load));

  // @@ Ossama: Make the 1.5 factor adjustable, it is how much
  // dispersion we tolerate before starting to send advisories.
  if (send_load_advisory == 2)
    {
      proxy->control_->high_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;

      return;  // We may not throw an exception, so explicitly return.
    }

  // @@ Ossama: notice that we wait until the load is signifcantly
  // lower before sending the nominal load advisory, it does not
  // matter that much because the replicas automatically restart after
  // rejecting one client....
  // @@ Ossama: make the 0.9 factor adjustable, at least at
  // construction time...
  if (send_load_advisory == 1)
    {
      proxy->control_->nominal_load_advisory (ACE_TRY_ENV);
      ACE_CHECK;
    }
}
