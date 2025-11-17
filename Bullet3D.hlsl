// REF: https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics

//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0);     // テクスチャー
SamplerState g_sampler : register(s0);  // サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 texMatrix; // テクスチャマトリクス
    float4x4 matW;
    float4x4 matRotateW;
    float4 lightDir; // ライトの向き
    float4 lightColor; // ライトの向き
    float ambientValue; // 環境光の量
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
    outData.uv = mul(uv, texMatrix);
    
    //まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    return g_texture.Sample(g_sampler, inData.uv.xy);
}
