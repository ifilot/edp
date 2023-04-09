# -*- coding: utf-8 -*-

#
# Plot the z-averaging results as produced by EDP
#

import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('spherical_average.txt')
plt.figure(dpi=72)
plt.plot(data[:,0], data[:,1], color='black')
plt.xlabel(r'r [$\AA$]')
plt.ylabel(r'$\rho$ [$\AA^{-3}$]')
plt.savefig('../docs/_static/img/ch4_spherical_average.jpg')