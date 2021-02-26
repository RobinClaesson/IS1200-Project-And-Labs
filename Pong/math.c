#define TERMS 7
#define PI 3.14159265359

// calculates the factorial of n
int factorial (int n){
  int i;
  int sum = 1;

  for (i = 1; i <= n; i++){
    sum *= i;
  }

  return sum;
}

// calculates x^n
double power (double x, int n){
  int i;

  for (i = 1; i <= n; i++){
    x *= x;
  }

  return x;
}

// Approximates sin(x) using maclaurin expansion
double sin (double x) {

  while( x > PI)
    x -= (2*PI);
  while( x < (-PI) )
    x += (2*PI);

  int i;
  double rsin = 0;

  for (i = 0; i < TERMS; i++){
    rsin += power(-1, i) * (power(x, 2*i + 1) / factorial(2 * i + 1));
  }

  return rsin;
}

// Approximates cos(x) using maclaurin expansion
double cos (double x) {

  while( x > PI)
    x -= (2*PI);
  while( x < (-PI) )
    x += (2*PI);

  int i;
  double rcos = 0;

  for (i = 0; i < TERMS; i++){
    rcos += power(-1, i) * (power(x, 2 * i) / factorial(2 * i));
  }

  return rcos;
}
