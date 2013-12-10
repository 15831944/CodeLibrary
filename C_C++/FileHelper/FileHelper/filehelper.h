#include <windows.h>

namespace filehp
{

/*
* 检查一个  路径 是否存在（绝对路径、相对路径，文件或文件夹均可）
* 存在则返回 1 (TRUE)
*/
BOOL FilePathExists(LPCTSTR lpPath);

}