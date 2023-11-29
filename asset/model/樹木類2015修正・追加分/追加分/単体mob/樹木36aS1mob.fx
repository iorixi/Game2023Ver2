////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ����36aS1mob.fx : MikuMikuMob ���f�������E�z�uFX
//
//  MME : �e�N�X�`�����ߕ������̃��l�ŃN���b�v���܂�
//  (���؂Ȃǂ̕��G�ȃe�N�X�`�����߃��f���ŃA���O���ɂ���ė֊s���������s���߂ɂ�镾�Q���y�����܂�)
//  �쐬: �j��P( ���͉��P����full.fx���� )
//
////////////////////////////////////////////////////////////////////////////////////////////////
// TexAlphaClip�p�����[�^��`

//���߃��l�̏���臒l
float DefAlphaThreshold = 0.1;


////////////////////////////////////////////////////////////////////////////////////////////////

// �p�����[�^��`
#define MMMob_ModelFileName "����36aS1mob.pmx" // ���f���t�@�C����
#define MMMob_ArrangeFileName "MMMob_����36aS1mob.png" // �z�u���t�@�C����
#define MMMob_TEX_WIDTH  1     // ���f���z�u�e�N�X�`���s�N�Z����
#define MMMob_TEX_HEIGHT 2     // ���f���z�u�e�N�X�`���s�N�Z������
#define MMMob_SRC_HEAD_MAT  "0"  // �탂�f���e�I�u�W�F�N�g�擪�̍ގ��ԍ�
#define MMMob_SRC_TAIL_MAT  "1"  // �탂�f���e�I�u�W�F�N�g�Ō�̍ގ��ԍ�
#define MMMob_EDGE_ON   "10000"  // �G�b�W��`�悷��ގ��ԍ�
#define MMMob_EDGE_THICK  1.00f    // �G�b�W�����䗦
#define MMMob_MODEL_COUNT 1     // �탂�f���I�u�W�F�N�g��
#define MMMob_VERTEX_COUNT 5546   // �탂�f�����_��
#define MMMob_ORG_POS   float3(0.00, 0.0, 0.00)       // �탂�f���z�u���W

//#define MMMob_FLAG_HEIGHTMAP   // �����}�b�v��`
#define MMMob_FOLLOWED_OBJ     // �����}�b�v�̓ǂݎ��ʒu�����f���̓����ɒǏ]������
//#define MMMob_FLAG_RANDMOVE    // �h�炬�v���[�V������`
//#define MMMob_FLAG_RANDX       // X�����h�炬�v���[�V������`
//#define MMMob_FLAG_RANDY       // Y�����h�炬�v���[�V������`
//#define MMMob_FLAG_RANDZ       // Z�����h�炬�v���[�V������`
//#define MMMob_FLAG_JUMP        // Y�����h�炬�W�����v���[�V������`

#define MMMob_RENDER_EDGE      // �G�b�W�`�揈�����s��
#define MMMob_RENDER_SHADOW    // �n�ʉe�`�揈�����s��
#define MMMob_RENDER_ZPLOT     // zplot�`�揈�����s��

#define MMMob_TRANSFORM_X     0.00f
#define MMMob_TRANSFORM_Y     MMMob_ColorToFloat(i+1, j)
#define MMMob_TRANSFORM_Z     0.00f
#define MMMob_TRANSFORM_RX    0.00000000f
#define MMMob_TRANSFORM_RY    0.00000000f
#define MMMob_TRANSFORM_RZ    0.00000000f
#define MMMob_TRANSFORM_SCALE 1.000f

#define MMMob_TRANSFORM_MODEL_X   0.00f
#define MMMob_TRANSFORM_MODEL_Y   0.00f
#define MMMob_TRANSFORM_MODEL_Z   0.00f

#define MMMob_TEX_WIDTH2    2     // MMMob_TEX_WIDTH*2
#define MMMob_TEX_WIDTH4    4     // MMMob_TEX_WIDTH*4
#define MMMob_TEX_WIDTH8    8     // MMMob_TEX_WIDTH*8
#define MMMob_TEX_WIDTH16   16     // MMMob_TEX_WIDTH*16
#define MMMob_TEX_WIDTH32   32     // MMMob_TEX_WIDTH*32
#define MMMob_TEX_WIDTH64   64     // MMMob_TEX_WIDTH*64
#define MMMob_TEX_WIDTH128  128     // MMMob_TEX_WIDTH*128

#define MMMob_EDGE_TEXCOORD  float4 MMMob_AddUV_Edge : TEXCOORD1  // �G�b�W�����܂ޒǉ�UV�̓��̓Z�}���e�B�b�N

//#define MMMob_DRAW_EDGE_HEAD   // �I�u�W�F�N�g�擪�ގ��ŃG�b�W�`�悷��
//#define MMMob_DRAW_EDGE_TAIL   // �I�u�W�F�N�g�ŏI�ގ��ŃG�b�W�`�悷��

int MMMob_RepertCount = 1;  // �V�F�[�_���`�攽����
int MMMob_RepertIndex;        // �V�F�[�_���`�攽���񐔂̃J�E���^

////////////////////////////////////////////////////////////////////////////////////////////////
// �{�[���E���[�t�p�����[�^

#ifndef MIKUMIKUMOVING
    float4x4 MMMob_ParentMat : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�S�Ă̐e"; >;
#else
    float4 MMMob_parentPos0 : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�S�Ă̐e"; >;
    float4x4 MMMob_parentMat : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�S�Ă̐e"; >;
    static float4x4 MMMob_ParentMat = float4x4(MMMob_parentMat[0], MMMob_parentMat[1], MMMob_parentMat[2], MMMob_parentPos0);
#endif
float3 MMMob_ParentPos : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�e�ړ�"; >;

#ifdef MMMob_FOLLOWED_OBJ
    float3 MMMob_CenterPos : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�Z���^�["; >;
#endif

#ifndef MIKUMIKUMOVING
#ifdef MMMob_RENDER_EDGE
    float MMMob_EdgeThick1 : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�G�b�W��"; >;
    float MMMob_EdgeThick2 : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�G�b�W��"; >;
    static float MMMob_EdgeThickness = (1.0f - MMMob_EdgeThick1 + 2.0f*MMMob_EdgeThick2)*MMMob_EDGE_THICK;
#endif
#endif

#ifdef MMMob_FLAG_RANDMOVE
    float MMMob_RandTime : TIME;
    float MMMob_MorphRandMove : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�h�����_��"; >;
    #ifdef MMMob_FLAG_RANDX
    float MMMob_MorphXAmp : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "X�U��"; >;
    float MMMob_MorphXFreq : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "X���g��"; >;
    float MMMob_MorphXPhase : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "X�ʑ�"; >;
    #endif
    #ifdef MMMob_FLAG_RANDY
    float MMMob_MorphYAmp : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Y�U��"; >;
    float MMMob_MorphYFreq : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Y���g��"; >;
    float MMMob_MorphYPhase : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Y�ʑ�"; >;
    #endif
    #ifdef MMMob_FLAG_RANDZ
    float MMMob_MorphZAmp : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Z�U��"; >;
    float MMMob_MorphZFreq : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Z���g��"; >;
    float MMMob_MorphZPhase : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "Z�ʑ�"; >;
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// �����}�b�v�֘A

#ifdef MMMob_FLAG_HEIGHTMAP
    float MMMob_MorphHMRightAng : CONTROLOBJECT < string name = MMMob_ModelFileName; string item = "�n�ʒ��p"; >;
    float3 MMMob_MapAreaPos  : CONTROLOBJECT < string name = "HeightMap.x"; >;
    float MMMob_MapAreaAcsRy : CONTROLOBJECT < string name = "HeightMap.x"; string item = "Ry"; >;
    float MMMob_MapAreaAcsSi : CONTROLOBJECT < string name = "HeightMap.x"; string item = "Si"; >;
    static float MMMob_MapAreaSinR = sin(MMMob_MapAreaAcsRy)/MMMob_MapAreaAcsSi;
    static float MMMob_MapAreaCosR = cos(MMMob_MapAreaAcsRy)/MMMob_MapAreaAcsSi;
    
    // �����}�b�v�e�N�X�`���̃T�C�Y
    #define MMMob_HeightMapWIDTH   1024
    #define MMMob_HeightMapHEIGHT  1024
    
    // �����}�b�v�Ɏg���I�t�X�N���[���o�b�t�@
    shared texture MMMob_HeightMapRT : OFFSCREENRENDERTARGET;
    sampler MMMob_SmpHeightMap : register(s2) = sampler_state
    {
        Texture = <MMMob_HeightMapRT>;
        AddressU = BORDER;
        AddressV = BORDER;
        BorderColor = 0;
        MinFilter = LINEAR;
        MagFilter = LINEAR;
    };
    
    // �����}�b�v����w��ʒu�̍������擾
    float MMMob_PickupHeight(float x, float z)
    {
        x -= MMMob_MapAreaPos.x;
        z -= MMMob_MapAreaPos.z;
        float x1 = MMMob_MapAreaCosR*x - MMMob_MapAreaSinR*z;
        float z1 = MMMob_MapAreaSinR*x + MMMob_MapAreaCosR*z;
        x1 = x1*0.1f + 0.5f + 0.5/MMMob_HeightMapWIDTH;
        z1 = 0.5f - z1*0.1f + 0.5/MMMob_HeightMapHEIGHT;
        float h = tex2Dlod(MMMob_SmpHeightMap, float4(x1, z1, 0, 0)).r;
        return h;
    }
    
    // �����}�b�v����w��ʒu�̒n�ʂɐ����ƂȂ��]�s����擾
    float3x3 MMMob_PickupRotMat(float x, float z, float hStep)
    {
        float3 PosX1 = float3(x-hStep, MMMob_PickupHeight(x-hStep, z), z);
        float3 PosX2 = float3(x+hStep, MMMob_PickupHeight(x+hStep, z), z);
        float3 PosZ1 = float3(x, MMMob_PickupHeight(x, z-hStep), z-hStep);
        float3 PosZ2 = float3(x, MMMob_PickupHeight(x, z+hStep), z+hStep);
        float lenX = length( (PosX2 - PosX1) * float3(1.0f, MMMob_MorphHMRightAng, 1.0f) );
        float lenZ = length( (PosZ2 - PosZ1) * float3(1.0f, MMMob_MorphHMRightAng, 1.0f) );
        float sinX = MMMob_MorphHMRightAng * (PosZ1.y - PosZ2.y) / lenZ;
        float cosX = 2.0f * hStep / lenZ;
        float sinZ = MMMob_MorphHMRightAng * (PosX2.y - PosX1.y) / lenX;
        float cosZ = 2.0f * hStep / lenX;
        float3x3 rotMat = { cosZ,  cosX*sinZ,  sinX*sinZ,
                           -sinZ,  cosX*cosZ,  sinX*cosZ,
                            0.0f,      -sinX,       cosX };
        return rotMat;
    }
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// �G�b�W,�n�ʉe,Z�v���b�g�֘A

#ifndef MIKUMIKUMOVING

#ifdef MMMob_RENDER_EDGE
    // �G�b�W�����E�F
    float4 MMMob_EdgeColor : EDGECOLOR;
    #ifdef MMMob_EDGE_TEXCOORD
        #define MMMob_EDGE_THICKNESS  (MMMob_AddUV_Edge.w * MMMob_EdgeThickness)
        #define MMMob_EDGE_COLOR      float4(MMMob_AddUV_Edge.xyz, MMMob_EdgeColor.a)
    #else
        #define MMMob_EDGE_THICKNESS  MMMob_EdgeThickness
        #define MMMob_EDGE_COLOR      MMMob_EdgeColor
        #define MMMob_EDGE_TEXCOORD   float4 MMMob_DummyTex : TEXCOORD5
    #endif

    // �I�u�W�F�N�g�`��e�N�j�b�N�� MMMob_EdgeColor ���擾���邽�߂̃_�~�[����
    // ���_�V�F�[�_
    float4 MMMob_DummyEdge_VS(float4 Pos : POSITION) : POSITION 
    {
        return float4(0,0,0,0);
    }
    // �s�N�Z���V�F�[�_
    float4 MMMob_DummyEdge_PS() : COLOR
    {
        return float4(0,0,0,0);
    }
    // �֊s�`��p�e�N�j�b�N
    technique MMMob_DummyEdgeTec < string MMDPass = "edge"; > {
        pass DrawEdge {
            AlphaBlendEnable = FALSE;
            AlphaTestEnable  = FALSE;
            VertexShader = compile vs_2_0 MMMob_DummyEdge_VS();
            PixelShader  = compile ps_2_0 MMMob_DummyEdge_PS();
        }
    }
#else
    #define MMMob_EDGE_THICKNESS  (0.0f)
    #define MMMob_EDGE_COLOR      float4(0,0,0,1);
    #ifndef MMMob_EDGE_TEXCOORD
        #define MMMob_EDGE_TEXCOORD   float4 MMMob_DummyTex : TEXCOORD5
    #endif
#endif

float4x4 MMMob_PrjMtx : PROJECTION;
#define MMMob_IsParth  (MMMob_PrjMtx._44 < 0.5f)

// MMD�̃G�b�W�`��͍s��Ȃ�
technique MMMob_EdgeTec < string MMDPass = "edge"; > { }

// Z�l�v���b�g�`���\���w��̏ꍇ
#ifndef MMMob_RENDER_ZPLOT
    technique MMMob_ZplotTec < string MMDPass = "zplot"; >{ }
#endif

#endif

// ��Z���t�V���h�E�n�ʉe�`���\���w��̏ꍇ
#ifndef MMMob_RENDER_SHADOW
    technique MMMob_ShadowTec < string MMDPass = "shadow"; >{ }
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// �z�u���e�N�X�`��

texture2D MMMob_ArrangeTex <
    string ResourceName = MMMob_ArrangeFileName;
>;
sampler MMMob_ArrangeSmp : register(s3) = sampler_state{
    texture = <MMMob_ArrangeTex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
};

////////////////////////////////////////////////////////////////////////////////////////////////

// �������Z
int MMMob_Div(int a, int b) {
    return floor((a+0.1f)/b);
}

// ������]�Z
int MMMob_Mod(int a, int b) {
    return (a - MMMob_Div(a,b)*b);
};

// �z�u���e�N�X�`������f�[�^�����o��
float MMMob_ColorToFloat(int i, int j)
{
    float4 d = tex2Dlod(MMMob_ArrangeSmp, float4((i+0.5)/MMMob_TEX_WIDTH8, (j+0.5)/MMMob_TEX_HEIGHT, 0, 0));
    float tNum = (65536.0f * d.x + 256.0f * d.y + d.z) * 255.0f;
    int pNum = round(d.w * 255.0f);
    int sgn = 1 - 2 * (pNum % 2);
    float data = tNum * pow(10.0f, pNum/2 - 64) * sgn;
    return data;
}

// �탂�f���I�u�W�F�N�g�ʒu�����o��
float3 MMMob_GetModelPos(int modelIndex)
{
    int index = MMMob_MODEL_COUNT * MMMob_RepertCount + modelIndex;
    int i = (index/MMMob_TEX_HEIGHT) * 8;
    int j = index % MMMob_TEX_HEIGHT;
    return float3(MMMob_TRANSFORM_MODEL_X, MMMob_TRANSFORM_MODEL_Y, MMMob_TRANSFORM_MODEL_Z);
}

// �h�炬�v���[�V�����␳�l
static float3 MMMob_RandomMove;

////////////////////////////////////////////////////////////////////////////////////////////////
// �������f���̔z�u���W�ϊ��s��
float4x4 MMMob_TransMatrixBase(int objIndex, int modelIndex)
{
    int i = (objIndex/MMMob_TEX_HEIGHT) * 8;
    int j = objIndex % MMMob_TEX_HEIGHT;

    float x = MMMob_TRANSFORM_X;
    float y = MMMob_TRANSFORM_Y;
    float z = MMMob_TRANSFORM_Z;
    float rx = MMMob_TRANSFORM_RX;
    float ry = MMMob_TRANSFORM_RY;
    float rz = MMMob_TRANSFORM_RZ;
    float scale = MMMob_TRANSFORM_SCALE;

    // �h�炬�ʒu�␳
    MMMob_RandomMove = float3(0.0f, 0.0f, 0.0f);
    #ifdef MMMob_FLAG_RANDMOVE
        float rand0 = MMMob_ColorToFloat(i+7, j);
        float3 rand = float3(frac((rand0+0.531f)*43.0f), rand0, frac((rand0+0.357f)*37.0f));
        float3 Random = float3(1.0f, 1.0f, 1.0f) - rand * MMMob_MorphRandMove * 0.5f;
        #ifdef MMMob_FLAG_RANDX
            MMMob_RandomMove.x = 10.0f * Random.x * MMMob_MorphXAmp
                          * sin(4.0f * 3.1416f * MMMob_MorphXFreq * Random.x * MMMob_RandTime + MMMob_MorphXPhase * 6.283f * rand.x);
            x += MMMob_RandomMove.x;
        #endif
        #ifdef MMMob_FLAG_RANDY
            #ifdef MMMob_FLAG_JUMP
            MMMob_RandomMove.y = 10.0f * Random.y * MMMob_MorphYAmp
                         * abs(sin(4.0f * 3.1416f * MMMob_MorphYFreq * Random.y * MMMob_RandTime + MMMob_MorphYPhase * 3.1416f * rand.y));
            #else
            MMMob_RandomMove.y = 10.0f * Random.y * MMMob_MorphYAmp
                         * sin(4.0f * 3.1416f * MMMob_MorphYFreq * Random.y * MMMob_RandTime + MMMob_MorphYPhase * 3.1416f * rand.y);
            #endif
            y += MMMob_RandomMove.y;
        #endif
        #ifdef MMMob_FLAG_RANDZ
            MMMob_RandomMove.z = 10.0f * Random.z * MMMob_MorphZAmp
                         * sin(4.0f * 3.1416f * MMMob_MorphZFreq * Random.z * MMMob_RandTime + MMMob_MorphZPhase * 6.283f * rand.z);
            z += MMMob_RandomMove.z;
        #endif
    #endif

    float sinx, cosx, siny, cosy, sinz, cosz;
    sincos(rx, sinx, cosx);
    sincos(ry, siny, cosy);
    sincos(rz, sinz, cosz);

    float4x4 TransMatrix;
    TransMatrix._11 = (cosy * cosz + sinx * siny * sinz) * scale;
    TransMatrix._12 = -cosx * sinz * scale;
    TransMatrix._13 = (siny * cosz - sinx * cosy * sinz) * scale;
    TransMatrix._14 = 0.0f;
    TransMatrix._21 = (cosy * sinz - sinx * siny * cosz) * scale;
    TransMatrix._22 = cosx * cosz * scale;
    TransMatrix._23 = (siny * sinz + sinx * cosy * cosz) * scale;
    TransMatrix._24 = 0.0f;
    TransMatrix._31 = -cosx * siny * scale;
    TransMatrix._32 = -sinx * scale;
    TransMatrix._33 = cosx * cosy * scale;
    TransMatrix._34 = 0.0f;
    TransMatrix._41 = x;
    TransMatrix._42 = y;
    TransMatrix._43 = z;
    TransMatrix._44 = 1.0f;

    // �탂�f���̃I�u�W�F�N�g�ʒu
    float3 modelPos = MMMob_GetModelPos(modelIndex);

    // �S�Ă̐e�{�[��WorldMatrix�̋t�s��
    float3x3 mat3x3_inv = transpose((float3x3)MMMob_ParentMat);
    float4x4 invParent = float4x4( mat3x3_inv[0], 0.0f, 
                                   mat3x3_inv[1], 0.0f, 
                                   mat3x3_inv[2], 0.0f, 
                                  -mul(MMMob_ParentMat._41_42_43, mat3x3_inv), 1.0f );
    invParent._41_42_43 -= modelPos;

    // �z�u�ϊ��s��
    TransMatrix = mul( mul( invParent, TransMatrix ), MMMob_ParentMat );
    TransMatrix._41_42_43 -= MMMob_ORG_POS;

    #ifdef MMMob_FLAG_HEIGHTMAP
        // �����}�b�v�ɂ��z�u�ϊ�
        #ifdef MMMob_FOLLOWED_OBJ
            float4 objPos = mul( float4(MMMob_CenterPos, 1.0f) + float4(mul(modelPos, (float3x3)MMMob_ParentMat), 0.0f), TransMatrix );
        #else
            float4 objPos = mul( float4(MMMob_ParentPos, 1.0f) + float4(mul(modelPos, (float3x3)MMMob_ParentMat), 0.0f), TransMatrix );
        #endif
        TransMatrix._42 += MMMob_PickupHeight(objPos.x, objPos.z);

        // �n�`�̌X���ɑ΂����]
        float3x3 rotMat = MMMob_PickupRotMat(objPos.x, objPos.z, 2.5f);
        float3 pos0 = float3(objPos.x, TransMatrix._42, objPos.z);
        TransMatrix = mul( TransMatrix, float4x4(rotMat[0], 0.0f,
                                                 rotMat[1], 0.0f,
                                                 rotMat[2], 0.0f,
                                                 pos0-mul(pos0, rotMat), 1.0f) );
    #endif

    return TransMatrix;
}


// �������㑤�Œ�`����Ă���֐��E�ϐ��E�}�N���̎g�p�͐������Ȃ�

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// �G�t�F�N�g�R�[�h�{�̂Ŏg�p����֐��E�}�N����`

//--------------------------------------------------
// �������f���̃I�u�W�F�N�g�ԍ��𓾂�
//--------------------------------------------------
int MOB_ObjectIndex(int vIndex)
{
    #if MMMob_MODEL_COUNT > 1
    int modelIndex = MMMob_Div(vIndex*MMMob_MODEL_COUNT, MMMob_VERTEX_COUNT);
    #else
    int modelIndex = 0;
    #endif
    return (MMMob_MODEL_COUNT * MMMob_RepertIndex + modelIndex);
}

//--------------------------------------------------
// �������f���̃I�u�W�F�N�g���̈�l�����𓾂�(0�`1)
//--------------------------------------------------
float MOB_ObjectRandom(int vIndex)
{
    int objIndex = MOB_ObjectIndex(vIndex);
    int i = (objIndex/MMMob_TEX_HEIGHT) * 8;
    int j = objIndex % MMMob_TEX_HEIGHT;
    return MMMob_ColorToFloat(i+7, j);
}

//--------------------------------------------------
// �������f���̔z�u���W�ϊ��s��(���_�C���f�b�N�X���w��)
//--------------------------------------------------
float4x4 MOB_TransMatrix(int vIndex)
{
    #if MMMob_MODEL_COUNT > 1
    int modelIndex = MMMob_Div(vIndex*MMMob_MODEL_COUNT, MMMob_VERTEX_COUNT);
    #else
    int modelIndex = 0;
    #endif
    int objIndex = MMMob_MODEL_COUNT * MMMob_RepertIndex + modelIndex;
    return MMMob_TransMatrixBase(objIndex, modelIndex);
}

//--------------------------------------------------
// �������f���̔z�u���W�ϊ��s��(�I�u�W�F�N�g�ԍ����w��)
//--------------------------------------------------
float4x4 MOB_TransMatrixObjIndex(int objIndex)
{
    #if MMMob_MODEL_COUNT > 1
    int modelIndex = MMMob_Mod(objIndex, MMMob_MODEL_COUNT);
    #else
    int modelIndex = 0;
    #endif
    return MMMob_TransMatrixBase(objIndex, modelIndex);
}

//--------------------------------------------------
// �z�u�ϊ���̍��W�Ɩ@�����擾����֐�
//--------------------------------------------------
// �z�u�ϊ���̍��W��@����`
struct MOB_TRANSFORM {
    float4 Pos;     // �������f���z�u�ϊ���̒��_���W
    float3 Normal;  // �������f���z�u�ϊ���̒��_�@��
};

MOB_TRANSFORM MOB_TransformPositionNormal(float4 Pos, float3 Normal, int vIndex)
{
    MOB_TRANSFORM Out;

    // �������f���̔z�u���W�ϊ�
    float4x4 TransMatrix = MOB_TransMatrix(vIndex);
    Out.Pos = mul( Pos, TransMatrix );

    // �z�u���W�ϊ��ɂ�钸�_�@��
    Out.Normal = normalize( mul( Normal, (float3x3)TransMatrix ) );

    return Out;
}

//--------------------------------------------------
// �z�u�ϊ���̍��W�݂̂��擾����֐�
//--------------------------------------------------
float4 MOB_TransformPosition(float4 Pos, int vIndex)
{
    return mul( Pos, MOB_TransMatrix(vIndex) );
}

//--------------------------------------------------
// �@�������ɉ����o�����G�b�W���_���W���擾����֐�
//--------------------------------------------------
float4 MOB_EdgePosition(float4 Pos, float3 Normal, float EdgeThick, float3 CamPos)
{
    #ifndef MIKUMIKUMOVING
    // �J�����Ƃ̋���
    float dist = max( length( CamPos - Pos.xyz ), 5.0f );
    // ���_��@�������ɉ����o��
    if( MMMob_IsParth ){
        // �p�[�X�y�N�e�B�uon
        Pos.xyz += Normal * ( EdgeThick * pow( abs(dist), 0.9f ) * 0.0015f * pow( abs(2.4142f / MMMob_PrjMtx._22), 0.7f ) );
    }else{
        // �p�[�X�y�N�e�B�uoff
        Pos.xyz += Normal * ( EdgeThick * 0.0025f / MMMob_PrjMtx._11 );
    }
    #else
    // �J�����Ƃ̋���
    float dist = distance(Pos.xyz, CamPos);
    // ���_��@�������ɉ����o��
    if (MMM_IsDinamicProjection){
        Pos += float4(Normal, 0) * EdgeThick * dist * MMM_GetDynamicFovEdgeRate(dist);
    }else{
        Pos += float4(Normal, 0) * EdgeThick * dist;
    }
    #endif

    return Pos;
}

//--------------------------------------------------
// �z�u�ϊ���̃G�b�W���_���W���擾����֐�
//--------------------------------------------------
float4 MOB_TransformEdgePosition(float4 Pos, float3 Normal, float EdgeThick, float3 CamPos, int vIndex)
{
    // �������f���̔z�u���W�ϊ�
    float4x4 TransMatrix = MOB_TransMatrix(vIndex);
    Pos = mul( Pos, TransMatrix );

    // �z�u���W�ϊ��ɂ�钸�_�@��
    Normal = normalize( mul( Normal, (float3x3)TransMatrix ) );

    return MOB_EdgePosition(Pos, Normal, EdgeThick, CamPos);
}

//--------------------------------------------------
// �z�u�ϊ���̃I�u�W�F�N�g���_�ʒu���擾����֐�
//--------------------------------------------------
float3 MOB_TransformOriginPosition(int vIndex)
{
    // �탂�f���̃I�u�W�F�N�g�ʒu
    #if MMMob_MODEL_COUNT > 1
    int modelIndex = MMMob_Div(vIndex*MMMob_MODEL_COUNT, MMMob_VERTEX_COUNT);
    float3 modelPos = MMMob_ORG_POS + MMMob_GetModelPos(modelIndex);
    #else
    float3 modelPos = MMMob_ORG_POS;
    #endif
    modelPos -= MMMob_RandomMove;

    // �������f���̔z�u���W�ϊ�
    float4x4 TransMatrix = MOB_TransMatrix(vIndex);

    return mul( float4(modelPos, 1.0f), TransMatrix ).xyz;
}

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#ifndef MIKUMIKUMOVING
    #define MOB_EDGE_TEXCOORD  MMMob_EDGE_TEXCOORD   // �G�b�W�����̓Z�}���e�b�N
    #define MOB_EDGE_THICK     MMMob_EDGE_THICKNESS  // �G�b�W�̑���
    #define MOB_EDGE_COLOR     MMMob_EDGE_COLOR      // �G�b�W�̐F
#endif

#ifdef MMMob_DRAW_EDGE_HEAD
    #define MOB_DRAW_EDGE_HEAD   // �I�u�W�F�N�g�擪�ގ��ŃG�b�W�`�悷��
#endif
#ifdef MMMob_DRAW_EDGE_TAIL
    #define MOB_DRAW_EDGE_TAIL   // �I�u�W�F�N�g�ŏI�ގ��ŃG�b�W�`�悷��
#endif

#define MOB_EDGE_ON       MMMob_EDGE_ON        // �G�b�W��`�悷��ގ��ԍ�
#define MOB_SRC_HEAD_MAT  MMMob_SRC_HEAD_MAT   // �탂�f���e�I�u�W�F�N�g�擪�̍ގ��ԍ�
#define MOB_SRC_TAIL_MAT  MMMob_SRC_TAIL_MAT   // �탂�f���e�I�u�W�F�N�g�Ō�̍ގ��ԍ�

#define MOB_TEX_WIDTH     MMMob_TEX_WIDTH      // ���f���z�u�e�N�X�`���s�N�Z����
#define MOB_TEX_HEIGHT    MMMob_TEX_HEIGHT     // ���f���z�u�e�N�X�`���s�N�Z������

#define MOB_TEX_WIDTH2    MMMob_TEX_WIDTH2     // MOB_TEX_WIDTH*2
#define MOB_TEX_WIDTH4    MMMob_TEX_WIDTH4     // MOB_TEX_WIDTH*4
#define MOB_TEX_WIDTH8    MMMob_TEX_WIDTH8     // MOB_TEX_WIDTH*8
#define MOB_TEX_WIDTH16   MMMob_TEX_WIDTH16    // MOB_TEX_WIDTH*16
#define MOB_TEX_WIDTH32   MMMob_TEX_WIDTH32    // MOB_TEX_WIDTH*32
#define MOB_TEX_WIDTH64   MMMob_TEX_WIDTH64    // MOB_TEX_WIDTH*64
#define MOB_TEX_WIDTH128  MMMob_TEX_WIDTH128   // MOB_TEX_WIDTH*128

//--------------------------------------------------
// �X�N���v�g��`
//--------------------------------------------------
#define MOB_LOOPSCRIPT_OBJECT       "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex; Pass=DrawObject; LoopEnd=;"
#define MOB_LOOPSCRIPT_OBJECT_EDGE  "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex; Pass=DrawObject; Pass=DrawEdge; LoopEnd=;"
#define MOB_LOOPSCRIPT_EDGE         "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex; Pass=DrawEdge; LoopEnd=;"
#define MOB_LOOPSCRIPT_SHADOW       "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex; Pass=DrawShadow; LoopEnd=;"
#define MOB_LOOPSCRIPT_ZPLOT        "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex; Pass=ZValuePlot; LoopEnd=;"
#define MOB_LOOPSCRIPT(n)           "LoopByCount=MMMob_RepertCount; LoopGetIndex=MMMob_RepertIndex;" n "LoopEnd=;"


////////////////////////////////////////////////////////////////////////////////////////////////
// �ȉ��G�t�F�N�g�{�̃R�[�h

////////////////////////////////////////////////////////////////////////////////////////////////

float AlphaClip : CONTROLOBJECT < string name = "(self)"; string item = "���د��"; >;
static float AlphaThreshold = lerp(DefAlphaThreshold, 0.99f, AlphaClip);

float Script : STANDARDSGLOBAL <
    string ScriptOutput = "color";
    string ScriptClass = "sceneorobject";
    string ScriptOrder = "standard";
> = 0.8;

// ���W�ϊ��s��
float4x4 WorldViewProjMatrix : WORLDVIEWPROJECTION;
float4x4 WorldMatrix         : WORLD;
float4x4 ViewMatrix          : VIEW;
float4x4 ViewProjMatrix      : VIEWPROJECTION;
float4x4 LightViewProjMatrix : VIEWPROJECTION < string Object = "Light"; >;

// ���C�g����,�J�����ʒu
float3 LightDirection : DIRECTION < string Object = "Light"; >;
float3 CameraPosition : POSITION  < string Object = "Camera"; >;

// �}�e���A���F
float4 MaterialDiffuse   : DIFFUSE  < string Object = "Geometry"; >;
float3 MaterialAmbient   : AMBIENT  < string Object = "Geometry"; >;
float3 MaterialEmmisive  : EMISSIVE < string Object = "Geometry"; >;
float3 MaterialSpecular  : SPECULAR < string Object = "Geometry"; >;
float  SpecularPower     : SPECULARPOWER < string Object = "Geometry"; >;
float3 MaterialToon      : TOONCOLOR;
float4 GroundShadowColor : GROUNDSHADOWCOLOR;
// ���C�g�F
float3 LightDiffuse  : DIFFUSE  < string Object = "Light"; >;
float3 LightAmbient  : AMBIENT  < string Object = "Light"; >;
float3 LightSpecular : SPECULAR < string Object = "Light"; >;
static float4 DiffuseColor  = MaterialDiffuse  * float4(LightDiffuse, 1.0f);
static float3 AmbientColor  = MaterialAmbient  * LightAmbient + MaterialEmmisive;
static float3 SpecularColor = MaterialSpecular * LightSpecular;

// �e�N�X�`���ގ����[�t�l
float4 TextureAddValue : ADDINGTEXTURE;
float4 TextureMulValue : MULTIPLYINGTEXTURE;
float4 SphereAddValue  : ADDINGSPHERETEXTURE;
float4 SphereMulValue  : MULTIPLYINGSPHERETEXTURE;

bool use_texture;       // �e�N�X�`���̗L��
bool use_spheremap;     // �X�t�B�A�}�b�v�̗L��
bool use_subtexture;    // �T�u�e�N�X�`���t���O

bool parthf;   // �p�[�X�y�N�e�B�u�t���O
bool transp;   // �������t���O
bool spadd;    // �X�t�B�A�}�b�v���Z�����t���O
#define SKII1  1500
#define SKII2  8000
#define Toon   3

// �I�u�W�F�N�g�̃e�N�X�`��
texture ObjectTexture: MATERIALTEXTURE;
sampler ObjTexSampler = sampler_state {
    texture = <ObjectTexture>;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MIPFILTER = LINEAR;
    ADDRESSU  = WRAP;
    ADDRESSV  = WRAP;
};

// �X�t�B�A�}�b�v�̃e�N�X�`��
texture ObjectSphereMap: MATERIALSPHEREMAP;
sampler ObjSphareSampler = sampler_state {
    texture = <ObjectSphereMap>;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MIPFILTER = LINEAR;
    ADDRESSU  = WRAP;
    ADDRESSV  = WRAP;
};

// �g�D�[���}�b�v�̃e�N�X�`��
texture ObjectToonTexture: MATERIALTOONTEXTURE;
sampler ObjToonSampler = sampler_state {
    texture = <ObjectToonTexture>;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MIPFILTER = NONE;
    ADDRESSU  = CLAMP;
    ADDRESSV  = CLAMP;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// �֊s�`��

struct VS_EDGE {
    float4 Pos       : POSITION; // �ˉe�ϊ����W
    float4 EdgeColor : COLOR0;   // �G�b�W�F
};

// ���_�V�F�[�_
VS_EDGE Edge_VS(float4 Pos : POSITION, float3 Normal : NORMAL, MOB_EDGE_TEXCOORD, int vIndex : _INDEX)
{
    VS_EDGE Out = (VS_EDGE)0;

    // �탂�f���̃��[���h���W�ϊ�
    Pos = mul( Pos, WorldMatrix );

    // ���[���h���W�ϊ��ɂ�钸�_�@��
    Normal = mul( Normal, (float3x3)WorldMatrix );

    // �z�u�ϊ���̃G�b�W���_���W
    Pos = MOB_TransformEdgePosition(Pos, Normal, MOB_EDGE_THICK, CameraPosition, vIndex);

    // �J�������_�̃r���[�ˉe�ϊ�
    Out.Pos = mul( Pos, ViewProjMatrix );

    // �G�b�W�F
    Out.EdgeColor = MOB_EDGE_COLOR;

    return Out;
}

// �s�N�Z���V�F�[�_
float4 Edge_PS(VS_EDGE IN) : COLOR
{
    // �֊s�F�œh��Ԃ�
    return IN.EdgeColor;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// ��Z���t�V���h�E�n�ʉe�`��

// ���_�V�F�[�_
float4 Shadow_VS(float4 Pos : POSITION, int vIndex : _INDEX) : POSITION
{
    // �������f���̔z�u���W�ϊ�
    Pos = MOB_TransformPosition(Pos, vIndex);

    // �J�������_�̃��[���h�r���[�ˉe�ϊ�(�A�N�Z�T���͐������ʒu�ւ̕ϊ��͏o���Ȃ�)
    return  mul( Pos, WorldViewProjMatrix );
}

// �s�N�Z���V�F�[�_
float4 Shadow_PS() : COLOR
{
    // �n�ʉe�F�œh��Ԃ�
    return GroundShadowColor;
}

// �e�`��p�e�N�j�b�N
technique ShadowTec < string MMDPass = "shadow"; string Script = MOB_LOOPSCRIPT_SHADOW; >
{
    pass DrawShadow {
        VertexShader = compile vs_3_0 Shadow_VS();
        PixelShader  = compile ps_3_0 Shadow_PS();
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// �I�u�W�F�N�g�`��i�Z���t�V���h�EOFF�j

struct VS_OUTPUT {
    float4 Pos    : POSITION;   // �ˉe�ϊ����W
    float2 Tex    : TEXCOORD1;  // �e�N�X�`��
    float3 Normal : TEXCOORD2;  // �@��
    float3 Eye    : TEXCOORD3;  // �J�����Ƃ̑��Έʒu
    float2 SpTex  : TEXCOORD4;  // �X�t�B�A�}�b�v�e�N�X�`�����W
    float4 Color  : COLOR0;     // �f�B�t���[�Y�F
};

// ���_�V�F�[�_
VS_OUTPUT Basic_VS(float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD0, float2 Tex2 : TEXCOORD1, int vIndex : _INDEX, uniform bool useToon)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    // �탂�f���̃��[���h���W�ϊ�
    Pos = mul( Pos, WorldMatrix );

    // ���[���h���W�ϊ��ɂ�钸�_�@��
    Normal = mul( Normal, (float3x3)WorldMatrix );

    // �z�u�ϊ���̍��W�Ɩ@�����擾
    MOB_TRANSFORM TrOut = MOB_TransformPositionNormal(Pos, Normal, vIndex);

    // �z�u�ϊ���̒��_�@��
    Out.Normal = TrOut.Normal;

    // �J�������_�̃r���[�ˉe�ϊ�
    Out.Pos = mul( TrOut.Pos, ViewProjMatrix );

    // �J�����Ƃ̑��Έʒu
    Out.Eye = CameraPosition - TrOut.Pos.xyz;

    // �f�B�t���[�Y�F�{�A���r�G���g�F �v�Z
    Out.Color.rgb = AmbientColor;
    if( !useToon ) {
        Out.Color.rgb += max(0, dot( Out.Normal, -LightDirection )) * DiffuseColor.rgb;
    }
    Out.Color.a = DiffuseColor.a;
    Out.Color = saturate( Out.Color );

    // �e�N�X�`�����W
    Out.Tex = Tex;

    if( use_spheremap ) {
        if( use_subtexture ) {
            // PMX�T�u�e�N�X�`�����W
            Out.SpTex = Tex2;
        } else {
            // �X�t�B�A�}�b�v�e�N�X�`�����W
            float2 NormalWV = mul( Out.Normal, (float3x3)ViewMatrix ).xy;
            Out.SpTex.x = NormalWV.x * 0.5f + 0.5f;
            Out.SpTex.y = NormalWV.y * -0.5f + 0.5f;
        }
    }

    return Out;
}

// �s�N�Z���V�F�[�_
float4 Basic_PS(VS_OUTPUT IN, uniform bool useToon) : COLOR0
{
    // �X�y�L�����F�v�Z
    float3 HalfVector = normalize( normalize(IN.Eye) + -LightDirection );
    float3 Specular = pow( max(0, dot( HalfVector, normalize(IN.Normal) )), SpecularPower ) * SpecularColor;

    float4 Color = IN.Color;
    if( use_texture ) {
        // �e�N�X�`���K�p
        Color *= tex2D( ObjTexSampler, IN.Tex );
        // ���l���ȉ��̃e�N�X�`�����ߕ��͕\�����Ȃ�
        clip(Color.a - AlphaThreshold);
    }
    if( use_spheremap ) {
        // �X�t�B�A�}�b�v�K�p
        float4 TexColor = tex2D(ObjSphareSampler,IN.SpTex);
        if(spadd) Color.rgb += TexColor.rgb;
        else      Color.rgb *= TexColor.rgb;
        Color.a *= TexColor.a;
    }

    if( useToon ) {
        // �g�D�[���K�p
        float LightNormal = dot( IN.Normal, -LightDirection );
        Color *= tex2D( ObjToonSampler, float2(0.0f, 0.5f-LightNormal*0.5f) );
    }

    // �X�y�L�����K�p
    Color.rgb += Specular;

    return Color;
}

///////////////////////////////////////////////////////////////////////////////////////////////

// �I�u�W�F�N�g�`��p�e�N�j�b�N�i�A�N�Z�T���p�j
technique MainTec0 < string MMDPass = "object"; bool UseToon = false; string Script = MOB_LOOPSCRIPT_OBJECT; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 Basic_VS(false);
        PixelShader  = compile ps_3_0 Basic_PS(false);
    }
}

// �I�u�W�F�N�g�`��p�e�N�j�b�N�iPMX���f���p,�G�b�W�L��j
technique MainTec1 < string MMDPass = "object"; string Subset = MOB_EDGE_ON; bool UseToon = true;
                     string Script = MOB_LOOPSCRIPT_OBJECT_EDGE; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 Basic_VS(true);
        PixelShader  = compile ps_3_0 Basic_PS(true);
    }
    pass DrawEdge {
        CullMode = CW;
        VertexShader = compile vs_3_0 Edge_VS();
        PixelShader  = compile ps_3_0 Edge_PS();
    }
}

// �I�u�W�F�N�g�`��p�e�N�j�b�N�iPMX���f���p,�G�b�W�����j
technique MainTec2 < string MMDPass = "object"; bool UseToon = true; string Script = MOB_LOOPSCRIPT_OBJECT; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 Basic_VS(true);
        PixelShader  = compile ps_3_0 Basic_PS(true);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// �Z���t�V���h�E�pZ�l�v���b�g

struct VS_ZValuePlot_OUTPUT {
    float4 Pos          : POSITION;   // �ˉe�ϊ����W
    float4 ShadowMapTex : TEXCOORD0;  // Z�o�b�t�@�e�N�X�`��
};

// ���_�V�F�[�_
VS_ZValuePlot_OUTPUT ZValuePlot_VS( float4 Pos : POSITION, int vIndex : _INDEX )
{
    VS_ZValuePlot_OUTPUT Out = (VS_ZValuePlot_OUTPUT)0;

    // �탂�f���̃��[���h���W�ϊ�
    Pos = mul( Pos, WorldMatrix );

    // �������f���̔z�u���W�ϊ�
    Pos = MOB_TransformPosition(Pos, vIndex);

    // ���C�g�̖ڐ��ɂ��r���[�ˉe�ϊ�
    Out.Pos = mul( Pos, LightViewProjMatrix );

    // �e�N�X�`�����W�𒸓_�ɍ��킹��
    Out.ShadowMapTex = Out.Pos;

    return Out;
}

// �s�N�Z���V�F�[�_
float4 ZValuePlot_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
    // R�F������Z�l���L�^����
    return float4(ShadowMapTex.z/ShadowMapTex.w, 0, 0, 1);
}

// Z�l�v���b�g�p�e�N�j�b�N
technique ZplotTec < string MMDPass = "zplot"; string Script = MOB_LOOPSCRIPT_ZPLOT; >
{
    pass ZValuePlot {
        AlphaBlendEnable = FALSE;
        VertexShader = compile vs_3_0 ZValuePlot_VS();
        PixelShader  = compile ps_3_0 ZValuePlot_PS();
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// �I�u�W�F�N�g�`��i�Z���t�V���h�EON�j

// �V���h�E�o�b�t�@�̃T���v���B"register(s0)"�Ȃ̂�MMD��s0���g���Ă��邩��
sampler DefSampler : register(s0);

struct BufferShadow_OUTPUT {
    float4 Pos      : POSITION;   // �ˉe�ϊ����W
    float4 ZCalcTex : TEXCOORD0;  // Z�l
    float2 Tex      : TEXCOORD1;  // �e�N�X�`��
    float3 Normal   : TEXCOORD2;  // �@��
    float3 Eye      : TEXCOORD3;  // �J�����Ƃ̑��Έʒu
    float2 SpTex    : TEXCOORD4;  // �X�t�B�A�}�b�v�e�N�X�`�����W
    float4 Color    : COLOR0;     // �f�B�t���[�Y�F
};

// ���_�V�F�[�_
BufferShadow_OUTPUT BufferShadow_VS(float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD0, float2 Tex2 : TEXCOORD1, int vIndex : _INDEX, uniform bool useToon)
{
    BufferShadow_OUTPUT Out = (BufferShadow_OUTPUT)0;

    // �탂�f���̃��[���h���W�ϊ�
    Pos = mul( Pos, WorldMatrix );

    // ���[���h���W�ϊ��ɂ�钸�_�@��
    Normal = mul( Normal, (float3x3)WorldMatrix );

    // �z�u�ϊ���̍��W�Ɩ@�����擾
    MOB_TRANSFORM TrOut = MOB_TransformPositionNormal(Pos, Normal, vIndex);

    // �z�u�ϊ���̒��_�@��
    Out.Normal = TrOut.Normal;

    // �J�������_�̃r���[�ˉe�ϊ�
    Out.Pos = mul( TrOut.Pos, ViewProjMatrix );

    // �J�����Ƃ̑��Έʒu
    Out.Eye = CameraPosition - TrOut.Pos.xyz;

    // ���C�g���_�ɂ��r���[�ˉe�ϊ�
    Out.ZCalcTex = mul( TrOut.Pos, LightViewProjMatrix );

    // �f�B�t���[�Y�F�{�A���r�G���g�F �v�Z
    Out.Color.rgb = AmbientColor;
    if ( !useToon ) {
        Out.Color.rgb += max(0, dot( Out.Normal, -LightDirection )) * DiffuseColor.rgb;
    }
    Out.Color.a = DiffuseColor.a;
    Out.Color = saturate( Out.Color );

    // �e�N�X�`�����W
    Out.Tex = Tex;

    if( use_spheremap ) {
        if( use_subtexture ) {
            // PMX�T�u�e�N�X�`�����W
            Out.SpTex = Tex2;
        } else {
            // �X�t�B�A�}�b�v�e�N�X�`�����W
            float2 NormalWV = mul( Out.Normal, (float3x3)ViewMatrix ).xy;
            Out.SpTex.x = NormalWV.x * 0.5f + 0.5f;
            Out.SpTex.y = NormalWV.y * -0.5f + 0.5f;
        }
    }

    return Out;
}

// �s�N�Z���V�F�[�_
float4 BufferShadow_PS(BufferShadow_OUTPUT IN, uniform bool useToon) : COLOR
{
    // �X�y�L�����F�v�Z
    float3 HalfVector = normalize( normalize(IN.Eye) + -LightDirection );
    float3 Specular = pow( max(0, dot( HalfVector, normalize(IN.Normal) )), SpecularPower ) * SpecularColor;

    float4 Color = IN.Color;
    float4 ShadowColor = float4(saturate(AmbientColor), Color.a);  // �e�̐F
    if( use_texture ) {
        // �e�N�X�`���K�p
        float4 TexColor = tex2D( ObjTexSampler, IN.Tex );
        // �e�N�X�`���ގ����[�t��
        TexColor.rgb = lerp(1, TexColor * TextureMulValue + TextureAddValue, TextureMulValue.a + TextureAddValue.a).rgb;
        Color *= TexColor;
        ShadowColor *= TexColor;
        // ���l���ȉ��̃e�N�X�`�����ߕ��͕\�����Ȃ�
        clip(Color.a - AlphaThreshold);
    }
    if( use_spheremap ) {
        // �X�t�B�A�}�b�v�K�p
        float4 TexColor = tex2D(ObjSphareSampler,IN.SpTex);
        // �X�t�B�A�e�N�X�`���ގ����[�t��
        TexColor.rgb = lerp(spadd?0:1, TexColor * SphereMulValue + SphereAddValue, SphereMulValue.a + SphereAddValue.a).rgb;
        if(spadd) {
            Color.rgb += TexColor.rgb;
            ShadowColor.rgb += TexColor.rgb;
        } else {
            Color.rgb *= TexColor.rgb;
            ShadowColor.rgb *= TexColor.rgb;
        }
        Color.a *= TexColor.a;
        ShadowColor.a *= TexColor.a;
    }
    // �X�y�L�����K�p
    Color.rgb += Specular;

    // �e�N�X�`�����W�ɕϊ�
    IN.ZCalcTex /= IN.ZCalcTex.w;
    float2 TransTexCoord;
    TransTexCoord.x = (1.0f + IN.ZCalcTex.x)*0.5f;
    TransTexCoord.y = (1.0f - IN.ZCalcTex.y)*0.5f;

    if( any( saturate(TransTexCoord) - TransTexCoord ) ) {
        // �V���h�E�o�b�t�@�O
        return Color;
    } else {
        float comp;
        if(parthf) {
            // �Z���t�V���h�E mode2
            comp=1-saturate(max(IN.ZCalcTex.z-tex2D(DefSampler,TransTexCoord).r , 0.0f)*SKII2*TransTexCoord.y-0.3f);
        } else {
            // �Z���t�V���h�E mode1
            comp=1-saturate(max(IN.ZCalcTex.z-tex2D(DefSampler,TransTexCoord).r , 0.0f)*SKII1-0.3f);
        }
        if ( useToon ) {
            // �g�D�[���K�p
            comp = min(saturate(dot(IN.Normal,-LightDirection)*Toon),comp);
            ShadowColor.rgb *= MaterialToon;
        }
        
        float4 ans = lerp(ShadowColor, Color, comp);
        if( transp ) ans.a = 0.5f;
        return ans;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

// �I�u�W�F�N�g�`��p�e�N�j�b�N�i�A�N�Z�T���p�j
technique MainTecBS0  < string MMDPass = "object_ss"; bool UseToon = false; string Script = MOB_LOOPSCRIPT_OBJECT; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 BufferShadow_VS(false);
        PixelShader  = compile ps_3_0 BufferShadow_PS(false);
    }
}

// �I�u�W�F�N�g�`��p�e�N�j�b�N�iPMX���f���p,�G�b�W�L��j
technique MainTecBS1  < string MMDPass = "object_ss"; string Subset = MOB_EDGE_ON; bool UseToon = true;
                        string Script = MOB_LOOPSCRIPT_OBJECT_EDGE; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 BufferShadow_VS(true);
        PixelShader  = compile ps_3_0 BufferShadow_PS(true);
    }
    pass DrawEdge {
        CullMode = CW;
        VertexShader = compile vs_3_0 Edge_VS();
        PixelShader  = compile ps_3_0 Edge_PS();
    }
}

// �I�u�W�F�N�g�`��p�e�N�j�b�N�iPMX���f���p,�G�b�W�����j
technique MainTecBS2  < string MMDPass = "object_ss"; bool UseToon = true; string Script = MOB_LOOPSCRIPT_OBJECT; >
{
    pass DrawObject {
        VertexShader = compile vs_3_0 BufferShadow_VS(true);
        PixelShader  = compile ps_3_0 BufferShadow_PS(true);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////

