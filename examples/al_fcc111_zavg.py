# -*- coding: utf-8 -*-

#
# Plot the z-averaging results as produced by EDP
#

import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('z_extraction.txt')
plt.figure(dpi=72)
plt.plot(data[:,0], data[:,1], color='black')
plt.xlabel(r'z [$\AA$]')
plt.ylabel(r'$\rho$ [$\AA^{-3}$]')
plt.savefig('../docs/_static/img/al_fcc111_zavg.jpg')