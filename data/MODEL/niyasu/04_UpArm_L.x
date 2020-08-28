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
 60;
 -2.87241;-2.43373;1.55143;,
 -2.72201;-2.82240;-0.67154;,
 -2.18815;-5.96236;0.36756;,
 -2.15473;-5.66184;2.29177;,
 2.33890;-2.02882;1.12392;,
 1.17164;-1.73519;2.31270;,
 1.24048;-4.97396;3.26674;,
 2.37514;-5.29996;1.68152;,
 -1.30022;-1.77611;3.03564;,
 -2.87241;-2.43373;1.55143;,
 -2.15473;-5.66184;2.29177;,
 -0.82564;-5.07224;3.62956;,
 -1.30022;-1.77611;3.03564;,
 -0.82564;-5.07224;3.62956;,
 1.47798;-2.29195;-0.70400;,
 2.33890;-2.02882;1.12392;,
 2.37514;-5.29996;1.68152;,
 2.01115;-5.34529;-0.34573;,
 -0.04732;-2.55924;-1.45579;,
 1.47798;-2.29195;-0.70400;,
 2.01115;-5.34529;-0.34573;,
 0.50095;-5.64948;-1.44810;,
 -2.72201;-2.82240;-0.67154;,
 -1.11555;-2.78524;-1.21937;,
 -1.10142;-5.82493;-1.12482;,
 -2.18815;-5.96236;0.36756;,
 -1.11555;-2.78524;-1.21937;,
 -0.04732;-2.55924;-1.45579;,
 0.50095;-5.64948;-1.44810;,
 -1.10142;-5.82493;-1.12482;,
 -3.36232;0.50948;0.72227;,
 -3.34921;0.35945;-1.25779;,
 -1.98751;0.67207;0.13363;,
 -1.76488;0.34965;-0.99855;,
 2.13669;1.09352;0.87274;,
 0.63458;1.45522;2.19874;,
 0.62740;0.62069;0.58229;,
 -0.11047;1.07399;1.08166;,
 -1.68458;1.22356;2.37194;,
 -3.36232;0.50948;0.72227;,
 -1.12112;1.04979;0.92604;,
 -1.98751;0.67207;0.13363;,
 -1.68458;1.22356;2.37194;,
 -1.12112;1.04979;0.92604;,
 1.39320;0.51738;-0.63202;,
 2.13669;1.09352;0.87274;,
 0.65681;0.10215;-0.02141;,
 0.62740;0.62069;0.58229;,
 0.79769;-0.07700;-1.69110;,
 1.39320;0.51738;-0.63202;,
 0.42532;-0.27702;-0.33094;,
 0.65681;0.10215;-0.02141;,
 -3.34921;0.35945;-1.25779;,
 -0.98153;-0.29370;-2.06368;,
 -1.76488;0.34965;-0.99855;,
 -0.36398;-0.34963;-0.48840;,
 -0.98153;-0.29370;-2.06368;,
 0.79769;-0.07700;-1.69110;,
 -0.36398;-0.34963;-0.48840;,
 0.42532;-0.27702;-0.33094;;
 
 24;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;5,12,13,6;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;30,31,1,0;,
 4;31,30,32,33;,
 4;34,35,5,4;,
 4;35,34,36,37;,
 4;38,39,9,8;,
 4;39,38,40,41;,
 4;35,42,12,5;,
 4;42,35,37,43;,
 4;44,45,15,14;,
 4;45,44,46,47;,
 4;48,49,19,18;,
 4;49,48,50,51;,
 4;52,53,23,22;,
 4;53,52,54,55;,
 4;56,57,27,26;,
 4;57,56,58,59;;
 
 MeshMaterialList {
  14;
  24;
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
  34;
  0.978368;0.114225;0.172478;,
  0.780488;0.045165;-0.623537;,
  0.166767;0.003136;-0.985991;,
  -0.393038;-0.112549;-0.912608;,
  -0.869038;-0.211719;-0.447155;,
  -0.915307;-0.095611;0.391245;,
  -0.279123;0.145749;0.949130;,
  0.522599;0.209595;0.826414;,
  0.980284;0.103901;0.168070;,
  0.796152;0.002618;-0.605091;,
  0.283430;-0.092780;-0.954494;,
  -0.274660;-0.187922;-0.942999;,
  -0.833384;-0.239792;-0.497968;,
  -0.920038;-0.078022;0.383983;,
  -0.292150;0.136313;0.946608;,
  0.488368;0.176554;0.854591;,
  -0.249616;0.864208;-0.436849;,
  -0.584352;0.756270;-0.294258;,
  -0.415888;0.907873;-0.052948;,
  0.088248;0.989121;-0.117697;,
  0.124379;0.959246;-0.253726;,
  -0.133480;0.972750;-0.189578;,
  -0.138328;0.970566;-0.197147;,
  -0.019713;0.944980;-0.326535;,
  0.698611;0.120671;0.705253;,
  0.809298;-0.037914;-0.586174;,
  0.393085;-0.186261;-0.900439;,
  -0.151263;-0.259067;-0.953941;,
  -0.983007;-0.183029;-0.014060;,
  -0.924438;-0.060310;0.376534;,
  -0.304898;0.126989;0.943881;,
  0.453020;0.143322;0.879904;,
  0.897527;0.029959;-0.439941;,
  -0.337305;-0.265129;-0.903289;;
  24;
  4;13,12,4,5;,
  4;8,15,7,0;,
  4;14,13,5,6;,
  4;15,14,6,7;,
  4;9,8,0,1;,
  4;10,9,1,2;,
  4;12,11,3,4;,
  4;11,10,2,3;,
  4;29,28,12,13;,
  4;20,21,21,20;,
  4;24,31,15,8;,
  4;23,16,16,23;,
  4;30,29,13,14;,
  4;21,22,22,21;,
  4;31,30,14,15;,
  4;22,23,23,22;,
  4;25,32,8,9;,
  4;16,17,17,16;,
  4;26,25,9,10;,
  4;17,18,18,17;,
  4;33,27,11,12;,
  4;19,20,20,19;,
  4;27,26,10,11;,
  4;18,19,19,18;;
 }
 MeshTextureCoords {
  60;
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;;
 }
}
