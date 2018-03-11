#ifndef CURSESOURCE_HPP
#define CURSESOURCE_HPP

#include <vector>
#include <Addon.hpp>
#include <addonsources/addonsource.hpp>

class Curse;

class Curse : public AddonSource {
	public:
		std::vector<Addon> search();
		bool updateDatabase();
	private:
		void downloadArchive();
		void decompressArchive();
};

#endif
