varying vec2 f_texcoord;
varying vec4 color;
uniform sampler2D mytexture;
 
void main(void) {
  gl_FragColor = color * texture2D(mytexture, f_texcoord);
}