

#include "instance.hpp"
#include "manager.hpp"
#include "broker.hpp"
#include "log.hpp"
#include "config.hpp"
#include "configloader.hpp"

#include "arch/singleton.hpp"

namespace cossb {
namespace core {


bool cossb_init(base::config* config)
{
	//1. create instances for system base
	//arch::singleton<manager::system_manager>::get().setup()

	//manager::system_manager::get()->setup(config);


	//2. system authentication from server

	//3. repository check

	//4. access to local database

	//5. start cossb engine



	return true;
}

void cossb_destroy()
{
	//1. disconnect to servers and databases

	//2. destroy all instances
	manager::component_manager::get()->destroy();
	broker::component_broker::get()->destroy();
}

bool cossb_sync()
{
	//1. check accessibility repository

	//2. transfer information to use services

	//3. clean and update all service components

	return true;
}



} /* namespace core */
} /* namespace cossb */
