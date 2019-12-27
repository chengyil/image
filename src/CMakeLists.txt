cmake_minimum_required(VERSION 3.1)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

PROJECT(feature)

######################## EDIT IF REQUIRED ####################
# ### If CMake is not able to find the path to OpenCV installation then Uncomment the line below and specify the path to OpenCV directory (i.e. the path to the OpenCVConfig.cmake file). Check the examples given below.
#SET(OpenCV_DIR Enter-the-path-of-OpenCV-installation-on-your-system)
SET(OpenCV_DIR /root/installation/OpenCV-3.4.4/share/OpenCV)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY /root/image/bin)

################### OpenCV_DIR Examples  #####################

### MACOS : /usr/local/Cellar/opencv/3.3.1_1/share/OpenCV/

### UBUNTU : /usr/local/share/OpenCV/

### WINDOWS : C:\Users\yourname\Documents\opencv-3.3.1\build\install

##############################################################




###################   ***DO NOT EDIT***  #####################

#############  Common Instructions for all Users  ############
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})
include(/root/common/dlib/dlib/cmake)

MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} dlib::dlib ${OpenCV_LIBS})
ENDMACRO()

MACRO(add_exe name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name})
ENDMACRO()


add_example(goldenRatio)
add_exe(tree)
