/*
    Copyright (C) 2006-2016 Paul Davis

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __ardour_gain_control_h__
#define __ardour_gain_control_h__

#include <string>
#include <set>

#include <boost/shared_ptr.hpp>
#include <glibmm/threads.h>

#include "pbd/controllable.h"

#include "evoral/Parameter.hpp"

#include "ardour/automation_control.h"
#include "ardour/libardour_visibility.h"

namespace ARDOUR {

class Session;

class LIBARDOUR_API GainControl : public AutomationControl {
  public:
	GainControl (Session& session, const Evoral::Parameter &param,
	             boost::shared_ptr<AutomationList> al = boost::shared_ptr<AutomationList>());

	void set_value (double val, PBD::Controllable::GroupControlDisposition group_override);
	void set_value_unchecked (double);

	double internal_to_interface (double) const;
	double interface_to_internal (double) const;
	double internal_to_user (double) const;
	double user_to_internal (double) const;
	std::string get_user_string () const;

	double lower_db;
	double range_db;

	gain_t get_master_gain () const;
	void add_master (boost::shared_ptr<VCA>);
	void remove_master (boost::shared_ptr<VCA>);
	void clear_masters ();
	bool slaved_to (boost::shared_ptr<VCA>) const;
	bool slaved () const;
	std::vector<uint32_t> masters () const;

	PBD::Signal0<void> VCAStatusChange;

	int set_state (XMLNode const&, int);
	XMLNode& get_state();

  private:
	void _set_value (double val, PBD::Controllable::GroupControlDisposition group_override);

	mutable Glib::Threads::Mutex master_lock;

	typedef std::set<boost::shared_ptr<GainControl> > Masters;
	Masters _masters;
	PBD::ScopedConnectionList masters_connections;
	std::set<uint32_t> _masters_numbers;
	std::string _masters_state_string ();

	gain_t get_master_gain_locked () const;
	void master_going_away (boost::weak_ptr<VCA>);
};

} /* namespace */

#endif /* __ardour_gain_control_h__ */
