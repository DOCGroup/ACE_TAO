// $Id$

#include "orbsvcs/Notify/Refcountable.h"
#include "tao/debug.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/Guard_T.h"

#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )

#include <map>
#include <string>
#include <typeinfo.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Tracker
 *
 * @brief A debugging tool to track the creation of TAO_Notify_Refcountable objects.
 *
 *        Usage:
 *        Set USE_TAO_NOTIFY_TRACKER defined above to 1
 *        At any significant point to check object usage
 *          TAO_Notify_Tracker::get_instance().dump();
 */
class TAO_Notify_Tracker
{
public:
  struct Entry
  {
    TAO_Notify_Refcountable*  obj;
    std::string               class_name;
  };

  static TAO_Notify_Tracker& get_instance();

  void add( TAO_Notify_Refcountable* p );

  void remove( const TAO_Notify_Refcountable* p );

  Entry find( const TAO_Notify_Refcountable* p ) const;

  void dump( const char* title = 0 );

private:
  TAO_Notify_Tracker();
  ~TAO_Notify_Tracker();

  friend class std::auto_ptr< TAO_Notify_Tracker >;
  static std::auto_ptr< TAO_Notify_Tracker > s_instance;
  mutable TAO_SYNCH_MUTEX lock_;
  typedef std::map<int, Entry> EntityMap;
  EntityMap map_;
  int id_counter_;
  int last_dump_count_;
};

void
TAO_Notify_Refcountable::diagnostic_dump( const char* title )
{
  TAO_Notify_Tracker::get_instance().dump( title );
}

#endif /* TAO_NOTIFY_REFCOUNT_DIAGNOSTICS */


TAO_Notify_Refcountable::TAO_Notify_Refcountable (void)
{
}

TAO_Notify_Refcountable::~TAO_Notify_Refcountable ()
{
#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )
  TAO_Notify_Tracker::Entry e = TAO_Notify_Tracker::get_instance().find( this );
  if ( e.obj != 0 )
  {
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x %s(%d) with refcount:%d destroyed incorrectly.\n",
        e.obj, e.class_name, e.obj->ref_id_, e.obj->refcount_.value() ));

    if ( e.obj != this || e.obj->ref_id_ != this->ref_id_ )
    {
      ORBSVCS_DEBUG ((LM_DEBUG, "  with an ID mismatch %x->%d != %x->%d!\n",
        this, ref_id_, e.obj, e.obj->ref_id_));
    }
    TAO_Notify_Tracker::get_instance().remove( this );
  }
#endif
  CORBA::ULong refcount = this->refcount_.value();
  ACE_ASSERT( refcount == 0 );
  ACE_UNUSED_ARG(refcount);
}


CORBA::ULong
TAO_Notify_Refcountable::_incr_refcnt (void)
{
  CORBA::Long refcount = ++this->refcount_;
  if (TAO_debug_level > 1 )
  {
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x incr refcount = %d\n", this, refcount ));
  }
#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )
  // Stack-instantiated-non-servants should never have _incr_refcnt called.
  // We do not care about stack-instances. Stack-instantiated servants break
  // the tracker.
  if ( refcount == 1 )
  {
    TAO_Notify_Tracker::get_instance().add( this );
  }
#endif
  return refcount;
}

CORBA::ULong
TAO_Notify_Refcountable::_decr_refcnt (void)
{
  CORBA::Long refcount = --this->refcount_;

  if (TAO_debug_level > 1 )
  {
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x decr refcount = %d\n", this, refcount ));
  }

  ACE_ASSERT(refcount >= 0);

  if (refcount == 0)
  {
    #if ( USE_TAO_NOTIFY_TRACKER != 0 )
      TAO_Notify_Tracker::get_instance().remove( this );
    #endif
    this->release ();
  }
  return refcount;
}

#if ( TAO_NOTIFY_REFCOUNT_DIAGNOSTICS != 0 )

std::auto_ptr< TAO_Notify_Tracker > TAO_Notify_Tracker::s_instance;

TAO_Notify_Tracker::TAO_Notify_Tracker()
: id_counter_(0)
, last_dump_count_(0)
{
}


TAO_Notify_Tracker::~TAO_Notify_Tracker()
{
  dump( "destruction" );
}


void
TAO_Notify_Tracker::dump( const char* title )
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  ORBSVCS_DEBUG ((LM_DEBUG,"\nTAO_Notify_Tracker: %s\n", (title ? title : "dump")));

  EntityMap::const_iterator iter( map_.begin() );
  while ( iter != map_.end() )
  {
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x %s(%d) with refcount:%d.\n",
      iter->second.obj, iter->second.class_name, iter->first, iter->second.obj->refcount_.value() ));
    iter++;
  }

  EntityMap::size_type current_count = map_.size();
  int diff = ( current_count - last_dump_count_ );
  ORBSVCS_DEBUG ((LM_DEBUG,"Count: %u\nDifference: %+d\n", current_count, diff ));
  last_dump_count_ = current_count;
}


TAO_Notify_Tracker&
TAO_Notify_Tracker::get_instance()
{
  if ( s_instance.get() == 0 )
  {
    s_instance.reset( new TAO_Notify_Tracker );
  }
  return *s_instance;
}


void
TAO_Notify_Tracker::add( TAO_Notify_Refcountable* p )
{
  if ( p == 0 ) return;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  int id = ++id_counter_;

  Entry e = { p, typeid(*p).name() };
  std::pair< EntityMap::iterator, bool > result =
    map_.insert( std::make_pair( id, e ) );
  EntityMap::iterator& it = result.first;
  if ( result.second == false )
  {
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x %s(%d) added twice.\n",
      it->second.obj, it->second.class_name.c_str(), it->first ));
  }
  else
  {
    p->ref_id_ = id;
  }
}


void
TAO_Notify_Tracker::remove( const TAO_Notify_Refcountable* p )
{
  if ( p == 0 ) return;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  int ref_id = p->ref_id_;
  EntityMap::iterator iter( map_.find( ref_id ) );
  if ( iter == map_.end() )
  {
    const char* class_name = typeid(*iter->second.obj).name();
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x %s(%d) not found.\n",
      p, class_name, ref_id ));
  }
  else
  {
    map_.erase( iter );
  }
}


TAO_Notify_Tracker::Entry
TAO_Notify_Tracker::find( const TAO_Notify_Refcountable* p ) const
{
  Entry e = { 0, "" };
  if ( p == 0 ) return e;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  int ref_id = p->ref_id_;
  EntityMap::const_iterator iter( map_.find( ref_id ) );
  if ( iter != map_.end() )
  {
    return iter->second;
  }
  return e;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_NOTIFY_REFCOUNT_DIAGNOSTICS */
