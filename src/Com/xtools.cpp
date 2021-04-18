#include "xtools.h"

#ifdef  _WIN32
#include <io.h>
#endif //  _WIN32

using namespace std;
COM_API string GetDirData(string path)
{
	string data = "";
#ifdef _WIN32
	// �洢�ļ���Ϣ
	_finddata_t file;
	string dirpath = path + "/*.*";
	// Ŀ¼������
	intptr_t dir = _findfirst(dirpath.c_str(), &file);
	if (dir < 0)
	{
		return data;
	}
	// Ŀ¼��Ϊ��
	do
	{
		// ��������Ŀ¼
		if (file.attrib & _A_SUBDIR) continue;
		char buf[1024] = { 0 };
		sprintf_s(buf, "%s,%u;", file.name, file.size);
		data += buf;
	} while (_findnext(dir, &file) == 0);


#else
#endif // _WIN32

	// ȥ������ĩβ�ķֺ�
	if (!data.empty())
	{
		data = data.substr(0, data.size() - 1);
	}
	return data;
}
