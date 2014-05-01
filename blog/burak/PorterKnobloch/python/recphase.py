#
# recphase.py
#

import numpy as np
import twomode
import sspsolver

x0 = [0.44, 0, -0.39, 0.07]
p = np.loadtxt('data/parameters.dat')

tf = 200;
dt = 0.025;
t = np.linspace(0, tf, np.floor(tf/dt)+1)
xsol = sspsolver.integrate(x0, p, t)

import matplotlib as mpl
from pylab import figure, plot, savefig
from matplotlib.font_manager import FontProperties
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.gca(projection='3d')

ax.w_xaxis.set_pane_color((1, 1, 1, 1.0))
ax.w_yaxis.set_pane_color((1, 1, 1, 1.0))
ax.w_zaxis.set_pane_color((1, 1, 1, 1.0))

ax.plot(xsol[:,0], xsol[:,2], xsol[:,3], linewidth=0.5, color='#3c5f96')

asol = np.array([xsol[:,0] + 1j*xsol[:,1], xsol[:,2] + 1j*xsol[:,3]], complex).transpose()

phi = np.array([- np.angle(xsol[i, 0] + xsol[i, 1]*1j) for i in range(np.size(xsol, 0))], float)
gUslice = np.array([[np.exp( 1j*phi[i]), np.exp( 1j*2*phi[i])]  for i in range(np.size(xsol, 0))], complex)

asolhat = np.multiply(gUslice, asol)

#asolhat = np.array([], complex)

#xsolhat = np.array([np.dot(twomode.LieElement(sphase[i]), xsol[i,:]) for i in range(np.size(xsol, 0))], float)

fighat = plt.figure()
axhat = fighat.gca(projection='3d')

axhat.w_xaxis.set_pane_color((1, 1, 1, 1.0))
axhat.w_yaxis.set_pane_color((1, 1, 1, 1.0))
axhat.w_zaxis.set_pane_color((1, 1, 1, 1.0))

axhat.plot(np.real(asolhat[:,0]), np.real(asolhat[:,1]), np.imag(asolhat[:,1]), linewidth=0.5, color='#3c5f96')

ephitilde = np.array([[np.exp(0), np.exp( 1j*np.angle(asolhat[i,1]))]  for i in range(np.size(xsol, 0))], complex)
asoltilde = np.multiply(ephitilde, asolhat)

figtilde = plt.figure()
axtilde = figtilde.gca(projection='3d')

axtilde.w_xaxis.set_pane_color((1, 1, 1, 1.0))
axtilde.w_yaxis.set_pane_color((1, 1, 1, 1.0))
axtilde.w_zaxis.set_pane_color((1, 1, 1, 1.0))

axtilde.plot(np.real(asoltilde[:,0]), np.real(asoltilde[:,1]), np.imag(asoltilde[:,1]), linewidth=0.5, color='#3c5f96')

plt.show()
