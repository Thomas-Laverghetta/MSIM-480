# MSIM480 Data Input Parser
# Thomas J Laverghetta

import csv

# Target dictionary
# maps raw taget data to matlab target data format 
TARGET_DICTIONARY = {
    '0' : '1,0,0,0,0,0,0,0,0,0,',
    '1' : '0,1,0,0,0,0,0,0,0,0,',
    '2' : '0,0,1,0,0,0,0,0,0,0,', 
    '3' : '0,0,0,1,0,0,0,0,0,0,', 
    '4' : '0,0,0,0,1,0,0,0,0,0,', 
    '5' : '0,0,0,0,0,1,0,0,0,0,', 
    '6' : '0,0,0,0,0,0,1,0,0,0,', 
    '7' : '0,0,0,0,0,0,0,1,0,0,', 
    '8' : '0,0,0,0,0,0,0,0,1,0,', 
    '9' : '0,0,0,0,0,0,0,0,0,1,'
}


def data_parser(filename, n=0):
    """ 
    Parses raw data file and outputs two .txt files delimited by commas. 
    Returns a input.txt and target.txt file.

    Will compress input data by factor n where n is a power of 2. 
    """

    infile = open(filename, mode='r')
    lines = infile.read().splitlines()
    infile.close()

    # input reduction
    Sum = 0 
    count = 0
    fact = 2**n

    outfilename = 'input_fact_' + str(n) + '.txt'
    outfile_input = open(outfilename, mode='w')
    outfile_target = open('target_1.txt', mode='w')

    for line in lines:
        # if len == 2, then target
        if len(line) == 2:
            # if target has been found, then end of previous input data
            outfile_input.write('\n')
            # writting target data
            outfile_target.write(TARGET_DICTIONARY[line[1]] + '\n')
        else:
            # else, then input
            # foreach char in line, write char in CSV format to input txt
            for char in line:
                # input reduction (default factor 1)
                count += 1
                Sum += int(char)
                if not (count % fact):
                    outfile_input.write(str(Sum / fact) + ',')
                    Sum = 0

    outfile_input.close()
    outfile_target.close()


def input_reduction(filename, n):
    """ 
    Reduce input_file by a n factor. Factor should be a power of 2.
    File must be in a row format.
    """
    infile = open(filename, mode='r')
    lines = infile.read().splitlines()
    infile.close()

    outfilename = 'input_fact_' + str(n) + '.txt'
    outfile_reduced = open(outfilename, mode='w')

    fact = 2**n
    Sum = 0
    count = 0

    for line in lines:
        for char in line:
            # skip commas
            if char != ',':
                count += 1
                Sum += int(char)
                if not (count % fact):
                    outfile_reduced.write(str(Sum / fact) + ',')
                    Sum = 0
        # end of line
        outfile_reduced.write('\n')

    outfile_reduced.close()
    
if __name__ == "__main__":
    data_parser("data_training/raw_data.txt",4)
    # input_reduction('input_fact_1.txt', 4)