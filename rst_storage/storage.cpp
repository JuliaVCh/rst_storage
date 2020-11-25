#include <fstream>
#include <filesystem>
#include <shlobj_core.h>

#include "storage.h"
#include "xml_save_load.h"


std::unique_ptr<Storage> Storage::Copy() const
{
	return std::make_unique<Storage>(*this);
}

bool Storage::LoadFromFile()
{
	TCHAR szPath[MAX_PATH + 1];

	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		throw std::runtime_error("");
	}

	auto path = std::filesystem::path(szPath) / "rst_storage";
	std::filesystem::create_directory(path);
	path /= "rst_db.xml";
	db_file_name = path.string();

	std::ifstream is(db_file_name);

	if (is.is_open())
	{
		data = Xml::Load(is);
		return true;
	}

	return false;
}

void Storage::SaveToFile()
{
	std::ofstream os(db_file_name);
	Xml::Save(os, data);
}

bool Storage::AddObj(const ID_STR& id_str, const Name& name)
{
	auto [_, success] = data.insert({StrToID(id_str), name});

	return success;
}

bool Storage::DelObj(const ID_STR& id_str)
{
	const auto success = data.erase(StrToID(id_str));

	return success;
}

const Storage::Name* Storage::GetObj(const ID_STR& id_str) const
{
	if (const auto it = data.find(StrToID(id_str)); it != data.end())
	{
		return &(it->second);
	}

	return nullptr;
}
