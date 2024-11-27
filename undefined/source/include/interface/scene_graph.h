#pragma once

#include <toolbox/Vector3.h>

#include "utils/flag.h"

class Object;

class SceneGraph
{
	STATIC_CLASS(SceneGraph)

public:
	static void DisplayWindow();

	static void Delete();

private:
	static void DisplayActualScene();
	static void DisplayObject(Object* object, unsigned int labelID);

	static void NodeInteraction(Object* object);

	static void ClickSelectObject(Object* object);
	static bool RightClickObject(Object* object);

	static void BeginDragObject(Object* object);
	static bool BeginDropOnObject(Object* object, bool setBefore = false);

	static void RenameObject(Object* object);
	static inline Object* mRenamingObject;

	static inline Object* mSelectedObject;

	static inline Vector3 mBaseCamPos = Vector3();
	static inline bool mLerpCam = false;
	static inline float mCamTravelTime = 0;

	static inline std::string loadString = "";
	static inline bool startLoad = false;
};