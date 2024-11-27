#include "resources/resource_manager.h"

#include <vector>

#include "Resources/model.h"
#include "Resources/texture.h"
#include "Resources/shader.h"
#include "resources/audio.h"


void ResourceManager::Load(const std::filesystem::path& path, bool recursivity)
{
	std::vector<std::string> mShader;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		for (int i = 0; i < mPathToIgnore.size(); i++)
		{
			if (entry.path().generic_string() == mPathToIgnore[i])
			{
				return;
			}
		}

		if (recursivity)
		{
			if (std::filesystem::is_directory(entry))
			{
				Load(entry.path().string() + "/", true);
			}
		}

		std::string mName = entry.path().string();
		std::string mFilename = entry.path().filename().generic_string();
		std::string parentName = entry.path().parent_path().filename().string();
		size_t pos = mName.find(parentName);
		std::string newName = mName.substr(pos);

		if (mName.ends_with(".obj"))
		{
			Create<Model>(newName, mName.c_str());
		}

		else if (mName.ends_with(".png") || mName.ends_with(".jpg"))
		{
			Create<Texture>(newName, mName.c_str());
		}

		else if (mName.ends_with(".wav"))
		{
			Create<Audio>(newName, mName.c_str());
		}

		else if (mName.ends_with(".fs"))
		{
			mShader.push_back(entry.path().generic_string());
		}

		else if (mName.ends_with(".vs"))
		{
			if (mShader.size() != 0)
			{
				std::string fragShaderName = entry.path().generic_string();
				fragShaderName.resize(fragShaderName.size() - 2);
				fragShaderName += "fs";
				if (std::find(mShader.begin(), mShader.end(), fragShaderName) != mShader.end())
				{
					mFilename.resize(mFilename.size() - 3);
					Create<Shader>(mFilename, mName.c_str(), fragShaderName.c_str());
					mShader.erase(std::find(mShader.begin(), mShader.end(), fragShaderName));
				}
			}
		}
	}
}

bool ResourceManager::Contains(const std::string& mName)
{
	if (mResources.find(mName) != mResources.end())
	{
		return true;
	}
		
	Logger::Warning("The resource manager does not contain : {}", mName);
	return false;
}

void ResourceManager::Unload(const std::string& mName)
{
	mResources[mName].reset();
	mResources.erase(mName);

	Logger::Info("{} unloaded", mName);
}

void ResourceManager::UnloadAll()
{
	for (auto&& p : mResources)
	{
		Logger::Info("{} {} unloaded", typeid(*p.second.get()).name(), p.first);
	}
	mResources.clear();
}

void ResourceManager::Rename(const std::string& oldName, const std::string& newName)
{
	auto&& p = mResources.find(oldName);

	if (p != mResources.end())
	{
		mResources.emplace(newName, p->second);

		if (p->second->IsValid())
		{
			Logger::Info("Resource {} renamed to {}", oldName, newName);
		}

		mResources.erase(p);
	}
	else
	{
		Logger::Warning("Resource {} not found", oldName);
	}
}


void ResourceManager::RenameFolder(const std::string& oldName, const std::string& newName)
{
	std::vector<std::pair<std::string, std::shared_ptr<Resource>>> test;
	for (auto& it : mResources)
	{
		if (it.first.find(oldName) == 0)
		{
			test.push_back(it);
		}
	}

	for (auto& it : test)
	{
		auto node = mResources.extract(it.first);
		node.key() = newName + it.first.substr(it.first.find_last_of("/"));

		mResources.insert(std::move(node));
	}
}
