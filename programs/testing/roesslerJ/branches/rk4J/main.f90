PROGRAM integr

USE nrtype
USE ifc_integr, ONLY: rk4Pdriver, rk4driver, rk4Jdriver
USE ifc_util, ONLY: UnitMatrix
USE F95_LAPACK, ONLY: LA_GEEV
USE parameters

IMPLICIT NONE

REAL(DP), DIMENSION(:,:), ALLOCATABLE ::  J
REAL(DP) :: xi=0.0_dp, xf
INTEGER(I4B) :: jnsteps=1000000, i
REAL(DP), DIMENSION(:), ALLOCATABLE :: yi
real(dp), allocatable :: WI(:), WR(:)
INTERFACE
	SUBROUTINE roesslerField(x,y,dydx)
		USE nrtype
		IMPLICIT NONE
		REAL(DP), INTENT(IN) :: x
		REAL(DP), DIMENSION(:), INTENT(IN) :: y
		REAL(DP), DIMENSION(:), INTENT(OUT) :: dydx	
	END SUBROUTINE roesslerField
END INTERFACE

interface
	function roesslerVar(x,y)
		USE nrtype
		IMPLICIT NONE
		REAL(DP), INTENT(IN) :: x
		REAL(DP), DIMENSION(:), INTENT(IN) :: y
		REAL(DP), DIMENSION(size(y),size(y)) :: roesslerVar
	end function 
end interface



INTEGER(I4B) :: idum, p=0

allocate(yi(d+1))
allocate(J(d,d))

open(9,file='initial.dat')

read(9,*)  yi(1:d)

close(9)

open(17,file='period.dat')

read(17,*) xf

close(17)

yi(d+1)=xi

p=0

print *,"stepsize",(xf-xi)/jnsteps

J=UnitMatrix(d)

call rk4Jdriver(xi,yi,xf,jnsteps,yi,J,J,roesslerVar,roesslerField)

!allocate(WR(size(J,1)),WI(size(J,1)))

!call LA_GEEV( J, WR, WI)

!print *, WR
!print *, WI

! open(7,file="cycle.dat")
! do i=1,size(y,1)
! 	write(7,format_label) y(i,1:d)
! end do
! close(7)

open(9,file='J.dat')

do i=1,d
	write(9,format_label) J(i,:)
end do

close(9)

!CLOSE(11)

!CLOSE(10) 


END PROGRAM