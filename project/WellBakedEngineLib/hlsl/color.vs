cbuffer MatrixBuffer
{
	matrix mWorldMatrix;
	matrix mViewMatrix;
	matrix mProjectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VertexOutputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexOutputType ColorVertexShader(VertexInputType input)
{
	VertexOutputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, mWorldMatrix);
	output.position = mul(output.position, mViewMatrix);
	output.position = mul(output.position, mProjectionMatrix);
	output.color = input.color;

	return output;
}