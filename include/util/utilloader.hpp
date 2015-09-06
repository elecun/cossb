/**
 * @file		utilloader.cpp
 * @brief		Utility library loader
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 9. 6
 * @details	cossb utility loader class and it derived from iutility class
 */

#ifndef _COSSB_UTIL_LOADER_HPP_
#define _COSSB_UTIL_LOADER_HPP_

#include "../interface/ilaunch.hpp"

namespace cossb {
namespace util {

class utilloader : public interface::iutility {
public:
	utilloader();
	virtual ~utilloader();

	/**
	 * @brief	launch interface
	 */
	bool launch(const char* utilname, int argc, char* argv[]);

private:
	void* _util_handle = nullptr;
};

} /* namespace util */
} /* namespace cossb */

#endif /* _COSSB_UTIL_LOADER_HPP_ */
