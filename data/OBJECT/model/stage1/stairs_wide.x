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
 18;
 207.68526;11.35846;-14.29391;,
 207.68526;5.63909;-14.29391;,
 -213.25493;5.66647;-14.28015;,
 -213.35589;11.35846;-14.61976;,
 207.68526;11.35846;-0.18787;,
 207.68526;11.35846;-14.29391;,
 -213.35589;11.35846;-14.61976;,
 -213.35589;11.35846;0.06066;,
 -212.88037;5.63909;-27.24068;,
 -213.25493;5.66647;-14.28015;,
 207.68526;5.63909;-14.29391;,
 207.68526;5.63909;-27.24068;,
 -212.90437;-0.12387;-27.14486;,
 207.68526;0.28103;-27.24068;,
 -213.47622;5.69384;-0.16034;,
 -213.31368;-0.01988;-14.14844;,
 -213.50054;-0.00143;-0.08857;,
 -213.31368;-0.01988;-14.14844;;
 
 14;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,10;,
 3;8,10,11;,
 3;12,8,11;,
 3;12,11,13;,
 3;2,14,7;,
 3;2,7,6;,
 3;15,16,14;,
 3;15,14,2;,
 3;12,17,9;,
 3;12,9,8;;
 
 MeshMaterialList {
  1;
  14;
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
   0.517600;0.517600;0.517600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.000773;-0.059546;-0.998225;,
  0.000370;-0.029786;-0.999556;,
  -0.000033;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000032;1.000000;-0.001055;,
  -0.000114;-0.008312;-0.999966;,
  -0.000228;0.000000;-1.000000;,
  -0.999817;0.011972;-0.014898;,
  -0.999765;0.007887;-0.020180;,
  -0.999903;0.004102;-0.013295;,
  0.000065;0.999998;-0.002110;,
  0.000000;-0.016624;-0.999862;,
  -0.999816;0.005472;-0.018372;,
  -0.999967;0.002058;-0.007856;,
  -0.999843;-0.017733;0.000000;,
  -0.999521;0.006638;-0.030224;,
  -0.999576;0.003682;-0.028896;;
  14;
  3;1,2,1;,
  3;1,1,0;,
  3;3,3,3;,
  3;3,3,4;,
  3;5,11,5;,
  3;5,5,3;,
  3;6,12,6;,
  3;6,6,7;,
  3;13,8,14;,
  3;13,14,15;,
  3;9,10,8;,
  3;9,8,13;,
  3;16,9,13;,
  3;16,13,17;;
 }
 MeshTextureCoords {
  18;
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
