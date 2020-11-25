#include <string>
#include <utility>
#include <string_view>

#include "xml_save_load.h"

namespace Xml {

	std::pair<std::string_view, std::string_view> Split(std::string_view line, char by) {
		std::size_t pos = line.find(by);
		std::string_view left = line.substr(0, pos);

		if (pos < line.size() && pos + 1 < line.size()) {
			return { left, line.substr(pos + 1) };
		}
		else {
			return { left, std::string_view() };
		}
	}

	std::string_view Lstrip(std::string_view line) {
		while (!line.empty() && std::isspace(line[0])) {
			line.remove_prefix(1);
		}
		return line;
	}

	std::string_view Unquote(std::string_view value) {
		if (!value.empty() && value.front() == '"') {
			value.remove_prefix(1);
		}
		if (!value.empty() && value.back() == '"') {
			value.remove_suffix(1);
		}
		return value;
	}


	std::unordered_map<std::array<std::uint32_t, 3>, std::string, IDHasher> Xml::Load(std::istream& is)
	{
		std::unordered_map<std::array<std::uint32_t, 3>, std::string, IDHasher> data;

		std::string dummy;

		for (size_t i = 0; i < 3; i++)
		{
			std::getline(is, dummy);
		}
		
		for (std::string line; std::getline(is, line) && line[1] != '/';)
		{
			auto [node_name, tail1] = Split(Lstrip(line), ' ');
			auto [attr, tail2] = Split(tail1, '>');
			auto id_str = Unquote(Split(attr, '=').second);
			auto obj_name = Split(tail2, '<').first;

			if (!id_str.empty() && !obj_name.empty())
			{
				data[StrToID(id_str)] = std::string(obj_name);
			}
		}

		return data;
	}

	void Xml::Save(std::ostream& os, const std::unordered_map<std::array<std::uint32_t, 3>, std::string, IDHasher>& data)
	{
		os << "<?xml version=\"1.0\" encoding=\"utf - 8\"?>\n<!DOCTYPE storage>\n";
		os << "<storage>";

		const std::string offset = "  ";
		for (const auto& [id, name] : data)
		{
			os << "\n" << offset << "<obj id=\"" << IDToStr(id) << "\">" << name << "</obj>";
		}

		os << "\n</storage>";
	}
}
