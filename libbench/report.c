/*
 * Copyright (c) 2001 Matteo Frigo
 * Copyright (c) 2001 Steven G. Johnson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* $Id: report.c,v 1.5 2001-07-23 21:39:10 athena Exp $ */

#include "config.h"
#include "bench.h"
#include <stdio.h>
#include <stdlib.h>

void (*report)(const struct problem *p, double *t, int st);

/* report WHAT */
enum {
     W_TIME = 0x1,
     W_MFLOPS = 0x2
};

/* report HOW */
enum { H_ALL, H_MIN, H_MAX, H_AVG };

static void mkstat(double *t, int st, double *m, double *M, double *s)
{
     int i;
     
     *m = t[0], *M = t[0], *s = 0.0;

     for (i = 0; i < st; ++i) {
	  if (t[i] < *m)
	       *m = t[i];
	  if (t[i] > *M)
	       *M = t[i];
	  *s += t[i];
     }
     *s /= (double)st;
}

static void report_generic(const struct problem *p, double *t, int st,
			   int what, int how)
{
     double m, M, s;

     mkstat(t, st, &m, &M, &s);

     if (what & W_TIME) {
	  switch (how) {
	      case H_ALL:
		   ovtpvt("(%g %g %g)\n", m, s, M);
		   break;
	      case H_MIN:
		   ovtpvt("%g\n", m);
		   break;
	      case H_MAX:
		   ovtpvt("%g\n", M);
		   break;
	      case H_AVG:
		   ovtpvt("%g\n", s);
		   break;
	  }
     }

     if (what & W_MFLOPS) {
	  switch (how) {
	      case H_ALL:
		   ovtpvt("(%g %g %g)\n", 
			  mflops(p, M), mflops(p, s), mflops(p, m));
		   break;
	      case H_MIN:
		   ovtpvt("%g\n", mflops(p, M));
		   break;
	      case H_MAX:
		   ovtpvt("%g\n", mflops(p, m));
		   break;
	      case H_AVG:
		   ovtpvt("%g\n", mflops(p, s));
		   break;
	  }
     }
}

void report_mflops(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_MFLOPS, H_ALL);
}

void report_max_mflops(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_MFLOPS, H_MAX);
}

void report_avg_mflops(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_MFLOPS, H_AVG);
}

void report_time(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_TIME, H_ALL);
}

void report_min_time(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_TIME, H_MIN);
}

void report_avg_time(const struct problem *p, double *t, int st)
{
     report_generic(p, t, st, W_TIME, H_AVG);
}

void report_benchmark(const struct problem *p, double *t, int st)
{
     double m, M, s;
     mkstat(t, st, &m, &M, &s);
     ovtpvt("%g %g\n", mflops(p, m), m);
}
