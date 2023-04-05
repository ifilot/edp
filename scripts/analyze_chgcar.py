# -*- coding: utf-8 -*-

import os
import numpy as np

def main():
    fname = os.path.join(os.path.dirname(__file__),
                         '..', 'src', 'test', 'testinput', 'CHGCAR_CH4')
    data = read_charge_file(fname)
    
    print('Min: %f' % np.min(data))
    print('Max: %f' % np.max(data))
    
    # get value center slice
    print('Sum xz slice: %f' % (0.5 * np.sum(data[:,49,:]) + 
                                0.5 * np.sum(data[:,50,:])))
    
def read_charge_file(filename):
    """
    Get atomic data from DDEC6 charge file
    """
    f = open(filename, 'r')
    for i in range(0,6):
        f.readline()
    nratoms = np.sum([int(i) for i in f.readline().strip().split()])
    
    for i in range(0,nratoms+2):
        f.readline()
    
    shape = [int(i) for i in f.readline().strip().split()]
    
    data = []
    while True:
        line = f.readline().strip()
        for v in line.split():
            data.append(float(v))
        if line == '' or len(data) >= np.prod(shape):
            break
    
    f.close()
    
    return np.array(data).reshape(shape)
    
if __name__ == '__main__':
    main()