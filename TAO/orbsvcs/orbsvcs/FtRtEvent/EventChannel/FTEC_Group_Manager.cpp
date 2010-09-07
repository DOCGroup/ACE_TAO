// // $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Group_Manager.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "orbsvcs/FtRtEvent/EventChannel/Fault_Detector.h"
#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "orbsvcs/FtRtEvent/EventChannel/GroupInfoPublisher.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "../Utils/Log.h"
#include "tao/CDR.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_Group_Manager,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int find_by_location(const FTRT::ManagerInfoList& list,
                const FTRT::Location & location)
{
    unsigned i;
    for (i = 0; i < list.length(); ++i) {
        if (list[i].the_location == location) {
            return i;
        }
    }
    return i;
}

/*
void print(const FTRT::Location& location)
{
  ACE_DEBUG((LM_DEBUG, "{"));
  for (size_t i = 0; i < location.length(); ++i) {
    ACE_DEBUG((LM_DEBUG, "<%s,%s>", location[i].id.in(), location[i].kind.in()));
  }
  ACE_DEBUG((LM_DEBUG, "}"));
}
*/

struct TAO_FTEC_Group_Manager_Impl
{
  FTRT::ManagerInfoList info_list;
  unsigned my_position;
};

TAO_FTEC_Group_Manager::TAO_FTEC_Group_Manager()
: impl_(new TAO_FTEC_Group_Manager_Impl)
{
  Fault_Detector::instance()->set_listener(this);
}

TAO_FTEC_Group_Manager::~TAO_FTEC_Group_Manager()
{
  delete impl_;
}

CORBA::Boolean TAO_FTEC_Group_Manager::start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out cur
      )
{
  listener_ = listener;
  ACE_NEW_RETURN(cur , FTRT::Location(Fault_Detector::instance()->my_location()), false);
  return true;
}


void TAO_FTEC_Group_Manager::create_group (
    const FTRT::ManagerInfoList & info_list,
    CORBA::ULong object_group_ref_version)
{
  TAO_FTRTEC::Log(1, ACE_TEXT("create_group\n"));

  impl_->info_list = info_list;
  impl_->my_position = find_by_location(info_list,
    Fault_Detector::instance()->my_location());

  GroupInfoPublisherBase* publisher = GroupInfoPublisher::instance();
  GroupInfoPublisherBase::Info_ptr info (
    publisher->setup_info(impl_->info_list,
                          impl_->my_position,
                          object_group_ref_version));

  publisher->update_info(info);

  IOGR_Maker::instance()->set_ref_version( object_group_ref_version );

  if (impl_->my_position > 0) {
    Fault_Detector* detector = Fault_Detector::instance();
    if ( detector->connect(impl_->info_list[impl_->my_position-1].the_location) == -1) {
      ACE_DEBUG((LM_DEBUG, "Cannot connect to predecesor\n"));
      throw FTRT::PredecessorUnreachable();
    }
  }

  FtRtecEventChannelAdmin::EventChannel_var  successor
    = publisher->successor();

  if (!CORBA::is_nil(successor.in())) {
    successor->create_group(info_list, object_group_ref_version);
  }
}

void TAO_FTEC_Group_Manager::join_group (
    const FTRT::ManagerInfo & info)
{
  TAO_FTRTEC::Log(1, ACE_TEXT("join group\n"));
  if (impl_->my_position == 0) {
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_WRITE_GUARD (FTRTEC::Replication_Service, lock, *svc);
    add_member(info, IOGR_Maker::instance()->get_ref_version()+1);
  }
}

void TAO_FTEC_Group_Manager::add_member (
    const FTRT::ManagerInfo & info,
    CORBA::ULong object_group_ref_version)
{
  TAO_FTRTEC::Log(1, ACE_TEXT("add_member location = <%s>\n"),
    (const char*)info.the_location[0].id);

  auto_ptr<TAO_FTEC_Group_Manager_Impl> new_impl(new TAO_FTEC_Group_Manager_Impl);

  new_impl->my_position = impl_->my_position;
  size_t pos = impl_->info_list.length();
  new_impl->info_list.length(pos+1);
  for (size_t i = 0; i < pos; ++i) {
    new_impl->info_list[i] = impl_->info_list[i];
  }
  new_impl->info_list[pos] = info;

  GroupInfoPublisherBase* publisher = GroupInfoPublisher::instance();
  GroupInfoPublisherBase::Info_ptr group_info (
    publisher->setup_info(new_impl->info_list,
                          new_impl->my_position,
                          object_group_ref_version));

  int last_one = (impl_->my_position == impl_->info_list.length()-1);

  if (!last_one)
  {
    // I am not the last of replica, tell my successor that
    // a new member has joined in.
    try{
      FTRTEC::Replication_Service::instance()->add_member(info, object_group_ref_version);
    }
    catch (const CORBA::Exception&){
      // Unable to send request to all the successors.
      // Now this node become the last replica of the object group.
      // update the info list again
      new_impl->info_list.length(new_impl->my_position+2);
      new_impl->info_list[new_impl->my_position+1] = info;

      /// group_info = publisher->set_info(..) should be enough.
      /// However, GCC 2.96 is not happy with that.

      GroupInfoPublisherBase::Info_ptr group_info1 (
        publisher->setup_info(new_impl->info_list,
                              new_impl->my_position,
                              object_group_ref_version));
      ACE_AUTO_PTR_RESET(group_info, group_info1.release(), GroupInfoPublisherBase::Info);

      last_one = true;
    }
  }

  if (last_one)
  {
    // this is the last replica in the list
    // synchornize the state with the newly joined replica.
    FtRtecEventChannelAdmin::EventChannelState state;
    get_state(state);

    TAO_OutputCDR cdr;
    cdr << state;

    FTRT::State s;
    if (cdr.begin()->cont()) {
      ACE_Message_Block* blk;
      ACE_NEW_THROW_EX(blk, ACE_Message_Block, CORBA::NO_MEMORY());
      ACE_CDR::consolidate(blk, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      s.replace(blk->length(), blk);
#else
      // If the replace method is not available, we will need
      // to do the copy manually.  First, set the octet sequence length.
      CORBA::ULong length = blk->length ();
      s.length (length);

      // Now copy over each byte.
      char* base = blk->data_block ()->base ();
      for(CORBA::ULong i = 0; i < length; i++)
      {
        s[i] = base[i];
      }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

      blk->release();
    }
    else {
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      s.replace(cdr.begin()->length(), cdr.begin());
#else
      // If the replace method is not available, we will need
      // to do the copy manually.  First, set the octet sequence length.
      CORBA::ULong length = cdr.begin ()->length ();
      s.length (length);

      // Now copy over each byte.
      char* base = cdr.begin()->data_block ()->base ();
      for(CORBA::ULong i = 0; i < length; i++)
      {
        s[i] = base[i];
      }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
    }

    TAO_FTRTEC::Log(2, ACE_TEXT("Setting state\n"));
    info.ior->set_state(s);
    info.ior->create_group(new_impl->info_list,
                           object_group_ref_version);
    TAO_FTRTEC::Log(2, ACE_TEXT("After create_group\n"));
  }

  // commit the changes
  IOGR_Maker::instance()->set_ref_version( object_group_ref_version );
  publisher->update_info(group_info);
  delete impl_;
  impl_ = new_impl.release();
}

template <class SEQ>
void remove_item(SEQ & seq, int position)
{
    int i = position;
    int len = seq.length()-1;
    for (;i < len; ++i)
        seq[i] = seq[i+1];
    seq.length(len);
}

void TAO_FTEC_Group_Manager::replica_crashed (
    const FTRT::Location & location)
{
  TAO_FTRTEC::Log(1, ACE_TEXT("TAO_FTEC_Group_Manager::replica_crashed\n"));
  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  ACE_WRITE_GUARD (FTRTEC::Replication_Service, lock, *svc);
  remove_member(location, IOGR_Maker::instance()->get_ref_version()+1);
}

void TAO_FTEC_Group_Manager::remove_member (
    const FTRT::Location & crashed_location,
    CORBA::ULong object_group_ref_version)
{
  // find my position of this EventChannel in the group
  unsigned  crashed_pos = find_by_location(impl_->info_list, crashed_location);
  if (crashed_pos >= impl_->info_list.length()) {
    return;
  }

  ACE_ASSERT(impl_->my_position != crashed_pos);
  remove_item(impl_->info_list, crashed_pos);

  if (impl_->my_position > crashed_pos)
    --impl_->my_position;

  GroupInfoPublisherBase* publisher = GroupInfoPublisher::instance();

  GroupInfoPublisherBase::Info_ptr info (
    publisher->setup_info(impl_->info_list,
                          impl_->my_position,
                          object_group_ref_version));
  publisher->update_info(info);

  FtRtecEventChannelAdmin::EventChannel_var successor =
    publisher->successor();

  IOGR_Maker::instance()->set_ref_version(object_group_ref_version);
  if (!CORBA::is_nil(successor.in())) {
    try{
    successor->remove_member(crashed_location,
                             object_group_ref_version);
    }
    catch (...){
    }
  }

  TAO_FTRTEC::Log(3, ACE_TEXT("my_position = %d, crashed_pos = %d\n"), impl_->my_position, crashed_pos);
  if (impl_->my_position == crashed_pos && impl_->my_position > 0)
    Fault_Detector::instance()->connect(impl_->info_list[impl_->my_position-1].the_location);
}

void TAO_FTEC_Group_Manager::connection_closed()
{
  TAO_FTRTEC::Log(1, ACE_TEXT("TAO_FTEC_Group_Manager::connection_closed\n"));
  ACE_ASSERT(impl_->my_position > 0);

  // do not use referere here, because the the value pointed by the pointer to
  // crashed_location will be repliaced by its successor.
  FTRT::Location crashed_location = impl_->info_list[impl_->my_position-1].the_location;


  if (impl_->my_position > 1) {
    // if I am not the new primary, tell the new primary
    try{
      TAO_IOP::TAO_IOR_Manipulation::IORList iors;
      iors.length(impl_->my_position-1);
      for (size_t i = 0; i < impl_->my_position-1; ++i)
        iors[i] = CORBA::Object::_duplicate(impl_->info_list[i].ior.in());

      CORBA::Object_var obj =
        IOGR_Maker::instance()->merge_iors(iors);

      FtRtecEventChannelAdmin::EventChannel_var primary =
        FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in());
      primary->replica_crashed(crashed_location);

      return;
    }
    catch (const CORBA::Exception&){
    }
  }

  try{
    remove_member(crashed_location,
                  IOGR_Maker::instance()->increment_ref_version());
  }
  catch (const CORBA::Exception&){
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
