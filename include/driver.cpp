/**
 * @file		driver.cpp
 * @brief		Component driver
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 21
 * @details	Load components with driver
 */

#include "driver.hpp"
#include <dlfcn.h>
#include <iostream>
#include "xmlprofile.hpp"
#include "logger.hpp"
#include "util/format.h"
#include <tuple>
#include "message.hpp"
#include "broker.hpp"
#include "exception.hpp"

using namespace std;

namespace cossb {
namespace driver {

component_driver::component_driver(const char* component_name)
:_component_name(component_name)
{
	try {
		if(load(component_name))
		{
			string profile_path = fmt::format("./{}.xml",component_name);
			if(!_ptr_component->set_profile(new profile::xmlprofile, profile_path.c_str()))
				unload();
			else
				regist_service_desc();	//regist service description
		}
		else
			throw exception(excode::COMPONENT_LOAD_FAIL);
	}
	catch(driver::exception& e) {
		cossb_log->log(log::loglevel::ERROR, e.what());
	}
}

component_driver::~component_driver()
{
	try {
		unload();
	}
	catch(driver::exception& e) {
		cossb_log->log(log::loglevel::ERROR, e.what());
	}
}

bool component_driver::load(const char* component_name)
{
	string component_path = fmt::format("./{}.comp",component_name);

	_handle = dlopen(component_path.c_str(), RTLD_LAZY|RTLD_GLOBAL);


	if(_handle)
	{
		create_component pfcreate = (create_component)dlsym(_handle, "create");
		if(!pfcreate)
		{
			dlclose(_handle);
			_handle = nullptr;

			return false;
		}

		_ptr_component = pfcreate();
		return true;
	}
	else
		throw exception(excode::COMPONENT_OPEN_ERROR, dlerror());

	return false;
}

void component_driver::unload()
{
	if(_ptr_component)
	{
		destroy_component pfdestroy = (destroy_component)dlsym(_handle, "destroy");

		destroy_task(_request_proc_task);

		if(pfdestroy) {
			pfdestroy();
		}
		else
			throw exception(excode::COMPONENT_UNLOAD_FAIL);

		_ptr_component = nullptr;
	}

	if(_handle)
	{
		dlclose(_handle);
		_handle = nullptr;
	}
}

void component_driver::setup()
{
	if(_ptr_component) {
		_ptr_component->setup();
	}
}

void component_driver::run()
{
	if(_ptr_component)
	{
		if(!_request_proc_task)
			_request_proc_task = create_task(component_driver::request_proc);

		_ptr_component->run();
	}

}

void component_driver::stop()
{
	if(_ptr_component)
		_ptr_component->stop();

	destroy_task(_request_proc_task);
}

bool component_driver::set_profile(interface::iprofile* profile, const char* path)
{
	return true;
}

void component_driver::request_proc()
{
	if(_ptr_component) {
		while(1)
		{
			try {
			boost::mutex::scoped_lock __lock(_mutex);
			_condition.wait(__lock);

			while(!_mailbox.empty()) {
				_ptr_component->request(_mailbox.front());
				_mailbox.pop();
			}

			boost::this_thread::sleep(boost::posix_time::milliseconds(0));
			}
			catch(thread_interrupted&) {
				break;
			}
		}
	}
}

void component_driver::regist_service_desc()
{
	if(_ptr_component) {
		interface::iprofile* profile = _ptr_component->get_profile();
		if(profile) {
			cout << "service desc : " << profile->get_service_descs() << endl;
			//cout << "profiles : " << profile->get_service_descs()->size() << endl;
			//cossb_broker->regist(_ptr_component, profile->g)
		}
	}
}


} /* namespace dirver */
} /* namespace cossb */
