copy ..\..\..\bin\*.dll .\\Release


del cifar10.model

.\Release\cifar10_test.exe ..\..\..\dataset\cifar10
