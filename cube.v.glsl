attribute vec3 v_coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


 
void main(void)
{
  gl_Position = projection*view*model * vec4( v_coord,1.0);
}

