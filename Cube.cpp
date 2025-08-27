#include "Cube.h"
#include <cassert>
#include <string>


using namespace DirectX;

Cube::Cube()
{
}

Cube::~Cube()
{
}

HRESULT Cube::Initialize()
{
	return Quad::Initialize("Dice.png");
}

void Cube::Draw(const XMMATRIX& worldMatrix)
{
	static const int QUADS_COUNT{ 6 };
	QuadData quads[QUADS_COUNT]
	{
		QuadData{ { 0, 0, 1 },  { 0, XM_PI, 0 },          { 1.0f / 4.0f * 0, 1.0f / 2.0f * 0 } },  // ����
		QuadData{ { 0, 0, -1 }, { 0, 0, 0 },              { 1.0f / 4.0f * 2, 1.0f / 2.0f * 0 } },  // ��
		QuadData{ { 1, 0, 0 },  { 0, -XM_PI / 2.0f, 0 },  { 1.0f / 4.0f * 1, 1.0f / 2.0f * 0 } },  // ��
		QuadData{ { -1, 0, 0 }, { 0, XM_PI / 2.0f, 0 },   { 1.0f / 4.0f * 3, 1.0f / 2.0f * 0 } },  // �E
		QuadData{ { 0, 1, 0 },  { XM_PI / 2.0f, 0, 0 },   { 1.0f / 4.0f * 0, 1.0f / 2.0f * 1 } },  // ��
		QuadData{ { 0, -1, 0 }, { -XM_PI / 2.0f, 0, 0 },  { 1.0f / 4.0f * 1, 1.0f / 2.0f * 1 } },  // ��
	};

	XMVECTOR moveV{};
	XMVECTOR rotateV{};
	for (int i = 0; i < QUADS_COUNT; i++)
	{
		moveV = XMLoadFloat3(&quads[i].position);
		rotateV = XMLoadFloat3(&quads[i].rotate);

		XMMATRIX worldMat
		{
			XMMatrixRotationX(quads[i].rotate.x)
			* XMMatrixRotationY(quads[i].rotate.y)
			* XMMatrixRotationZ(quads[i].rotate.z)
			* XMMatrixTranslation(quads[i].position.x, quads[i].position.y, quads[i].position.z)
			* worldMatrix
		};

		XMFLOAT2 scale{ 1.0f / 4.0f, 1.0f / 2.0f };

		XMMATRIX uvMat
		{
			/*XMMatrixAffineTransformation(
				XMLoadFloat2(&scale),
				XMVectorZero(),
				XMVectorZero(),
				XMLoadFloat2(&quads[i].uv))*/
			/*XMMatrixScaling(1.0f / 4.0f, 1.0f / 2.0f, 1.0f)
			* XMMatrixTranslation(quads[i].uv.x, quads[i].uv.y, 0.0f)*/

			XMMatrixTransformation2D(
				XMVectorZero(),  // �X�P�[�����O�̒��S
				0,  // ��]�W��
				XMLoadFloat2(&scale),  // x����y���̔{�����܂�2D�x�N�^�[ //XMLoadFloat2(&scale),
				XMVectorZero(),
				0.0f,
				//XMVectorZero())
				XMLoadFloat2(&quads[i].uv))
		};

		char buffer[1024]{};
		sprintf_s<1024>(buffer, "(%f, %f)\n", quads[i].uv.x, quads[i].uv.y);
		std::string str{ buffer };
		OutputDebugString(std::wstring{ str.begin(), str.end() }.c_str());

		/*uvMat.r[2].m128_f32[0] = quads[i].uv.x;
		uvMat.r[2].m128_f32[1] = quads[i].uv.y;*/

		Quad::Draw(worldMat, uvMat);
	}
}

void Cube::Draw(const Transform& _transform)
{
	static const int QUADS_COUNT{ 6 };
	QuadData quads[QUADS_COUNT]
	{
		QuadData{ { 0, 0, 1 },  { 0, XM_PI, 0 },          { 1.0f / 4.0f * 0, 1.0f / 2.0f * 0 } },  // ����
		QuadData{ { 0, 0, -1 }, { 0, 0, 0 },              { 1.0f / 4.0f * 2, 1.0f / 2.0f * 0 } },  // ��
		QuadData{ { 1, 0, 0 },  { 0, -XM_PI / 2.0f, 0 },  { 1.0f / 4.0f * 1, 1.0f / 2.0f * 0 } },  // ��
		QuadData{ { -1, 0, 0 }, { 0, XM_PI / 2.0f, 0 },   { 1.0f / 4.0f * 3, 1.0f / 2.0f * 0 } },  // �E
		QuadData{ { 0, 1, 0 },  { XM_PI / 2.0f, 0, 0 },   { 1.0f / 4.0f * 0, 1.0f / 2.0f * 1 } },  // ��
		QuadData{ { 0, -1, 0 }, { -XM_PI / 2.0f, 0, 0 },  { 1.0f / 4.0f * 1, 1.0f / 2.0f * 1 } },  // ��
	};

	XMVECTOR moveV{};
	XMVECTOR rotateV{};
	for (int i = 0; i < QUADS_COUNT; i++)
	{
		moveV = XMLoadFloat3(&quads[i].position);
		rotateV = XMLoadFloat3(&quads[i].rotate);

		XMMATRIX worldMat
		{
			XMMatrixRotationX(quads[i].rotate.x)
			* XMMatrixRotationY(quads[i].rotate.y)
			* XMMatrixRotationZ(quads[i].rotate.z)
			* XMMatrixTranslation(quads[i].position.x, quads[i].position.y, quads[i].position.z)
			* _transform.GetWorldMatrix()
		};

		XMFLOAT2 scale{ 1.0f / 4.0f, 1.0f / 2.0f };

		XMMATRIX uvMat
		{
			/*XMMatrixAffineTransformation(
				XMLoadFloat2(&scale),
				XMVectorZero(),
				XMVectorZero(),
				XMLoadFloat2(&quads[i].uv))*/
				/*XMMatrixScaling(1.0f / 4.0f, 1.0f / 2.0f, 1.0f)
				* XMMatrixTranslation(quads[i].uv.x, quads[i].uv.y, 0.0f)*/

				XMMatrixTransformation2D(
					XMVectorZero(),  // �X�P�[�����O�̒��S
					0,  // ��]�W��
					XMLoadFloat2(&scale),  // x����y���̔{�����܂�2D�x�N�^�[ //XMLoadFloat2(&scale),
					XMVectorZero(),
					0.0f,
					//XMVectorZero())
					XMLoadFloat2(&quads[i].uv))
		};

		char buffer[1024]{};
		sprintf_s<1024>(buffer, "(%f, %f)\n", quads[i].uv.x, quads[i].uv.y);
		std::string str{ buffer };
		OutputDebugString(std::wstring{ str.begin(), str.end() }.c_str());

		/*uvMat.r[2].m128_f32[0] = quads[i].uv.x;
		uvMat.r[2].m128_f32[1] = quads[i].uv.y;*/

		Quad::Draw(worldMat, uvMat);
	}
}
