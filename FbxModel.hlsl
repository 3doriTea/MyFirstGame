// REF: https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics

//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    //float4x4 texMatrix; // テクスチャマトリクス
    //float4x4 matW;
    float4x4 matRotateW;
    //float4 lightDir; // ライトの向き
    //float4 lightColor; // ライトの向き
    //float ambientValue; // 環境光の量
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
		// セマンティクス 
    float4 pos : SV_POSITION; // 位置
    float4 uv : TEXCOORD; // UV座標
    float4 color : COLOR; // 色 (明るさ)
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float4 uv : TEXCOORD
)  // 2個ポジションがあれば POSITION0 POSITION1 とか
	//float4 normal : NORMAL,
	//float4 uv : TEXCOORD
{ // メモリの都合でflaot4で受け取っている
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    //outData.uv = mul(uv, texMatrix);
	
    //float4 light = normalize(lightDir);
	
    //normal = mul(normal, matRotateW);
    //normal.w = 0;
	//normal = float4(0, 0, -1, 0);
	//normal = float4(0, 0, 1, 0);
    //outData.color = saturate(dot(normal, light));
    //outData.color = saturate(dot(normal, light));
	//outData.color = normal;
	
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    return float4(0, 0, 1, 1);
    
    float4 textureColor = g_texture.Sample(g_sampler, inData.uv.xy);
    //float4 ambient = textureColor * float4(ambientValue, ambientValue, ambientValue, 1);
    float4 diffuse = textureColor * inData.color;
	
    //diffuse = saturate(diffuse * (lightColor + float4(1, 1, 1, 1)));
    diffuse = saturate(diffuse);
    float4 color = diffuse; // + ; //ambient;
}
