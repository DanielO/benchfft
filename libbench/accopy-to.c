/* not worth copyrighting */
/* $Id: accopy-to.c,v 1.3 2002-08-16 12:09:55 athena Exp $ */
#include "bench.h"

/* default routine, can be overridden by user */
void after_problem_ccopy_to(struct problem *p, bench_complex *out)
{
     UNUSED(p);
     UNUSED(out);
}
