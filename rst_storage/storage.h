#pragma once

#include <unordered_map>
#include <array>
#include <cstdint>
#include <string>
#include <memory>

#include "id_conversion.h"


class Storage {
public:
	using ID_STR = std::string;
	using Name = std::string;

	std::unique_ptr<Storage> Copy() const;

	bool LoadFromFile();
	void SaveToFile();

	bool AddObj(const ID_STR& id_str, const Name& name);
	bool DelObj(const ID_STR& id_str);
	const Name* GetObj(const ID_STR& id_str) const;
	const std::unordered_map<ID, Name, IDHasher>& GetData() const noexcept { return data; }

private:
	std::string db_file_name;
	std::unordered_map<ID, Name, IDHasher> data;
};

