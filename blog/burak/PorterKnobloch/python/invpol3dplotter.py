#
# plotter3d.py
#
"""
Plot the solution generated by invpolsolver.py
"""

from numpy import loadtxt
from pylab import figure, plot, xlabel, ylabel, grid, hold, legend, title, savefig
from matplotlib.font_manager import FontProperties
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

t, u, v, w, q = loadtxt('data/invpolsolution.dat', unpack=True)

fig = plt.figure()
ax = fig.gca(projection='3d')

ax.plot(u, v, w, linewidth=0.3)
ax.set_xlabel('u')
ax.set_ylabel('v')
ax.set_zlabel('w')
ax.view_init(15,30)
savefig('image/uvw.png', bbox_inches='tight', dpi=100)

plt.show()
