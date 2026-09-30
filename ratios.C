#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "THStack.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;


//---------------------CORRECTIONS---------------------------------------------
//--------------------------------------------------------------------------------

//---------POSITRON NEW CORRECTIONS---------------- 

/*//Model 6 Version: S19pos
Double_t x_6bdt_S19pos[15]={1.000000, 0.999960, 0.998785, 0.999169, 0.994314, 0.994867, 0.990049, 0.986557, 0.984458, 0.980628, 0.978559, 0.966788, 0.935319, 0.669791, 0.058302}; 
Double_t xerrB_6bdt_S19pos[15]={0.000190, 0.000210, 0.000517, 0.000441, 0.001055, 0.001004, 0.001382, 0.001600, 0.001717, 0.001911, 0.002007, 0.002479, 0.003398, 0.006491, 0.003238}; 

//Model 9 Version: S19pos
Double_t x_9bdt_S19pos[15]={1.000000, 1.000000, 0.999317, 0.997561, 0.993546, 0.992194, 0.989255, 0.984202, 0.985313, 0.982095, 0.976892, 0.955843, 0.904446, 0.522455, 0.036725}; 
Double_t xerrB_9bdt_S19pos[15]={0.000190, 0.000191, 0.000408, 0.000707, 0.001121, 0.001229, 0.001435, 0.001731, 0.001670, 0.001839, 0.002081, 0.002841, 0.004060, 0.006893, 0.002602}; 

//Model 6 Version: F18inpos
Double_t x_6bdt_F18inpos[15]={1.000007, 0.999991, 0.999945, 0.998958, 0.994955, 0.992459, 0.990583, 0.987427, 0.986563, 0.984746, 0.980417, 0.970375, 0.942586, 0.724119, 0.061210}; 
Double_t xerrB_6bdt_F18inpos[15]={0.000280, 0.000287, 0.000296, 0.001421, 0.002358, 0.001478, 0.001631, 0.001848, 0.001891, 0.001980, 0.002227, 0.002651, 0.003595, 0.006727, 0.003269}; 

//Model 9 Version: F18inpos
Double_t x_9bdt_F18inpos[15]={1.000007, 0.999983, 0.999813, 0.998853, 0.993112, 0.991666, 0.990309, 0.983953, 0.982207, 0.976846, 0.970913, 0.947719, 0.889529, 0.491480, 0.023803}; 
Double_t xerrB_9bdt_F18inpos[15]={0.000280, 0.000290, 0.000747, 0.001370, 0.002706, 0.001549, 0.001635, 0.002067, 0.002179, 0.002454, 0.002707, 0.003449, 0.004755, 0.007485, 0.002152}; 

//Model 6 Version: F18outpos
Double_t x_6bdt_F18outpos[15]={0.999999, 1.000001, 1.002254, 0.999004, 0.996276, 0.994376, 0.975957, 0.977324, 0.973649, 0.961885, 0.947982, 0.912231, 0.843371, 0.527918, 0.082146}; 
Double_t xerrB_6bdt_F18outpos[15]={0.005080, 0.005079, 0.003475, 0.005126, 0.006164, 0.007129, 0.011540, 0.010810, 0.012059, 0.013603, 0.016148, 0.020052, 0.025903, 0.035322, 0.019976}; 

//Model 9 Version: F18outpos
Double_t x_9bdt_F18outpos[15]={0.999999, 1.000001, 1.000283, 0.999289, 0.997228, 0.968708, 0.978142, 0.957380, 0.946402, 0.922903, 0.911375, 0.865270, 0.824587, 0.495624, 0.070487}; 
Double_t xerrB_9bdt_F18outpos[15]={0.005080, 0.005079, 0.004919, 0.005194, 0.008279, 0.013081, 0.011487, 0.014441, 0.016511, 0.019278, 0.020505, 0.024555, 0.027191, 0.035224, 0.018729}; 


//---------ELECTRON NEW CORRECTIONS---------------- 

//Model 6 Version: S19neg Initial events: 2188604.615946
Double_t x_6bdt_S19neg[15]={1.000000, 1.000000, 1.000000, 0.999975, 0.999781, 0.999385, 0.998354, 0.996481, 0.994876, 0.992381, 0.988428, 0.968187, 0.912504, 0.634702, 0.090336}; 
Double_t xerrB_6bdt_S19neg[15]={0.000000, 0.000000, 0.000000, 0.000003, 0.000010, 0.000017, 0.000027, 0.000040, 0.000048, 0.000059, 0.000072, 0.000119, 0.000191, 0.000325, 0.000194};

//Model 9 Version: S19neg Initial events: 2188604.615946
Double_t x_9bdt_S19neg[15]={1.000000, 1.000000, 1.000000, 0.999992, 0.999779, 0.999210, 0.997454, 0.993818, 0.990676, 0.985973, 0.979232, 0.947369, 0.875070, 0.490026, 0.027465}; 
Double_t xerrB_9bdt_S19neg[15]={0.000000, 0.000000, 0.000001, 0.000002, 0.000010, 0.000019, 0.000034, 0.000053, 0.000065, 0.000079, 0.000096, 0.000151, 0.000223, 0.000338, 0.000110};

//Model 6 Version: F18inneg Initial events: 957426.366174
Double_t x_6bdt_F18inneg[15]={1.000000, 1.000000, 1.000001, 0.999976, 0.999767, 0.999328, 0.998281, 0.996484, 0.994779, 0.991849, 0.987413, 0.964075, 0.904264, 0.590834, 0.096853}; 
Double_t xerrB_6bdt_F18inneg[15]={0.000001, 0.000001, 0.000001, 0.000005, 0.000016, 0.000027, 0.000042, 0.000061, 0.000074, 0.000092, 0.000114, 0.000190, 0.000301, 0.000502, 0.000302};

//Model 9 Version: F18inneg Initial events: 1008716.852479
Double_t x_9bdt_F18inneg[15]={1.000000, 1.000000, 1.000001, 0.999996, 0.999767, 0.999119, 0.997276, 0.993572, 0.990184, 0.984888, 0.978010, 0.944635, 0.867729, 0.466995, 0.029417}; 
Double_t xerrB_9bdt_F18inneg[15]={0.000001, 0.000001, 0.000000, 0.000002, 0.000015, 0.000030, 0.000052, 0.000080, 0.000098, 0.000121, 0.000146, 0.000228, 0.000337, 0.000497, 0.000168};

//Model 6 Version: F18outneg Initial events: 1605314.362772
Double_t x_6bdt_F18outneg[15]={1.000000, 1.000000, 0.999997, 0.999815, 0.998549, 0.997030, 0.994072, 0.989825, 0.986959, 0.983472, 0.978980, 0.963314, 0.936407, 0.753080, 0.275379}; 
Double_t xerrB_6bdt_F18outneg[15]={0.000001, 0.000001, 0.000002, 0.000011, 0.000030, 0.000043, 0.000061, 0.000079, 0.000090, 0.000101, 0.000113, 0.000148, 0.000193, 0.000340, 0.000353};

//Model 9 Version: F18outneg Initial events: 1605314.362772
Double_t x_9bdt_F18outneg[15]={1.000000, 1.000000, 1.000000, 0.999979, 0.999145, 0.997718, 0.994772, 0.990529, 0.987478, 0.983359, 0.978212, 0.956457, 0.914551, 0.625317, 0.091954}; 
Double_t xerrB_9bdt_F18outneg[15]={0.000001, 0.000001, 0.000001, 0.000004, 0.000023, 0.000038, 0.000057, 0.000076, 0.000088, 0.000101, 0.000115, 0.000161, 0.000221, 0.000382, 0.000228};


    //---------------------POSITRON NO CORRECTIONS---------------------------------------------
    //--------------------------------------------------------------------------------
  */ 
   //SPRING 2019 POSITRONS 
    //MODEL 6
    Double_t x_6bdt_S19pos[15] ={0.999999, 0.999829, 0.997093, 0.999687, 0.994773, 0.992199, 0.990564, 0.985054, 0.979130, 0.968402, 0.961722, 0.929594, 0.843146, 0.385972, 0.007473};
    Double_t xerrB_6bdt_S19pos[15] ={0.000193, 0.000228, 0.000770, 0.000814, 0.001005, 0.001234, 0.001294, 0.001682, 0.001983, 0.002432, 0.002666, 0.003548, 0.005044, 0.006750, 0.001189}; 
    //MODEL 9
    Double_t x_9bdt_S19pos[15] ={0.999999, 1.000051, 1.000656, 0.998909, 0.998027, 0.994461, 0.991145, 0.981158, 0.975131, 0.965424, 0.953708, 0.899528, 0.770670, 0.266459, 0.010517}; 
    Double_t xerrB_9bdt_S19pos[15] ={0.000193, 0.000156, 0.000329, 0.000855, 0.001140, 0.001017, 0.001295, 0.001887, 0.002160, 0.002536, 0.002917, 0.004169, 0.005830, 0.006130, 0.001417}; 
    
    //FALL IN 2018  POSITRONS 
    //MODEL 6
    Double_t x_6bdt_F18inpos[15] ={1.000000, 0.999838, 1.000340, 0.999756, 0.998804, 0.997805, 0.992298, 0.985255, 0.980048, 0.972718, 0.964648, 0.929359, 0.851448, 0.433276, 0.014856}; 
    Double_t xerrB_6bdt_F18inpos[15] ={0.000103, 0.000150, 0.000107, 0.000301, 0.000366, 0.000461, 0.000891, 0.001226, 0.001419, 0.001651, 0.001873, 0.002596, 0.003594, 0.004996, 0.001199}; 
    //MODEL 9
    Double_t x_9bdt_F18inpos[15] ={1.000000, 1.000034, 1.000316, 1.001474, 1.000314, 0.996122, 0.988208, 0.977088, 0.969052, 0.957489, 0.942895, 0.879423, 0.750369, 0.263628, 0.004083}; 
    Double_t xerrB_9bdt_F18inpos[15] ={0.000107, 0.000069, 0.000273, 0.000156, 0.000201, 0.000651, 0.001120, 0.001548, 0.001788, 0.002078, 0.002387, 0.003336, 0.004424, 0.004500, 0.000660}; 
 
    //FALL OUT 2018  POSITRONS 
    //MODEL 6
    Double_t x_6bdt_F18outpos[15] ={0.999999, 1.003030, 1.000175, 1.001268, 0.979101, 0.981579, 0.955260, 0.939843, 0.927846, 0.916749, 0.893088, 0.792378, 0.741073, 0.236377, 0.098377}; 
    Double_t xerrB_6bdt_F18outpos[15] ={0.005080, 0.002321, 0.004603, 0.003665, 0.010973, 0.010388, 0.015096, 0.017215, 0.018541, 0.019801, 0.022242, 0.028907, 0.030989, 0.030184, 0.019999}; 
    //MODEL 9
    Double_t x_9bdt_F18outpos[15] ={0.999999, 1.000810, 0.998798, 1.006802, 0.974212, 0.973396, 0.952395, 0.908792, 0.884586, 0.875993, 0.816709, 0.771490, 0.653442, 0.214782, 0.000685}; 
    Double_t xerrB_9bdt_F18outpos[15] ={0.005080, 0.004653, 0.005578, 0.006533, 0.012256, 0.012333, 0.015272, 0.020669, 0.023050, 0.023727, 0.027487, 0.029827, 0.033679, 0.028893, 0.005403}; 
 

     //---------------------ELECTRON NO CORRECTIONS---------------------------------------------
    //--------------------------------------------------------------------------------
    
    //SPRING 2019 ELECTRONS 
    //MODEL 6
    Double_t x_6bdt_S19neg[15] ={1.000000, 1.000000, 0.999986, 0.999840, 0.998641, 0.996366, 0.990633, 0.980373, 0.971642, 0.959282, 0.942079, 0.865435, 0.717115, 0.200693, 0.001651}; 
    Double_t xerrB_6bdt_S19neg[15] ={0.000000, 0.000000, 0.000003, 0.000009, 0.000025, 0.000041, 0.000065, 0.000094, 0.000112, 0.000134, 0.000158, 0.000231, 0.000304, 0.000271, 0.000027};
    //MODEL 9
    Double_t x_9bdt_S19neg[15] ={1.000000, 1.000000, 1.000000, 0.999909, 0.998673, 0.995756, 0.987518, 0.971942, 0.958950, 0.940710, 0.915547, 0.810425, 0.624289, 0.123006, 0.002004}; 
    Double_t xerrB_9bdt_S19neg[15] ={0.000000, 0.000001, 0.000000, 0.000006, 0.000025, 0.000044, 0.000075, 0.000112, 0.000134, 0.000160, 0.000188, 0.000265, 0.000327, 0.000222, 0.000030};
    
    //FALL IN 2018  ELECTRONS 
    //MODEL 6
    Double_t x_6bdt_F18inneg[15] ={1.000000, 1.000000, 0.999996, 0.999904, 0.998989, 0.997306, 0.993065, 0.984816, 0.977097, 0.965466, 0.948471, 0.868587, 0.711979, 0.235625, 0.005478}; 
    Double_t xerrB_6bdt_F18inneg[15] = {0.000001, 0.000001, 0.000002, 0.000010, 0.000033, 0.000053, 0.000085, 0.000125, 0.000153, 0.000187, 0.000226, 0.000345, 0.000463, 0.000434, 0.000075};
    //MODEL 9
    Double_t x_9bdt_F18inneg[15]  ={1.000000, 1.000000, 1.000000, 0.999933, 0.998887, 0.996251, 0.989300, 0.975405, 0.963539, 0.946383, 0.922221, 0.819899, 0.641378, 0.157697, 0.002342}; 
    Double_t xerrB_9bdt_F18inneg[15] ={0.000001, 0.000001, 0.000001, 0.000008, 0.000033, 0.000061, 0.000102, 0.000154, 0.000187, 0.000224, 0.000267, 0.000383, 0.000478, 0.000363, 0.000048};
 
    //FALL OUT 2018  ELECTRONS 
    //MODEL 6
    Double_t x_6bdt_F18outneg[15] ={1.000000, 1.000000, 0.999905, 0.998927, 0.993828, 0.987055, 0.974229, 0.955473, 0.941271, 0.922401, 0.896521, 0.790592, 0.606701, 0.157000, 0.002274}; 
    Double_t xerrB_6bdt_F18outneg[15] ={0.000001, 0.000001, 0.000008, 0.000026, 0.000062, 0.000089, 0.000125, 0.000163, 0.000186, 0.000211, 0.000240, 0.000321, 0.000386, 0.000287, 0.000038};
    //MODEL 9
    Double_t x_9bdt_F18outneg[15]  ={1.000000, 1.000000, 0.999999, 0.999755, 0.996993, 0.991613, 0.978404, 0.956427, 0.938743, 0.914331, 0.879914, 0.730910, 0.482807, 0.059842, 0.000785}; 
    Double_t xerrB_9bdt_F18outneg[15] = {0.000001, 0.000001, 0.000001, 0.000012, 0.000043, 0.000072, 0.000115, 0.000161, 0.000189, 0.000221, 0.000257, 0.000350, 0.000394, 0.000187, 0.000022};



// SPRING 2019 POSITRONS 
// MODEL 6
Double_t y_6bdt_S19pos[15] = {
    0.999775,
    0.994161,
    0.911644,
    0.724325,
    0.466315,
    0.339718,
    0.242140,
    0.158453,
    0.125680,
    0.098165,
    0.081634,
    0.033701,
    0.007374,
    0.000730,
    0.001869,
};
Double_t yerrB_6bdt_S19pos[15] = {
    0.000234,
    0.001178,
    0.004384,
    0.006891,
    0.007695,
    0.007307,
    0.006609,
    0.005636,
    0.005117,
    0.004594,
    0.004229,
    0.002793,
    0.001340,
    0.000480,
    0.000707,
};
// MODEL 9
Double_t y_9bdt_S19pos[15] = {
    0.999747,
    0.997784,
    0.930063,
    0.692551,
    0.353072,
    0.242958,
    0.131823,
    0.079767,
    0.062797,
    0.047457,
    0.034016,
    0.012247,
    0.000357,
    0.000071,
    0.000035,
};
Double_t yerrB_9bdt_S19pos[15] = {
    0.000348,
    0.000734,
    0.003934,
    0.007119,
    0.007373,
    0.006617,
    0.005222,
    0.004184,
    0.003748,
    0.003287,
    0.002805,
    0.001713,
    0.000376,
    0.000271,
    0.000255,
};

// FALL IN 2018  POSITRONS
// MODEL 6
Double_t y_6bdt_F18inpos[15] = {1.000000, 0.992761, 0.919888, 0.773758, 0.579520, 0.431408, 0.354105, 0.274470, 0.233413, 0.156612, 0.114380, 0.044340, 0.019166, 0.002459, 0.002442};
Double_t yerrB_6bdt_F18inpos[15] = {0.000466, 0.001953, 0.006082, 0.009359, 0.011039, 0.011075, 0.010695, 0.009981, 0.009463, 0.008134, 0.007127, 0.004626, 0.003103, 0.001214, 0.001211};
// MODEL 9
Double_t y_9bdt_F18inpos[15] = {1.000000, 0.997262, 0.937316, 0.749899, 0.449542, 0.333106, 0.223482, 0.116092, 0.082696, 0.058521, 0.050196, 0.024687, 0.004065, 0.003549, 0.001242};
Double_t yerrB_9bdt_F18inpos[15] = {0.000466, 0.001266, 0.005440, 0.009685, 0.011123, 0.010541, 0.009319, 0.007173, 0.006173, 0.005267, 0.004903, 0.003502, 0.001507, 0.001420, 0.000932};

// FALL OUT 2018  POSITRONS
// MODEL 6
Double_t y_6bdt_F18outpos[15] = {
    1.000000,
    0.996795,
    0.949976,
    0.835284,
    0.692183,
    0.569604,
    0.409326,
    0.310984,
    0.259280,
    0.225194,
    0.190335,
    0.088515,
    0.040188,
    0.004372,
    0.020812,
};
Double_t yerrB_6bdt_F18outpos[15] = {
    0.000792,
    0.001781,
    0.006190,
    0.010475,
    0.013023,
    0.013964,
    0.013868,
    0.013058,
    0.012365,
    0.011789,
    0.011082,
    0.008037,
    0.005587,
    0.002021,
    0.004098,
};
// MODEL 9
Double_t y_9bdt_F18outpos[15] = {
    1.000000,
    0.999310,
    0.969909,
    0.839767,
    0.578303,
    0.440737,
    0.283102,
    0.197035,
    0.159955,
    0.142022,
    0.112390,
    0.061506,
    0.021853,
    0.002677,
    0.007090,
};
Double_t yerrB_9bdt_F18outpos[15] = {
    0.000792,
    0.001072,
    0.004876,
    0.010359,
    0.013928,
    0.014003,
    0.012710,
    0.011228,
    0.010352,
    0.009861,
    0.008929,
    0.006812,
    0.004193,
    0.001658,
    0.002493,
};

/*
// SPRING 2019 ELECTRONS
Double_t x_9bdt_S19neg[15] = {1.000000, 1.000000, 1.000000, 0.999917, 0.998701, 0.995848, 0.987736, 0.972365, 0.959515, 0.941466, 0.916516, 0.812074, 0.626494, 0.124212, 0.002040};
Double_t xerrB_9bdt_S19neg[15] = {0.000000, 0.000001, 0.000001, 0.000006, 0.000024, 0.000043, 0.000074, 0.000110, 0.000132, 0.000157, 0.000185, 0.000262, 0.000324, 0.000221, 0.000030};

Double_t x_6bdt_S19neg[15] = {1.000000, 1.000000, 0.999989, 0.999845, 0.998667, 0.996441, 0.990821, 0.980738, 0.972163, 0.959980, 0.942971, 0.867014, 0.719352, 0.202283, 0.001695};
Double_t xerrB_6bdt_S19neg[15] = {0.000000, 0.000000, 0.000002, 0.000008, 0.000024, 0.000040, 0.000064, 0.000092, 0.000110, 0.000131, 0.000155, 0.000228, 0.000301, 0.000269, 0.000028};

// FALL 2018 in ELECTRONS
Double_t x_9bdt_F18inneg[15] = {1.000000, 1.000000, 1.0000030, 0.999939, 0.998915, 0.996333, 0.989499, 0.975808, 0.964085, 0.947217, 0.923288, 0.821697, 0.643917, 0.159175, 0.002367};
Double_t xerrB_9bdt_F18inneg[15] = {0.000001, 0.000001, 0.000001, 0.000008, 0.000033, 0.000060, 0.000101, 0.000152, 0.000184, 0.000221, 0.000263, 0.000378, 0.000473, 0.000362, 0.000048};

Double_t x_6bdt_F18inneg[15] = {1.000000, 1.000000, 0.999995, 0.999908, 0.999009, 0.997361, 0.993222, 0.985135, 0.977563, 0.966125, 0.949401, 0.870391, 0.714685, 0.237590, 0.005593};
Double_t xerrB_6bdt_F18inneg[15] = {0.000001, 0.000001, 0.000002, 0.000010, 0.000031, 0.000051, 0.000081, 0.000120, 0.000146, 0.000179, 0.000217, 0.000332, 0.000446, 0.000421, 0.000074};

// FALL 2018 out ELECTRONS cuts[15]={-0.6,-0.5,-0.4, -0.3,-0.2, -0.15,-0.1,-0.06,-0.04,-0.02,0.0,0.05,0.1, 0.2,0.3};
Double_t x_9bdt_F18outneg[15] = {1.000000, 1.000000, 1.000000, 0.999788, 0.997113, 0.991916, 0.979017, 0.957411, 0.939974, 0.915831, 0.881744, 0.733672, 0.486228, 0.061078, 0.000824};
Double_t xerrB_9bdt_F18outneg[15] = {0.000001, 0.000001, 0.000001, 0.000011, 0.000042, 0.000070, 0.000112, 0.000158, 0.000186, 0.000217, 0.000252, 0.000345, 0.000391, 0.000187, 0.000022};

Double_t x_6bdt_F18outneg[15] = {1.000000, 1.000000, 0.999913, 0.998955, 0.993975, 0.987316, 0.974733, 0.956402, 0.942476, 0.923968, 0.898606, 0.794311, 0.611804, 0.159863, 0.002404};
Double_t xerrB_6bdt_F18outneg[15] = {0.000001, 0.000001, 0.000007, 0.000025, 0.000060, 0.000087, 0.000123, 0.000160, 0.000182, 0.000207, 0.000236, 0.000316, 0.000381, 0.000286, 0.000038};
*/
void setupCanvas(TCanvas *canvas)
{

    canvas->SetFillColor(0);
    canvas->SetBorderMode(0);
    canvas->SetBorderSize(0);
    canvas->SetFrameFillColor(0);
    canvas->SetFrameBorderMode(0);
    // canvas->SetGrid();
}

void EqualArray(Double_t *NEWArray, Double_t *Array, bool bgk = false)
{
    for (int i = 0; i < 15; i++)
    {
        if (!bgk)
            NEWArray[i] = Array[i];
        else
            NEWArray[i] = 1.0 - Array[i];
    }
}

void SelectData(Double_t *Var, Double_t *VarErr, int model, bool type, TString name)
{
    if (name == "S19_positives")
    {
        if (type == 0)
        { // background
            if (model == 6)
            {
                EqualArray(Var, y_6bdt_S19pos, true);
                EqualArray(VarErr, yerrB_6bdt_S19pos);
            }
            else
            {
                EqualArray(Var, y_9bdt_S19pos, true);
                EqualArray(VarErr, yerrB_9bdt_S19pos);
            }
        }
        else
        { // Signal
            if (model == 6)
            {
                EqualArray(Var, x_6bdt_S19pos);
                EqualArray(VarErr, xerrB_6bdt_S19pos);
            }
            else
            {
                EqualArray(Var, x_9bdt_S19pos);
                EqualArray(VarErr, xerrB_9bdt_S19pos);
            }
        }
    }
    else if (name == "S19_negatives")
    {
        if (model == 6)
        {
            EqualArray(Var, x_6bdt_S19neg);
            EqualArray(VarErr, xerrB_6bdt_S19neg);
        }
        else
        {
            EqualArray(Var, x_9bdt_S19neg);
            EqualArray(VarErr, xerrB_9bdt_S19neg);
        }
    }
    else if (name == "F18in_positives")
    {
        if (type == 0)
        { // background
            if (model == 6)
            {
                EqualArray(Var, y_6bdt_F18inpos, true);
                EqualArray(VarErr, yerrB_6bdt_F18inpos);
            }
            else
            {
                EqualArray(Var, y_9bdt_F18inpos, true);
                EqualArray(VarErr, yerrB_9bdt_F18inpos);
            }
        }
        else
        { // Signal
            if (model == 6)
            {
                EqualArray(Var, x_6bdt_F18inpos);
                EqualArray(VarErr, xerrB_6bdt_F18inpos);
            }
            else
            {
                EqualArray(Var, x_9bdt_F18inpos);
                EqualArray(VarErr, xerrB_9bdt_F18inpos);
            }
        }
    }
    else if (name == "F18in_negatives")
    {
        if (model == 6)
        {
            EqualArray(Var, x_6bdt_F18inneg);
            EqualArray(VarErr, xerrB_6bdt_F18inneg);
        }
        else
        {
            EqualArray(Var, x_9bdt_F18inneg);
            EqualArray(VarErr, xerrB_9bdt_F18inneg);
        }
    }
    else if (name == "F18out_positives")
    {
        if (type == 0)
        { // background
            if (model == 6)
            {
                EqualArray(Var, y_6bdt_F18outpos, true);
                EqualArray(VarErr, yerrB_6bdt_F18outpos);
            }
            else
            {
                EqualArray(Var, y_9bdt_F18outpos, true);
                EqualArray(VarErr, yerrB_9bdt_F18outpos);
            }
        }
        else
        { // Signal
            if (model == 6)
            {
                EqualArray(Var, x_6bdt_F18outpos);
                EqualArray(VarErr, xerrB_6bdt_F18outpos);
            }
            else
            {
                EqualArray(Var, x_9bdt_F18outpos);
                EqualArray(VarErr, xerrB_9bdt_F18outpos);
            }
        }
    }
    else if (name == "F18out_negatives")
    {
        if (model == 6)
        {
            EqualArray(Var, x_6bdt_F18outneg);
            EqualArray(VarErr, xerrB_6bdt_F18outneg);
        }
        else
        {
            EqualArray(Var, x_9bdt_F18outneg);
            EqualArray(VarErr, xerrB_9bdt_F18outneg);
        }
    }
    else
    {
        cout << "ERROR: " << name << " not found. Setting all to zero" << endl;
    }
}

// Electron (negatives) FPR was never measured on real calibration data the way the
// positron FPR (y_*_pos arrays) was, so there is no hardcoded array to read it from.
// This computes it live from the MC validation tree instead -- note this is an MC
// estimate, not a real-data measurement like the electron TPR (x_*_neg) values are.
void GetMC_FPR(int model, TString name, Double_t cut, Double_t &fpr, Double_t &fpr_err)
{
    TString base = (model == 6) ? "6-BDT/" : "9-BDT/";
    TString file = "Files/" + base + name + "_Final.root";

    TFile *f = TFile::Open(file);
    if (!f || f->IsZombie())
    {
        cout << "ERROR: could not open " << file << endl;
        fpr = 0.0;
        fpr_err = 0.0;
        return;
    }
    TTree *tree = (TTree *)f->Get("resN");

    Int_t TN = tree->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cut, 4.5, 8.0, 0.05, 0.3));
    Int_t FP = tree->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cut, 4.5, 8.0, 0.05, 0.3));
    Double_t N = TN + FP;

    fpr = (N > 0) ? FP / N : 0.0;
    fpr_err = (N > 0) ? sqrt(fpr * (1.0 - fpr) / N) : 0.0;

    f->Close();
    delete f;
}

void Print_Values(TString name)
{
    Double_t x_6bdt[15];
    Double_t xerr_6bdt[15];

    Double_t x_9bdt[15];
    Double_t xerr_9bdt[15];

    Double_t cuts[15] = {-0.6, -0.5, -0.4, -0.3, -0.2, -0.15, -0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1, 0.2, 0.3};

    cout << fixed;
    cout << setprecision(5);
    SelectData(x_6bdt, xerr_6bdt, 6, 0, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 0, name);
    cout << "----" << name << "-----" << endl;
    cout << "MODEL: 6" << endl;
    // cout<<"Cut [-0.06] = "<<1.0-x_6bdt[7]<<"+-"<<xerr_6bdt[7]<<endl;
    cout << "Cut [ 0.00] = " << 1.0 - x_6bdt[10] << "+-" << xerr_6bdt[10] << endl;

    cout << "MODEL: 9" << endl;
    // cout<<"Cut [-0.06] = "<<1.0-x_9bdt[7]<<"+-"<<xerr_9bdt[7]<<endl;
    cout << "Cut [ 0.00] = " << 1.0 - x_9bdt[10] << "+-" << xerr_9bdt[10] << endl;
    cout << endl;
}

void Print_Values_LATEX(int cut_n, TString name)
{
    Double_t x_6bdt[15];
    Double_t xerr_6bdt[15];
    Double_t y_6bdt[15];
    Double_t yerr_6bdt[15];

    Double_t x_9bdt[15];
    Double_t xerr_9bdt[15];
    Double_t y_9bdt[15];
    Double_t yerr_9bdt[15];

    Double_t cuts[15] = {-0.6, -0.5, -0.4, -0.3, -0.2, -0.15, -0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1, 0.2, 0.3};

    // Open a file in write mode
    FILE *fptr;
    fptr = fopen(name.Data(), "w");

    // Check if file opened successfully
    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Write the LaTeX table to the file
    // NOTE: SelectData(..., type=0, ...) returns 1-FPR for positrons (EqualArray applies a
    // 1-x flip internally, which is correct for the ROC graphs' "1-False Positives Rate"
    // axis, but wrong for this table's plain "FPR" column) -- so it is un-flipped here
    // with (1.0 - y_*bdt[cut_n]) to get the real FPR value. The error is unaffected by the
    // flip (error on 1-x equals error on x).
    fprintf(fptr, "\\begin{tabular}{|c|c|c|c|}\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    Model & Configuration & TPR & FPR \\\\\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    \\hline\n");
    // Get model 6 values
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "S19_positives");
    SelectData(y_6bdt, yerr_6bdt, 6, 0, "S19_positives");
    fprintf(fptr, "    \\multirow{3}{*}{BDT-6} & Spring 2019 & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, (1.0 - y_6bdt[cut_n]) * 100, yerr_6bdt[cut_n] * 100);
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "F18in_positives");
    SelectData(y_6bdt, yerr_6bdt, 6, 0, "F18in_positives");
    fprintf(fptr, "    & Fall 2018 Inbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, (1.0 - y_6bdt[cut_n]) * 100, yerr_6bdt[cut_n] * 100);
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "F18out_positives");
    SelectData(y_6bdt, yerr_6bdt, 6, 0, "F18out_positives");
    fprintf(fptr, "    & Fall 2018 Outbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, (1.0 - y_6bdt[cut_n]) * 100, yerr_6bdt[cut_n] * 100);
    fprintf(fptr, "    \\hline\n");
    // Get model 9 values
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "S19_positives");
    SelectData(y_9bdt, yerr_9bdt, 9, 0, "S19_positives");
    fprintf(fptr, "    \\multirow{3}{*}{BDT-9} & Spring 2019 & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, (1.0 - y_9bdt[cut_n]) * 100, yerr_9bdt[cut_n] * 100);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "F18in_positives");
    SelectData(y_9bdt, yerr_9bdt, 9, 0, "F18in_positives");
    fprintf(fptr, "    & Fall 2018 Inbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, (1.0 - y_9bdt[cut_n]) * 100, yerr_9bdt[cut_n] * 100);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "F18out_positives");
    SelectData(y_9bdt, yerr_9bdt, 9, 0, "F18out_positives");
    fprintf(fptr, "    & Fall 2018 Outbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, (1.0 - y_9bdt[cut_n]) * 100, yerr_9bdt[cut_n] * 100);
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "\\end{tabular}\n");

    fprintf(fptr, "\n\n\n");

    // Electron FPR has no real-data measurement hardcoded anywhere (unlike positron FPR,
    // which comes from the y_*_pos arrays). Computed live from the MC validation tree via
    // GetMC_FPR() instead -- this is an MC estimate, not a real-data measurement like the
    // TPR column (or like the positron FPR above).
    Double_t y_mc, yerr_mc;
    fprintf(fptr, "\\begin{tabular}{|c|c|c|c|}\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    Model & Configuration & TPR & FPR (MC) \\\\\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    \\hline\n");
    // Get model 6 values
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "S19_negatives");
    GetMC_FPR(6, "S19_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    \\multirow{3}{*}{BDT-6} & Spring 2019 & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "F18in_negatives");
    GetMC_FPR(6, "F18in_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    & Fall 2018 Inbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    SelectData(x_6bdt, xerr_6bdt, 6, 1, "F18out_negatives");
    GetMC_FPR(6, "F18out_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    & Fall 2018 Outbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_6bdt[cut_n] * 100, xerr_6bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    fprintf(fptr, "    \\hline\n");
    // Get model 9 values
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "S19_negatives");
    GetMC_FPR(9, "S19_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    \\multirow{3}{*}{BDT-9} & Spring 2019 & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "F18in_negatives");
    GetMC_FPR(9, "F18in_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    & Fall 2018 Inbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, "F18out_negatives");
    GetMC_FPR(9, "F18out_negatives", cuts[cut_n], y_mc, yerr_mc);
    fprintf(fptr, "    & Fall 2018 Outbending & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n", x_9bdt[cut_n] * 100, xerr_9bdt[cut_n] * 100, y_mc * 100, yerr_mc * 100);
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "\\end{tabular}\n");

    // Close the file
    fclose(fptr);

    printf("Table written to %s \n", name.Data());
}

void ROC_All(TString infile, TString infile2, TString name, bool endf = false, bool cuts_b = true)
{

    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height

    TFile *Data_file = new TFile(infile + ".root");
    TTree *Data_tree_P = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N = (TTree *)Data_file->Get("resN");

    TFile *Data_file2 = new TFile(infile2 + ".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");

    Float_t scoreBDT_P, scoreBDT_N, score, scoreMLP_P, scoreMLP_N;

    //---------------File 1-------------------------------

    Data_tree_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree_P->SetBranchAddress("scoreMLP_P", &scoreMLP_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    Data_tree_N->SetBranchAddress("scoreMLP_N", &scoreMLP_N);

    //---------------File 2-------------------------------

    Data_tree2_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree2_P->SetBranchAddress("scoreMLP_P", &scoreMLP_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    Data_tree2_N->SetBranchAddress("scoreMLP_N", &scoreMLP_N);

    Float_t segments, start, end;
    Float_t start_BDT, end_BDT;
    Float_t TPR, FPR;
    Float_t score_cut;
    Int_t max = 1000;
    int TP = 0;
    int FP = 0;
    int TN = 0;
    int FN = 0;
    int i = 1;
    Double_t x_bdt[max], y_bdt[max];

    Double_t x2_bdt[max], y2_bdt[max];

    start_BDT = -0.8;
    end_BDT = 0.8;

    i = 1;

    cout << "ROC " << infile << "--BDT" << endl;
    

    start = start_BDT;
    end = end_BDT;
    segments = (end - start) / max;
    score_cut = start;

    while (score_cut <= end)
    {
        if (cuts_b == false)
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g", score_cut));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g", score_cut));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g", score_cut));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g", score_cut));
        }
        else
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);

        x_bdt[i - 1] = TPR;
        y_bdt[i - 1] = 1 - FPR;

        score_cut = start + i * segments;
        i++;
    }

    i = 1;

    cout << "ROC " << infile2 << "--BDT" << endl;

    start = start_BDT;
    end = end_BDT;
    segments = (end - start) / max;
    score_cut = start;

    while (score_cut <= end)
    {
        if (cuts_b == false)
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g", score_cut));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g", score_cut));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g", score_cut));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g", score_cut));
        }
        else
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);

        x2_bdt[i - 1] = TPR;
        y2_bdt[i - 1] = 1 - FPR;

        score_cut = start + i * segments;
        i++;
    }
    // Print_Table(TP,FP,TN,FN);

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
    TGraph *gr_BDT = new TGraph(max, x_bdt, y_bdt);
    TGraph *gr2_BDT = new TGraph(max, x2_bdt, y2_bdt);

    // x is true positives rate: signal efficiency
    // y is 1-False positive rate: 1-background suppresion.
    Double_t y_6bdt[15];
    Double_t x_6bdt[15];
    Double_t yerr_6bdt[15];
    Double_t xerr_6bdt[15];

    Double_t y_9bdt[15];
    Double_t x_9bdt[15];
    Double_t yerr_9bdt[15];
    Double_t xerr_9bdt[15];

    SelectData(x_6bdt, xerr_6bdt, 6, 1, name);
    SelectData(y_6bdt, yerr_6bdt, 6, 0, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, name);
    SelectData(y_9bdt, yerr_9bdt, 9, 0, name);

    TGraphErrors *gr_6BDT_data = new TGraphErrors(15, x_6bdt, y_6bdt, xerr_6bdt, yerr_6bdt);
    TGraphErrors *gr_9BDT_data = new TGraphErrors(15, x_9bdt, y_9bdt, xerr_9bdt, yerr_9bdt);
    // TGraph *gr_9BDT_data  = new TGraph(11,x2_bdt,y2_bdt);

    cout << "PLOT ROC BDT" << endl;
    Float_t areaBDT6, areaBDT9;
    Float_t areaBDT6_data, areaBDT9_data;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; True Positives Rate; 1- False Positives Rate");

    auto legend = new TLegend(0.15, 0.15, 0.35, 0.35);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.03);
    legend->SetLineWidth(0);
    mg->GetXaxis()->SetRangeUser(0.75, 1.025);
    mg->GetYaxis()->SetRangeUser(0.75, 1.01);

    gr_BDT->SetName("BDT9");
    gr_BDT->SetLineWidth(4);
    gr_BDT->SetLineColorAlpha(kRed, 0.45);

    gr2_BDT->SetName("BDT6");
    gr2_BDT->SetLineWidth(4);
    gr2_BDT->SetLineColorAlpha(kMagenta, 0.45);

    gr_6BDT_data->SetName("BDT6_data");
    gr_6BDT_data->SetMarkerStyle(21);
    gr_6BDT_data->SetMarkerColor(kMagenta);
    gr_6BDT_data->SetLineColor(kMagenta);

    gr_9BDT_data->SetName("BDT9_data");
    gr_9BDT_data->SetMarkerStyle(21);
    gr_9BDT_data->SetMarkerColor(kRed);
    gr_9BDT_data->SetLineColor(kRed);
    // gr_9BDT_data->SetLineWidth(4);

    mg->Add(gr_BDT);
    mg->Add(gr2_BDT);
    mg->Add(gr_6BDT_data, "pl");
    mg->Add(gr_9BDT_data, "pl");
    mg->Draw("AL");

    int n_mc = gr2_BDT->GetN();
    int n_data = gr_6BDT_data->GetN();
    std::vector<std::pair<double, double>> pointsBDT6(n_mc);
    std::vector<std::pair<double, double>> pointsBDT9(n_mc);


    for (int i = 0; i < n_mc; ++i) {
        double x, y;
        gr2_BDT->GetPoint(i, x, y);
        pointsBDT6[i] = std::make_pair(x, y);
        gr_BDT->GetPoint(i, x, y);
        pointsBDT9[i] = std::make_pair(x, y);
    }
    std::sort(pointsBDT6.begin(), pointsBDT6.end());
    std::sort(pointsBDT9.begin(), pointsBDT9.end());

    double auc6 = 0.0;
    double auc9 = 0.0;
    for (int i = 1; i < n_mc; ++i) {
        double x0 = pointsBDT6[i - 1].first;
        double x1 = pointsBDT6[i].first;
        double y0 = pointsBDT6[i - 1].second;
        double y1 = pointsBDT6[i].second;
        auc6 += 0.5 * (y0 + y1) * (x1 - x0); // Trapezoidal area
         x0 = pointsBDT9[i - 1].first;
         x1 = pointsBDT9[i].first;
         y0 = pointsBDT9[i - 1].second;
         y1 = pointsBDT9[i].second;
        auc9 += 0.5 * (y0 + y1) * (x1 - x0); // Trapezoidal area
    }

    std::cout << "AUC6 = " << auc6 << std::endl;
    std::cout << "AUC6 = " << auc9 << std::endl;
    areaBDT6=auc6;
    areaBDT9=auc9;

    std::vector<std::pair<double, double>> pointsBDT6_data(n_data);
    std::vector<std::pair<double, double>> pointsBDT9_data(n_data);

    
    for (int i = 0; i < n_data; ++i) {
        double x, y;
        gr_6BDT_data->GetPoint(i, x, y);
        pointsBDT6_data[i] = std::make_pair(x, y);

        gr_9BDT_data->GetPoint(i, x, y);
        pointsBDT9_data[i] = std::make_pair(x, y);
    }
    // 2. Append ROC curve endpoints (0,1) and (1,0)
    pointsBDT6_data.push_back({0.0, 1.0});
    pointsBDT6_data.push_back({1.0, 0.0});

    pointsBDT9_data.push_back({0.0, 1.0});
    pointsBDT9_data.push_back({1.0, 0.0});

    std::sort(pointsBDT6_data.begin(), pointsBDT6_data.end());
    std::sort(pointsBDT9_data.begin(), pointsBDT9_data.end());

    auc6 = 0.0;
    auc9 = 0.0;
    for (int i = 1; i < n_data; ++i) {
        double x0 = pointsBDT6_data[i - 1].first;
        double x1 = pointsBDT6_data[i].first;
        double y0 = pointsBDT6_data[i - 1].second;
        double y1 = pointsBDT6_data[i].second;
        auc6 += 0.5 * (y0 + y1) * (x1 - x0); // Trapezoidal area
         x0 = pointsBDT9_data[i - 1].first;
         x1 = pointsBDT9_data[i].first;
         y0 = pointsBDT9_data[i - 1].second;
         y1 = pointsBDT9_data[i].second;
        auc9 += 0.5 * (y0 + y1) * (x1 - x0); // Trapezoidal area
    }

    

    areaBDT6_data=auc6;
    areaBDT9_data=auc9;

    // legend->AddEntry("gr1",Form("MLP 9 variables (%f)",areaMLP),"l");
    legend->AddEntry("BDT6", Form("BDT 6 MC   (AUC=%f)",areaBDT6), "l");
    //legend->AddEntry("BDT6_data", Form("BDT 6 Data"), "p");
    legend->AddEntry("BDT6_data", Form("BDT 6 Data (AUC=%f)",areaBDT6_data), "p");
    legend->AddEntry("BDT9", Form("BDT 9 MC   (AUC=%f)",areaBDT9), "l");
    //legend->AddEntry("BDT9_data",  Form("BDT 9 Data"), "p");
    legend->AddEntry("BDT9_data",  Form("BDT 9 Data (AUC=%f)",areaBDT9_data), "p");
    legend->Draw();

    //TPaveLabel *t = new TPaveLabel(0.3, 0.92, 0.6, 1, name, "brNDC"); // left-up
    //t->Draw();


    c1->SaveAs("ROC_"+name+"_AUCinMCDATA.pdf");
}

void ROC_All_Article(TString infile, TString infile2, TString name, bool endf = false, bool cuts_b = true)
{
    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.045, "xyz");
    gStyle->SetTitleSize(.045, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8);
    gStyle->SetTitleH(0.1);

    TFile *Data_file = new TFile(infile + ".root");
    TTree *Data_tree_P = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N = (TTree *)Data_file->Get("resN");

    TFile *Data_file2 = new TFile(infile2 + ".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");

    Float_t scoreBDT_P, scoreBDT_N, score, scoreMLP_P, scoreMLP_N;

    Data_tree_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree_P->SetBranchAddress("scoreMLP_P", &scoreMLP_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    Data_tree_N->SetBranchAddress("scoreMLP_N", &scoreMLP_N);

    Data_tree2_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree2_P->SetBranchAddress("scoreMLP_P", &scoreMLP_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    Data_tree2_N->SetBranchAddress("scoreMLP_N", &scoreMLP_N);

    Float_t segments, start, end;
    Float_t start_BDT, end_BDT;
    Float_t TPR, FPR;
    Float_t score_cut;
    Int_t max = 1000;
    int TP = 0;
    int FP = 0;
    int TN = 0;
    int FN = 0;
    int i = 1;
    Double_t x_bdt[max], y_bdt[max];
    Double_t x2_bdt[max], y2_bdt[max];

    start_BDT = -0.8;
    end_BDT = 0.8;
    i = 1;

    cout << "ROC " << infile << "--BDT" << endl;

    start = start_BDT;
    end = end_BDT;
    segments = (end - start) / max;
    score_cut = start;

    while (score_cut <= end)
    {
        if (cuts_b == false)
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g", score_cut));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g", score_cut));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g", score_cut));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g", score_cut));
        }
        else
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);
        x_bdt[i - 1] = TPR;
        y_bdt[i - 1] = 1 - FPR;
        score_cut = start + i * segments;
        i++;
    }

    i = 1;
    cout << "ROC " << infile2 << "--BDT" << endl;

    start = start_BDT;
    end = end_BDT;
    segments = (end - start) / max;
    score_cut = start;

    while (score_cut <= end)
    {
        if (cuts_b == false)
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g", score_cut));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g", score_cut));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g", score_cut));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g", score_cut));
        }
        else
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", score_cut, 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);
        x2_bdt[i - 1] = TPR;
        y2_bdt[i - 1] = 1 - FPR;
        score_cut = start + i * segments;
        i++;
    }

    TString save_dir = "/Users/mariana/Work/Results/Article/Figure_9/";
    gSystem->mkdir(save_dir, kTRUE);

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
    c1->SetLeftMargin(0.14);
    TGraph *gr_BDT  = new TGraph(max, x_bdt,  y_bdt);
    TGraph *gr2_BDT = new TGraph(max, x2_bdt, y2_bdt);

    Double_t y_6bdt[15], x_6bdt[15], yerr_6bdt[15], xerr_6bdt[15];
    Double_t y_9bdt[15], x_9bdt[15], yerr_9bdt[15], xerr_9bdt[15];

    SelectData(x_6bdt, xerr_6bdt, 6, 1, name);
    SelectData(y_6bdt, yerr_6bdt, 6, 0, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, name);
    SelectData(y_9bdt, yerr_9bdt, 9, 0, name);

    TGraphErrors *gr_6BDT_data = new TGraphErrors(15, x_6bdt, y_6bdt, xerr_6bdt, yerr_6bdt);
    TGraphErrors *gr_9BDT_data = new TGraphErrors(15, x_9bdt, y_9bdt, xerr_9bdt, yerr_9bdt);

    cout << "PLOT ROC BDT" << endl;
    Float_t areaBDT6, areaBDT9;
    Float_t areaBDT6_data, areaBDT9_data;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; True Positives Rate; 1- False Positives Rate");
    mg->GetYaxis()->SetTitleOffset(1.5);

    Color_t colorBDT6 = kGreen+2; // stronger green (less teal): contrasts with red, still distinct in grayscale

    auto legend = new TLegend(0.15, 0.15, 0.35, 0.35);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.04);
    legend->SetLineWidth(0);
    mg->GetXaxis()->SetRangeUser(0.75, 1.025);
    mg->GetYaxis()->SetRangeUser(0.75, 1.01);

    gr_BDT->SetName("BDT9");
    gr_BDT->SetLineWidth(4);
    gr_BDT->SetLineColorAlpha(kRed, 0.45);

    gr2_BDT->SetName("BDT6");
    gr2_BDT->SetLineWidth(4);
    gr2_BDT->SetLineColorAlpha(colorBDT6, 0.45);

    gr_6BDT_data->SetName("BDT6_data");
    gr_6BDT_data->SetMarkerStyle(22);
    gr_6BDT_data->SetMarkerSize(2);
    gr_6BDT_data->SetMarkerColor(colorBDT6);
    //gr_6BDT_data->SetLineColor(colorBDT6);

    gr_9BDT_data->SetName("BDT9_data");
    gr_9BDT_data->SetMarkerStyle(21);
    gr_9BDT_data->SetMarkerSize(2);
    gr_9BDT_data->SetMarkerColor(kRed);
    //gr_9BDT_data->SetLineColor(kRed);

    mg->Add(gr_BDT);
    mg->Add(gr2_BDT);
    mg->Add(gr_6BDT_data, "p");
    mg->Add(gr_9BDT_data, "p");
    mg->Draw("AL");

    int n_mc   = gr2_BDT->GetN();
    int n_data = gr_6BDT_data->GetN();
    std::vector<std::pair<double, double>> pointsBDT6(n_mc);
    std::vector<std::pair<double, double>> pointsBDT9(n_mc);

    for (int i = 0; i < n_mc; ++i) {
        double x, y;
        gr2_BDT->GetPoint(i, x, y);
        pointsBDT6[i] = std::make_pair(x, y);
        gr_BDT->GetPoint(i, x, y);
        pointsBDT9[i] = std::make_pair(x, y);
    }
    std::sort(pointsBDT6.begin(), pointsBDT6.end());
    std::sort(pointsBDT9.begin(), pointsBDT9.end());

    double auc6 = 0.0;
    double auc9 = 0.0;
    for (int i = 1; i < n_mc; ++i) {
        double x0 = pointsBDT6[i - 1].first,  x1 = pointsBDT6[i].first;
        double y0 = pointsBDT6[i - 1].second, y1 = pointsBDT6[i].second;
        auc6 += 0.5 * (y0 + y1) * (x1 - x0);
        x0 = pointsBDT9[i - 1].first;  x1 = pointsBDT9[i].first;
        y0 = pointsBDT9[i - 1].second; y1 = pointsBDT9[i].second;
        auc9 += 0.5 * (y0 + y1) * (x1 - x0);
    }
    std::cout << "AUC6 = " << auc6 << std::endl;
    std::cout << "AUC9 = " << auc9 << std::endl;
    areaBDT6 = auc6;
    areaBDT9 = auc9;

    std::vector<std::pair<double, double>> pointsBDT6_data(n_data);
    std::vector<std::pair<double, double>> pointsBDT9_data(n_data);

    for (int i = 0; i < n_data; ++i) {
        double x, y;
        gr_6BDT_data->GetPoint(i, x, y);
        pointsBDT6_data[i] = std::make_pair(x, y);
        gr_9BDT_data->GetPoint(i, x, y);
        pointsBDT9_data[i] = std::make_pair(x, y);
    }
    pointsBDT6_data.push_back({0.0, 1.0});
    pointsBDT6_data.push_back({1.0, 0.0});
    pointsBDT9_data.push_back({0.0, 1.0});
    pointsBDT9_data.push_back({1.0, 0.0});
    std::sort(pointsBDT6_data.begin(), pointsBDT6_data.end());
    std::sort(pointsBDT9_data.begin(), pointsBDT9_data.end());

    auc6 = 0.0;
    auc9 = 0.0;
    for (int i = 1; i < n_data; ++i) {
        double x0 = pointsBDT6_data[i - 1].first,  x1 = pointsBDT6_data[i].first;
        double y0 = pointsBDT6_data[i - 1].second, y1 = pointsBDT6_data[i].second;
        auc6 += 0.5 * (y0 + y1) * (x1 - x0);
        x0 = pointsBDT9_data[i - 1].first;  x1 = pointsBDT9_data[i].first;
        y0 = pointsBDT9_data[i - 1].second; y1 = pointsBDT9_data[i].second;
        auc9 += 0.5 * (y0 + y1) * (x1 - x0);
    }
    areaBDT6_data = auc6;
    areaBDT9_data = auc9;

    legend->AddEntry("BDT6",      Form("BDT 6 MC   (AUC=%.3f)", areaBDT6),      "l");
    legend->AddEntry("BDT6_data", Form("BDT 6 Data (AUC=%.3f)", areaBDT6_data), "p");
    legend->AddEntry("BDT9",      Form("BDT 9 MC   (AUC=%.3f)", areaBDT9),      "l");
    legend->AddEntry("BDT9_data", Form("BDT 9 Data (AUC=%.3f)", areaBDT9_data), "p");
    legend->Draw();

    TString outname = save_dir + "ROC_" + name + "_AUCinMCDATA.pdf";
    cout << "Saving in " << outname << endl;
    c1->SaveAs(outname);

    TString outroot = outname;
    outroot.ReplaceAll(".pdf", ".root");
    TFile *fout = new TFile(outroot, "RECREATE");
    gr_BDT->Write("BDT9");
    gr2_BDT->Write("BDT6");
    gr_6BDT_data->Write("BDT6_data");
    gr_9BDT_data->Write("BDT9_data");
    mg->Write("MultiGraph");
    c1->Write("Canvas");
    fout->Close();
    delete fout;
}

/*
void ROC_All_Training(TString infile, TString infile2, TString name, bool endf=false)
{

    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height

    TFile *Data_file = new TFile(infile + ".root");
    TTree *Data_tree = (TTree *)Data_file->Get("tree");

    Float_t P, Theta, Phi,SFPCAL,SFECIN,SFECOUT, m2PCAL, m2ECIN, m2ECOUT;

    //---------------File 1-------------------------------

    Data_tree->SetBranchAddress("P", &P);
    Data_tree->SetBranchAddress("Theta", &Theta);
    Data_tree->SetBranchAddress("Phi", &Phi);
    Data_tree->SetBranchAddress("SFPCAL", &SFPCAL);
    Data_tree->SetBranchAddress("SFECIN", &SFECIN);
    Data_tree->SetBranchAddress("SFECOUT", &SFECOUT);
    Data_tree->SetBranchAddress("m2PCAL", &m2PCAL);
    Data_tree->SetBranchAddress("m2ECIN", &m2ECIN);
    Data_tree->SetBranchAddress("m2ECOUT", &m2ECOUT);


    Float_t segments, start, end;
    Float_t start_BDT, end_BDT;
    Float_t TPR, FPR;
    Float_t score_cut;
    Int_t max = 15;
    Double_t cuts[15] = {-0.6, -0.5, -0.4, -0.3, -0.2, -0.15, -0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1, 0.2, 0.3};
    int TP[15]={0};
    int i = 1;
    Double_t x_bdt[max], y_bdt[max];

    Double_t x2_bdt[max], y2_bdt[max];

    start_BDT = -0.8;
    end_BDT = 0.8;

    i = 1;
    TMVA::Reader *reader_6 = new TMVA::Reader( "!Color:Silent" );
    TMVA::Reader *reader_9 = new TMVA::Reader( "!Color:Silent" );


    // Declare variables
    float P, Theta, Phi;
    float SFPCAL, SFECIN, SFECOUT, m2PCAL, m2ECIN, m2ECOUT;

    // Reader1 reads the last six variables
    reader_6->AddVariable("SFPCAL", &SFPCAL);
    reader_6->AddVariable("SFECIN", &SFECIN);
    reader_6->AddVariable("SFECOUT", &SFECOUT);
    reader_6->AddVariable("m2PCAL", &m2PCAL);
    reader_6->AddVariable("m2ECIN", &m2ECIN);
    reader_6->AddVariable("m2ECOUT", &m2ECOUT);

    // Reader2 reads all variables
    reader_9->AddVariable("P", &P);
    reader_9->AddVariable("Theta", &Theta);
    reader_9->AddVariable("Phi", &Phi);
    reader_9->AddVariable("SFPCAL", &SFPCAL);
    reader_9->AddVariable("SFECIN", &SFECIN);
    reader_9->AddVariable("SFECOUT", &SFECOUT);
    reader_9->AddVariable("m2PCAL", &m2PCAL);
    reader_9->AddVariable("m2ECIN", &m2ECIN);
    reader_9->AddVariable("m2ECOUT", &m2ECOUT);


    TString weightfile6="6-BDT-MLP/dataset_"+name+"_mod/weights/TMVAClassification_BDT.weights.xml";
    TString weightfile9="9-BDT-MLP/dataset_"+name"/weights/TMVAClassification_BDT.weights.xml";
    reader_6->BookMVA("BDT 6", weightfile6);
    reader_9->BookMVA("BDT 9", weightfile9);


    cout << "ROC " << infile << "--BDT" << endl;

    start = start_BDT;
    end = end_BDT;
    segments = (end - start) / max;
    score_cut = start;


    for (Long64_t ievt=0; ievt<Data_tree->GetEntries();ievt++)
    {
        Data_tree->GetEntry(ievt);
        float score6 = reader_6->EvaluateMVA("BDT 6");
        float score9 = reader_9->EvaluateMVA("BDT 9");


        for(int i_c=0;i_c<15;i_c++){
            f(score6>cuts[i_c])
                TP[i_c]++;
        }


        x_bdt[i - 1] = TPR;
        y_bdt[i - 1] = 1 - FPR;

        x2_bdt[i - 1] = TPR;
        y2_bdt[i - 1] = 1 - FPR;

        score_cut = start + i * segments;
        i++;
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
    TGraph *gr_BDT = new TGraph(max, x_bdt, y_bdt);
    TGraph *gr2_BDT = new TGraph(max, x2_bdt, y2_bdt);

    // x is true positives rate: signal efficiency
    // y is 1-False positive rate: 1-background suppresion.
    Double_t y_6bdt[15];
    Double_t x_6bdt[15];
    Double_t yerr_6bdt[15];
    Double_t xerr_6bdt[15];

    Double_t y_9bdt[15];
    Double_t x_9bdt[15];
    Double_t yerr_9bdt[15];
    Double_t xerr_9bdt[15];

    SelectData(x_6bdt, xerr_6bdt, 6, 1, name);
    SelectData(y_6bdt, yerr_6bdt, 6, 0, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, name);
    SelectData(y_9bdt, yerr_9bdt, 9, 0, name);

    TGraphErrors *gr_6BDT_data = new TGraphErrors(15, x_6bdt, y_6bdt, xerr_6bdt, yerr_6bdt);
    TGraphErrors *gr_9BDT_data = new TGraphErrors(15, x_9bdt, y_9bdt, xerr_9bdt, yerr_9bdt);
    // TGraph *gr_9BDT_data  = new TGraph(11,x2_bdt,y2_bdt);

    cout << "PLOT ROC BDT" << endl;
    Float_t areaMLP, areaBDT;
    Float_t areaMLP2, areaBDT2;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; True Positives Rate; 1- False Positives Rate");

    auto legend = new TLegend();
    legend->SetFillStyle(0);
    legend->SetLineWidth(0);
    mg->GetXaxis()->SetRangeUser(0.750, 1.025);
    mg->GetYaxis()->SetRangeUser(0.750, 1.01);

    gr_BDT->SetName("BDT9");
    gr_BDT->SetLineWidth(4);
    gr_BDT->SetLineColorAlpha(kRed, 0.45);

    gr2_BDT->SetName("BDT6");
    gr2_BDT->SetLineWidth(4);
    gr2_BDT->SetLineColorAlpha(kMagenta, 0.45);

    gr_6BDT_data->SetName("BDT6_data");
    gr_6BDT_data->SetMarkerStyle(21);
    gr_6BDT_data->SetMarkerColor(kMagenta);
    gr_6BDT_data->SetLineColor(kMagenta);

    gr_9BDT_data->SetName("BDT9_data");
    gr_9BDT_data->SetMarkerStyle(21);
    gr_9BDT_data->SetMarkerColor(kRed);
    gr_9BDT_data->SetLineColor(kRed);
    // gr_9BDT_data->SetLineWidth(4);

    mg->Add(gr_BDT);
    mg->Add(gr2_BDT);
    mg->Add(gr_6BDT_data, "pl");
    mg->Add(gr_9BDT_data, "pl");
    mg->Draw("AL");

    // legend->AddEntry("gr1",Form("MLP 9 variables (%f)",areaMLP),"l");
    legend->AddEntry("BDT6", "BDT 6 variables Simulations", "l");
    legend->AddEntry("BDT6_data", "BDT 6 variables Data", "p");
    legend->AddEntry("BDT9", "BDT 9 variables Simulations", "l");
    legend->AddEntry("BDT9_data", "BDT 9 variables Data", "p");
    legend->Draw();

    TPaveLabel *t = new TPaveLabel(0.3, 0.92, 0.6, 1, name, "brNDC"); // left-up
    t->Draw();


    if (endf)
        c1->SaveAs("Results/SEP24/ROC_zoom.pdf)");
    else
        c1->SaveAs("Results/SEP24/ROC_zoom.pdf(");
}
*/

void Get_ratios(TString infile, TString infile2, TString name, bool end = false, bool cuts_b = true)
{

    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.04, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height

    TFile *Data_file = new TFile(infile + ".root");
    TTree *Data_tree_P = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N = (TTree *)Data_file->Get("resN");

    TFile *Data_file2 = new TFile(infile2 + ".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");

    Float_t scoreBDT_P, scoreBDT_N, score;

    //---------------File 1-------------------------------

    Data_tree_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    //---------------File 2-------------------------------

    Data_tree2_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);

    Float_t TPR, FPR;
    Int_t max = 500;
    int TP = 0;
    int FP = 0;
    int TN = 0;
    int FN = 0;
    Double_t x_mc_9bdt[max], y_mc_9bdt[max];
    Double_t x_mc_6bdt[max], y_mc_6bdt[max];

    Double_t cuts[15] = {-0.6, -0.5, -0.4, -0.3, -0.2, -0.15, -0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1, 0.2, 0.3};
    Double_t cuts_err[15] = {0};

    // cout<<"ROC "<<infile<<"--9BDT"<<endl;

    for (int i = 0; i < 15; i++)
    {
        if (cuts_b == false)
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g", cuts[i]));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g", cuts[i]));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g", cuts[i]));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g", cuts[i]));
        }
        else
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);

        x_mc_9bdt[i] = TPR;
        y_mc_9bdt[i] = 1 - FPR;
    }

    // cout<<"ROC "<<infile2<<"--6BDT"<<endl;

    for (int i = 0; i < 15; i++)
    {
        if (cuts_b == false)
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g", cuts[i]));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g", cuts[i]));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g", cuts[i]));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g", cuts[i]));
        }
        else
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
        }

        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);

        x_mc_6bdt[i] = TPR;
        y_mc_6bdt[i] = 1 - FPR;
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 700, 500);
    //c1->Divide(1, 2);
    c1->SetLeftMargin(0.15);

    // x is true positives rate: signal efficiency
    // y is 1-False positive rate: 1-background suppresion.

    Double_t x_6bdt[15];
    Double_t xerr_6bdt[15];

    Double_t x_9bdt[15];
    Double_t xerr_9bdt[15];

    SelectData(x_6bdt, xerr_6bdt, 6, 1, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, name);

    Double_t ratio_efficiency9[15];
    Double_t ratio_efficiency6[15];
    Double_t ratio_efficiency9_err[15] = {0};
    Double_t ratio_efficiency6_err[15] = {0};

    Double_t cut_zoom[7] = {-0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1};
    // Double_t cut_err[14]={0};

    // for(int i=6;i<13;i++){
    for (int i = 0; i < 15; i++)
    {
        ratio_efficiency9[i] = x_mc_9bdt[i] / x_9bdt[i];
        ratio_efficiency9_err[i] = ratio_efficiency9[i] * sqrt(pow(xerr_9bdt[i] / x_9bdt[i], 2));

        ratio_efficiency6[i] = x_mc_6bdt[i] / x_6bdt[i];
        ratio_efficiency6_err[i] = ratio_efficiency6[i] * sqrt(pow(xerr_6bdt[i] / x_6bdt[i], 2));
        // cout<<"ratio_efficiency "<<ratio_efficiency9[i]<<"+-"<<ratio_efficiency9_err[i-6]<<endl;
    }
    cout << fixed;
    cout << setprecision(3);

    TGraphErrors *gr_eff_9 = new TGraphErrors(14, cuts, ratio_efficiency9, cuts_err, ratio_efficiency9_err);
    TGraphErrors *gr_eff_6 = new TGraphErrors(14, cuts, ratio_efficiency6, cuts_err, ratio_efficiency6_err);

    gStyle->SetLabelSize(0.04, "x");
    gStyle->SetLabelSize(0.04, "y");

    gr_eff_9->SetName("BDT9");
    gr_eff_9->SetMarkerStyle(7);
    gr_eff_9->SetMarkerColor(kRed);
    gr_eff_9->SetLineColor(kRed);
    gr_eff_9->SetLineWidth(2);

    gr_eff_6->SetName("BDT6");
    gr_eff_6->SetMarkerStyle(7);
    gr_eff_6->SetMarkerColorAlpha(kMagenta, 0.75);
    gr_eff_6->SetLineColorAlpha(kMagenta, 0.75);
    gr_eff_6->SetLineWidth(2);

    for (int graph = 1; graph <= 2; graph++)
    {
        //c1->cd(graph);
        TMultiGraph *mg = new TMultiGraph();
        mg->SetTitle("; BDT Response Cut; Ratio Efficiency MC/Data");
        mg->Add(gr_eff_9);
        mg->Add(gr_eff_6);
        mg->Draw("APL");
        mg->SetMinimum(0.50);
        mg->SetMaximum(1.20);
        //TPaveLabel *t = new TPaveLabel(0.3, 0.92, 0.6, 1, name, "brNDC"); // left-up
        //t->Draw();

        auto legend = new TLegend(0.15, 0.15, 0.35, 0.35);
        legend->SetFillStyle(0);
        legend->SetTextSize(0.04);
        legend->SetLineWidth(0);
        legend->AddEntry("BDT6", "BDT 6 variables", "pl");
        legend->AddEntry("BDT9", "BDT 9 variables", "pl");
        legend->Draw();

       
        if (graph == 2)
        {
            
            // Define the fit function
            TF1 *fitFunc1 = new TF1("fitFunc1", "[0] + [1]*x + [2]*x^2", -0.2, 0.11);
            fitFunc1->SetLineColor(kGreen + 4);

            // Fit the first graph independently
            gr_eff_6->Fit(fitFunc1, "", "", -0.2, 0.11);

            // Retrieve fit parameters for the first fit
            double p0_1 = fitFunc1->GetParameter(0);
            double p1_1 = fitFunc1->GetParameter(1);
            double p2_1 = fitFunc1->GetParameter(2);

            // Define a second independent fit function
            TF1 *fitFunc2 = new TF1("fitFunc2", "[0] + [1]*x + [2]*x^2", -0.2, 0.11);
            fitFunc2->SetLineColor(kBlue + 2);  // Different color for distinction

            // Fit the second graph independently
            gr_eff_9->Fit(fitFunc2, "", "", -0.2, 0.11);

            // Retrieve fit parameters for the second fit
            double p0_2 = fitFunc2->GetParameter(0);
            double p1_2 = fitFunc2->GetParameter(1);
            double p2_2 = fitFunc2->GetParameter(2);

            // Draw fit equations for both fits
            TLatex eq;
            eq.SetNDC();
            eq.SetTextSize(0.04);
            eq.SetTextColor(kGreen + 4);
            eq.DrawLatex(0.15, 0.85, Form(" y = %.2f + %.2f*x + %.2f*x^{2}", p0_1, p1_1, p2_1));

            //eq.SetTextColor(kBlue + 2);
            //eq.DrawLatex(0.15, 0.80, Form("BDT 9: y = %.2f + %.2f*x + %.2f*x^{2}", p0_2, p1_2, p2_2));

            
            mg->GetXaxis()->SetLimits(-0.21, 0.11);
            mg->SetMinimum(0.90);
            mg->SetMaximum(1.15); // 1.90 for electrons
            //  cout<<std::max(ratio_efficiency9[12],ratio_efficiency9[12]) * 1.1<<endl;
            //gPad->Modified();
            //gPad->Update();
            //c1->Modified();
            c1->SaveAs("Ratios_"+name+".pdf");
        }
        else
        {
            // Create a TBox to represent the square
            TBox *square = new TBox(-0.21, 0.90, 0.11, 1.15);

            // Set square attributes (optional)
            square->SetFillStyle(0); // Transparent
            square->SetLineStyle(3);
            square->SetLineColor(kBlue); // blue border
            square->SetLineWidth(2);     // thicker border

            // Draw the square
            square->Draw();
        }
        
         
    }

    
       
    // legend->Clear();
}

void Get_ratios_Article(TString infile, TString infile2, TString name, bool end = false, bool cuts_b = true)
{
    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.045, "xyz");
    gStyle->SetTitleSize(.045, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8);
    gStyle->SetTitleH(0.1);

    TFile *Data_file = new TFile(infile + ".root");
    TTree *Data_tree_P = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N = (TTree *)Data_file->Get("resN");

    TFile *Data_file2 = new TFile(infile2 + ".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");

    Float_t scoreBDT_P, scoreBDT_N, score;

    Data_tree_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);
    Data_tree2_P->SetBranchAddress("scoreBDT_P", &scoreBDT_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N", &scoreBDT_N);

    Float_t TPR, FPR;
    Int_t max = 500;
    int TP = 0;
    int FP = 0;
    int TN = 0;
    int FN = 0;
    Double_t x_mc_9bdt[max], y_mc_9bdt[max];
    Double_t x_mc_6bdt[max], y_mc_6bdt[max];

    Double_t cuts[15] = {-0.6, -0.5, -0.4, -0.3, -0.2, -0.15, -0.1, -0.06, -0.04, -0.02, 0.0, 0.05, 0.1, 0.2, 0.3};
    Double_t cuts_err[15] = {0};

    for (int i = 0; i < 15; i++)
    {
        if (cuts_b == false)
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g", cuts[i]));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g", cuts[i]));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g", cuts[i]));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g", cuts[i]));
        }
        else
        {
            TP = Data_tree_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);
        x_mc_9bdt[i] = TPR;
        y_mc_9bdt[i] = 1 - FPR;
    }

    for (int i = 0; i < 15; i++)
    {
        if (cuts_b == false)
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g", cuts[i]));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g", cuts[i]));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g", cuts[i]));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g", cuts[i]));
        }
        else
        {
            TP = Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            TN = Data_tree2_N->GetEntries(Form("scoreBDT_N<%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FP = Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g&&P_N>=%g&&P_N<%g&&Theta_N>%g&&Theta_N<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
            FN = Data_tree2_P->GetEntries(Form("scoreBDT_P<%g&&P_P>=%g&&P_P<%g&&Theta_P>%g&&Theta_P<=%g", cuts[i], 4.5, 8.0, 0.05, 0.3));
        }
        TPR = (1.0 * TP) / (TP + FN);
        FPR = (1.0 * FP) / (FP + TN);
        x_mc_6bdt[i] = TPR;
        y_mc_6bdt[i] = 1 - FPR;
    }

    TString save_dir = "/Users/mariana/Work/Results/Article/Figure_10/";
    gSystem->mkdir(save_dir, kTRUE);

    TCanvas *c1 = new TCanvas("c1", "c1", 700, 500);
    c1->SetLeftMargin(0.15);

    Double_t x_6bdt[15];
    Double_t xerr_6bdt[15];
    Double_t x_9bdt[15];
    Double_t xerr_9bdt[15];

    SelectData(x_6bdt, xerr_6bdt, 6, 1, name);
    SelectData(x_9bdt, xerr_9bdt, 9, 1, name);

    Double_t ratio_efficiency9[15];
    Double_t ratio_efficiency6[15];
    Double_t ratio_efficiency9_err[15] = {0};
    Double_t ratio_efficiency6_err[15] = {0};

    for (int i = 0; i < 15; i++)
    {
        ratio_efficiency9[i] = x_mc_9bdt[i] / x_9bdt[i];
        ratio_efficiency9_err[i] = ratio_efficiency9[i] * sqrt(pow(xerr_9bdt[i] / x_9bdt[i], 2));
        ratio_efficiency6[i] = x_mc_6bdt[i] / x_6bdt[i];
        ratio_efficiency6_err[i] = ratio_efficiency6[i] * sqrt(pow(xerr_6bdt[i] / x_6bdt[i], 2));
    }
    cout << fixed;
    cout << setprecision(3);

    TGraphErrors *gr_eff_9 = new TGraphErrors(14, cuts, ratio_efficiency9, cuts_err, ratio_efficiency9_err);
    TGraphErrors *gr_eff_6 = new TGraphErrors(14, cuts, ratio_efficiency6, cuts_err, ratio_efficiency6_err);

    Color_t colorBDT6 = kGreen+2; // matches Figure_9: stronger green (less teal), contrasts with red

    gStyle->SetLabelSize(0.045, "x");
    gStyle->SetLabelSize(0.045, "y");

    gr_eff_9->SetName("BDT9");
    gr_eff_9->SetMarkerStyle(7);
    gr_eff_9->SetMarkerColor(kRed);
    gr_eff_9->SetLineColor(kRed);
    gr_eff_9->SetLineWidth(2);

    gr_eff_6->SetName("BDT6");
    gr_eff_6->SetMarkerStyle(7);
    gr_eff_6->SetMarkerColorAlpha(colorBDT6, 0.75);
    gr_eff_6->SetLineColorAlpha(colorBDT6, 0.75);
    gr_eff_6->SetLineWidth(2);

    for (int graph = 1; graph <= 2; graph++)
    {
        TMultiGraph *mg = new TMultiGraph();
        mg->SetTitle("; BDT Response Cut; Ratio Efficiency MC/Data");
        mg->Add(gr_eff_9);
        mg->Add(gr_eff_6);
        mg->Draw("APL");
        mg->SetMinimum(0.50);
        mg->SetMaximum(1.20);

        auto legend = new TLegend(0.15, 0.15, 0.35, 0.35);
        legend->SetFillStyle(0);
        legend->SetTextSize(0.04);
        legend->SetLineWidth(0);
        legend->AddEntry("BDT6", "BDT 6 variables", "pl");
        legend->AddEntry("BDT9", "BDT 9 variables", "pl");
        legend->Draw();

        if (graph == 2)
        {
            // Fit is computed (and saved to the .root file below) but not drawn on the canvas ("N" option).
            TF1 *fitFunc1 = new TF1("fitFunc1", "[0] + [1]*x + [2]*x^2", -0.2, 0.11);
            gr_eff_6->Fit(fitFunc1, "N", "", -0.2, 0.11);

            TF1 *fitFunc2 = new TF1("fitFunc2", "[0] + [1]*x + [2]*x^2", -0.2, 0.11);
            gr_eff_9->Fit(fitFunc2, "N", "", -0.2, 0.11);

            mg->GetXaxis()->SetLimits(-0.21, 0.11);
            mg->SetMinimum(0.90);
            mg->SetMaximum(1.15);

            TString outname = save_dir + "Ratios_" + name + ".pdf";
            cout << "Saving in " << outname << endl;
            c1->SaveAs(outname);

            TString outroot = outname;
            outroot.ReplaceAll(".pdf", ".root");
            TFile *fout = new TFile(outroot, "RECREATE");
            gr_eff_9->Write("BDT9");
            gr_eff_6->Write("BDT6");
            fitFunc1->Write("fit_BDT6");
            fitFunc2->Write("fit_BDT9");
            c1->Write("Canvas");
            fout->Close();
            delete fout;
        }
        else
        {
            TBox *square = new TBox(-0.21, 0.90, 0.11, 1.15);
            square->SetFillStyle(0);
            square->SetLineStyle(3);
            square->SetLineColor(kBlue);
            square->SetLineWidth(2);
            square->Draw();
        }
    }
}

int ratios()
{

    // Print_Values_LATEX(7,"Cut7.txt");
    Print_Values_LATEX(10, "/Users/mariana/Work/Results/Article/Table2_TPR_FPR.txt");

    TString name, file, file2;

    name = "F18in_positives";
    file = "Files/9-BDT/" + name + "_Final";
    file2 = "Files/6-BDT/" + name + "_Final";
    Get_ratios_Article(file, file2, name,false);
    ROC_All_Article(file, file2, name);
    //  Print_Values(name);

    name = "F18out_positives";
    file = "Files/9-BDT/" + name + "_Final";
    file2 = "Files/6-BDT/" + name + "_Final";
    Get_ratios_Article(file, file2, name,false);
    ROC_All_Article(file, file2, name);
    //  Print_Values(name);

    name="F18in_negatives";
     file="Files/9-BDT/"+name+"_Final";
     file2="Files/6-BDT/"+name+"_Final";
     Get_ratios_Article(file,file2,name,false);
     //ROC_All_Article(file, file2,name);
     //Print_Values(name);

     name="F18out_negatives";
     file="Files/9-BDT/"+name+"_Final";
     file2="Files/6-BDT/"+name+"_Final";
    Get_ratios_Article(file,file2,name,true);
    //ROC_All_Article(file, file2,name);
     //Print_Values(name);

    /*name = "S19_positives";
    file = "Files/9-BDT/" + name + "_Final";
    file2 = "Files/6-BDT/" + name + "_Final";
    //Get_ratios(file, file2, name, false);
    ROC_All(file, file2, name, true);
    // Print_Values(name);*/

   /*name="F18in_negatives";
     file="Files/9-BDT/"+name+"_Final";
     file2="Files/6-BDT/"+name+"_Final";
     Get_ratios(file,file2,name,false);
     //ROC_All(file, file2,name);
     //Print_Values(name);

     name="F18out_negatives";
     file="Files/9-BDT/"+name+"_Final";
     file2="Files/6-BDT/"+name+"_Final";
    Get_ratios(file,file2,name,false);
    // ROC_All(file, file2,name);
     //Print_Values(name);

     name="S19_negatives";
     file="Files/9-BDT/"+name+"_Final";
     file2="Files/6-BDT/"+name+"_Final";
     Get_ratios(file,file2,name,true);
     //ROC_All(file, file2,name);
     //Print_Values(name);*/

    gApplication->Terminate();
    return 0;
}
