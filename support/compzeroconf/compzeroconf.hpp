/**
 * @file		compzeroconf.hpp
 * @brief		Zero Configuration Component
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 9. 12
 * @details	Zero Configuration component for COSSB
 */

#ifndef SUPPORT_COMPZEROCONF_COMPZEROCONF_HPP_
#define SUPPORT_COMPZEROCONF_COMPZEROCONF_HPP_

#include <interface.hpp>
#include <string>
#include <vector>
#include <map>

using namespace std;

using namespace cossb;
class libzeroconf;
class compzeroconf : public interface::icomponent {
public:
	compzeroconf();
	virtual ~compzeroconf();

	bool setup();
	bool run();
	bool stop();
	void request(cossb::message::message* msg);

private:
	void browse_task();

	void update_services();
	static void on_change();

private:
	libzeroconf* _zeroconf = nullptr;

	/**
	 * @brief	service browse task using Avahi
	 */
	base::task _browse_task = nullptr;

};

COMPONENT_EXPORT

#endif /* SUPPORT_COMPZEROCONF_COMPZEROCONF_HPP_ */
