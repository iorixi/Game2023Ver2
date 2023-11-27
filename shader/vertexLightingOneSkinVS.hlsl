#include "common.hlsl"

void main(in VSONESKIN_IN In, out PS_IN Out)
{
	// ワンスキン頂点ブレンドの処理
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
		// 重みを計算しながら行列生成
        comb += BoneMatrix[In.BoneIndex[i]] * In.BoneWeight[i];
    }

    float4 Pos;    
    Pos = mul(comb,In.Position);
    In.Position = Pos;

    // 法線ベクトルを補正
    float4 Normal;
    Normal = float4(In.Normal.xyz, 0.0);
    
    comb._41 = 0.0f;                            // 移動成分を消す
    comb._42 = 0.0f;
    comb._43 = 0.0f;
    comb._44 = 1.0f;

    Normal = mul(comb, In.Normal);
    normalize(Normal);
    In.Normal = Normal;
    
	//　ワールド変換行列を反映させる
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
	
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);

    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);

    float light = -dot(Light.Direction.xyz, worldNormal.xyz) * 0.5 + 0.5;
    light = saturate(light);

    Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
    Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse += Material.Emission;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
}
