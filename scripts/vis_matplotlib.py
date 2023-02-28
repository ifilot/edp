# -*- coding: utf-8 -*-

import struct
import numpy as np
import matplotlib.pyplot as plt

# open data file
f = open('planedata-real.bin', 'rb')

# capture dimensions of data
nx = struct.unpack('I', f.read(4))[0]
ny = struct.unpack('I', f.read(4))[0]

# read data
ip = struct.iter_unpack('f', f.read(nx*ny*4))

# unpack data
data = np.array([d[0] for d in ip])
data.resize(ny,nx)

# close file
f.close()

# print dimensions of the data
print("%i x %i" % (nx,ny))

# perform some operations to remove negative values
minv = np.min(np.abs(data))
minv = 1e-8 if minv == 0 else minv
#print('Minimum value: %f' % minv)
data = np.maximum(minv, data)

# create a figure
plt.figure(dpi=144, figsize=(4,4))
plt.imshow(np.log(data))
plt.colorbar()