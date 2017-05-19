#
Simple Software Development Kit(SDK) for easy trial of Deeplearning  
For personal use
-------------------------------------------
note  
-------------------------------------------
**Optimizer**    
Adam,  new -> AdaGrad,RMSProp,AdaDelta  

**Initial Weight**  
he : for relu  
xavier : for tanh, sigmoid... 

note  
-------------------------------------------
It is better to use <https://github.com/Sanaxen/Deeplearning> because quality is still bad.  
* The quality of <https://github.com/Sanaxen/Deeplearning> is higher

**Procedure for running example**
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
