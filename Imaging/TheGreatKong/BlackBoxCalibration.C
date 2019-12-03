
void BlackBoxCalibration(){
	
	H_data bb[5][7]; ////   bb[POSITION][DETECTOR]

bb[0][0].filename = (const char*)"pos_1.root";
bb[0][0].dgtz = 0;
bb[0][0].ch = 0;

bb[0][0]= CalibAnalysis(bb[0][0],0,400,1,0.6);

}