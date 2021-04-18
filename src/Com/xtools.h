#pragma once
#ifdef _WIN32
#ifdef COM_EXPORTS
#define COM_API __declspec(dllexport)
#else
#define COM_API __declspec(dllimport)
#endif
#else
#define COM_API
#endif
#include <string>
COM_API std::string GetDirData(std::string path);


