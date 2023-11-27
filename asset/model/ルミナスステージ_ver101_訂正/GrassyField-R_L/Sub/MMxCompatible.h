#ifndef	__MIKU_MIKU_DANCE_AND_MOVING_COMPATIBLE_HEADER__
#define	__MIKU_MIKU_DANCE_AND_MOVING_COMPATIBLE_HEADER__

#ifdef	MIKUMIKUMOVING

#define	MM_INPUT	MMM_SKINNING_INPUT
#define	MM_LOCAL	MMM_SKINNING_OUTPUT
#define	MMTransform( IN )	MMM_SkinnedPositionNormal( IN.Pos, IN.Normal, IN.BlendWeight, IN.BlendIndices, IN.SdefC, IN.SdefR0, IN.SdefR1 );

#else

struct MM_INPUT {
	float4	Pos				: POSITION;		//	���_�ʒu
	float3	Normal			: NORMAL;		//	�@��
	float2	Tex				: TEXCOORD0;	//	UV
	float4	AddUV1			: TEXCOORD1;	//	�ǉ�UV1
	float4	AddUV2			: TEXCOORD2;	//	�ǉ�UV2
	float4	AddUV3			: TEXCOORD3;	//	�ǉ�UV3
	float4	AddUV4			: TEXCOORD4;	//	�ǉ�UV4
	float	Index			: PSIZE15;		//	���_�C���f�b�N�X�l �iVertexIndexOffset���Q�Ƃ̂��Ɓj
};


struct MM_LOCAL {
	float4	Position;
	float3	Normal;
};

MM_LOCAL	MMTransform( MM_INPUT IN ) {
	MM_LOCAL	pn;
	pn.Position	= IN.Pos;
	pn.Normal	= IN.Normal;
	return	pn;
}

#endif	//	MIKUMIKUMOVING

#endif	//	__MIKU_MIKU_DANCE_AND_MOVING_COMPATIBLE_HEADER__

