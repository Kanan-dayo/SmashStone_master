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
 49;
 -122.53841;-0.44941;-4.81130;,
 -121.81492;-0.58328;-4.90392;,
 -118.99302;5.65265;-4.83398;,
 -122.53841;-0.44941;4.24928;,
 -121.81492;-0.58328;-4.90392;,
 -122.53841;-0.44941;-4.81130;,
 0.64970;44.88546;-4.81130;,
 0.70023;41.25693;-4.81130;,
 -54.13851;41.38145;-4.83398;,
 -118.77346;44.88546;-4.81130;,
 -118.99302;41.38145;-4.83398;,
 -122.48682;41.25693;-4.81130;,
 -118.77346;44.88546;4.24928;,
 -122.48682;41.25693;4.24928;,
 -118.99302;41.38145;4.27188;,
 -118.77346;44.88546;-4.81130;,
 -122.48682;41.25693;-4.81130;,
 -122.48682;41.25693;4.24928;,
 -118.77346;44.88546;4.24928;,
 -122.53841;-0.44941;4.24928;,
 0.64970;44.88546;-4.81130;,
 -118.77346;44.88546;-4.81130;,
 0.66161;-0.46914;-4.81130;,
 -74.79166;30.33217;-4.83398;,
 0.66161;-0.46914;-4.81130;,
 -122.53841;-0.44941;-4.81130;,
 -54.13851;41.38145;4.27188;,
 -74.79166;30.33217;4.27188;,
 0.66161;-0.46914;4.24928;,
 0.70023;41.25693;4.24928;,
 -122.53841;-0.44941;4.24928;,
 -118.99302;5.65265;4.27188;,
 -122.53841;-0.44941;4.24928;,
 0.66161;-0.46914;4.24928;,
 0.64970;44.88546;-4.81130;,
 -118.77346;44.88546;-4.81130;,
 -118.77346;44.88546;4.24928;,
 0.64970;44.88546;4.24928;,
 -118.99302;41.38145;-0.45966;,
 -54.13851;41.38145;-0.45966;,
 -74.79166;30.33217;-0.45966;,
 -118.99302;5.65265;-0.45966;,
 -54.13851;41.38145;0.25779;,
 -118.99302;41.38145;0.25779;,
 -118.99302;5.65265;0.25779;,
 -74.79166;30.33217;0.25779;,
 -118.99302;41.38145;4.27188;,
 0.70023;41.25693;4.24928;,
 0.64970;44.88546;4.24928;;
 
 47;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,16,17;,
 3;15,17,18;,
 3;16,4,19;,
 3;16,19,17;,
 3;20,8,10;,
 3;20,10,21;,
 3;11,10,2;,
 3;11,2,1;,
 3;8,7,22;,
 3;8,22,23;,
 3;2,23,24;,
 3;2,24,25;,
 3;26,27,28;,
 3;26,28,29;,
 3;13,30,31;,
 3;13,31,14;,
 3;31,32,33;,
 3;31,33,27;,
 3;34,35,36;,
 3;34,36,37;,
 3;38,10,8;,
 3;38,8,39;,
 3;39,8,23;,
 3;39,23,40;,
 3;40,23,2;,
 3;40,2,41;,
 3;41,2,10;,
 3;41,10,38;,
 3;38,39,40;,
 3;38,40,41;,
 3;42,26,14;,
 3;42,14,43;,
 3;43,14,31;,
 3;43,31,44;,
 3;44,31,27;,
 3;44,27,45;,
 3;45,27,26;,
 3;45,26,42;,
 3;43,44,45;,
 3;43,45,42;,
 3;46,47,48;,
 3;46,48,18;;
 
 MeshMaterialList {
  1;
  47;
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
   0.160000;0.160000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  44;
  -0.003357;0.006660;0.999972;,
  -0.006523;0.002293;0.999976;,
  0.181946;0.983308;0.000000;,
  -0.003369;0.006684;-0.999972;,
  0.001993;0.003139;-0.999993;,
  -0.003307;0.004960;-0.999982;,
  0.000249;0.001540;-0.999999;,
  0.115205;-0.063251;0.991326;,
  0.000172;-0.000315;-1.000000;,
  0.000189;-0.001087;-0.999999;,
  0.000138;0.004317;-0.999991;,
  0.000414;0.000003;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000300;0.000001;1.000000;,
  0.000291;-0.000159;1.000000;,
  -0.003248;0.003331;0.999989;,
  0.000171;-0.000314;1.000000;,
  -0.003171;-0.001001;0.999995;,
  0.000189;-0.001083;0.999999;,
  0.000000;-1.000000;0.000000;,
  -0.487502;0.873122;0.000000;,
  -0.482261;0.876028;0.000000;,
  -0.477002;0.878902;0.000000;,
  1.000000;0.000000;-0.000000;,
  -0.498543;0.866865;0.000000;,
  -0.967459;0.251452;-0.028193;,
  -0.858593;0.512658;0.000000;,
  -0.249272;0.968434;0.000000;,
  -0.996784;-0.015833;-0.078556;,
  -0.999200;-0.007304;-0.039310;,
  0.003222;-0.000376;-0.999995;,
  0.019207;0.002522;-0.999812;,
  -0.000001;-0.003716;-0.999993;,
  -0.003194;-0.001848;0.999993;,
  -0.471727;0.881745;0.000000;,
  -0.471727;0.881745;0.000000;,
  -0.477002;0.878902;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.487502;0.873122;0.000000;,
  -0.482261;0.876028;0.000000;,
  -0.471727;0.881745;0.000000;,
  -0.471727;0.881745;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000094;0.003226;0.999995;;
  47;
  3;7,7,7;,
  3;2,2,2;,
  3;10,11,6;,
  3;3,5,4;,
  3;0,1,15;,
  3;24,25,26;,
  3;24,26,27;,
  3;25,28,29;,
  3;25,29,26;,
  3;10,6,5;,
  3;10,5,3;,
  3;4,5,30;,
  3;4,30,31;,
  3;6,11,9;,
  3;6,9,8;,
  3;30,8,9;,
  3;30,9,32;,
  3;14,16,18;,
  3;14,18,13;,
  3;1,33,17;,
  3;1,17,15;,
  3;17,33,18;,
  3;17,18,16;,
  3;12,24,27;,
  3;12,27,12;,
  3;19,19,19;,
  3;19,19,19;,
  3;34,35,36;,
  3;34,36,21;,
  3;21,36,20;,
  3;21,20,20;,
  3;23,23,23;,
  3;23,23,23;,
  3;37,37,37;,
  3;37,37,37;,
  3;19,19,19;,
  3;19,19,19;,
  3;23,23,23;,
  3;23,23,23;,
  3;38,38,39;,
  3;38,39,22;,
  3;22,39,40;,
  3;22,40,41;,
  3;42,42,42;,
  3;42,42,42;,
  3;15,13,43;,
  3;15,43,0;;
 }
 MeshTextureCoords {
  49;
  0.166670;1.000000;,
  0.000000;0.666670;,
  0.333330;0.666670;,
  0.000000;0.833330;,
  1.000000;0.666670;,
  1.000000;1.000000;,
  0.833330;0.000000;,
  1.000000;0.333330;,
  0.666670;0.333330;,
  0.166670;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.833330;0.000000;,
  1.000000;0.333330;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.000000;0.333330;,
  0.000000;0.000000;,
  0.000000;0.666670;,
  0.666670;0.000000;,
  0.333330;0.000000;,
  1.000000;0.666670;,
  0.666670;0.666670;,
  0.666670;1.000000;,
  0.333330;1.000000;,
  0.333330;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.000000;0.333330;,
  1.000000;0.666670;,
  0.666670;0.666670;,
  0.666670;1.000000;,
  0.333330;1.000000;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.000000;0.666670;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  0.333330;0.666670;,
  0.333330;0.333330;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  0.333330;0.666670;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}