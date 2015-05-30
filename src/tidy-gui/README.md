# Tidy GUI2

There had been some previous attempts at a Tidy GUI. Some had used the Tidy binary as an external app, which is ok but not totally satisfying. Others used a built-in tidy library, which of course ages, and others were not cross-platform.

This project uses Qt to create a cross-platform application, and uses an external libtidy some can be kept up-to-date. Additionally, user a perl script to generate all the config pages, so can be easily kept up-to-date as features are added or removed from libtidy source.

#### Dependencies

##### Qt

Qt can be downloaded from https://www.qt.io/download/. As well as offering 'enterprise' versions, there is also a 'free' community version. This was built against Qt4, but should not be too much effort to use later versions.

##### CMake

While Qt has is own 'qmake' system, and a Qt Designer GUI IDE, this project is built using CMake, from Kitware - http://www.cmake.org/ - which is also a robust cross-platform build file generator. In each platform is supports a range of 'native' build systems. In unix this include the 'standard' Unix Makefiles, Ninja, etc... In windows this includes many versions of MSVC, MinGW, MSys, etc... And others in the MAC...

##### LibTidy

And of course is dependant on library Tidy.

#### Building

##### Unix

Using the native make build tool, this can be as simple as -

 1. cd tidy-gui2
 2. cmake .
 3. make
 4. [sudo] make install
 
 #### Windows
 
 If a version of MSVC is installed, this again this can be a simple -
  
  1. cd tidy-gui2
  2. cmake .
  3. cmake --build . --config Release
  4. copy the Release\tidy-gui2.exe to a folder in your path
  5. if by chance you are using the DLL version of libtidy, then the DLL must also be copied.

There is also a CMake GUI which can be used instead of the above CLI.

20150530

; eof
