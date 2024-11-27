#pragma once
#include <vector>

#include "utils/flag.h"

#include <reflection/attributes.h>

/// <summary>
/// Base Class for all our Resources
/// </summary>
class Resource
{
public:
	/// <summary>
	/// Constructor of Resource
	/// </summary>
	UNDEFINED_ENGINE Resource();
	/// <summary>
	/// Destructor of Resource (can be override)
	/// </summary>
	UNDEFINED_ENGINE virtual ~Resource();
	/// <summary>
	/// Check if the Resource is valid
	/// </summary>
	/// <returns>Return either true if it is valid or false</returns>
	UNDEFINED_ENGINE virtual bool IsValid();

	std::string Name;
};

REFL_AUTO(type(Resource),
	field(Name, HideInInspector())
);