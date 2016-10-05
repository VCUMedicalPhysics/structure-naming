# structure-naming
A series of scripts were developed to aid structure naming in the Pinnacle Treatment Planning System. The first script loads a graphical interface that allows user specified names to auto-populate the Pinnacle region of interest list.  This interface shows the master list of TG-263 approved structure names that can be used in Pinnacle. The user can select names from the master list or from a disease site specific list containing a subset of the master name list. Target and non-standard structures can be added using the TG-263 approved nomenclature. 
<br><br>
Once the desired list of structure names is chosen, the names are saved to disk. A secondary Pinnacle script loads the new list of names into the active Pinnacle session and populates the region of interest list. Duplicate names are not inserted.
In addition, each disease site has a script that can simply load all of the TG-263 approved structure names that are deemed commonly used by a radiation oncologist for a given diagnosis. The final script removes all structures from the Pinnacle region of interest list if no contouring was done. This feature provides a quick way to remove unnecessary structures added by the site specific bulk import.

#Installing
To fully use the structure-naming program and associated scripts you will need to install both Qt4 and the structure-naming program.

#Installing Qt4 without Internet Access
If your Pinnacle workstation does not have Internet access you will need to manually install the Qt4-dev package and dependencies. You will need to download the file found here: <b><i>https://github.com/VCUMedicalPhysics/structure-naming/blob/master/qt4/qt4_dev_all_i386.pkg</i></b> and move it to the <b><i>qt4</i></b> subdirectory. If you are using git to get these files you can use the command <b><i>git lfs pull</i></b> command. To do this you will need to install Git Large File Support, information can be found at https://git-lfs.github.com/.

Under the <b><i>qt4</i></b> subdirectory you will find a script named <b><i>install_qt4_dev_i386.sh</i></b>. Run this program with the command <b><i>./install_qt4_dev_i386.sh</i></b> to install the <b><i>qt4_dev_all_i386.pkg</i></b> package. You will need to do this as root or superuser. This will install the necessary programs and libraries for Qt4 under the <b><i>/opt/csw</i></b> directory.

#Installation Qt4 with Internet Access
If you have Internet access on your Pinnacle workstation you can install Qt4 manually or use the opencsw.org update tool. 
opencsw.org is a website that provides precompiled programs and libraries for Solaris including the Qt4 development package. The following instructions are for installing that tool, getting an updated list of packages, and downloading the Qt4 development files. The structure-naming tool was development using Qt4 version 5.8, as it is the most recent one available from csw.

<br>
<b>For Solaris 10/11</b>
<br>As super user/root:
<br><b><i>pkgadd -d http://get.opencsw.org/now
<br>pkgutil -U -u -y
<br>/opt/csw/bin/pkgutil -U
<br>/opt/csw/bin/pkgutil -y -i qt4_dev</i></b>
<br>
You can list the new Qt4 files with this command:
<br><b><i>/usr/sbin/pkgchk -L CSWqt4-dev</i></b>

#After the Qt4 Install
Once Qt4 is installed, your environmental variables must be updated. Using the instructions above will put the Qt4 files under <b></i>/opt/csw/bin</i></b> and <b><i>/opt/csw/lib</i></b> which is not a default location for Solaris. If you are using the c-shell (csh) add the following to your .cshrc file in your home holder:
<br><b>setenv PATH ${PATH}:/opt/csw/bin 
<br>setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/opt/csw/lib
<br>setenv STRUCTURENAMING "your install directory"</b>
<br><br>
The exact variables to set will be provided at the end of the install process. This document assumes you will be using c-shell (csh). There will be a slightly different process of setting the environmental variables if you use a different shell, but instructions can be found online.
<br>
Even if you are using c-shell your environmental variables may need to be set in somewhere other than .cshrc. If that is the case try looking at <b><i>/usr/local/etc/environment</i></b>.

#Installing the structure-naming Package
Once the program is working you will need to install it to your desired location using the command ./install.sh. This will prompt you to give a directory to install the structure-naming program and supporting files. If the specified installation directory already exits, this installer will not continue and instruct you to choose another location as this will not overwrite any existing directory.
<br><br>Next the two Pinnacle scripts need to be set in your HotScripts list to use this program in your Pinnacle session. These scripts are <b><i>load_structure_naming</i></b>.Script and <b><i>add_structures.Script</i></b> and can be found under <b><i>{your install directory}/scripts/</i></b>. Once these scripts are added to your script list you should be able to use the structure-naming tool.

Also, you can ignore the follow error message about loading the icui18n library.

<b>Please note that your zip file and resulting files will have names like structure-naming-master instead of structure-naming. This is becuase you are downloading the zip file from the master code branch. The names will be just structure-naming if you are cloning from this repository (see below).</b>

#Running the Program
To load this program from Pinnacle you will need to add <b><i>load_structure_naming.Script</i></b> and <b><i>add_structures.Script</i></b> to your HotScripts list. These files can be found in the scripts directory under the previously specified installation location. The first script will load the GUI and will allow you to save the new structure list. Once the structure names are saved and the GUI is closed, run <b><i>add_structures.Script</i></b>. Any structures in your new list, not already in the current ROI list, will be loaded. For more information about how to use this program please read the attached ClinicalStructureNamingToolUserGuide.pdf file.

#Compiling structure-naming Manually
To compile the structure-naming program use the following commands in the main structure-naming(-master) directory:
<br><b>/opt/csw/bin/qmake -project</b> # this create the .pro file
<br><b>/opt/csw/bin/qmake structure-naming.pro</b> or <b>/opt/csw/bin/qmake structure-naming-master.pro</b> # creates Makefile
<br><b>/usr/ccs/bin/make<b> # compiles the program
<br><br>
You may be able to avoid using the full paths for make/qmake programs if you add the paths to your PATH environmental variable.





