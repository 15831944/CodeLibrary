#include "loger.h"

void InitLoger()
{
    FLAGS_log_dir = "./log/";
    FLAGS_minloglevel = 0;
    FLAGS_max_log_size = 10;

    google::GetLoggingDirectories();
    google::InitGoogleLogging(FILE_LOG_NAME);
}