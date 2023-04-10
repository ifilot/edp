# -*- coding: utf-8 -*-

#
# Plot the z-averaging results as produced by EDP
#

import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('line_extraction.txt')
plt.figure(dpi=72)
plt.plot(np.linalg.norm(data[:,0:3], axis=1), data[:,3], color='black')
plt.xlabel(r'r [$\AA$]')
plt.ylabel(r'$\rho$ [$\AA^{-3}$]')
plt.savefig('../docs/_static/img/ch4_line.jpg')