#ifndef CURSESOURCE_HPP
#define CURSESOURCE_HPP

#include <vector>
#include <Addon.hpp>
#include <json.hpp>
#include <addonsources/addonsource.hpp>

class Curse;

class Curse : AddonSource {
	public:
		Curse();
		~Curse();
		virtual std::vector<Addon> search(std::string name) override;
		bool updateDatabase() override;
	private:
		void downloadArchive();
		void decompressArchive();
		Addon parseAddonData(nlohmann::json jsonAddon);
		nlohmann::json getLatestFile(nlohmann::json jsonAddon);

		nlohmann::json jsonData;
};

#endif
