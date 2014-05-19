varying vec3 normal, lightDir, eyeDir;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeDir = -vVertex;

	gl_Position = ftransform();		
	//gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix * gl_Vertex;
}