import re
import sys
import os

SOURCES = 'bin data *.py mmseg-cpp/{*.h,*.cpp,build.py} README'
if sys.platform == 'win32':
    BINARIES = '*.pyc mmseg-cpp/mmseg.dll'
else:
    BINARIES = '*.pyc mmseg-cpp/mmseg.so'

def do_cmd(cmd):
    print cmd
    os.system(cmd)

def adjust_path(files):
    return ' '.join(['pymmseg/%s' % f
                     for f in files.split(' ')])

def release_source(version):
    do_cmd('(cd .. && tar czf pymmseg-cpp-src-%s.tar.gz %s)' %
           (version, adjust_path(SOURCES)))

def build():
    do_cmd("""python -c 'import compileall as c; c.compile_dir(".", 1)'""")
    do_cmd("(cd mmseg-cpp && python build.py)")

def release_binary(version):
    build()
    do_cmd("(cd .. && tar czf pymmseg-cpp-linux-i386-%s.tar.gz %s %s)" %
           (version, adjust_path(SOURCES), adjust_path(BINARIES)))

def release(version):
    release_source(version)
    release_binary(version)

def die(msg):
    print msg
    sys.exit(1)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        die('Usage: %s version' % __file__)
    version = sys.argv[1]
    if not re.match(r'\d+\.\d+\.\d+', version):
        die('version should be of the form X.Y.Z, bug got %s' % version)

    release(version)

