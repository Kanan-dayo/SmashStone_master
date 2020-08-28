xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 178;
 -0.08943;-4.54934;1.47715;,
 0.06208;-5.00205;0.30141;,
 0.03976;-5.15319;1.65562;,
 1.43442;-4.34155;1.98563;,
 1.54855;-4.92970;2.15379;,
 1.89442;-4.74843;1.13013;,
 0.52342;-4.25670;2.05817;,
 -0.08943;-4.54934;1.47715;,
 0.03976;-5.15319;1.65562;,
 0.65278;-5.27757;2.15401;,
 0.52342;-4.25670;2.05817;,
 0.65278;-5.27757;2.15401;,
 1.54855;-4.92970;2.15379;,
 -0.47835;-3.39245;0.97646;,
 -0.33185;-3.50154;-0.42308;,
 -0.08943;-4.54934;1.47715;,
 0.38170;-3.50411;-1.19842;,
 1.48992;-3.45396;-0.89943;,
 1.29065;-5.01182;-0.37337;,
 0.57749;-5.05632;-0.36283;,
 -0.33185;-3.50154;-0.42308;,
 0.38170;-3.50411;-1.19842;,
 0.57749;-5.05632;-0.36283;,
 0.06208;-5.00205;0.30141;,
 0.24705;-3.15387;2.09324;,
 -0.47835;-3.39245;0.97646;,
 -0.08943;-4.54934;1.47715;,
 0.52342;-4.25670;2.05817;,
 1.48992;-3.45396;-0.89943;,
 1.99121;-3.19876;0.18857;,
 1.94464;-4.85316;0.17883;,
 1.29065;-5.01182;-0.37337;,
 1.99121;-3.19876;0.18857;,
 1.86364;-3.05296;1.57222;,
 1.89442;-4.74843;1.13013;,
 1.94464;-4.85316;0.17883;,
 1.20247;-3.01752;2.34630;,
 0.24705;-3.15387;2.09324;,
 0.52342;-4.25670;2.05817;,
 1.43442;-4.34155;1.98563;,
 1.86364;-3.05296;1.57222;,
 1.20247;-3.01752;2.34630;,
 1.43442;-4.34155;1.98563;,
 -0.88592;-1.97334;0.64013;,
 -0.67843;-2.02487;-0.76120;,
 -0.33185;-3.50154;-0.42308;,
 -0.47835;-3.39245;0.97646;,
 0.20349;-1.71873;-1.51477;,
 1.55186;-1.19277;-1.17558;,
 1.48992;-3.45396;-0.89943;,
 0.38170;-3.50411;-1.19842;,
 -0.67843;-2.02487;-0.76120;,
 0.20349;-1.71873;-1.51477;,
 0.38170;-3.50411;-1.19842;,
 -0.33185;-3.50154;-0.42308;,
 -0.04016;-1.40162;1.78988;,
 -0.88592;-1.97334;0.64013;,
 -0.47835;-3.39245;0.97646;,
 0.24705;-3.15387;2.09324;,
 1.55186;-1.19277;-1.17558;,
 2.12098;-0.69720;-0.06219;,
 1.99121;-3.19876;0.18857;,
 1.48992;-3.45396;-0.89943;,
 2.12098;-0.69720;-0.06219;,
 1.93181;-0.59570;1.32445;,
 1.86364;-3.05296;1.57222;,
 1.99121;-3.19876;0.18857;,
 1.10956;-0.84214;2.07907;,
 -0.04016;-1.40162;1.78988;,
 0.24705;-3.15387;2.09324;,
 1.20247;-3.01752;2.34630;,
 1.93181;-0.59570;1.32445;,
 1.10956;-0.84214;2.07907;,
 1.20247;-3.01752;2.34630;,
 1.86364;-3.05296;1.57222;,
 0.86395;-5.41243;1.06427;,
 0.65278;-5.27757;2.15401;,
 0.03976;-5.15319;1.65562;,
 0.06208;-5.00205;0.30141;,
 0.57749;-5.05632;-0.36283;,
 1.29065;-5.01182;-0.37337;,
 1.94464;-4.85316;0.17883;,
 1.54855;-4.92970;2.15379;,
 1.89442;-4.74843;1.13013;,
 -1.13186;-0.37845;2.28976;,
 0.56231;-0.21741;2.42371;,
 1.16717;0.86853;1.58563;,
 -0.63934;1.57052;1.65882;,
 -1.13186;-0.37845;2.28976;,
 0.30683;-0.03183;1.70065;,
 -0.65164;-0.57265;1.46128;,
 -0.89929;-0.75843;1.75628;,
 -0.70271;-1.13499;1.80535;,
 0.56231;-0.21741;2.42371;,
 -0.63934;1.57052;1.65882;,
 1.16717;0.86853;1.58563;,
 1.83871;1.34418;-0.20266;,
 -0.15488;2.20268;0.09661;,
 -0.15488;2.20268;0.09661;,
 1.83871;1.34418;-0.20266;,
 1.09510;0.94014;-1.74412;,
 -0.63783;1.61947;-1.47765;,
 -0.89929;-0.75843;1.75628;,
 -0.65164;-0.57265;1.46128;,
 -1.01800;-1.19001;0.09438;,
 -1.37692;-1.42402;0.09102;,
 -1.22050;-1.47711;-0.02128;,
 -0.70271;-1.13499;1.80535;,
 -1.10238;-0.32209;-2.08694;,
 0.16693;-0.41089;-2.19613;,
 -0.85133;-1.08056;-1.53577;,
 -0.95773;-0.70011;-1.55977;,
 -0.63783;1.61947;-1.47765;,
 1.09510;0.94014;-1.74412;,
 0.16693;-0.41089;-2.19613;,
 -1.10238;-0.32209;-2.08694;,
 -1.37692;-1.42402;0.09102;,
 -1.01800;-1.19001;0.09438;,
 -0.65164;-0.57265;-1.27253;,
 -0.95773;-0.70011;-1.55977;,
 -0.85133;-1.08056;-1.53577;,
 -1.22050;-1.47711;-0.02128;,
 -1.22050;-1.47711;-0.02128;,
 -1.02522;-2.08331;-0.99649;,
 -1.32176;-1.97133;0.70099;,
 0.16693;-0.41089;-2.19613;,
 1.85043;-1.20296;-1.46854;,
 0.16492;-1.80155;-1.92315;,
 -0.85133;-1.08056;-1.53577;,
 0.16492;-1.80155;-1.92315;,
 -1.02522;-2.08331;-0.99649;,
 -0.70271;-1.13499;1.80535;,
 -1.32176;-1.97133;0.70099;,
 -0.33492;-1.39185;2.10928;,
 1.09510;0.94014;-1.74412;,
 2.56327;-0.62680;-0.10495;,
 1.85043;-1.20296;-1.46854;,
 1.83871;1.34418;-0.20266;,
 2.26462;-0.48099;1.55383;,
 2.56327;-0.62680;-0.10495;,
 0.56231;-0.21741;2.42371;,
 -0.33492;-1.39185;2.10928;,
 1.13154;-0.77585;2.48988;,
 1.16717;0.86853;1.58563;,
 1.13154;-0.77585;2.48988;,
 2.26462;-0.48099;1.55383;,
 -0.44141;-2.24438;-0.59815;,
 -0.67843;-2.02487;-0.76120;,
 -0.88592;-1.97334;0.64013;,
 -0.63975;-2.19736;0.64968;,
 1.50064;-1.51521;-0.89333;,
 1.55186;-1.19277;-1.17558;,
 0.20349;-1.71873;-1.51477;,
 0.35092;-1.97151;-1.22392;,
 0.35092;-1.97151;-1.22392;,
 0.20349;-1.71873;-1.51477;,
 -0.67843;-2.02487;-0.76120;,
 -0.44141;-2.24438;-0.59815;,
 -0.63975;-2.19736;0.64968;,
 -0.88592;-1.97334;0.64013;,
 -0.04016;-1.40162;1.78988;,
 0.13117;-1.69268;1.66159;,
 1.97883;-1.09438;0.04943;,
 2.12098;-0.69720;-0.06219;,
 1.55186;-1.19277;-1.17558;,
 1.50064;-1.51521;-0.89333;,
 1.83246;-1.00827;1.21291;,
 1.93181;-0.59570;1.32445;,
 2.12098;-0.69720;-0.06219;,
 1.97883;-1.09438;0.04943;,
 0.13117;-1.69268;1.66159;,
 -0.04016;-1.40162;1.78988;,
 1.10956;-0.84214;2.07907;,
 1.14604;-1.21147;1.87742;,
 1.14604;-1.21147;1.87742;,
 1.10956;-0.84214;2.07907;,
 1.93181;-0.59570;1.32445;,
 1.83246;-1.00827;1.21291;;
 
 71;
 3;0,1,2;,
 3;3,4,5;,
 4;6,7,8,9;,
 4;3,10,11,12;,
 4;13,14,1,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,5;,
 4;43,44,45,46;,
 4;47,48,49,50;,
 4;51,52,53,54;,
 4;55,56,57,58;,
 4;59,60,61,62;,
 4;63,64,65,66;,
 4;67,68,69,70;,
 4;71,72,73,74;,
 3;75,12,76;,
 3;75,9,77;,
 3;75,2,78;,
 3;75,23,79;,
 3;75,19,80;,
 3;75,31,81;,
 3;75,5,82;,
 3;75,35,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;91,92,93,88;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;105,106,107,102;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;119,120,121,116;,
 4;122,120,123,124;,
 4;125,113,126,127;,
 4;128,109,129,130;,
 4;131,106,132,133;,
 4;134,99,135,136;,
 4;137,95,138,139;,
 4;140,92,141,142;,
 4;143,85,144,145;,
 4;46,124,123,45;,
 4;146,147,148,149;,
 4;45,146,149,46;,
 4;50,127,126,49;,
 4;150,151,152,153;,
 4;49,150,153,50;,
 4;54,130,129,53;,
 4;154,155,156,157;,
 4;53,154,157,54;,
 4;58,133,132,57;,
 4;158,159,160,161;,
 4;57,158,161,58;,
 4;62,136,135,61;,
 4;162,163,164,165;,
 4;61,162,165,62;,
 4;66,139,138,65;,
 4;166,167,168,169;,
 4;65,166,169,66;,
 4;70,142,141,69;,
 4;170,171,172,173;,
 4;69,170,173,70;,
 4;74,145,144,73;,
 4;174,175,176,177;,
 4;73,174,177,74;;
 
 MeshMaterialList {
  12;
  71;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.668000;0.376800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.112800;0.800000;0.024800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.536800;0.455200;0.323200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.671200;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.100000;0.083900;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.132000;0.558400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.401600;0.225600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.787200;0.000000;1.000000;;
   27.000000;
   0.130000;0.130000;0.130000;;
   0.130000;0.127920;0.000000;;
  }
  Material {
   0.100000;0.800000;0.775200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   17.000000;
   0.150000;0.150000;0.150000;;
   0.150000;0.150000;0.150000;;
  }
  Material {
   0.800000;0.404800;0.050400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.615200;0.034400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  89;
  0.895336;0.274110;0.351051;,
  -0.360840;0.111286;0.925964;,
  -0.913896;-0.108088;0.391293;,
  0.122069;-0.991810;-0.037579;,
  0.934974;0.060688;0.349487;,
  0.905478;-0.399198;-0.144048;,
  0.400142;-0.730617;-0.553249;,
  -0.238826;-0.798617;-0.552425;,
  -0.745359;-0.632980;-0.209226;,
  -0.916063;-0.167827;0.364230;,
  -0.409032;-0.021918;0.912256;,
  0.872272;0.098594;0.478979;,
  -0.538858;0.812860;0.221111;,
  -0.589314;0.807225;0.033127;,
  -0.640054;0.690169;-0.337635;,
  -0.637085;0.581754;-0.505653;,
  0.986445;-0.056843;-0.153930;,
  -0.854952;-0.281609;-0.435608;,
  0.650519;-0.118197;-0.750236;,
  -0.280211;0.112774;0.953291;,
  -0.578183;0.027828;0.815432;,
  -0.214563;-0.222082;-0.951127;,
  -0.949422;-0.179618;0.257556;,
  0.902848;0.029778;0.428928;,
  -0.025880;0.116509;0.992852;,
  -0.589936;-0.325878;0.738769;,
  -0.936369;-0.345880;0.059834;,
  -0.705858;-0.361166;-0.609363;,
  -0.132272;-0.155997;-0.978861;,
  0.425568;0.399592;-0.811922;,
  0.693841;0.717633;-0.059897;,
  0.469758;0.591453;0.655371;,
  0.236238;0.673381;0.700535;,
  0.369062;0.929369;0.008188;,
  0.143236;0.656705;-0.740420;,
  -0.191565;-0.127250;-0.973196;,
  -0.562616;-0.553349;-0.614222;,
  -0.077692;-0.039962;0.996176;,
  0.989043;0.085222;-0.120550;,
  -0.802475;-0.334798;-0.493906;,
  0.660699;-0.105302;-0.743229;,
  0.150781;0.098628;0.983635;,
  -0.610146;-0.064147;0.789688;,
  -0.112959;-0.314715;-0.942441;,
  -0.958857;-0.193083;0.208119;,
  0.822146;0.184356;0.538600;,
  -0.977506;0.173834;0.119433;,
  0.740762;0.503346;0.444876;,
  -0.695265;0.284544;0.660031;,
  -0.333560;0.104895;-0.936875;,
  0.461417;0.196237;-0.865208;,
  0.065583;0.440566;0.895321;,
  0.867471;0.398601;-0.297676;,
  -0.899222;0.110814;-0.423226;,
  0.976435;-0.125506;-0.175566;,
  -0.832330;-0.395237;-0.388606;,
  0.610966;-0.248439;-0.751664;,
  0.314261;-0.038037;0.948574;,
  -0.519961;-0.107056;0.847455;,
  -0.228574;-0.382264;-0.895337;,
  -0.915832;-0.276422;0.291276;,
  0.911645;-0.048548;0.408101;,
  -0.051579;0.003131;0.998664;,
  -0.005285;0.181316;0.983411;,
  0.713949;0.084155;0.695123;,
  0.451405;-0.885665;0.108769;,
  0.054166;-0.989640;0.132960;,
  -0.312682;-0.949658;-0.019481;,
  0.352774;-0.923190;-0.152549;,
  0.536567;-0.840340;0.076978;,
  -0.402539;-0.663438;0.630724;,
  -0.725134;-0.688244;-0.022365;,
  0.474823;0.802879;0.360456;,
  0.631983;0.707573;-0.316129;,
  0.882631;0.119121;0.454723;,
  0.971432;0.033049;-0.235006;,
  -0.687019;0.524582;0.502811;,
  -0.099844;0.726659;0.679704;,
  -0.643062;0.010189;0.765746;,
  0.232438;0.055038;0.971053;,
  0.261637;0.522522;-0.811491;,
  0.593814;-0.165280;-0.787444;,
  -0.936675;0.343759;0.066850;,
  -0.985658;-0.004417;0.168697;,
  -0.857185;0.311868;-0.409845;,
  -0.902886;-0.098526;-0.418436;,
  -0.379449;0.408321;-0.830237;,
  0.293465;-0.240832;-0.925137;,
  -0.710904;-0.163668;-0.683979;;
  71;
  3;9,8,2;,
  3;11,0,4;,
  4;10,9,2,1;,
  4;62,10,1,63;,
  4;60,55,8,9;,
  4;59,56,6,7;,
  4;55,59,7,8;,
  4;58,60,9,10;,
  4;56,54,5,6;,
  4;54,61,4,5;,
  4;57,58,10,62;,
  4;61,57,11,4;,
  4;22,17,55,60;,
  4;21,18,56,59;,
  4;17,21,59,55;,
  4;20,22,60,58;,
  4;18,16,54,56;,
  4;16,23,61,54;,
  4;19,20,58,57;,
  4;23,64,57,61;,
  3;3,65,66;,
  3;3,66,67;,
  3;3,67,8;,
  3;3,8,7;,
  3;3,7,6;,
  3;3,6,68;,
  3;3,69,65;,
  3;3,5,69;,
  4;37,24,31,32;,
  4;15,15,14,14;,
  4;70,25,24,37;,
  4;32,31,30,33;,
  4;33,30,29,34;,
  4;14,14,13,13;,
  4;71,26,25,70;,
  4;35,28,27,36;,
  4;34,29,28,35;,
  4;13,13,12,12;,
  4;36,27,26,71;,
  4;26,27,39,44;,
  4;28,29,40,43;,
  4;27,28,43,39;,
  4;25,26,44,42;,
  4;29,30,38,40;,
  4;30,31,45,38;,
  4;24,25,42,41;,
  4;31,24,41,45;,
  4;60,44,39,55;,
  4;47,72,73,52;,
  4;74,47,52,75;,
  4;59,43,40,56;,
  4;48,76,77,51;,
  4;78,48,51,79;,
  4;55,39,43,59;,
  4;51,77,72,47;,
  4;79,51,47,74;,
  4;58,42,44,60;,
  4;52,73,80,50;,
  4;75,52,50,81;,
  4;56,40,38,54;,
  4;46,82,76,48;,
  4;83,46,48,78;,
  4;54,38,45,61;,
  4;53,84,82,46;,
  4;85,53,46,83;,
  4;57,41,42,58;,
  4;50,80,86,49;,
  4;81,50,49,87;,
  4;61,45,41,57;,
  4;49,86,84,53;,
  4;88,49,53,85;;
 }
 MeshTextureCoords {
  178;
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;;
 }
}
