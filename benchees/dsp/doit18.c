/* this program is in the public domain */

#include "bench-user.h"
#include <math.h>

BEGIN_BENCH_DOC
BENCH_DOC("name", "dsp79-rader")
BENCH_DOC("author", "C. M. Rader, after Brenner")
BENCH_DOC("year", "1979, after Brenner 1967")
BENCH_DOC("language", "Fortran 77")
BENCH_DOC("notes", "The backward transform is scaled")
BENCH_DOC("notes", "DSP Section 1.1")
END_BENCH_DOC

int can_do(struct problem *p)
{
     return (SINGLE_PRECISION &&
	     p->rank == 1 &&
	     problem_complex_power_of_two(p, 1) &&
	     ((log_2(p->n[0]) & 1) == 0) && 	     /* power of 4 */
	     p->n[0] > 4 &&
	     p->n[0] <= 1024);
}

void problem_alloc(struct problem *p, int in_place)
{
     /* the routine expects arguments in common block AA */
     extern char F77_FUNC(aa, AA)[];
     UNUSED(in_place);

     p->in = p->out = F77_FUNC(aa, AA);
}


void problem_free(struct problem *p)
{
     UNUSED(p);
     /* nothing to deallocate */
}

void after_problem_ccopy_to(struct problem *p, bench_complex *out)
{
     unnormalize(p, out, 1);
}

extern void F77_FUNC(radix4, RADIX4)();

void setup(struct problem *p)
{
     int mm = log_2(p->n[0]) / 2;
     int iflag = 1;  /* first pass */
     int jflag = p->sign;

     BENCH_ASSERT(can_do(p));
     F77_FUNC(radix4, RADIX4)(&mm, &iflag, &jflag);
}

void doit(int iter, struct problem *p)
{
     int mm = log_2(p->n[0]) / 2;
     int iflag = 0;
     int jflag = p->sign;
     int i;

     for (i = 0; i < iter; ++i) {
	  F77_FUNC(radix4, RADIX4)(&mm, &iflag, &jflag);
     }
}

void done(struct problem *p)
{
     UNUSED(p);
}
