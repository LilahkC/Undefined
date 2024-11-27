#pragma once
#include "utils/flag.h"

#include <vector>
#include <numeric>


/// <summary>
/// Contains multiple functions usefull
/// </summary>
namespace Utils
{
	/// <summary>
	/// Do the average of an array
	/// </summary>
	/// <typeparam name="Type">: Type of the array</typeparam>
	/// <param name="array">: Array you want to average</param>
	/// <returns>Return the average of the array</returns>
	template<typename Type>
	Type Average(const std::vector<Type>& array)
	{
		return std::reduce(array.begin(), array.end()) / array.size();
	}
	
	/// <summary>
	/// Function to use when you want to verify if something is equal to zero without being disturbed by the float precision
	/// </summary>
	/// <param name="value">: The value you want to verify</param>
	/// <returns>Return either true if it is equal to zero or false</returns>
	constexpr bool IsZero(float value) { return (value <= 1e-6f); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="value"></param>
	/// <param name="lastValue"></param>
	/// <param name="interval"></param>
	/// <returns></returns>
	constexpr bool OnInterval(float value, float lastValue, float interval) { return Utils::IsZero(interval) || ((int)(lastValue / interval) != (int)(value / interval)); }

	/// <summary>
	/// Convert an int to a pointer
	/// </summary>
	/// <typeparam name="PtrT"></typeparam>
	/// <typeparam name="IntT"></typeparam>
	/// <param name="number">: The int you want to convert into pointer</param>
	/// <returns>Return the pointer from the int given</returns>
	template <typename PtrT, typename IntT>
	constexpr PtrT IntToPointer(const IntT number)
	{
		static_assert(std::is_pointer_v<PtrT>, "PtrT must be a raw pointer type, ex: PtrT=int*");
		static_assert(std::is_integral_v<IntT>, "IntT must be an integral type, ex: IntT=int");

		return reinterpret_cast<PtrT>(reinterpret_cast<uint8_t*>(1) + static_cast<const size_t>(number) - 1);
	}
};
