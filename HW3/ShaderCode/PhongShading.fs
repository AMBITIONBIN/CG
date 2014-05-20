varying vec3 normal, lightDir, eyeDir;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D colorTexture;

void main (void)
{
	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
							
	vec3 N = normalize(texture2D(normalTexture, gl_TexCoord[0].xy).rgb * 2.0 - 1.0);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
		               texture2D(diffuseTexture, gl_TexCoord[0].xy).rgba *
					   lambertTerm;	
		
		vec3 E = normalize(eyeDir);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * 
		               texture2D(colorTexture, gl_TexCoord[0].xy).rgba * 
					   specular;	
	}

	gl_FragColor = final_color;
    //gl_FragColor = texture2D(colorTexture, gl_TexCoord[0].xy).rgba;
}
