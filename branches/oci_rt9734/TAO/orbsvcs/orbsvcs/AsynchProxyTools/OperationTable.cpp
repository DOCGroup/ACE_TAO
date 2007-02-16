// $Id$
//
//  Operation Table impl

#include "OperationTable.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/corbafwd.h"
#include "tao/CORBA_String.h"
#include "tao/Any_Unknown_IDL_Type.h"
#include "ace/OS_NS_string.h"


//---------------------------------------------------------------------------

Arg::Arg ()
  : tc_ (CORBA::TypeCode::_nil ()),
    mode_ (0),
    must_proxy_ (-1)
{
}

Arg::~Arg (void)
{
  CORBA::release (this->tc_);
}

void
Arg::init (CORBA::TypeCode_ptr tc, CORBA::Flags m)
{
  CORBA::release (this->tc_);
  this->tc_ = CORBA::TypeCode::_duplicate (tc);
  this->mode_ = m;
  this->must_proxy_ = -1;
}

//---------------------------------------------------------------------------

ArgList::ArgList ()
  : result_ (),
    num_args_ (0),
    args_ (0),
    num_exceptions_ (0),
    exceptions_ (0),
    is_oneway_ (0),
    refcount_ (1)
{
  this->must_proxy_[0] = this->must_proxy_[1] = -1;
}

ArgList::ArgList (CORBA::ULong argc,
                        CORBA::ULong excepc,
                        int oneway)
  : result_ (),
    num_args_ (0),
    args_ (0),
    num_exceptions_ (0),
    exceptions_ (0),
    is_oneway_ (0),
    refcount_ (1)
{
  this->must_proxy_[0] = this->must_proxy_[1] = -1;
  this->init (argc,
              excepc, oneway);
}

ArgList::~ArgList (void)
{
  delete [] args_;
  delete [] exceptions_;
}

void
ArgList::init (CORBA::ULong argc,
                  CORBA::ULong excepc,
                  int oneway)
{
  this->args_ = new Arg [argc];
  this->num_args_ = argc;
  this->exceptions_ = new Arg [excepc];
  this->num_exceptions_ = excepc;
  this->is_oneway_ = oneway;
}

void
ArgList::prepare_request(CORBA::NVList_ptr args,
                            CORBA::TypeCode_ptr &result_tc,
                            int &is_oneway)
{
  is_oneway = this->is_oneway_;
  if (!is_oneway)
    result_tc = CORBA::TypeCode::_duplicate (this->result_.tc_);
  for (CORBA::ULong i = 0; i < this->num_args_; i++)
    {
      TAO::Unknown_IDL_Type *ut = 0;
      ACE_NEW(ut, TAO::Unknown_IDL_Type(this->args_[i].tc_));
      CORBA::Any_var value = new CORBA::Any;
      value->replace(ut);
      args->add_value("",value.in(), this->args_[i].mode_);
    }
}

Arg *
ArgList::find_exception (const char *id,
                            CORBA::Any *& value)
{
  Arg *result = 0;
  ACE_NEW_RETURN (value,CORBA::Any,0);

  for (CORBA::ULong i = 0; i < this->num_exceptions_; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "   exceps[%d] = %s\n",
                  i, this->exceptions_[i].tc_->id()));
      if (ACE_OS::strcmp(id,
                         this->exceptions_[i].tc_->id()) == 0)
        {
          result = &this->exceptions_[i];
          ACE_DEBUG ((LM_DEBUG, "   got a match\n"));
          TAO::Unknown_IDL_Type *ut = 0;
          ACE_NEW_RETURN(ut,
                         TAO::Unknown_IDL_Type(this->exceptions_[i].tc_),
                         0);
          value->replace (ut);
        }
    }
  return result;
}

  // Reference count manipulators. Object will destroy self when count
  // goes to zero
int
ArgList::add_ref (void)
{
  return ++this->refcount_;
}

int
ArgList::remove_ref (void)
{
  int r = --this->refcount_;
  if (r == 0)
    delete this;
  return r;
}

int
ArgList::must_proxy(CORBA::Flags mode) const
{
  return this->must_proxy_[mode == CORBA::ARG_IN ? 0 : 1];
}

void
ArgList::must_proxy (CORBA::Flags mode, int p)
{
  this->must_proxy_[mode == CORBA::ARG_IN ? 0 : 1] = p;
}

int
ArgList::is_oneway (void) const
{
  return this->is_oneway_;
}


// notice that this assignment takes ownership of the value.
void
ArgList::result (CORBA::TypeCode_ptr tc)
{
  this->result_.init(tc,0);
}

Arg &
ArgList::result ()
{
  return this->result_;
}

CORBA::ULong
ArgList::num_args(void) const
{
  return this->num_args_;
}

void
ArgList::set_arg (CORBA::ULong ndx,
                     CORBA::TypeCode_ptr tc,
                     CORBA::Flags mode)
{
  this->args_[ndx].init(tc,mode);
}

Arg *
ArgList::args ()
{
  return this->args_;
}

CORBA::ULong
ArgList::num_exceps(void) const
{
  return this->num_exceptions_;
}

void
ArgList::set_excep (CORBA::ULong ndx,
                       CORBA::TypeCode_ptr tc)
{
  this->exceptions_[ndx].init(tc,0);
}

Arg *
ArgList::exceps ()
{
  return this->exceptions_;
}

//---------------------------------------------------------------------------

OpArgs::OpArgs (const char *ref_id, ArgList *args)
  :next_ (0),
   args_(0),
   id_(0)
{
  if (args != 0)
    {
      args_ = args;
      args_->add_ref();
    }
  id_ = ACE_OS::strdup (ref_id);
}

OpArgs::~OpArgs (void)
{
  args_->remove_ref();
  free (id_);
  delete this->next_;
}

ArgList *
OpArgs::args (void)
{
  this->args_->add_ref();
  return this->args_;
}

const char *
OpArgs::id () const
{
  return this->id_;
}

OpArgs *
OpArgs::next ()
{
  return this->next_;
}

void
OpArgs::next (OpArgs *n)
{
  this->next_ = n;
}

//---------------------------------------------------------------------------

Operation::Operation (const char *opname)
  : name_(0),
    left_(0),
    right_(0),
    arg_set_ (0)
{
  if (opname)
    name_ = ACE_OS::strdup(opname);
}

Operation::~Operation()
{
  if (name_)
    free(name_);
  delete left_;
  delete right_;
  delete arg_set_;
}

ArgList *
Operation::arg_list (const char * rep_id)
{
  OpArgs *a = this->args (rep_id);
  if (a == 0)
    return 0;
  return a->args();
}

const char *
Operation::name() const
{
  return this->name_;
}

Operation *
Operation::left()
{
  return this->left_;
}

Operation *
Operation::right ()
{
  return this->right_;
}

void
Operation::left(Operation *o)
{
  this->left_ = o;
}

void
Operation::right (Operation *o)
{
  this->right_ = o;
}

OpArgs *
Operation::args (const char *rep_id)
{
  OpArgs *a = this->arg_set_;
  while (a != 0)
    {
      if (ACE_OS::strcmp (rep_id,a->id()) == 0)
        return a;
      a = a->next();
    }
  return a;
}

// this assignment takes ownership of argument.
void
Operation::add_set (OpArgs *arglist)
{
  if (this->arg_set_ == 0)
    {
      this->arg_set_ = arglist;
      return;
    }

  OpArgs *a = this->arg_set_;
  while (a->next() != 0)
    a = a->next();
  a->next (arglist);
}

//----------------------------------------------------------------------------
OperationTable::OperationTable (/* const char * persistence_file? */)
  : head_ (0)
{
  ArgList *al = this->add_is_a("IDL:org.omg/CORBA/Object:1.0");
  al->remove_ref();
  al = this->add_non_existent ("IDL:org.omg/CORBA/Object:1.0");
  al->remove_ref();
}

OperationTable::~OperationTable ()
{
  delete head_;
}

ArgList *
OperationTable::find (const char *opname,
                         const char *rep_id)
{
  ACE_Read_Guard<ACE_RW_Thread_Mutex>(this->lock_);
  Operation *op = head_;
  while (op != 0)
    {
      int match = ACE_OS::strcmp(opname,op->name());
      if (match == 0)
        {
          OpArgs *oa = op->args (rep_id);
          if (oa == 0)
            return 0;
          return oa->args();
        }
      op = match == 1 ? op->right() : op->left();
    }
  return 0;
}

Operation *
OperationTable::find_or_add (const char *opname)
{
  if (head_ == 0)
    {
      head_ = new Operation (opname);
      return head_;
    }
  Operation *result = head_;
  while (result != 0)
    {
      int match = ACE_OS::strcmp(opname,result->name());
      if (match == 0)
        return result;
      if (match == 1)
        if (result->right() == 0)
          {
            Operation *temp = new Operation(opname);
            result->right(temp);
            return temp;
          }
        else
          result = result->right();
      else
        if (result->left() == 0)
          {
            Operation *temp = new Operation(opname);
            result->left(temp);
            return temp;
          }
        else
          result = result->left();
    }
  return result;
}

ArgList *
OperationTable::add_interface (CORBA::InterfaceDef_ptr intDef,
                                  const char * desired_op)
{
  ACE_Write_Guard<ACE_RW_Thread_Mutex>(this->lock_);
  ArgList *result = 0;
  CORBA::InterfaceDefSeq_var bases = intDef->base_interfaces ();
  CORBA::String_var *derived = new CORBA::String_var[bases->length()];
  CORBA::ULong n_bases = bases->length();
  CORBA::ULong i = 0;
  for (; i <= n_bases; i++)
    {
      CORBA::String_var rep_id = intDef->id();
      if (i == 0)
        {
          ArgList *al = this->add_is_a (rep_id.in());
          if (desired_op && ACE_OS::strcmp("_is_a",desired_op) == 0)
            result = al;
          else
            al->remove_ref();

          al = this->add_non_existent (rep_id.in());
          if (desired_op && ACE_OS::strcmp("_non_existent",desired_op) == 0)
            result = al;
          else
            al->remove_ref();
        }
      CORBA::ContainedSeq_var operations =
        intDef->contents (CORBA::dk_Operation,1);

      CORBA::ULong n_ops = operations->length ();
      for (CORBA::ULong op = 0; op < n_ops; ++op)
        {
          CORBA::String_var op_name = operations[op]->name();
          Operation *op = this->find_or_add(op_name.in());

          CORBA::OperationDef_var opDef =
            CORBA::OperationDef::_narrow (operations[op]);
          CORBA::ParDescriptionSeq_var params = opDef->params ();
          CORBA::ExceptionDefSeq_var excepts = opDef->exceptions();

          int is_oneway = opDef->mode() == CORBA::OP_ONEWAY;
          ArgList *arg_list = new ArgList (params->length(),
                                                 excepts->length(),
                                                 is_oneway);
          if (!is_oneway)
            arg_list->result (opDef->result());

          for (CORBA::ULong p = 0; p < params->length (); ++p)
            {
              CORBA::Flags mode = CORBA::ARG_IN;
              if (params[p].mode == CORBA::PARAM_OUT)
                mode = CORBA::ARG_OUT;
              else if (params[p].mode == CORBA::PARAM_INOUT)
                mode = CORBA::ARG_INOUT;

              arg_list->set_arg(p, params[p].type.in(),mode);
            }

          for (CORBA::ULong e = 0; e < excepts->length (); ++e)
            {
              CORBA::TypeCode_var tc = excepts[e]->type();
              arg_list->set_excep(e, tc.in());
            }

          if (desired_op && ACE_OS::strcmp(op_name,desired_op) == 0)
            {
              arg_list->add_ref();
              result = arg_list;
            }

          op->add_set (new OpArgs(rep_id.in(),arg_list));
          for (CORBA::ULong d = 0; d < i; d++)
            op->add_set (new OpArgs(derived[d],arg_list));
          arg_list->remove_ref();
        }

      if (i < bases->length())
        {
          derived[i] = rep_id;
          intDef = bases[i];
        }
    }
  delete [] derived;
  return result;
}

ArgList *
OperationTable::add_is_a (const char *rep_id)
{
  Operation *op = this->find_or_add("_is_a");
  int is_oneway = 0;
  ArgList *arg_list = new ArgList (1,0, is_oneway);
  arg_list->result (CORBA::_tc_boolean);

  arg_list->set_arg(0, CORBA::_tc_string,CORBA::ARG_IN);
  op->add_set (new OpArgs(rep_id,arg_list));
  return arg_list;
}

ArgList *
OperationTable::add_non_existent (const char *rep_id)
{
  Operation *op = this->find_or_add("_non_existent");
  int is_oneway = 0;
  ArgList *arg_list = new ArgList (0,0, is_oneway);
  arg_list->result (CORBA::_tc_boolean);

  op->add_set (new OpArgs(rep_id,arg_list));
  return arg_list;
}
