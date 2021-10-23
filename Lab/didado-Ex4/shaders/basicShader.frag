 #version 330 core

 in vec3  fvertex;
 in vec3  fnormal;
 in vec3  fmatamb;
 in vec3  fmatdiff;
 in vec3  fmatspec;
 in float fmatshin;

 uniform vec3 posFocus;
 uniform vec3 colFocus;

 uniform mat4 TG;
 uniform mat4 Proj;
 uniform mat4 View;

 out vec4 FragColor;

 uniform vec3 colFocus1;
 uniform vec3 colFocus2;
 uniform vec3 colFocus3;

 uniform vec3 posFocus1;
 uniform vec3 posFocus2;
 uniform vec3 posFocus3;

 uniform int b;

 vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

 vec3 Ambient() {
   return llumAmbient*fmatamb;
 }


 vec3 Lambert (vec3 NormSCO, vec3 L, vec3 color)
 {
   // Fixeu-vos que SOLS es retorna el terme de Lambert!
   // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
   vec3 colRes = vec3(0);
   if (dot (L, NormSCO) > 0)
     colRes = color * fmatdiff * dot (L, NormSCO);
   return (colRes);
 }

 vec3 Phong (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 color)
 {
   // Fixeu-vos que SOLS es retorna el terme especular!
   // Assumim que els vectors estan normalitzats
   vec3 colRes = vec3 (0);
   // Si la llum ve de darrera o el material és mate no fem res
   if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
     return colRes;  // no hi ha component especular

   vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
   vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

   if (dot(R, V) < 0)
     return colRes;  // no hi ha component especular

   float shine = pow(max(0.0, dot(R, V)), fmatshin);
   return (colRes + fmatspec * color * shine);
 }

 void main()
 {
   vec3 color;
   if (b == 0){
     vec3 L     = normalize(posFocus - fvertex);
     color = Ambient() +
                 Lambert(normalize(fnormal), L, colFocus) +
                 Phong (normalize(fnormal), L, fvertex, colFocus);
   }
   else {
     color = Ambient() +
            Lambert(normalize(fnormal), normalize(posFocus1 - fvertex), colFocus1) +
            Lambert(normalize(fnormal), normalize(posFocus2 - fvertex), colFocus2) +
            Lambert(normalize(fnormal), normalize(posFocus3 - fvertex), colFocus3) +
            Phong(normalize(fnormal), normalize(posFocus1 - fvertex), normalize(fvertex), colFocus1) +
            Phong(normalize(fnormal), normalize(posFocus2 - fvertex), normalize(fvertex), colFocus2) +
            Phong(normalize(fnormal), normalize(posFocus3 - fvertex), normalize(fvertex), colFocus3);

   }

   FragColor = vec4(color, 1);
 }
