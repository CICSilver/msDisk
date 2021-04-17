// Com.cpp : 定义 DLL 的导出函数。
//

#include "framework.h"
#include "Com.h"


// 这是导出变量的一个示例
COM_API int nCom=0;

// 这是导出函数的一个示例。
COM_API int fnCom(void)
{
    return 0;
}

// 这是已导出类的构造函数。
CCom::CCom()
{
    return;
}
