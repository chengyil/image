# Prerequisite
- docker version 19.03.4
- docker-compose version 1.24.0
- Linux environment

# Setup
``` clone repo
git clone git@github.com:chengyil/image.git
```
```
* build
```shell
cd image
bin/build && bin/make
```
```
:~/image# bin/build && bin/make
WARNING: The DISPLAY variable is not set. Defaulting to a blank string.
-- Searching for BLAS and LAPACK
-- Checking for module 'cblas'
--   No package 'cblas' found
-- Found OpenBLAS library
-- Found LAPACK library
-- Configuring done
-- Generating done
-- Build files have been written to: /root/image/src/build
WARNING: The DISPLAY variable is not set. Defaulting to a blank string.
[  3%] Built target tree
[ 96%] Built target dlib
Scanning dependencies of target goldenRatio
[ 98%] Building CXX object CMakeFiles/goldenRatio.dir/goldenRatio.cpp.o
[100%] Linking CXX executable /root/image/bin/goldenRatio
[100%] Built target goldenRatio
```

# Run GoldenRatio
Note that the image file has to be in fixtures/images
result will be saved into result folder
```shell
cd image
./bin/executeGoldenRatio test.jpg
```
```
~/image# ./bin/executeGoldenRatio test.jpg
WARNING: The DISPLAY variable is not set. Defaulting to a blank string.
Number of faces detected: 1
Golden Ratio : 0.7
Saving output image to ../result/test.jpg
```

# Run Tree
```shell
cd image 
bin/tree
```
```
:~/image# bin/tree      
building 0                                      
building 1                                      
building 4                                      
building 5                                      
building 6                                      
building 2                                      
building 7                                      
building 8                                      
building 9                                      
building 3                                      
building 10                                     
building 11                                     
building 12                                     
Current val : 0                                 
Current val : 1                                 
Current val : 2                                 
Current val : 3                                 
Current val : 4                                 
Current val : 5                                 
Current val : 6                                 
Current val : 7                                 
Current val : 8                                 
Current val : 9                                 
Current val : 10                                
Current val : 11                                
Current val : 12                                
```
