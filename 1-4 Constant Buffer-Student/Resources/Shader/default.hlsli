
//DECLARE  a cbuffer called TEST_B0 mapped to register b0, it is similar to a stuct. Add a variable called offset0 of type float4
cbuffer TEST_B0
{
    float4 offset0 : b0;
};

//DECLARE a cbuffer called TEST_B1 mapped to register b1, it is similar to a stuct. Add a variable called offset1 of type float4
cbuffer TEST_B0
{
    float4 offset1 : b1;
};

struct VS_IN
{
     //DECLARE a float3 variable called pos which is a mapping to :POSITION
    float3 pos : POSITION;
     //DECLARE a float4 variable called color which is mapping to :COLOR
    float4 color : COLOR;
};

struct VS_OUT
{
     //DECLARE a float4 variable called pos which is mapping to SV_Position (SV is System Value)
    float4 pos : SV_Position;
     //DECLARE a float4 variable called color which is mapping to :COLOR
    float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN input)
{
    //DECLARE a variable called output of type VS_OUT and initialize it to 0
    VS_OUT output = (VS_OUT)0;
    //SET output.pos to a new float4 initialized to input.pos, 1.f
    output.pos = float4(input.pos, 1.f);

	//INCREMENT output.pos by offset0
    output.pos += offset0;
    
	//SET output.color to input.color
    output.color = input.color;

	//INCREMENT output.color by offset1
    output.color += offset1;
    

	//RETURN output
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    //RETURN input.color
    return input.color;
}