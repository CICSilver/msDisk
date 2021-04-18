#include "xtools.h"

#ifdef  _WIN32
#include <io.h>
#endif //  _WIN32

using namespace std;
COM_API string GetDirData(string path)
{
	string data = "";
#ifdef _WIN32
	// 存储文件信息
	_finddata_t file;
	string dirpath = path + "/*.*";
	// 目录上下文
	intptr_t dir = _findfirst(dirpath.c_str(), &file);
	if (dir < 0)
	{
		return data;
	}
	// 目录不为空
	do
	{
		// 不处理子目录
		if (file.attrib & _A_SUBDIR) continue;
		char buf[1024] = { 0 };
		sprintf_s(buf, "%s,%u;", file.name, file.size);
		data += buf;
	} while (_findnext(dir, &file) == 0);


#else
#endif // _WIN32

	// 去除正文末尾的分号
	if (!data.empty())
	{
		data = data.substr(0, data.size() - 1);
	}
	return data;
}
