// "startFade" must be less than "radius"

uniform float radius;
uniform float startFade;
uniform vec2 currentPos;
uniform vec4 centerColor;

void main() {
    gl_FragColor = centerColor;

    float distance = sqrt(pow(currentPos.x - gl_FragCoord.x, 2.0) +
        pow(currentPos.y - gl_FragCoord.y, 2.0));

    if ( distance >= radius ) {
        gl_FragColor = vec4( 0.0 , 0.0 , 0.0 , gl_FragColor.a );
    }
    else if ( distance > startFade ) { // if outside of range, start gradienting color to black
        float factor = ( 1.0 - ( distance - startFade ) / ( radius - startFade ) );
        gl_FragColor = vec4( factor * vec3( gl_FragColor ) , gl_FragColor.a );
    }
}
