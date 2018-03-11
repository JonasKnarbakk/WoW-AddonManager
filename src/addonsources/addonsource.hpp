#ifndef ADDONSOURCE_HPP
#define ADDONSOURCE_HPP

#include <vector>
#include <Addon.hpp>

class AddonSource {
	public:
		virtual std::vector<Addon> search(std::string name) = 0;
		virtual bool updateDatabase() = 0;
};

#endif
