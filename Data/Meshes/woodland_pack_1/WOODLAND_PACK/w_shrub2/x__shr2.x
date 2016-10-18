xof 0303txt 0032

// DirectX file
// Creator: Ultimate Unwrap3D v2.24
// Time: Sun Dec 18 11:08:31 2005

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
    13;
    -18.569704; 98.881691; -59.055477;,
    -36.763966; 0.000000; -35.395092;,
    -0.362177; 50.000000; 0.147041;,
    35.474506; 0.000000; 33.754059;,
    23.416618; 99.764343; 55.239933;,
    -39.281364; 98.947113; 43.526939;,
    -24.049395; 0.000000; 43.687263;,
    22.892559; 0.000000; -44.610268;,
    44.948471; 117.383385; -35.626095;,
    49.642822; 0.000000; -1.898515;,
    59.142288; 98.947113; -13.278704;,
    -50.370888; 0.000000; 0.122999;,
    -60.334244; 75.002449; 22.116245;;
    24;
    3;0, 1, 2;,
    3;2, 1, 0;,
    3;3, 2, 1;,
    3;1, 2, 3;,
    3;3, 4, 2;,
    3;2, 4, 3;,
    3;0, 2, 4;,
    3;4, 2, 0;,
    3;5, 6, 2;,
    3;2, 6, 5;,
    3;7, 2, 6;,
    3;6, 2, 7;,
    3;7, 8, 2;,
    3;2, 8, 7;,
    3;5, 2, 8;,
    3;8, 2, 5;,
    3;2, 9, 10;,
    3;10, 9, 2;,
    3;9, 2, 11;,
    3;11, 2, 9;,
    3;2, 12, 11;,
    3;11, 12, 2;,
    3;12, 2, 10;,
    3;10, 2, 12;;

   MeshNormals {
    13;
    -0.752577; -0.658505; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    -0.666667; -0.333333; -0.666667;,
    0.000000; -1.000000; 0.000000;,
    -0.588172; -0.808736; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    0.000000; -1.000000; 0.000000;,
    0.000000; -0.242536; -0.970143;,
    0.000000; 0.242536; -0.970143;,
    0.970143; 0.242536; 0.000000;,
    0.000000; 1.000000; 0.000000;,
    0.000000; 0.000000; 0.000000;,
    -0.371391; 0.928477; 0.000000;;
    24;
    3;0, 1, 2;,
    3;2, 1, 0;,
    3;3, 2, 1;,
    3;1, 2, 3;,
    3;3, 4, 2;,
    3;2, 4, 3;,
    3;0, 2, 4;,
    3;4, 2, 0;,
    3;5, 6, 2;,
    3;2, 6, 5;,
    3;7, 2, 6;,
    3;6, 2, 7;,
    3;7, 8, 2;,
    3;2, 8, 7;,
    3;5, 2, 8;,
    3;8, 2, 5;,
    3;2, 9, 10;,
    3;10, 9, 2;,
    3;9, 2, 11;,
    3;11, 2, 9;,
    3;2, 12, 11;,
    3;11, 12, 2;,
    3;12, 2, 10;,
    3;10, 2, 12;;
   }

   MeshTextureCoords {
    13;
    -0.000637; -0.000353;,
    -0.002219; 0.998294;,
    0.499436; 0.496639;,
    0.997185; 0.998294;,
    0.997957; -0.000708;,
    -0.000637; -0.000353;,
    -0.002219; 0.998294;,
    0.997185; 0.998294;,
    0.997957; -0.000708;,
    -0.002219; 0.998294;,
    -0.000637; -0.000353;,
    0.997185; 0.998294;,
    0.997957; -0.000708;;
   }

   MeshMaterialList {
    1;
    24;
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
      "shr2.tga";
     }
    }

   }
  }
}
