copy ..\..\..\bin\*.dll .\\Release

del Learned*.*
.\Release\mnist_test3.exe ..\..\..\dataset\mnist
