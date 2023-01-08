import sys
import os

if __name__=="__main__":
    print(F"Arguments")
    print(sys.argv)
    args = sys.argv
    file_path = os.getcwd()
    print(F"Path = {file_path}")
    ryem_path = os.path.join(file_path, 'Ryem')
    cmd = F'arduino-cli compile --library "{ryem_path}"'
    if '-v' in args: cmd += ' -v'
    if '--verbose' in args: cmd += ' -v'
    if '-u' in args: cmd += ' -u'
    if '--upload' in args: cmd += ' -u'
    print(F"cmd = {cmd}")
    os.system(cmd)
    #arduino-cli compile --library "C:\Users\cruz_\Documents\emosy_app\Ryem" -v
    #if %1%=="u" (echo "The value of variable c is 15") else (echo "Unknown value") 