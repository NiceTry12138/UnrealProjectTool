#ifndef COMMONDATA_H
#define COMMONDATA_H

enum class EErrorType : int
{
    E_NONE,                         // 没有错误
    E_SAMETYPE,                     // 存在相同类型
    E_ERRORCONTENT,                 // 错误的输入内容
    E_NOEXISTS,                     // 不存在的项目
};


#endif // COMMONDATA_H
