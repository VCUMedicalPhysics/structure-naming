# structure-naming
A series of scripts were developed to aid structure naming in the Pinnacle Treatment Planning System. The first script loads a graphical interface that allows user specified names to auto-populate the Pinnacle region of interest list.  This interface shows the master list of TG-263 approved structure names that can be used in Pinnacle. The user can select names from the master list or from a disease site specific list containing a subset of the master name list. Target and non-standard structures can be added using the TG-263 approved nomenclature. 
<br><br>
Once the desired list of structure names is chosen, the names are saved to disk. A secondary Pinnacle script loads the new list of names into the active Pinnacle session and populates the region of interest list. Duplicate names are not inserted.
In addition, each disease site has a script that can simply load all of the TG-263 approved structure names that are deemed commonly used by a radiation oncologist for a given diagnosis. The final script removes all structures from the Pinnacle region of interest list if no contouring was done. This feature provides a quick way to remove unnecessary structures added by the site specific bulk import.

#Installation
To use the structure-naming tool Qt4 will need to be installed. The easiest way to do this is to use the opencsw.org update tool. The following instructions are for installing that tool, getting an updated list of packages, and downloading the Qt4 development files. The structure-naming tool was development using Qt4 version 5.8, as it is the most recent one available from csw.
<br><br>
<b>For Solaris 10/11</b>
<br>As super user/root:
<br>pkgadd -d http://get.opencsw.org/now
<br>pkgutil -U -u -y
<br>/opt/csw/bin/pkgutil -U
<br>/opt/csw/bin/pkgutil -y -i qt4_dev 
<br><br>
You can list the new Qt4 with this command:
<br>/usr/sbin/pkgchk -L CSWqt4-dev
<br><br>Once Qt4 is installed, your environmental variables must be updated. Using the instructions above will but the Qt4 files under /opt/csw/bin and /opt/csw/lib which is not a default location for Solaris. If you are using the c-shell (csh) add the following to your .cshrc file in your home holder:
<br>setenv PATH ${PATH}:/opt/csw/bin 
<br>setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/opt/csw/lib
<br>setenv STRUCTURENAMING "your install directory"
<br><br>
There will be a slightly different process of setting the environmental variables if you use a different shell, but instructions can be found online.
<br><br>
After Qt4 is installed, download and unzip the structure-naming git repository. In the structure-naming directory run the following commands to compile the program:
<br>qmake -project
<br>qmake structure-naming.pro
<br>/usr/ccs/bin/make
<br><br>
For the Solaris 10 machine used for testing this program, the program “make” was installed under /usr/ccs/bin/ but it might be different for other configurations and versions of Solaris.
<br><br>
Finally, you can test the program by running the command to see if it will run
<br>./structure-naming
<br>Once the program is working you will need to install it to your desired location using the command ./install.sh. This will prompt you to give a directory to install the structure-naming program and supporting files. If the specified installation directory already exits, this installer will not continue and instruct you to choose another location as this will not overwrite any existing directory.
<br><br>Next the two Pinnacle scripts need to be set in your HotScripts list to use this program in your Pinnacle session. These scripts are load_structure_naming.Script and add_structures.Script and can be found under {your install directory}/scripts/. Once these scripts are added to your script list you should be able to use the structure-naming tool.

Also, you can ignore the follow error message if you see it in the console when running this program:
"Unable to load library icui18n “Cannot load library icui18n"

#Running the Program
You can start the <b>structure-naming</b> program by running the load_structure_naming.Script. After you run the program and select your structure names, save the new structure list. To load the new structures in your open plan run the add_structures.Script. Any structures in your new list, not already in the current ROI list, will be loaded. For more information about how to use this program please read the attached ClinicalStructureNamingToolUserGuide.pdf file.
