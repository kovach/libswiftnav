/* slahilb.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static real c_b4 = 0.f;

/* Subroutine */ int slahilb_(integer *n, integer *nrhs, real *a, integer *
	lda, real *x, integer *ldx, real *b, integer *ldb, real *work, 
	integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, x_dim1, x_offset, b_dim1, b_offset, i__1, i__2;
    real r__1;

    /* Local variables */
    integer i__, j, m, r__, ti, tm;
    extern /* Subroutine */ int xerbla_(char *, integer *), slaset_(
	    char *, integer *, integer *, real *, real *, real *, integer *);


/*  -- LAPACK auxiliary test routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     28 August, 2006 */

/*     David Vu <dtv@cs.berkeley.edu> */
/*     Yozo Hida <yozo@cs.berkeley.edu> */
/*     Jason Riedy <ejr@cs.berkeley.edu> */
/*     D. Halligan <dhalligan@berkeley.edu> */

/*     .. Scalar Arguments .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLAHILB generates an N by N scaled Hilbert matrix in A along with */
/*  NRHS right-hand sides in B and solutions in X such that A*X=B. */

/*  The Hilbert matrix is scaled by M = LCM(1, 2, ..., 2*N-1) so that all */
/*  entries are integers.  The right-hand sides are the first NRHS */
/*  columns of M * the identity matrix, and the solutions are the */
/*  first NRHS columns of the inverse Hilbert matrix. */

/*  The condition number of the Hilbert matrix grows exponentially with */
/*  its size, roughly as O(e ** (3.5*N)).  Additionally, the inverse */
/*  Hilbert matrices beyond a relatively small dimension cannot be */
/*  generated exactly without extra precision.  Precision is exhausted */
/*  when the largest entry in the inverse Hilbert matrix is greater than */
/*  2 to the power of the number of bits in the fraction of the data type */
/*  used plus one, which is 24 for single precision. */

/*  In single, the generated solution is exact for N <= 6 and has */
/*  small componentwise error for 7 <= N <= 11. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The dimension of the matrix A. */

/*  NRHS    (input) NRHS */
/*          The requested number of right-hand sides. */

/*  A       (output) REAL array, dimension (LDA, N) */
/*          The generated scaled Hilbert matrix. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= N. */

/*  X       (output) REAL array, dimension (LDX, NRHS) */
/*          The generated exact solutions.  Currently, the first NRHS */
/*          columns of the inverse Hilbert matrix. */

/*  LDX     (input) INTEGER */
/*          The leading dimension of the array X.  LDX >= N. */

/*  B       (output) REAL array, dimension (LDB, NRHS) */
/*          The generated right-hand sides.  Currently, the first NRHS */
/*          columns of LCM(1, 2, ..., 2*N-1) * the identity matrix. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= N. */

/*  WORK    (workspace) REAL array, dimension (N) */


/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          = 1: N is too large; the data is still generated but may not */
/*               be not exact. */
/*          < 0: if INFO = -i, the i-th argument had an illegal value */

/*  ===================================================================== */
/*     .. Local Scalars .. */
/*     .. Parameters .. */
/*     NMAX_EXACT   the largest dimension where the generated data is */
/*                  exact. */
/*     NMAX_APPROX  the largest dimension where the generated data has */
/*                  a small componentwise relative error. */
/*     .. */
/*     .. External Functions */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    --work;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    if (*n < 0 || *n > 11) {
	*info = -1;
    } else if (*nrhs < 0) {
	*info = -2;
    } else if (*lda < *n) {
	*info = -4;
    } else if (*ldx < *n) {
	*info = -6;
    } else if (*ldb < *n) {
	*info = -8;
    }
    if (*info < 0) {
	i__1 = -(*info);
	xerbla_("SLAHILB", &i__1);
	return 0;
    }
    if (*n > 6) {
	*info = 1;
    }
/*     Compute M = the LCM of the integers [1, 2*N-1].  The largest */
/*     reasonable N is small enough that integers suffice (up to N = 11). */
    m = 1;
    i__1 = (*n << 1) - 1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	tm = m;
	ti = i__;
	r__ = tm % ti;
	while(r__ != 0) {
	    tm = ti;
	    ti = r__;
	    r__ = tm % ti;
	}
	m = m / ti * i__;
    }
/*     Generate the scaled Hilbert matrix in A */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    a[i__ + j * a_dim1] = (real) m / (i__ + j - 1);
	}
    }
/*     Generate matrix B as simply the first NRHS columns of M * the */
/*     identity. */
    r__1 = (real) m;
    slaset_("Full", n, nrhs, &c_b4, &r__1, &b[b_offset], ldb);
/*     Generate the true solutions in X.  Because B = the first NRHS */
/*     columns of M*I, the true solutions are just the first NRHS columns */
/*     of the inverse Hilbert matrix. */
    work[1] = (real) (*n);
    i__1 = *n;
    for (j = 2; j <= i__1; ++j) {
	work[j] = work[j - 1] / (j - 1) * (j - 1 - *n) / (j - 1) * (*n + j - 
		1);
    }
    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    x[i__ + j * x_dim1] = work[i__] * work[j] / (i__ + j - 1);
	}
    }
    return 0;
} /* slahilb_ */
