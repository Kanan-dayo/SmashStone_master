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
 116;
 -1.05559;-6.31123;1.75084;,
 -0.62769;-10.45847;2.04550;,
 0.39247;-9.72536;2.68728;,
 0.43291;-6.04623;2.44363;,
 -1.43118;-6.63046;0.02354;,
 -1.05010;-10.33298;0.69972;,
 -0.62769;-10.45847;2.04550;,
 -1.05559;-6.31123;1.75084;,
 -1.05559;-6.81691;-0.79807;,
 -0.61019;-10.20762;-0.71350;,
 -1.05010;-10.33298;0.69972;,
 -1.43118;-6.63046;0.02354;,
 0.43291;-6.76134;-1.54564;,
 0.48296;-10.06586;-1.22101;,
 -0.61019;-10.20762;-0.71350;,
 -1.05559;-6.81691;-0.79807;,
 1.41326;-6.49635;-0.85284;,
 1.88211;-9.89930;-0.70335;,
 0.48296;-10.06586;-1.22101;,
 0.43291;-6.76134;-1.54564;,
 2.02982;-6.17712;-0.05391;,
 2.30267;-9.89692;0.71409;,
 1.88211;-9.89930;-0.70335;,
 1.41326;-6.49635;-0.85284;,
 1.92142;-5.99067;1.69608;,
 1.54910;-10.15013;2.31698;,
 2.30267;-9.89692;0.71409;,
 2.02982;-6.17712;-0.05391;,
 0.43291;-6.04623;2.44363;,
 0.39247;-9.72536;2.68728;,
 1.54910;-10.15013;2.31698;,
 1.92142;-5.99067;1.69608;,
 -1.47671;-0.56929;1.52655;,
 -1.44029;-2.82248;1.52975;,
 -0.03684;-2.66081;2.36447;,
 -0.16365;0.70980;2.29770;,
 -2.00516;-1.21182;-0.41593;,
 -1.77609;-3.42209;-0.39935;,
 -1.44029;-2.82248;1.52975;,
 -1.47671;-0.56929;1.52655;,
 -1.47671;-0.59271;-1.55587;,
 -1.44029;-2.85547;-1.26948;,
 -1.77609;-3.42209;-0.39935;,
 -2.00516;-1.21182;-0.41593;,
 -0.33844;0.22289;-2.38591;,
 -0.03684;-2.70747;-2.07080;,
 -1.44029;-2.85547;-1.26948;,
 -1.47671;-0.59271;-1.55587;,
 1.06217;1.56468;-1.64048;,
 1.23121;-2.25753;-1.65875;,
 -0.03684;-2.70747;-2.07080;,
 -0.33844;0.22289;-2.38591;,
 1.62350;1.77351;-0.41593;,
 1.87459;-2.03839;-0.40483;,
 1.23121;-2.25753;-1.65875;,
 1.06217;1.56468;-1.64048;,
 1.40581;1.30569;1.60913;,
 1.76998;-2.22454;1.56314;,
 1.87459;-2.03839;-0.40483;,
 1.62350;1.77351;-0.41593;,
 -0.16365;0.70980;2.29770;,
 -0.03684;-2.66081;2.36447;,
 1.76998;-2.22454;1.56314;,
 1.40581;1.30569;1.60913;,
 1.54910;-10.15013;2.31698;,
 0.39247;-9.72536;2.68728;,
 0.53958;-11.63156;2.67829;,
 1.63359;-11.50974;2.16786;,
 2.30267;-9.89692;0.71409;,
 1.54910;-10.15013;2.31698;,
 1.63359;-11.50974;2.16786;,
 2.44190;-11.66889;0.91342;,
 1.88211;-9.89930;-0.70335;,
 2.30267;-9.89692;0.71409;,
 2.44190;-11.66889;0.91342;,
 2.02478;-11.41467;-0.62154;,
 0.48296;-10.06586;-1.22101;,
 1.88211;-9.89930;-0.70335;,
 2.02478;-11.41467;-0.62154;,
 0.60301;-11.41344;-1.18099;,
 -0.61019;-10.20762;-0.71350;,
 0.48296;-10.06586;-1.22101;,
 0.60301;-11.41344;-1.18099;,
 -0.47417;-11.68641;-0.61645;,
 -1.05010;-10.33298;0.69972;,
 -0.61019;-10.20762;-0.71350;,
 -0.47417;-11.68641;-0.61645;,
 -0.84688;-12.23349;0.93671;,
 -0.62769;-10.45847;2.04550;,
 -1.05010;-10.33298;0.69972;,
 -0.84688;-12.23349;0.93671;,
 -0.52621;-11.90898;2.18433;,
 0.39247;-9.72536;2.68728;,
 -0.62769;-10.45847;2.04550;,
 -0.52621;-11.90898;2.18433;,
 0.53958;-11.63156;2.67829;,
 0.60301;-11.41344;-1.18099;,
 0.78193;-13.02090;-0.60574;,
 -0.32889;-12.99157;-0.14940;,
 -0.47417;-11.68641;-0.61645;,
 2.02478;-11.41467;-0.62154;,
 1.83703;-12.66525;-0.17401;,
 0.78193;-13.02090;-0.60574;,
 0.60301;-11.41344;-1.18099;,
 2.44190;-11.66889;0.91342;,
 1.83703;-12.66525;-0.17401;,
 2.02478;-11.41467;-0.62154;,
 -0.47417;-11.68641;-0.61645;,
 -0.84688;-12.23349;0.93671;,
 0.93640;-12.95493;1.35368;,
 1.83703;-12.66525;-0.17401;,
 0.78193;-13.02090;-0.60574;,
 1.83703;-12.66525;-0.17401;,
 -0.32889;-12.99157;-0.14940;,
 0.78193;-13.02090;-0.60574;,
 -0.32889;-12.99157;-0.14940;;
 
 44;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;33,0,3,34;,
 4;36,37,38,39;,
 4;37,4,7,38;,
 4;40,41,42,43;,
 4;41,8,11,42;,
 4;44,45,46,47;,
 4;45,12,15,46;,
 4;48,49,50,51;,
 4;49,16,19,50;,
 4;52,53,54,55;,
 4;53,20,23,54;,
 4;56,57,58,59;,
 4;57,24,27,58;,
 4;60,61,62,63;,
 4;61,28,31,62;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 3;104,105,106;,
 3;107,98,108;,
 3;109,67,66;,
 3;109,71,70;,
 3;109,110,74;,
 3;109,111,112;,
 3;109,113,114;,
 3;109,87,115;,
 3;109,91,90;,
 3;109,95,94;;
 
 MeshMaterialList {
  14;
  44;
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7;;
  Material {
   0.512000;0.512000;0.512000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.605600;0.009600;0.000000;1.000000;;
   17.000000;
   0.130000;0.130000;0.130000;;
   0.068130;0.001080;0.000000;;
  }
  Material {
   0.179200;0.439200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.040800;0.040800;0.040800;1.000000;;
   7.000000;
   0.110000;0.110000;0.110000;;
   0.002550;0.002550;0.002550;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.596000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.097600;0.044000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.084800;0.279200;0.460800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.432800;0.295200;0.021600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.668000;0.555200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.659200;0.000000;1.000000;;
   28.000000;
   0.150000;0.150000;0.150000;;
   0.180000;0.148320;0.000000;;
  }
  Material {
   0.501600;0.501600;0.501600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.238400;0.200800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.019200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  49;
  -0.062981;0.014785;0.997905;,
  0.992234;0.083651;0.092059;,
  0.980767;0.069378;-0.182437;,
  0.686506;0.015067;-0.726968;,
  -0.059652;-0.085283;-0.994569;,
  -0.730139;-0.112397;-0.673992;,
  -0.991626;-0.075179;-0.105003;,
  -0.802938;-0.033621;0.595113;,
  -0.782414;-0.022206;0.622362;,
  -0.994395;-0.102492;-0.025951;,
  -0.741665;-0.118341;-0.660249;,
  -0.009155;-0.053941;-0.998502;,
  0.726171;0.016421;-0.687318;,
  0.998681;0.051334;0.001263;,
  0.712627;0.042771;0.700238;,
  -0.052913;0.026735;0.998241;,
  -0.790744;-0.037824;0.610977;,
  -0.037446;0.045371;0.998268;,
  -0.990395;-0.116798;-0.074002;,
  -0.724642;-0.142495;-0.674233;,
  0.015488;-0.102674;-0.994595;,
  0.700723;-0.035292;-0.712560;,
  0.983888;0.016514;-0.178023;,
  0.793584;0.063671;0.605120;,
  -0.798371;-0.045442;0.600449;,
  -0.043786;0.020310;0.998834;,
  -0.989737;-0.097787;-0.104205;,
  -0.724197;-0.135589;-0.676132;,
  -0.025288;-0.115327;-0.993006;,
  0.686730;-0.029696;-0.726305;,
  0.976217;0.035057;-0.213941;,
  0.995222;0.062943;0.074636;,
  0.150865;-0.896516;0.416531;,
  0.627287;-0.311124;0.713942;,
  0.047842;-0.383325;0.922373;,
  0.934838;-0.320083;0.153705;,
  0.737244;-0.139892;-0.660986;,
  -0.012135;-0.205511;-0.978580;,
  -0.734092;-0.209529;-0.645915;,
  -0.985096;-0.133828;-0.108052;,
  -0.590699;-0.482102;0.647033;,
  -0.708887;-0.306272;-0.635356;,
  0.014720;-0.356771;-0.934076;,
  0.696990;-0.664878;-0.268594;,
  0.407961;0.048156;0.911728;,
  0.395503;0.030832;0.917947;,
  0.154513;-0.987766;0.021075;,
  -0.225970;-0.950475;0.213390;,
  -0.420570;-0.836345;0.351635;;
  44;
  4;16,8,15,17;,
  4;18,9,8,16;,
  4;19,10,9,18;,
  4;20,11,10,19;,
  4;21,12,11,20;,
  4;22,13,12,21;,
  4;23,14,13,22;,
  4;17,15,14,23;,
  4;7,24,25,0;,
  4;24,16,17,25;,
  4;6,26,24,7;,
  4;26,18,16,24;,
  4;5,27,26,6;,
  4;27,19,18,26;,
  4;4,28,27,5;,
  4;28,20,19,27;,
  4;3,29,28,4;,
  4;29,21,20,28;,
  4;2,30,29,3;,
  4;30,22,21,29;,
  4;1,31,30,2;,
  4;31,23,22,30;,
  4;0,25,44,45;,
  4;25,17,23,44;,
  4;14,15,34,33;,
  4;13,14,33,35;,
  4;12,13,35,36;,
  4;11,12,36,37;,
  4;10,11,37,38;,
  4;9,10,38,39;,
  4;8,9,39,40;,
  4;15,8,40,34;,
  4;37,42,41,38;,
  4;36,43,42,37;,
  3;35,43,36;,
  3;38,41,39;,
  3;32,33,34;,
  3;32,35,33;,
  3;32,43,35;,
  3;32,46,43;,
  3;32,47,46;,
  3;32,48,47;,
  3;32,40,48;,
  3;32,34,40;;
 }
 MeshTextureCoords {
  116;
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  -0.000000;0.750000;,
  -0.000000;0.750000;,
  1.000000;0.750000;,
  -0.000000;0.750000;,
  1.000000;0.750000;,
  1.000000;0.750000;;
 }
}
