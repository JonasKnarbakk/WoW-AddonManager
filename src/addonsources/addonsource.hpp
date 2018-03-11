#ifndef ADDONSOURCE_HPP
#define ADDONSOURCE_HPP

#include <vector>
#include <Addon.hpp>

class AddonSource;

class AddonSource {
	public:
		virtual std::vector<Addon> search() = 0;
};

#endif
