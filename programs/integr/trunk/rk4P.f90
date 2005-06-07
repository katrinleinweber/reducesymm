Subroutine rk4P(x,y,dydx,h,yout,derivs,p,sect)

USE nrtype ; USE nrutil, ONLY : assert_eq

IMPLICIT NONE

REAL(DP), DIMENSION(:), INTENT(IN) :: y,dydx
REAL(DP), INTENT(IN) :: x,h
REAL(DP), DIMENSION(:), INTENT(OUT) :: yout
INTEGER(I4B), INTENT(IN) :: p,sect
INTERFACE
	SUBROUTINE derivs(x,y,dydx,kappa)
		USE nrtype
		IMPLICIT NONE
		REAL(DP), INTENT(IN) :: x, kappa
		REAL(DP), DIMENSION(:), INTENT(IN) :: y
		REAL(DP), DIMENSION(:), INTENT(OUT) :: dydx	
	END SUBROUTINE derivs
END INTERFACE



REAL(DP), DIMENSION(size(y)) :: k1,k2,k3,k4,v
REAL(DP) :: kappa
integer(i4b) :: ndum

ndum=assert_eq(size(y),size(dydx),size(yout),'rk4P')


kappa=1.0_dp


k1 = h*dydx ! First step



if (p == 0 ) then ! Didn't cross Poincare Section
	call derivs(x + h/2.0_dp , y + k1/2.0_dp, v, kappa) !Second Step
	k2 = h*v
	call derivs(x + h/2.0_dp , y + k2/2.0_dp, v, kappa) !Third Step
	k3 = h*v
else		! Crossed Poincare Section, change integration variable
	kappa=1.0_dp
	call derivs(x + h/2.0_dp , y + k1/2.0_dp, v, kappa) 
	kappa=1.0_dp/v(sect)
	call derivs(x + h/2.0_dp , y + k1/2.0_dp, v, kappa) !Second step
	k2 = h*v
	kappa=1.0_dp
	call derivs(x + h/2.0_dp , y + k2/2.0_dp, v, kappa)
	kappa=1.0_dp/v(sect)
	call derivs(x + h/2.0_dp , y + k2/2.0_dp, v, kappa) !Third step
	k3 = h*v
	kappa=1.0_dp
	call derivs(x + h, y + k3,v, kappa)
	kappa=1.0_dp/v(sect)
end if 

call derivs(x + h, y + k3,v, kappa) 			! Fourth step
k4 = h*v
yout = y + k1/6.0_dp+ k2/3.0_dp+ k3/3.0_dp+ k4/6.0_dp	! Accumulate increments

end subroutine