__author__ = 'fsleeman'

from os.path import isdir, join, exists
from os import system, makedirs, listdir
from shutil import copytree, copy

install_directory = None
structure_naming_placeholder = 'STRUCTURENAMINGPATH'


def main():
    global install_directory
    print('\nPinnacle Scripting System Installer')
    install_directory = raw_input('Structure Naming Install Path: ')
    print(install_directory)

    if isdir(install_directory):
        print('\nDirectory ' + install_directory + ' already exists. Please remove that directory before installing or '
                                                   'choose another directory')
        return
    else:
        print('\nCopying files...')
        if not exists(install_directory):
            makedirs(install_directory)
        copy('install.py', join(install_directory, 'install.py'))
        copy('install.sh', join(install_directory, 'install.sh'))
        copytree('scripts', join(install_directory, 'scripts'))
        copytree('structures', join(install_directory, 'structures')) 
	try:
            copy('structure-naming', install_directory)
        except:
            pass
        try:
            copy('structure-naming-master', install_directory)
        except:
            pass

    updated_save_path = ''
    structure_naming_path = '/tmp/tmp2/scripts'#join(install_directory, '/scripts')
    print(structure_naming_path)
    #for file in listdir(structure_naming_path):
    #    print(file)
    #return

    for element in install_directory:
        if element == '/':
            updated_save_path += '\/'
        else:
            updated_save_path += element
    rename_command = 'find ' + structure_naming_path + ' -type f -name \*.Script -exec perl -pi -e \'s/' + structure_naming_placeholder + '/' \
                 + updated_save_path + '/g\' {} \;'
    system(rename_command)

    #rename_command = 'find ' + structure_naming_path + ' -type f -name install.Store -exec perl -pi -e \'s/' + structure_naming_placeholder + '/' \
    #                 + updated_save_path + '/g\' {} \;'
    #system(rename_command)

    # write store file
    store_file = open(join(structure_naming_path, 'install.Store'), 'w')
    store_file.write('At .STRUCTURENAMING = SimpleString {\n')
    store_file.write('\tString = "' + install_directory + '";\n')
    store_file.write('};')
    store_file.close()

    print('\nFile copy complete, script system is now installed')

    print('\n\nPlease add the following lines to your .cshrc profile:')
    print('\tsetenv PATH ${PATH}:/opt/csw/bin\n')
    print('\tsetenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/opt/csw/lib\n')


if __name__ == "__main__":
    main()
