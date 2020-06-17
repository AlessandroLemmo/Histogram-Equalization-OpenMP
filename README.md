# Histogram-Equalization-OpenMP

The aim of the project is to compare two different executions of histogram equalization made with parallel programming modes CUDA and OpenMP.

This repository reports the project developed with OpenMP. The CUDA implementation and the results of both applications compared is available at the following link:
[https://github.com/AlessandroLemmo/Histogram-Equalization-CUDA](https://github.com/AlessandroLemmo/Histogram-Equalization-CUDA)

The project was developed with the operating system Windows and the IDE Visual Studio. Require the installation of OpenCV. For replicate the result download the project, and compute the following steps:

1. Go in OpenCV installation folder -> build-> x64 -> vc14 -> bin -> copy the files opencv_world420.dll, opencv_world420d.dll
2. Paste it in the HistogramEqualizationOpenCV folder of the project
3. Open Visual Studio -> Click on File -> Open -> Project/Solution
4. Select the .sln project file and click to Open

If you want to replicate the same project from zero compute the following steps:

1. Create new CUDA project in Visual Studio
2. Go in OpenCV installation folder -> build-> x64 -> vc14 -> bin -> copy the files opencv_world420.dll, opencv_world420d.dll
3. Paste it in the HistogramEqualizationCUDA folder of the project
4. Configuration Manager -> Active solution configuration -> Release
5. Configuration Manager -> Active solution platform -> x64
7. Properties -> Tab VC++ Directories -> Include Directories -> C:\opencv\build\include (if opencv is installed in disk C)
8. Propertirs -> Tab C/C++ -> Language -> OpenMP support -> set Yes
9. Propertirs -> Tab C/C++ -> Command Line -> Additional Options -> add /Zc:twoPhase- 
10. Properties -> Tab Linker -> Sub-Tab General -> Additional Library Directories -> C:\opencv\build\x64\vc14\lib (if opencv is installed in disk C)
11. Properties -> Tab Linker -> Sub-Tab Input -> Additional Dependencies -> opencv_world420.lib, opencv_world420d.lib 

