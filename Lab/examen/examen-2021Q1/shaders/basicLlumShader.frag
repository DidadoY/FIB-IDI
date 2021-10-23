#version 330 core

in vec4 vertexSCO;
in vec3 normalSCO;
in vec3 fmatamb;
in vec3 fmatdiff;
in vec3 fmatspec;
in float fmatshin;

out vec4 FragColor;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(0, 0, 0);

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 colFocus;
uniform vec3 colFocus1;
uniform vec3 colFocus2;
uniform vec3 colFocus3;

uniform vec3 PosFocus1;
uniform vec3 PosFocus2;
uniform vec3 PosFocus3;
uniform int b;

void Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Lambert (vec3 NormSCO, vec3 L, vec3 color)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * fmatamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + color * fmatdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 color)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (fmatshin == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + fmatspec * color * shine);
}

void main()
{
  printf("Hola");
  if (b == 0) color = Phong(normalize(normalSCO), normalize(posFocus - vertexSCO.xyz), vertexSCO);
  else {
    color = Ambient() +
    Lambert(normalize(fnormal), normalize(posFocus1 - vertexSCO.xyz), colFocus1) +
    Lambert(normalize(fnormal), normalize(posFocus2 - vertexSCO.xyz), colFocus2) +
    Lambert(normalize(fnormal), normalize(posFocus3 - vertexSCO.xyz), colFocus3) +
    Phong(normalize(fnormal), normalize(posFocus1 - vertexSCO.xyz), normalize(fvertex), colFocus1) +
    Phong(normalize(fnormal), normalize(posFocus1 - vertexSCO.xyz), normalize(fvertex), colFocus2) +
    Phong(normalize(fnormal), normalize(posFocus1 - vertexSCO.xyz), normalize(fvertex), colFocus3);

  }

  FragColor = vec4(color, 1);
}
