
'''
import sys
import requests


if 2==len(sys.argv) :
    responed=requests.post('http://tool.lu/pyc/pycupload.html',cookies=cookies,files={'pyfile':('test.pyc',open(sys.argv[1],'rb'),'application/octet-stream')})

    if 200==responed.status_code :
        if not -1==responed.text.find('\u4e0a\u4f20\u51fa\u9519') :
            print responed.text
        else :  #  upload error
            print 'Upload ERROR ..'
else :
    print 'take a .pyc path ..'

'''

import os
import sys


def decrypt_pyc(pyc_file) :
    os.system(get_py_dir()+'\\python_decrypt.exe '+pyc_file)

def get_py_dir() :
    return sys.argv[0][:sys.argv[0].rfind('\\')]

def get_decrypt_output_dir() :
    return get_py_dir()+'\\decrypt_output'

def get_dir_files(file_dir_path) :
    return_dirs=[]
    
    for root,dirs,files in os.walk(file_dir_path) :
        for files_index in files :
            return_dirs.append(root+'\\'+files_index)
            
    return return_dirs

'''
if 2==len(sys.argv) :
    if os.path.exists(sys.argv[1]) :
        if not os.path.exists(get_decrypt_output_dir()) :
            os.mkdir(get_decrypt_output_dir())
            
        os.chdir(get_decrypt_output_dir())
            
        if os.path.isdir(sys.argv[1]) :
            files=get_dir_files(sys.argv[1])
            
            if len(files) :
                for file_index in files :
                    decrypt_pyc(file_index)
        else :
            decrypt_pyc(sys.argv[1])
            
'''            
'''
output_file=open(get_py_dir()+'\\output_files.txt','w')
c_type_array='unsigned char decrypt_list[]={\r\n'

for index in get_dir_files(get_py_dir()+'\\output') :
    c_type_array+='    "'+index+'" ,\r\n'

c_type_array+='};'
    
output_file.write(c_type_array)
output_file.close()

'''


poc=get_dir_files(get_py_dir()+'\\output')

for poc_index in poc :
    if -1==poc_index.find('pyc.decrypt.pyc') :
        os.remove(poc_index)
#    os.remove(poc_index)


