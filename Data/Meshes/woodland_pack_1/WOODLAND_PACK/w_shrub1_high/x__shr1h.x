xof 0303txt 0032

// DirectX file
// Creator: Ultimate Unwrap3D v2.24
// Time: Sun Dec 18 11:07:17 2005

// Start of Templates

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template FVFData {
 <b6e70a0e-8ef9-4e83-94ad-ecc8b0c04897>
 DWORD dwFVF;
 DWORD nDWords;
 array DWORD data[nDWords];
}

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

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
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

template FrameTransformMatrix {
 <F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}

template Frame {
 <3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}

template FloatKeys {
 <10DD46A9-775B-11cf-8F52-0040333594A3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <F406B180-7B3B-11cf-8F52-0040333594A3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10DD46A8-775B-11cf-8F52-0040333594A3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}

template AnimationOptions {
 <E2BF56C0-840F-11cf-8F52-0040333594A3>
 DWORD openclosed;
 DWORD positionquality;
}

template Animation {
 <3D82AB4F-62DA-11cf-AB39-0020AF71E433>
 [...]
}

template AnimationSet {
 <3D82AB50-62DA-11cf-AB39-0020AF71E433>
 [Animation]
}

// Start of Frames

Frame Body {
   FrameTransformMatrix {
    1.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 1.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 1.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 1.000000;;
   }

   Mesh staticMesh {
    28;
    21.794304; 90.611763; -17.981720;,
    -15.608535; 59.949070; 23.422024;,
    20.790157; 46.889095; -6.749910;,
    -14.575082; 98.095665; -2.833495;,
    -6.020776; 84.368233; 10.769156;,
    8.034110; 21.030008; 24.993349;,
    -12.915911; 44.016788; -12.933316;,
    -2.350863; 66.275810; 42.377697;,
    23.623896; 85.676949; 15.537254;,
    -14.775744; 40.505386; -7.666928;,
    14.925350; 44.225803; 31.188787;,
    9.229972; 77.789879; -21.052570;,
    22.772482; 57.662163; -7.473398;,
    1.682359; 10.356696; 20.926296;,
    -0.705811; 24.640091; -19.513292;,
    34.902542; 38.185513; 17.397415;,
    0.341719; 73.033890; 4.347548;,
    -10.022126; 6.108279; 17.438347;,
    12.026477; 30.284239; -19.100452;,
    -27.114643; 54.375942; 26.846920;,
    11.446054; 49.325314; 22.111042;,
    -11.018942; 0.277274; -21.489815;,
    11.446054; 0.277274; 22.111042;,
    2.810146; 49.325314; -26.775946;,
    -24.257015; 49.319954; 0.064546;,
    24.791027; 0.271910; 0.064546;,
    -24.257015; 0.271910; 0.064546;,
    23.156094; 49.319954; 14.778959;;
    28;
    3;0, 1, 2;,
    3;2, 1, 0;,
    3;3, 1, 0;,
    3;0, 1, 3;,
    3;4, 5, 6;,
    3;6, 5, 4;,
    3;7, 5, 4;,
    3;4, 5, 7;,
    3;8, 9, 10;,
    3;10, 9, 8;,
    3;11, 9, 8;,
    3;8, 9, 11;,
    3;12, 13, 14;,
    3;14, 13, 12;,
    3;15, 13, 12;,
    3;12, 13, 15;,
    3;16, 17, 18;,
    3;18, 17, 16;,
    3;19, 17, 16;,
    3;16, 17, 19;,
    3;20, 21, 22;,
    3;22, 21, 20;,
    3;23, 21, 20;,
    3;20, 21, 23;,
    3;24, 25, 26;,
    3;26, 25, 24;,
    3;27, 25, 24;,
    3;24, 25, 27;;

   MeshNormals {
    28;
    -0.707107; -0.707107; 0.000000;,
    -0.707107; -0.707107; 0.000000;,
    -1.000000; 0.000000; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    0.000000; 0.812015; -0.583636;,
    0.000000; 0.812015; -0.583636;,
    0.000000; 0.600000; -0.800000;,
    0.000000; 0.000000; 1.000000;,
    0.000000; 0.992278; -0.124035;,
    0.000000; 0.992278; -0.124035;,
    -0.707107; 0.707107; 0.000000;,
    0.696311; 0.696311; -0.174078;,
    -0.534522; 0.801784; 0.267261;,
    -0.534522; 0.801784; 0.267261;,
    -0.707107; 0.707107; 0.000000;,
    0.000000; 0.707107; 0.707107;,
    -0.916157; 0.400819; 0.000000;,
    -0.916157; 0.400819; 0.000000;,
    0.000000; 1.000000; 0.000000;,
    -0.992278; 0.124035; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;;
    28;
    3;0, 1, 2;,
    3;2, 1, 0;,
    3;3, 1, 0;,
    3;0, 1, 3;,
    3;4, 5, 6;,
    3;6, 5, 4;,
    3;7, 5, 4;,
    3;4, 5, 7;,
    3;8, 9, 10;,
    3;10, 9, 8;,
    3;11, 9, 8;,
    3;8, 9, 11;,
    3;12, 13, 14;,
    3;14, 13, 12;,
    3;15, 13, 12;,
    3;12, 13, 15;,
    3;16, 17, 18;,
    3;18, 17, 16;,
    3;19, 17, 16;,
    3;16, 17, 19;,
    3;20, 21, 22;,
    3;22, 21, 20;,
    3;23, 21, 20;,
    3;20, 21, 23;,
    3;24, 25, 26;,
    3;26, 25, 24;,
    3;27, 25, 24;,
    3;24, 25, 27;;
   }

   MeshTextureCoords {
    28;
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;,
    0.996832; 0.003168;,
    0.003168; 0.996832;,
    0.996832; 0.996832;,
    0.003168; 0.003168;;
   }

   MeshMaterialList {
    1;
    28;
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
    0;

    Material {
     0.752941; 0.752941; 0.752941; 1.000000;;
     40.000000;
     0.000000; 0.000000; 0.000000;;
     0.000000; 0.000000; 0.000000;;

     TextureFilename {
      "shr1h.tga";
     }
    }

   }
  }
}
