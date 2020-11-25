#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <functional>
#include <string_view>

using ID = std::array<std::uint32_t, 3>;

int ConvertToInt(const std::string& str);
std::vector<std::string> SplitBy(std::string_view what, const std::string& by);
std::string StrToUpper(std::string str);

ID StrToID(std::string_view id_str);

std::string IDToStr(const ID& id);

template <typename T>
size_t ComputeHash(const T& obj) {
	return std::hash<T>{}(obj);
}

struct IDHasher {
	size_t operator()(const ID& key) const;
};

