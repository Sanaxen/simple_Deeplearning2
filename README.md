#
Simple Software Development Kit(SDK) for easy trial of Deeplearning  
For personal use

## What's New
- 2017/8/07 CIFAR10 **76%**  
- 2017/7/23 MNIST **99%**  
- 2017/6/02 Optimizer SMORMS3	However, it is not working properly  
- 2017/6/02 Add Automatic augmentation image data API  
- 2017/5/30 Optimizer SGD  
- 2017/5/24 Optimizer AdaGrad,RMSProp,AdaDelta  

-------------------------------------------
note  
-------------------------------------------
- real time Plotting  (gnuplot)

You need gnuplot to display the graph.  
- <img src="https://github.com/Sanaxen/simple_Deeplearning2/blob/master/image/plot.png"/>  
- <img src="https://github.com/Sanaxen/simple_Deeplearning2/blob/master/image/accuracy_plot.png"/>  

- Initial Weight he : for relu  xavier : for tanh, sigmoid... 


note  
-------------------------------------------
It is better to use <https://github.com/Sanaxen/Deeplearning> because quality is still bad.  
* The quality of <https://github.com/Sanaxen/Deeplearning> is higher

**Procedure for running example MNIST**
-------------------------------

**STEP 1**
----------------
Installing Visual Studio 2012 Update 4 Visual C ++ Redistributable Package  
us <https://www.microsoft.com/en-us/download/details.aspx?id=30679>   
jp <https://www.microsoft.com/ja-jp/download/details.aspx?id=30679>

**STEP 2**
----------------
Download dataset<http://yann.lecun.com/exdb/mnist/>  
The following four are downloaded (image data set of handwritten characters) totaling 60 thousands.

Train-images-idx3-ubyte.gz: training set images (9912422 bytes)  
Train-labels-idx1-ubyte.gz: training set labels (28881 bytes)  
T10k-images-idx3-ubyte.gz: test set images (1648877 bytes)  
T10k-labels-idx1-ubyte.gz: test set labels (4542 bytes)

Extract each (file name as below)

T10k-images.idx3-ubyte  
T10k-labels.idx1-ubyte  
Train-images.idx3-ubyte  
Train-labels.idx1-ubyte  

* If the extension is not in the proper state at the time of decompression, change the file name so that it becomes the above name.

(Example) t10k-images-idx3-ubyte -> t10k-images.idx3-ubyte

Move these 4 files to the **dataset\mnist** folder Create the following directory under **dataset\mnist**  
**train**   
**test**

**STEP 3**
----------------
Move to **example\mnist_test3\x64**    

run clear_test.bat.  
It takes time to move the data set to the image only for the first time.
