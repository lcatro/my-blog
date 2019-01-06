
import os


def read_file(path) :
    file = open(path)
    data = file.read()
    
    file.close()
    
    return data
    
def write_file(path,data) :
    file = open(path,'w')
    
    file.write(data)
    file.close()
    

current_path = os.getcwd()

for year_index in os.listdir('.') :
    for markdown_index in os.listdir('./%s/' % (year_index)) :
        path = './%s/%s' % (year_index,markdown_index)
        data = read_file(path).replace(current_path)
            
        write_file(path,data)
