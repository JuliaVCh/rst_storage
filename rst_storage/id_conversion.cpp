#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <cctype>
#include <charconv>

#include "id_conversion.h"


int ConvertToInt(const std::string& str) {
	int result;
	const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

	if (ec != std::errc()) {
		throw std::invalid_argument(str);
	}

	return result;
}

std::vector<std::string> SplitBy(std::string_view what, const std::string& by) {
	std::vector<std::string> result;

	auto pos = what.find(by);
	while (what.size() > 0)
	{
		const auto part = what.substr(0, pos);
		if (!part.empty())
		{
			result.push_back(std::string(part));
		}

		if (pos == std::string_view::npos)
		{
			break;
		}
		
		pos = part.size() + by.size();
		what.remove_prefix(pos);
		pos = what.find(by);
	}

	return result;
}


std::string StrToUpper(std::string str)
{
	for (auto& ch : str)
	{
		if ('a' <= ch && ch <= 'f')
		{
			ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
		}
	}

	return str;
}

const constexpr int id_batch_size = sizeof(ID::value_type) * 2;
const constexpr int BASE = 16;

ID StrToID(std::string_view id_str)
{
	ID result;

	for (size_t i = 0; i < result.size(); i++)
	{
		auto& part = result[i];
		int power = 0;

		for (const auto& ch : id_str.substr(i * id_batch_size, id_batch_size))
		{
			part *= BASE;

			if ('A' <= ch && ch <= 'F')
			{
				part += ch - 'A' + 0xA;
			}
			else if ('a' <= ch && ch <= 'f')
			{
				part += ch - 'a' + 0xA;
			}
			else
			{
				part += ch - '0';
			}
		}
	}

	return result;
}

std::string IDToStr(const ID& id)
{
	std::string result;
	result.reserve(24);

	for (auto part : id)
	{
		for (size_t i = 0; i < id_batch_size; i++)
		{
			auto digit = part >> (id_batch_size - 1) * 4;
			char ch = digit < 0xA ? digit + '0' : digit + 'A' - 0xA;
			result.append(1, ch);
			part <<= 4;
		}

		//std::ostringstream os;

		//os << std::hex << part;
		//auto part_str = os.str();

		//result += std::string(id_batch_size - part_str.size(), '0') + std::move(part_str);
	}
	
	return result;
}

size_t IDHasher::operator()(const ID& key) const
{
	size_t result = ComputeHash(key[0]);
	static constexpr size_t POWER = 4'294'967'291;

	for (size_t i = 1; i < key.size(); i++)
	{
		result += result * POWER;
	}

	return result;
}
