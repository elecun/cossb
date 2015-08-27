/**
 * @file		cblog.hpp
 * @brief		COSSB log library (on console stream)
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 8. 13
 * @details	this library will support developer to debug your application
 */

#ifndef _COSSB_SUPPORT_LIBCBLOG_CBLOG_HPP_
#define _COSSB_SUPPORT_LIBCBLOG_CBLOG_HPP_

#include "../../include/interface/ilog.hpp"
#include "../../include/util/localtime.hpp"

namespace cossb {
namespace base {

class cblog : public interface::ilog {
public:
	cblog();
	virtual ~cblog();

	void log(log::loglevel& level, const char* logstr);

private:
	util::systime _time;
};

} /* namespace base */
} /* namespace cossb */

#endif /* _COSSB_SUPPORT_LIBCBLOG_CBLOG_HPP_ */
