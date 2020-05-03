#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#pragma once

#include <string_view>
#include <unordered_map>
#include "IrretrievableResourceException.hpp"

template <typename T> //We using a class template to manage both images and sound
class ResourcesManager
{
public:
	ResourcesManager() = delete;

	static T const& getResource(std::string_view const& pathResource)
	{
		auto resultFind = resources.find(pathResource);
		if (resultFind == std::end(resources))
		{
			if (!resources[pathResource].loadFromFile(pathResource.data()))
			{
				throw IrretrievableResourceException(pathResource);
			}
			return resources[pathResource];
		}
		return resultFind->second;
	}

private:
	static std::unordered_map<std::string_view, T> resources; // Map that associate the path of the asset and the loaded ressource
};

#endif // !RESOURCEMANAGER_HPP
