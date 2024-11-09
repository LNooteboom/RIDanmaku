/* vertex attributes go here to input to the vertex shader */
struct vs_in
{
    float3 position : POS;
    float3 normal : NORM;
    float2 uv : UV;
    float4 color : COL;
};

struct vs_anim_in
{
    float3 position : POS;
    float3 normal : NORM;
    float2 uv : UV;
    float4 weigths : WEIGHTS;
    uint4 bones : BONES;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out
{
    float4 projPos : SV_POSITION; // required output of VS
    float3 viewPos : POS;
    float3 normal : NORM;
    float2 uv : UV;
    float4 color : COL;
};

cbuffer vs_constants : register(b0)
{
    column_major float4x4 model_view;
    column_major float4x4 projection;
    column_major float4x4 norm;
    
    float4 args1;
    float4 args2;
}

#define MAX_BONE 32
cbuffer vs_anim_constants : register(b1)
{
    float4x4 final_bone_matrices[MAX_BONE];
}


vs_out stdvs_main(vs_in input)
{
    vs_out output = (vs_out) 0; // zero the memory first
    float4 pos = mul(model_view, float4(input.position, 1.0));
    output.projPos = mul(projection, pos);
    output.viewPos = pos.xyz;
    output.normal = mul(norm, float4(input.normal, 1.0)).xyz;
    output.uv = input.uv;
    output.color = input.color;

    return output;
}

void do_bone(inout float4 total_position, inout float3 total_normal, float3 pos, float3 norm, uint bone_id, float weight)
{
    if (bone_id < MAX_BONE)
    {
        total_position += mul(final_bone_matrices[bone_id], float4(pos, 1)) * weight;
        total_normal += mul((float3x3) final_bone_matrices[bone_id], norm) * weight;
    }
}

vs_out stdvs_anim_main(vs_anim_in input)
{
    float4 total_position = float4(0, 0, 0, 0);
    float3 total_normal = float3(0, 0, 0);
    do_bone(total_position, total_normal, input.position, input.normal, input.bones.x, input.weigths.x);
    do_bone(total_position, total_normal, input.position, input.normal, input.bones.y, input.weigths.y);
    do_bone(total_position, total_normal, input.position, input.normal, input.bones.z, input.weigths.z);
    do_bone(total_position, total_normal, input.position, input.normal, input.bones.w, input.weigths.w);
    
    vs_out output = (vs_out) 0; // zero the memory first
    float4 pos = mul(model_view, float4(total_position.xyz, 1.0));
    output.projPos = mul(projection, pos);
    output.viewPos = pos.xyz;
    output.normal = mul(norm, float4(total_normal, 1.0)).xyz;
    output.uv = input.uv;
    output.color = float4(0.5, 0.5, 0.5, 1);

    return output;
}

vs_out distort_main(vs_in input)
{
    vs_out output = (vs_out) 0; // zero the memory first
    
    // args1.xyz: rgb color
    // args1.w: radius
    // args2.xy: ripple phase (increment by (PI/16,PI/32) each frame)
    
    float2 ripple = args2.xy + ((input.uv.y + 17 * input.uv.x) * 17) * float2(3.14159 / 32.0, -3.14159 / 64.0);
    float3 vtx = input.position * (args1.w + 20);
    float size = args1.w * args1.w;
    float dist_from_center = vtx.x * vtx.x + vtx.y * vtx.y;
    float4 color = float4(1, 1, 1, 0);
    if (dist_from_center < size)
    {
        float bulge_base_multi = (size - dist_from_center) / size;
        color = color - float4(args1.xyz * bulge_base_multi, -1);
        float2 vec = 0;
        if (dist_from_center > 0.001f)
        {
            dist_from_center = sqrt(1.0f / dist_from_center);
            vec = vtx.xy * dist_from_center;
        }
        vtx += float3((32 * bulge_base_multi) * vec   +   (8 * bulge_base_multi) * sin(ripple), 0);
    }
    
    float4 pos = mul(model_view, float4(vtx, 1.0));
    output.projPos = mul(projection, pos);
    output.viewPos = pos.xyz;
    output.normal = mul(norm, float4(input.normal, 1.0)).xyz;
    output.uv = input.uv;
    output.color = color;

    return output;
}