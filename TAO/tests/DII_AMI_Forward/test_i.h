/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: f; c-basic-offset: 2 -*- */

/*
 *    Lorica header file.
 *    Copyright (C) 2007 OMC Denmark ApS.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

class Forward_Test_i : public POA_Forward::Test
{
	// = TITLE
	//   Location Forward exception tester
	//
	// = DESCRIPTION
	//   servant throws a location forward exception when constructed with a
	//   target IOR, otherwise it echos the source string.

public:
	Forward_Test_i (CORBA::ORB_ptr orb);

  char * do_forward (const char *text);
	void shutdown ();

private:
	CORBA::ORB_var orb_;
};

#endif /* TEST_I_H */
