/*
 * client.hpp
 *
 *  Created on: 2015. 10. 3.
 *      Author: hwang
 */

#ifndef INCLUDE_NET_CLIENT_HPP_
#define INCLUDE_NET_CLIENT_HPP_

namespace cossb {
namespace net {

class client {
public:
	client(sockType type);
	virtual ~client();
};

} /* namespace net */
} /* namespace cossb */



#endif /* INCLUDE_NET_CLIENT_HPP_ */
