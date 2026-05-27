#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double var;
typedef double* vars;
typedef char* string;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef abs
#define abs(x) fabs(x)
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#define clamp(v,lo,hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define fix0(x) (fabs(x) < 1e-10 ? ((x) < 0 ? -1e-10 : 1e-10) : (x))
#define invalid(x) ((x) != (x) || (x) > DBL_MAX || (x) < -DBL_MAX)
#define ifelse(a,b,c) ((a) ? (b) : (c))
#define roundto(x,step) ((step) > 0 ? round((x)/(step))*(step) : (x))

#define LINE 0
#define BLUE 1
#define RED 2
#define GREEN 3

static var g_dummy_price = 0.;
static var g_dummy_series_buffer[8] = {0.,0.,0.,0.,0.,0.,0.,0.};

var priceClose(int bar)
{
  (void)bar;
  return g_dummy_price;
}

vars series(var source)
{
  (void)source;
  return g_dummy_series_buffer;
}

void plot(string label,var value,int style,int color)
{
  (void)label;
  (void)value;
  (void)style;
  (void)color;
}

void enterLong(void)
{
}

void enterShort(void)
{
}

#include "../../src/litec/aa_arima_all.c"
