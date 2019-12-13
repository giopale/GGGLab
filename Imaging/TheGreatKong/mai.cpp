#include <iostream>
#using namespace std;

int main()
{

	double xAlGrPoly[7];

	xAlGrPoly[0]=2.04;
	xAlGrPoly[1]=2.018;
	xAlGrPoly[2]=2.004;
	xAlGrPoly[3]=2.00;
	xAlGrPoly[4]=2.004;
	xAlGrPoly[5]=2.018;
	xAlGrPoly[6]= 2.040;

	for (int i = 0; i < 7; ++i)
	{
		cout << xAlGrPoly[i]*0.94 << endl;
		
	}


	return 0;
}