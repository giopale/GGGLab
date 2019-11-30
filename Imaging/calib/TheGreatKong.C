#include "CalibAnalysis.C"

void TheGreatKong(){

struct H_data day1d1 = {(const char*)"spec0_d1.root",1,0};
struct H_data day1d2 = {(const char*)"spec0_d2.root",0,1};
struct H_data day1d3 = {(const char*)"spec0_d3.root",0,2};
struct H_data day1d4 = {(const char*)"spec0_d4.root",0,3};




day1d1 = CalibAnalysis(day1d1,1);
// day1d2 = CalibAnalysis(day1d2);
// day1d3 = CalibAnalysis(day1d3,0,1500);
// day1d4 = CalibAnalysis(day1d4,1,1200);



}