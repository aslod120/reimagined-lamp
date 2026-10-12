#version 330

// Input fragment attributes (from fragment shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

bool CheckCollisionPointTriangle(vec2 point, vec2 p1, vec2 p2, vec2 p3);
uniform vec2 playerPos;
uniform vec2 triangle[3];

void main()
{

    // Get the position of the current fragment (screen coordinates!)
    vec2 pos = vec2(gl_FragCoord.x, gl_FragCoord.y);
    float alpha = 1.0;
    // dtp (distance to player) stores the distance between the player and the pixel
    float dtp = distance(playerPos, pos);
    float radius = 30; // radius surrounding the player
    
    if(CheckCollisionPointTriangle(pos, triangle[0], triangle[1], triangle[2]) == true || (dtp <= radius) == true)
    {
        alpha = 0.0;
    }
    else
    {
        alpha = 1.0;
    }

    
    finalColor = vec4(0, 0, 0, alpha);
}

bool CheckCollisionPointTriangle(vec2 point, vec2 p1, vec2 p2, vec2 p3)
{
    bool collision = false;

    float alpha = ((p2.y - p3.y)*(point.x - p3.x) + (p3.x - p2.x)*(point.y - p3.y)) /
                  ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));

    float beta = ((p3.y - p1.y)*(point.x - p3.x) + (p1.x - p3.x)*(point.y - p3.y)) /
                 ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));

    float gamma = 1.0f - alpha - beta;

    if ((alpha > 0) && (beta > 0) && (gamma > 0)) collision = true;

    return collision;
}