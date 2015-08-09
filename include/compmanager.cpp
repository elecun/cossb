/**
 * @file		compmanager.cpp
 * @brief		COSSB component manager
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 21
 * @details
 */

#include "manager.hpp"
#include "container.hpp"
#include "broker.hpp"
#include "driver.hpp"
#include <list>

using namespace std;

namespace cossb {
namespace manager {

component_manager::component_manager()
{

}

component_manager::~component_manager()
{
	stop();
	cossb_component_container->destroy();
}

bool component_manager::install(const char* component_name)
{
	if(cossb_component_container->add(component_name, new driver::component_driver(component_name)))
	{
		cossb_component_container->get_driver(component_name)->setup();
		return true;
	}
	return false;
}

types::returntype component_manager::uninstall(const char* component_name)
{
	if(cossb_component_container->exist(component_name))
	{
		cossb_component_container->get_driver(component_name)->stop();
		cossb_component_container->remove(component_name);
		return types::returntype::SUCCESS;
	}
	return types::returntype::FAIL;
}


types::returntype component_manager::run(const char* component_name)
{
	if(cossb_component_container->exist(component_name))
	{
		cossb_component_container->get_driver(component_name)->run();
		return types::returntype::SUCCESS;
	}
	return types::returntype::FAIL;
}

types::returntype component_manager::run()
{
	return types::returntype::SUCCESS;
}

types::returntype component_manager::stop(const char* component_name)
{
	if(cossb_component_container->exist(component_name))
		cossb_component_container->get_driver(component_name)->stop();
	return types::returntype::SUCCESS;
}

types::returntype component_manager::stop()
{
	for(auto itr:cossb_component_container->_container)
		itr.second->stop();
	return types::returntype::SUCCESS;
}

int component_manager::count()
{
	return 0;
}

} /* namespace manager */
} /* namespace cossb */
