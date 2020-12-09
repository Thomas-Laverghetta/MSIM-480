## Neural Network for Recongizing Handwritting Digits (0-9)
In this program, I create a neural network to recongize handwritten digits using MATLAB's deep-learning toolbox. Where the digits range from 0 to 9. 

The raw data for training, validation, and testing were gathered from https://archive.ics.uci.edu/ml/machine-learning-databases/optdigits/optdigits-orig.windep.Z.

Before running the neural network, raw data was parsed into input and target data files using Python Script. The data parser also allows for users to choose different levels (factor) of data compression for input data. Where each factor is power of 2 of compression. 

# How to Use
To parse raw data, run matlab_data_parser.py's data_parser method. data_parser has two paramters: filename/path and compression factor (default 0). data_parser will generate two files when executed, input and target CSV files. 

To run neural network, run MATLAB's deep-learning toolbox:
```
nnstart
```
within the toolbox, you will select classification, your input and target data, sample profile, and number of hidden neurons. After which, you can execute the neural network and produce confusion matrix and method to input user-defined handwrittings.  