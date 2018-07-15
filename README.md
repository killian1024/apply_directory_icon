### apply_directory_icons ###

apply_directory_icons is a simple C++ program whose propose is apply pictures as directories icons 
recursively editing the GNOME Virtual file system directories metatada. For more information use 
`./apply_directory_icons --help` command.

### Build ###

Use the folowing commands to buil the CMake project.

    cmake .
    cmake --build .

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following libraries are necessary:
- speed (https://github.com/killian1024/speed)
- libglib-2.0
- libgio-2.0
