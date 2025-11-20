/*
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
    float4x4 matRotateW;
    float4x4 texMatrix; // テクスチャマトリクス
    float4x4 matW;
    float4 diffuse; // ディフューズ
    float4 lightDir; // ライトの向き
    float4 lightColor; // ライトの向き
    float ambientValue; // 環境光の量
    bool materialFlag; // テクスチャ画像が適用されているか
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
{ // メモリの都合でflaot4で受け取っている
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv; //;mul(uv, texMatrix);
	
    float4 light = normalize(lightDir);
	
    normal = mul(normal, matRotateW);
    normal.w = 0;
	//normal = float4(0, 0, -1, 0);
	//normal = float4(0, 0, 1, 0);
    outData.color = saturate(dot(normal, light));
	//outData.color = normal;
	
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //return float4(1, 1, 0, 1);
    float4 textureColor = g_texture.Sample(g_sampler, inData.uv.xy);
    float4 diffuse = textureColor;
    float4 ambient = float4(ambientValue, ambientValue, ambientValue, 1);
	
    diffuse = saturate(diffuse * (lightColor + float4(1, 1, 1, 1)));
    float4 color = diffuse * inData.color + textureColor * ambient;
    //color.a = fwidth(color.a); // MEMO: お遊び abs(ddx(color)) + abs(ddy(color))
    //float4 d = ddx(color) * 1;
    //color += float4(d.xyz, 1);
    //return g_texture.Sample(g_sampler, inData.uv.xy);
    return color;

}
*/


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
    //float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    //float4x4 matNormal; // ワールド行列
    ////float4x4 matWorld; // ワールド行列
    //float4 diffuse;
    //bool materialFlag;
    
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matNormal;
    float4x4 texMatrix; // テクスチャマトリクス
    float4x4 matW;
    float4 diffuseColor; // ディフューズ
    float4 lightDir; // ライトの向き
    float4 lightColor; // ライトの向き
    float ambientValue; // 環境光の量
    bool materialFlag; // テクスチャ画像が適用されているか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
                 //セマンティクス
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 normal : NORMAL, float4 uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy; //UV座標はそのまま

    normal = mul(normal, matNormal); //法線ベクトルをワールド・ビュー・プロジェクション行列で変換
    normal = normalize(normal); //法線ベクトルを正規化=長さ1に)
    normal.w = 0; //w成分は0にする
    float4 light = normalize(lightDir);
    outData.color = dot(normal, light);

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 diffuse;
    if (materialFlag)
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv); //テクスチャーから色を取得
    }
    else
    {
        diffuse = saturate(diffuseColor);
    }
    //return float4(1, 1, 0, 1);
    //float4 color = g_texture.Sample(g_sampler, inData.uv); // * inData.color; //テクスチャーから色を取得
    //float4 ret = float4(inData.uv.x, inData.uv.y, 0, 1);
    return diffuse * inData.color + diffuse * ambientValue;

}
