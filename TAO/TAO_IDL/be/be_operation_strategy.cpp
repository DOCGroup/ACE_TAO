// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation_strategy.cpp
//
// = DESCRIPTION
//    A strategy to cover the differences between various variants
//    of an operations, e.g. AMI sendc_, AMI raise_
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_operation_strategy, "$Id$")


be_operation_strategy::be_operation_strategy (be_operation *node,
Strategy_Kind strategy_type)
: node_ (node),
strategy_type_ (strategy_type)
{
}

be_operation_strategy::~be_operation_strategy (void)
{
}

int
be_operation_strategy::strategy_type (void)
{
return strategy_type_;
}

int
be_operation_strategy::has_extra_code_generation (
TAO_CodeGen::CG_STATE /* current_state */
)
{
return 0;
}

be_operation *
be_operation_strategy::marshaling (void)
{
return 0;
}


be_operation *
be_operation_strategy::arguments (void)
{
return 0;
}

// ****************************************************************
// AMI sendc_ operation strategy

be_operation_default_strategy::be_operation_default_strategy (
be_operation *node
)
: be_operation_strategy (node,
DEFAULT)
{
}

be_operation_default_strategy::~be_operation_default_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_default_strategy::next_state (
TAO_CodeGen::CG_STATE current_state,
int /* is_extra_state */
)
{
return current_state;
}



// ****************************************************************
// AMI sendc_ operation strategy

be_operation_ami_sendc_strategy::be_operation_ami_sendc_strategy (
be_operation *node,
be_operation *marshaling,
be_operation *arguments
)
: be_operation_strategy (node,
AMI_SENDC),
marshaling_ (marshaling),
arguments_ (arguments)
{
}

be_operation_ami_sendc_strategy::~be_operation_ami_sendc_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_ami_sendc_strategy::next_state (
TAO_CodeGen::CG_STATE current_state,
int is_extra_state
)
{
if (is_extra_state)
{
switch (current_state)
{
case TAO_CodeGen::TAO_OPERATION_CH:
return TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CH;
case TAO_CodeGen::TAO_OPERATION_CS:
return TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CS;
default:
return current_state;
}
}
else
{
return current_state;
}
}

int
be_operation_ami_sendc_strategy::has_extra_code_generation (
TAO_CodeGen::CG_STATE current_state
)
{
if (current_state == TAO_CodeGen::TAO_OPERATION_CH
|| current_state == TAO_CodeGen::TAO_OPERATION_CS)
{
return 1;
}
else
{
return 0;
}
}


be_operation*
be_operation_ami_sendc_strategy::marshaling (void)
{
return marshaling_;
}


be_operation*
be_operation_ami_sendc_strategy::arguments (void)
{
return arguments_;
}

// ****************************************************************
// AMI ExceptionHolder raise_ operation strategy

be_operation_ami_exception_holder_raise_strategy
::be_operation_ami_exception_holder_raise_strategy (be_operation *node)
: be_operation_strategy (node,
AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_ami_exception_holder_raise_strategy
::~be_operation_ami_exception_holder_raise_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_ami_exception_holder_raise_strategy::next_state (
TAO_CodeGen::CG_STATE current_state,
int is_extra_state
)
{
if (!is_extra_state)
{
switch (current_state)
{
case TAO_CodeGen::TAO_OPERATION_CS:
return TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS;
default:
return current_state;
}
}
else
{
return current_state;
}
}


// ****************************************************************
// AMI Reply Handler reply stub operation strategy

be_operation_ami_handler_reply_stub_strategy
::be_operation_ami_handler_reply_stub_strategy (be_operation *node)
: be_operation_strategy (node,
AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_ami_handler_reply_stub_strategy
::~be_operation_ami_handler_reply_stub_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_ami_handler_reply_stub_strategy::next_state (
TAO_CodeGen::CG_STATE current_state,
int is_extra_state
)
{
if (is_extra_state)
{
switch (current_state)
{
case TAO_CodeGen::TAO_OPERATION_CH:
return TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CH;

case TAO_CodeGen::TAO_OPERATION_CS:
return TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS;

default:
return current_state;
}
}
else
{
return current_state;
}
}


int
be_operation_ami_handler_reply_stub_strategy::has_extra_code_generation (
TAO_CodeGen::CG_STATE current_state
)
{
if (current_state == TAO_CodeGen::TAO_OPERATION_CH
|| current_state == TAO_CodeGen::TAO_OPERATION_CS)
{
return 1;
}
else
{
return 0;
}
}


// ****************************************************************
// AMH Strategy

be_operation_amh_strategy::be_operation_amh_strategy (be_operation *node)
: be_operation_strategy (node, AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_amh_strategy::~be_operation_amh_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_amh_strategy::next_state (
TAO_CodeGen::CG_STATE current_state,
int /*is_extra_state*/
)
{
switch (current_state)
{
case TAO_CodeGen::TAO_OPERATION_SS:
return TAO_CodeGen::TAO_OPERATION_AMH_SS;
case TAO_CodeGen::TAO_INTERFACE_SS:
return TAO_CodeGen::TAO_INTERFACE_AMH_SS;
default:
return current_state;
}
}

be_operation*
be_operation_amh_strategy::arguments (void)
{
return arguments_;
}

int
be_operation_amh_strategy::has_extra_code_generation (
TAO_CodeGen::CG_STATE
)
{
return 0;
}
