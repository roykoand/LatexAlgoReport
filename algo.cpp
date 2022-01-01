#include <iostream>
#include <cmath>
#include <functional>
#include <limits>
#include <fstream>
#include <bits/stdc++.h>

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    return y;
}

void change_minmax(float error, float &max_error, float &min_error, float &x_max, float &x_min, float x) {
    if (max_error < error){
        max_error = error;
        x_max = x;
    }
    
    if (min_error > error){
        min_error = error;
        x_min = x;
    }
}

void testing(std::function<float(float)> accelerating_func, std::function<float(float)> true_func){
    clock_t start, end;
    float min_abs_err = INFINITY, max_abs_err = -1 * INFINITY;
    float min_rel_err = INFINITY, max_rel_err = -1 * INFINITY;
    float x_max_rel, x_min_rel;
    float x_max_abs, x_min_abs;
    float rel_err, abs_err;
    float x = 0.25, b = 8;
    int n_record = 0;

    std::fstream info_file, errors_file;   
    info_file.open("info.txt", std::ios::out | std::ios::app);
    errors_file.open("errors.csv", std::ios::out | std::ios::app);
    errors_file << "X,Error\n";
    info_file << "a," << x << "\n"; 
    info_file << "b," << b << "\n"; 
    start = clock();
    while (x < b){
        // Relative error
        rel_err = accelerating_func(x) * true_func(x) - 1;
        // Absolute error
        abs_err = accelerating_func(x) - true_func(x);    
        change_minmax(rel_err, max_rel_err, min_rel_err, x_max_rel, x_min_rel, x);
        change_minmax(abs_err, max_abs_err, min_abs_err, x_max_abs, x_min_abs, x);
        if (n_record % 10000 == 0){
            errors_file << x << "," << rel_err << "\n";
        }
        x = nextafterf(x, b);
        n_record += 1;
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    info_file << "MaxRelError," << max_rel_err << "\n";
    info_file << "ArgMaxRel," << x_max_rel << "\n";
    info_file << "MinRelError," << min_rel_err << "\n";
    info_file << "ArgMinRel," << x_min_rel << "\n";    
    info_file << "Time," << time_taken;

}


int main(){
    testing(Q_rsqrt, sqrt);
}