#include <iostream>
#include <cmath>
#include <functional>
#include <limits>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>

struct config
{
    std::string algoName; // your algorithm name 
    std::string author; // name of the algorithm author
    float a; // start value for analyzing of the algorithm
    float b; // end value for analyzing of the algorithm
    float step; // step for analyzing of the algorithm, -1 if you want to analyze
    bool need_absolute_error; // if you need absolute errors
    bool need_relative_error; // if you need relative errors
    bool standard_rel_formula; // if you want to analyze relative error with formula:  your_func(x) / true_func(x) - 1
    bool modified_rel_formula; // if you want to analyze relative error with formula:  your_func(x) * true_func(x) - 1
    int pass_every_n_record; // pass every n record for plot, 1 if you want all points
};

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

void testing(config config, std::function<float(float)> accelerating_func, std::function<float(float)> true_func){
    clock_t start, end;
    float min_abs_err = INFINITY, max_abs_err = -1 * INFINITY;
    float min_rel_err = INFINITY, max_rel_err = -1 * INFINITY;
    float x_max_rel, x_min_rel;
    float x_max_abs, x_min_abs;
    float rel_err, abs_err;
    float x = config.a, b = config.b;
    int n_record = 0;

    std::fstream info_file, errors_file;   
    info_file.open("info.txt", std::ios::out | std::ios::app);
    errors_file.open("errors.csv", std::ios::out | std::ios::app);
    errors_file << "X,RelError,AbsError\n";
    info_file << "AlgoName," << config.algoName << "\n";
    info_file << "Author," << config.author << "\n"; 
    info_file << "a," << x << "\n";
    info_file << "b," << b << "\n"; 

    start = clock();
    while (x < b){  

        // Relative error
        if (config.need_relative_error){
            if (config.standard_rel_formula){
                rel_err = accelerating_func(x) / true_func(x) - 1;
            } else {
                rel_err = accelerating_func(x) * true_func(x) - 1;
            }
            change_minmax(rel_err, max_rel_err, min_rel_err, x_max_rel, x_min_rel, x);
        }
        
        // Absolute error
        if (config.need_absolute_error){
            abs_err = accelerating_func(x) - true_func(x);  
            change_minmax(abs_err, max_abs_err, min_abs_err, x_max_abs, x_min_abs, x);
        } 

        if (n_record % config.pass_every_n_record == 0){
            std::string row_info = std::to_string(x) + ",";
            if (config.need_relative_error){
                row_info += std::to_string(rel_err) + ",";
            } else {
                row_info += ",";
            }
            if (config.need_absolute_error){
                row_info += std::to_string(abs_err) + "\n";
            } else {
                row_info += "\n";
            }
            errors_file << row_info;
        }
        if (config.step == -1){
            x = nextafterf(x, b);
        } else {
            x += config.step;
        }
        n_record += 1;
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    
 
    if (config.need_relative_error){
        info_file << "MaxRelError," << max_rel_err << "\n";
        info_file << "ArgMaxRel," << x_max_rel << "\n";
        info_file << "MinRelError," << min_rel_err << "\n";
        info_file << "ArgMinRel," << x_min_rel << "\n";
    }

    if (config.need_absolute_error){
        info_file << "MaxAbsError," << max_abs_err << "\n";
        info_file << "ArgMaxAbs," << x_max_abs << "\n";
        info_file << "MinAbsError," << min_abs_err << "\n";
        info_file << "ArgMinAbs," << x_min_abs << "\n";  
    }

    info_file << "Time," << time_taken << "\n";
    info_file << "Step," << config.step; 

}
