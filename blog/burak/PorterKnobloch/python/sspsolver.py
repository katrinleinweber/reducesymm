#
# invpolsolver.py
#
"""
Use odeint to solve differential equations defined by vinvpol in twomode.py
"""

from scipy.integrate import odeint
import twomode
import numpy as np

#Callable function version:
def integrate(x0, p, t, abserror=1.0e-8, relerror=1.0e-6):
	"""
	Takes the initial condition, parameters and the time interval
	returns the result as a series in time.
	"""
	xsol = odeint(twomode.vfullssp, x0, t, args=(p,), atol = abserror, rtol = relerror)
	
	return xsol

if __name__ == "__main__":
	
	#Load parameters:
	p = np.loadtxt('data/parameters.dat')

	#Initial conditions:
	x10=-0.713404949283 
	x20=-0.713404949282 
	y10=0.084155488181 
	y20=-1.5962565603 

	# ODE solver parameters
	abserr = 1.0e-8
	relerr = 1.0e-6
	stoptime = 300
	numpoints = 30000

	# Create the time samples for the output of the ODE solver:
	t = [stoptime * float(i) / (numpoints - 1) for i in range(numpoints)]
	
	# Pack up the initial conditions:
	x0 = [x10,x20,y10,y20]

	# Call the ODE solver
	xsol = odeint(twomode.vfullssp, x0, t, args=(p,), atol = abserr, rtol = relerr)

	#Print the solution
	for t1, x1 in zip(t,xsol):
		print t1, x1[0], x1[1], x1[2], x1[3]