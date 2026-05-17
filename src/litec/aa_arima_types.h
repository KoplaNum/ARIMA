#ifndef AA_ARIMA_TYPES_H
#define AA_ARIMA_TYPES_H

#ifndef AA_EPS
#define AA_EPS 0.0000000001
#endif

#ifndef AA_BIG
#define AA_BIG 1000000000000000000.
#endif

#define AA_SCORE_AIC 1
#define AA_SCORE_AICC 2
#define AA_SCORE_BIC 3
#define AA_SCORE_HQIC 4

#define AA_TRANSFORM_NONE 0
#define AA_TRANSFORM_LOG 1
#define AA_TRANSFORM_BOXCOX 2
#define AA_TRANSFORM_RETURN 3
#define AA_TRANSFORM_DIFF 4

typedef struct
{
  int p;
  int d;
  int q;
  int P;
  int D;
  int Q;
  int m;
  int hasConstant;
  int transformMode;
  int residualCount;
  int forecastCount;
  int paramCount;
  int converged;
  int iterations;
  int status;
  var mean;
  var constant;
  var sigma2;
  var sse;
  var logLik;
  var aic;
  var aicc;
  var bic;
  var hqic;
  var score;
  var forecast;
  var forecastSE;
  var lastPrice;
  var lambda;
  var gradNorm;
  var* ar;
  var* ma;
  var* sar;
  var* sma;
  var* beta;
  var* residuals;
  var* forecastPath;
  int arCap;
  int maCap;
  int sarCap;
  int smaCap;
  int betaCap;
  int residualCap;
  int forecastCap;
} ARIMA_MODEL;

typedef struct
{
  int capacityN;
  int capacityP;
  int capacityQ;
  int capacitySP;
  int capacitySQ;
  int capacityH;
  int candidateCap;
  int matrixCap;
  var* rawBuffer;
  int rawBufferCount;
  var* tmp1;
  var* tmp2;
  var* tmp3;
  var* tmp4;
  var* tmp5;
  var* tmp6;
  var* tmp7;
  var* tmp8;
  var* acf;
  var* pacf;
  var* residuals;
  var* forecast;
  var* lower;
  var* upper;
  var* grad;
  var* hessian;
  var* matrix;
  var* vector;
  var* params;
  var* params2;
  var* workspace;
} ARIMA_WORK;

typedef struct
{
  int p;
  int d;
  int q;
  int P;
  int D;
  int Q;
  int m;
  int valid;
  int converged;
  var score;
} ARIMA_CANDIDATE;

typedef struct
{
  int p;
  int d;
  int q;
  int converged;
  var sse;
  var aicc;
  var forecast;
  var* ar;
  var* ma;
  int arCap;
  int maCap;
} AUTO_ARIMA_RESULT;

typedef struct
{
  ARIMA_WORK core;
} AUTO_ARIMA_WORK;

#endif
