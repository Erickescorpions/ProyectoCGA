#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_uv;

out vec2 our_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float scaleUV;  // Escala en Y
uniform float offsetY;   // Desplazamiento en Y para el estado actual

void main(){
	gl_Position = projection * view * model * vec4(in_position, 1.0);

	// Si scaleUV es cero, no modificamos las coordenadas UV
	if (scaleUV == 0.0) {
		our_uv = in_uv;  // Usamos las coordenadas UV tal cual están
	} else {
		// Escalamos verticalmente la textura para mostrar solo una parte
		our_uv = in_uv;  // Usamos las coordenadas UV originales
		our_uv.y = our_uv.y * scaleUV + offsetY;  // Ajustamos en el eje Y
	}

	// Invertimos el eje Y si es necesario
	our_uv.y = 1.0 - our_uv.y;
}
