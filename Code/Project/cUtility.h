#pragma once

#include <string>
#include <iostream>
#include <sstream>

template <typename T> std::string tostr(const T& t) { std::ostringstream os; os.precision(1); os<<std::fixed; os<<t; return os.str(); } 


