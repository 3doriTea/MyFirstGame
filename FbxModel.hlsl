// REF: https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics

//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    //float4x4 texMatrix; // �e�N�X�`���}�g���N�X
    //float4x4 matW;
    float4x4 matRotateW;
    //float4 lightDir; // ���C�g�̌���
    //float4 lightColor; // ���C�g�̌���
    //float ambientValue; // �����̗�
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
		// �Z�}���e�B�N�X 
    float4 pos : SV_POSITION; // �ʒu
    float4 uv : TEXCOORD; // UV���W
    float4 color : COLOR; // �F (���邳)
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float4 uv : TEXCOORD
)  // 2�|�W�V����������� POSITION0 POSITION1 �Ƃ�
	//float4 normal : NORMAL,
	//float4 uv : TEXCOORD
{ // �������̓s����flaot4�Ŏ󂯎���Ă���
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
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
	
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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
