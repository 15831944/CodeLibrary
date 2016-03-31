# -*- coding:utf-8 -*-
import re
import os
import sys
import time
import StringIO

def file_proc(path):
    file_name_len = 0
    for file_name in os.listdir(path):
        file_name_path = os.path.join(path, file_name)
        #若为目录，则不处理
        if os.path.isdir(file_name_path):
            continue

        if file_name_len < len(file_name):
            file_name_len = len(file_name)

        file_name_new = file_name
        file_name_new = file_name_new.replace(' ', '')
        if file_name_new != file_name:
            file_name_path_new = os.path.join(path, file_name_new)

            try:
                os.rename(file_name_path, file_name_path_new)
            except:
                print('file_name_path_new:%s.file_name_path=%s.' % (file_name_path_new, file_name_path))

    print file_name_len            

proc_dir = 'd:\\BaiduYunPan\\Kindle\\CalibreLibrary'
start = time.time()
print('proc_dir:%s' % proc_dir)
file_proc(proc_dir)
c = time.time() - start
print('as cost time:%0.2f'%(c))
