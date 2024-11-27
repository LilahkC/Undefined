#pragma once 

#include <vector>
#include <glfw/glfw3.h>

#include "utils/flag.h"

/// <summary>
/// A Class for a graph to moniotor the FPS
/// </summary>
class FPSGraph
{
	STATIC_CLASS(FPSGraph)

public:

	/// <summary>
	/// Display the FPS graph window and update it
	/// </summary>
	static void ShowWindow();
	/// <summary>
	/// Delete the FPS graph
	/// </summary>
	static void Delete();

private:
	/// <summary>
	/// 
	/// </summary>
	static inline size_t mArrayIndex = 0;
	/// <summary>
	/// 
	/// </summary>
	static inline size_t mArraySizeOffset = 5;

	/// <summary>
	/// 
	/// </summary>
	static inline float mLastTotalTime = (float)glfwGetTime();
	/// <summary>
	/// 
	/// </summary>
	static inline float mUpdateTime = 0.1f;
	/// <summary>
	/// Maximum FPS display by the graph
	/// </summary>
	static inline float mMaxFPS = 200;

	/// <summary>
	/// Size of sample we take for the graph
	/// </summary>
	static inline int mArraySize = 50;
	/// <summary>
	/// 
	/// </summary>
	static inline int mMinFpsOffset = 5;
	
	/// <summary>
	/// 
	/// </summary>
	static inline std::vector<float> mFrameRateArray;
};