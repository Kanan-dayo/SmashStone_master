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
 84;
 0.61330;0.06592;-0.06842;,
 0.61330;0.06592;0.10064;,
 0.61330;-0.82548;0.10064;,
 0.61330;-0.82548;-0.06842;,
 -0.84123;0.06592;0.10064;,
 -0.84123;0.06592;-0.06842;,
 -0.84123;-0.82548;-0.06842;,
 -0.84123;-0.82548;0.10064;,
 0.43335;-0.56171;-0.33011;,
 0.43335;-0.02270;-0.33011;,
 0.61330;0.06592;-0.06842;,
 0.61330;-0.82548;-0.06842;,
 -0.66126;-0.56171;-0.33011;,
 -0.84123;-0.82548;-0.06842;,
 -0.66126;-0.02270;-0.33011;,
 -0.84123;0.06592;-0.06842;,
 -0.63724;-0.54988;0.39830;,
 -0.63724;-0.03453;0.39830;,
 -0.84123;0.06592;0.10064;,
 -0.84123;-0.82548;0.10064;,
 0.40932;-0.54988;0.39830;,
 0.61330;-0.82548;0.10064;,
 0.40932;-0.03453;0.39830;,
 0.61330;0.06592;0.10064;,
 0.44437;-0.94236;-0.06842;,
 -0.67229;-0.94236;-0.06842;,
 -0.84123;-0.82548;-0.06842;,
 0.61330;-0.82548;-0.06842;,
 0.44437;-0.94236;0.10064;,
 -0.67229;-0.94236;0.10064;,
 0.22190;-5.78283;-0.11495;,
 -0.52157;-5.78612;-0.12236;,
 0.23057;-5.78612;0.20912;,
 -0.52157;-5.78612;0.20912;,
 0.44997;0.16343;-0.03722;,
 0.44997;0.16344;0.08370;,
 0.61330;0.06592;-0.06842;,
 -0.67789;0.16344;0.08370;,
 -0.67789;0.16343;-0.03722;,
 -0.84123;0.06592;-0.06842;,
 -0.53834;0.07215;-0.20422;,
 0.31042;0.07215;-0.20422;,
 -0.67789;0.16343;-0.03722;,
 0.44997;0.16343;-0.03722;,
 0.29180;0.06037;0.27231;,
 -0.51972;0.06037;0.27231;,
 0.44997;0.16344;0.08370;,
 -0.67789;0.16344;0.08370;,
 -0.84123;-10.51688;-0.12236;,
 -0.84123;-10.51688;0.20912;,
 -0.84123;-8.38312;0.20912;,
 -0.84123;-8.38312;-0.12236;,
 0.61330;-10.51688;0.20912;,
 0.61330;-10.51688;-0.12236;,
 0.61330;-8.38312;-0.12236;,
 0.61330;-8.38312;0.20912;,
 -0.84123;-10.51688;-0.12236;,
 0.61330;-10.51688;-0.12236;,
 -0.66126;-10.30475;-0.63546;,
 0.43335;-10.30475;-0.63546;,
 0.61330;-10.51688;-0.12236;,
 -0.84123;-10.51688;-0.12236;,
 -0.66126;-9.01455;-0.63546;,
 -0.84123;-8.38312;-0.12236;,
 0.43335;-9.01455;-0.63546;,
 0.61330;-8.38312;-0.12236;,
 0.40932;-10.27642;0.79275;,
 -0.63724;-10.27642;0.79275;,
 -0.84123;-10.51688;0.20912;,
 0.61330;-10.51688;0.20912;,
 0.40932;-9.04288;0.79275;,
 0.61330;-8.38312;0.20912;,
 -0.63724;-9.04288;0.79275;,
 -0.84123;-8.38312;0.20912;,
 -0.67229;-8.10340;-0.12236;,
 0.44437;-8.10340;-0.12236;,
 0.61330;-8.38312;-0.12236;,
 -0.84123;-8.38312;-0.12236;,
 -0.67229;-8.10340;0.20912;,
 0.44437;-8.10340;0.20912;,
 -0.52157;-5.78612;-0.12236;,
 0.22190;-5.78283;-0.11495;,
 -0.52157;-5.78612;0.20912;,
 0.23057;-5.78612;0.20912;;
 
 52;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,8,11,13;,
 4;14,12,13,15;,
 4;14,9,8,12;,
 4;16,17,18,19;,
 4;20,16,19,21;,
 4;22,20,21,23;,
 4;22,17,16,20;,
 4;24,25,26,27;,
 4;28,24,27,2;,
 4;29,28,2,7;,
 4;25,29,7,26;,
 4;30,31,25,24;,
 4;32,30,24,28;,
 4;33,32,28,29;,
 4;31,33,29,25;,
 4;31,30,32,33;,
 4;34,35,1,36;,
 4;37,38,39,4;,
 4;37,35,34,38;,
 4;40,41,9,14;,
 4;42,40,14,15;,
 4;41,43,10,9;,
 4;41,40,42,43;,
 4;44,45,17,22;,
 4;46,44,22,23;,
 4;45,47,18,17;,
 4;45,44,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;52,49,56,57;,
 4;58,59,60,61;,
 4;62,58,61,63;,
 4;64,62,63,65;,
 4;59,64,65,60;,
 4;59,58,62,64;,
 4;66,67,68,69;,
 4;70,66,69,71;,
 4;72,70,71,73;,
 4;67,72,73,68;,
 4;67,66,70,72;,
 4;74,75,76,77;,
 4;78,74,77,50;,
 4;79,78,50,55;,
 4;75,79,55,76;,
 4;80,81,75,74;,
 4;82,80,74,78;,
 4;83,82,78,79;,
 4;81,83,79,75;,
 4;81,80,82,83;;
 
 MeshMaterialList {
  6;
  52;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.800000;0.737600;0.172800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.056800;0.056800;0.056800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.091200;0.244800;0.288800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.228800;0.220000;0.132000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.016000;0.900000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   1.000000;0.000000;0.020000;;
  }
 }
 MeshNormals {
  64;
  -0.954976;0.000000;-0.296685;,
  -0.922851;-0.317156;-0.218536;,
  0.954983;0.000000;-0.296660;,
  0.922859;-0.317146;-0.218517;,
  0.955222;0.000000;0.295891;,
  0.923014;-0.317079;0.217959;,
  -0.955218;0.000000;0.295902;,
  -0.923009;-0.317088;0.217968;,
  -0.325864;0.478918;-0.815138;,
  0.000000;-0.380854;-0.924635;,
  0.325877;0.478922;-0.815130;,
  0.465505;0.000000;-0.885045;,
  0.332031;0.473541;0.815790;,
  0.000000;-0.400379;0.916350;,
  -0.332024;0.473541;0.815794;,
  -0.466207;0.000000;0.884676;,
  0.002479;0.005264;-0.999983;,
  0.874930;-0.484190;-0.007582;,
  0.000000;0.011196;0.999937;,
  0.004971;0.005900;-0.999970;,
  0.999618;0.024249;-0.013239;,
  -0.205860;0.939762;-0.272889;,
  0.205871;0.939760;-0.272889;,
  0.212101;0.937503;0.275865;,
  -0.212089;0.937506;0.275864;,
  -0.087891;0.832054;-0.547687;,
  0.087894;0.832054;-0.547686;,
  0.095472;0.828518;0.551764;,
  -0.095469;0.828519;0.551763;,
  0.985812;0.000000;-0.167856;,
  0.976798;0.180368;-0.115468;,
  -0.985809;0.000000;-0.167873;,
  -0.976795;0.180377;-0.115480;,
  -0.985901;0.000000;0.167332;,
  -0.976841;0.180362;0.115109;,
  0.985902;0.000000;0.167325;,
  0.976844;0.180354;0.115104;,
  0.000000;-0.924136;-0.382064;,
  0.000000;0.334607;-0.942358;,
  -0.943637;0.000000;-0.330981;,
  0.000000;-0.924599;0.380942;,
  0.000000;0.354261;0.935147;,
  0.944005;0.000000;0.329932;,
  0.002492;0.000636;-0.999997;,
  -0.954105;0.299471;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.704302;-0.709900;,
  -0.823956;0.000000;-0.566653;,
  0.000000;-0.733774;0.679393;,
  0.824897;0.000000;0.565284;,
  -0.878379;-0.477965;0.000000;,
  -0.999857;0.016921;0.000000;,
  0.002212;-0.999985;-0.005076;,
  0.417570;0.860603;0.291545;,
  0.405861;0.867910;-0.286372;,
  -0.405841;0.867919;-0.286374;,
  -0.417548;0.860614;0.291544;,
  0.000000;-0.980969;0.194166;,
  0.000000;-0.980851;-0.194761;,
  0.943649;0.000000;-0.330948;,
  0.000000;0.000000;-1.000000;,
  -0.944000;0.000000;0.329946;,
  0.949662;0.313206;-0.006579;,
  -0.002212;0.999985;0.005076;;
  52;
  4;2,4,5,3;,
  4;6,0,1,7;,
  4;11,10,2,3;,
  4;9,46,9,9;,
  4;8,47,1,0;,
  4;8,10,11,9;,
  4;15,14,6,7;,
  4;13,48,13,13;,
  4;12,49,5,4;,
  4;12,14,15,13;,
  4;16,16,9,9;,
  4;17,17,3,5;,
  4;18,18,13,13;,
  4;50,50,7,1;,
  4;19,19,16,16;,
  4;20,20,17,17;,
  4;18,18,18,18;,
  4;51,51,50,50;,
  4;52,52,52,52;,
  4;22,23,53,54;,
  4;24,21,55,56;,
  4;24,23,22,21;,
  4;25,26,10,8;,
  4;21,25,8,55;,
  4;26,22,54,10;,
  4;26,25,21,22;,
  4;27,28,14,12;,
  4;23,27,12,53;,
  4;28,24,56,14;,
  4;28,27,23,24;,
  4;31,33,34,32;,
  4;35,29,30,36;,
  4;57,57,58,58;,
  4;37,37,58,58;,
  4;39,39,31,32;,
  4;38,38,38,38;,
  4;59,59,30,29;,
  4;60,60,38,38;,
  4;40,40,57,57;,
  4;42,42,35,36;,
  4;41,41,41,41;,
  4;61,61,34,33;,
  4;45,45,41,41;,
  4;43,43,38,38;,
  4;44,44,32,34;,
  4;45,45,41,41;,
  4;62,62,36,30;,
  4;19,19,43,43;,
  4;51,51,44,44;,
  4;18,18,45,45;,
  4;20,20,62,62;,
  4;63,63,63,63;;
 }
 MeshTextureCoords {
  84;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
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
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;;
 }
}