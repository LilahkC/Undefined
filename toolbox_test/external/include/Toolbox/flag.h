#ifdef TOOLBOX_EXPORT
#define TOOLBOX __declspec(dllexport)
#else
#define TOOLBOX __declspec(dllimport)
#endif