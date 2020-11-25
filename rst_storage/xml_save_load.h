#pragma once

#include <iostream>
#include <array>
#include <string>
#include <unordered_map>

#include "id_conversion.h"


namespace Xml {

	std::unordered_map<std::array<std::uint32_t, 3>, std::string, IDHasher> Load(std::istream& is);

	void Save(std::ostream& os, const std::unordered_map<std::array<std::uint32_t, 3>, std::string, IDHasher>& data);

}

