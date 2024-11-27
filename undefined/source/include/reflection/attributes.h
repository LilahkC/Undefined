#pragma once

#include <refl.hpp>
#include <toolbox/Vector2.h>
#include <imgui/imgui.h>

using FieldAttribute = refl::attr::usage::field;

struct DontDisplayName : FieldAttribute
{
};

struct SameLine : FieldAttribute
{
};

struct Spacing : FieldAttribute
{
	ImVec2 size;

	constexpr Spacing(const ImVec2 s)
		: size(s) {}
};

struct ToDeg : FieldAttribute
{
};

struct HideInInspector : FieldAttribute
{
};

template <typename T>
struct NotifyChange : FieldAttribute
{
	using PtrT = bool T::*;

	PtrT ptr;

	constexpr NotifyChange(const PtrT p)
		: ptr(p) {}
};

template <typename T>
struct Callback : FieldAttribute
{
	using FuncT = void (T::*)();

	FuncT func;

	constexpr Callback(const FuncT f)
		: func(f) {}
};

template <typename DescriptorT, typename AttributeT>
constexpr bool HasAttribute()
{
	return refl::descriptor::has_attribute<AttributeT>(DescriptorT{});
}

template <typename DescriptorT, typename AttributeT>
constexpr AttributeT GetAttribute()
{
	return refl::descriptor::get_attribute<AttributeT>(DescriptorT{});
}
