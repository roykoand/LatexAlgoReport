#include "algo_analyzer.hpp"

float Q_rsqrt(float number)
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

int main(){
    struct config config;
    config.algoName = "Fast Inverse Square root";
    config.author = "Not John D. Carmack";
    config.a = 1.0f;
    config.b = 4.0f;
    config.step = -1.0f;
    config.need_absolute_error = true;
    config.need_relative_error = true;
    config.standard_rel_formula = false;
    config.modified_rel_formula = true;
    config.pass_every_n_record = 100;

    testing(config, Q_rsqrt, sqrt);
}