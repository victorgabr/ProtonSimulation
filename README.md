# ProtonSimulation
ProtonSimulation calculates pristine Bragg Peak in a phantom defined using GDML files.

![geometry](https://user-images.githubusercontent.com/6777517/34076063-a55b0aa8-e2c1-11e7-927b-f9782534b38e.jpg)

This code is based on my master's degree [dissertation](http://www1.inca.gov.br/pqrt/download/trab/dissertacao_mestrado_victor_gabriel_leandro_alves.pdf) (in Portuguese with english abstract)

This code uses physics lists implementation from [Hadrontherapy](https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy)

This code used the GDML auxiliary information for associating a sensitive detector to a volume.
The detector construction consists of a call to GDMLProcessor which parses a GDML file and returns the pointer to the world volume.
The scoring volumes use a GDML tag to create indexed dose results for each scoring volume.

This code implemented methods to extract each scorer position at cartesian axis (x,y,z).
More details at DetectorConstruction.cc class, G4PhysicalVolumeModel.cc, [hypernews forum](http://hypernews.slac.stanford.edu/HyperNews/geant4/get/geometry/138/1/1.html)

GDML (Geometry Description Markup Language) is an XML schema for detector description.
To use Geant4 and ProtonSimulation needs to be built with GDML usage on (default true). For more
information please refer to the [GDML website](http://gdml.web.cern.ch/GDML) and
[manual](http://gdml.web.cern.ch/GDML/doc/GDMLmanual.pdf).

This format is widely supported and other geometry software may be able to export
geometry in GDML format.
     The Geant4 GDML parser used in ProtonSimulation dynamically downloads the small
     schema file at run time, so internet access is therefore required to
     load GDML in ProtonSimulation at run time. But you can use the GDML SCHEMA
     located at: /path/to/geant4/source/persistency/gdml/schema/schema.xsd

Take a look at *.gdml file and change line 3.
     
     xsi:noNamespaceSchemaLocation="http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd">
     
     to:
     
     xsi:noNamespaceSchemaLocation="/path/to/geant4/source/persistency/gdml/schema/gdml.xsd">


## Getting Started

On terminal build-folder:

    ./ProtonSimulation G4_WATER_GEOMETRY.gdml output.csv run.mac

It saves output.csv file containing kerma results for each scoring ring.


using python.

     python Geant4Simulation.py G4_WATER_GEOMETRY.gdml output.csv run.mac

requirements: [pandas](https://pandas.pydata.org/) and [matplotlib](https://matplotlib.org/)

it saves and reads output.csv to plot proton pristine Bragg curve:

It saves above figure at build folder.

The file run.mac is based on [GATE proton example](https://dsarrut.gitbooks.io/gate-exercises/content/ex3.html)
Proton source is defined using GEANT4 [GPS](http://nngroup.physics.sunysb.edu/captain/reference/master/detSim/dox/detSimGPS.html)

Run 10000 particles should be enough.

/run/beamOn 10000

Example result:
![g4_water_geometry gdml - 226 mev- pristine bragg peak](https://user-images.githubusercontent.com/6777517/34075937-409cef8a-e2be-11e7-8cdd-fc9772981484.jpg)

### Prerequisites

Build GEANT4 10.x with GDML and QT suport.

See details at [GEANT4 documentation](https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch02.html).

### Installing

Tested on [Lubuntu linux 16.04](http://lubuntu.net/) - The lightest linux distribution I have found.

Use CMakeLists.txt

Easy way:

Try it using [cmake-qt-gui](https://launchpad.net/ubuntu/xenial/+package/cmake-qt-gui)

Hard way:

Building ProtonSimulation with CMake on Unix with Makefiles
We'll assume, for illustration only, that you've copied the ProtonSimulation sources into a directory under your home area so that we have:

    +- /home/you/ProtonSimulation/
       +- CMakeLists.txt
       +- ProtonSimulation.cc
       +- include/
       +- src/
       +- ...

Here, the directory is /home/you/ProtonSimulation, in other words the directory holding the CMakeLists.txt file.

Let's also assume that you have already installed Geant4 in your home area under, for illustration only, /home/you/geant4-install.

Our first step is to create a build directory in which build the example. We will create this alongside our ProtonSimulation source directory as follows:

    $ cd $HOME
    $ mkdir ProtonSimulation-build

We now change to this build directory and run CMake to generate the Makefiles needed to build the B1 application. We pass CMake two arguments:

    $ cd $HOME/ProtonSimulation-build
    $ cmake -DGeant4_DIR=/home/you/geant4-install/lib64/Geant4-10.3.0 $HOME/ProtonSimulation

Here, the first argument points CMake to our install of Geant4. Specifically, it is the directory holding the Geant4Config.cmake file that Geant4 installs to help CMake find and use Geant4. You should of course adapt the value of this variable to the location of your actual Geant4 install. This provides the most specific way to point CMake to the Geant4 install you want to use. You may also use the CMAKE_PREFIX_PATH variable, e.g.

    $ cd $HOME/ProtonSimulation-build
    $ cmake -DCMAKE_PREFIX_PATH=/home/you/geant4-install $HOME/ProtonSimulation

This is most useful for system integrators as it may be extended with paths to the install prefixes of additional required software packages and also may be set as an environment variable that CMake will use at configuration time.

The second argument to CMake is the path to the source directory of the application we want to build. Here it's just the ProtonSimulation directory as discussed earlier. You should of course adapt the value of that variable to where you copied the ProtonSimulation source directory.

Note the Makefile and that all the scripts for running the ProtonSimulation application we're about to build have been copied across. With the Makefile available, we can now build by simply running make:

        $ make -jN

[More details](https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch02s08.html)

##  Comming soon: 

Windows 10, Qt5.10.0, MSVC-2017 64 bits. XercesC XLM Parser and ICU builded from source.  

## Running the tests

TODO: [Test Driven Development](http://agiledata.org/essays/tdd.html)

## Built With

* [CMAKE](https://cmake.org/)
* [QT-Creator](https://en.wikipedia.org/wiki/Qt_Creator)

## Contributing

Any bug fixes or improvements are welcome.

## Versioning

[SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

## Author
    Victor Gabriel Leandro Alves, D.Sc.
    Copyright 2017

## License

This project is licensed under http://geant4.web.cern.ch/geant4/license/LICENSE.html

## Acknowledgments

Thanks to:
* The [GEANT4 developers](http://geant4.web.cern.ch/geant4/collaboration/contacts.shtml)
* The best free C++ IDE if have found: [QT-creator Open Source](https://www.qt.io/download-qt-for-application-development)
* John Purcell's [free C++ course](https://www.udemy.com/free-learn-c-tutorial-beginners/) - It helped me understand a lot of C++ details to refactor this code.
