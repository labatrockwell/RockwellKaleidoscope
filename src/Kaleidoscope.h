/*
 *  Kaleidoscope.h
 *  kaleidoscope
 *
 *  Created by Brett Renfer on 3/2/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 */

class Kaleidoscope
{
    public :
    
    float gDrawWidth, gDrawHeight;
    float count; /// point speed of rotation
    float spinStep; // speed of the step of each rotation///
    float triangleCameraGrab;
    int imgLifeCap;
    float ksx;
    float ksy;
    bool predatorMode = false;
    
    float  u1, v1, u2, v2, u3, v3;
    float  u4, v4, u5, v5, u6, v6;
    float vw, vh;
    
    ofFbo mFbo;
    
    //texture coords
    ofPoint texCoord1;
    ofPoint texCoord2;
    ofPoint texCoord3;
    
    ofPoint centerPoint;
    ofPoint centerPoint2;
    
    bool bDrawDebug;
    bool bDragging[2];
    
    float radius[2];
    float offSet[2];
    
    ofShader shader;
    
    Kaleidoscope(){};
    
    void setup( float _gDrawWidth, float _gDrawHeight, float _spinStep){
        count= 0.0f; /// point speed of rotation
        spinStep = _spinStep; // speed of the step of each rotation///
        triangleCameraGrab = 640;
        imgLifeCap 	= 900;
        
        ksx=400;//150;
        ksy=400;//150;
        
        gDrawWidth = _gDrawWidth;
        gDrawHeight = _gDrawHeight;
        bDrawDebug = false;
        centerPoint.x = 0.0;
        centerPoint.y = 0.0;
        centerPoint2.x = 0.0;
        centerPoint2.y = 0.0;
        
        std::cout << "Width: " << gDrawWidth << " || Height: " << gDrawHeight << endl;
        
        shader.load("sinWave.vert", "predator.frag");
        
        for (int i=0; i<2; i++){
            radius[i] = 0;
            offSet[i] = 0;
        }        
        //construct FBO with texture width and height
        mFbo.allocate(640, 480, GL_RGBA);

    };
    
    void toggeleDrawDebug(){
        bDrawDebug = !bDrawDebug;
    }
    
    void togglePredator(){
        predatorMode = !predatorMode;
    }
    
    void update(ofTexture& textureImg, float _waveAmp, float _kaleidoscopeSpeed, float _brightness, float _contrast ){
        
        if (spinStep > count){
            spinStep = _kaleidoscopeSpeed;
        } else {
            spinStep = -_kaleidoscopeSpeed;
        }
        
        // this controls the speed of triangle rotation
        count  -= spinStep;
        // make decrement variable...
        
        //fbo begin
        mFbo.begin();
        ofClear(255,255,255,255);
        
        
        shader.begin();
        
//      pass in texture
        shader.setUniformTexture("iChannel0", textureImg, 0.f);
        shader.setUniform3f("iResolution", ofVec3f( (float)gDrawWidth, (float)gDrawHeight, 0.f) );
        shader.setUniform1f("iGlobalTime", ofGetElapsedTimef() );
        shader.setUniform1f("waveAmplitude", _waveAmp);
        shader.setUniform1f("predatorMode", predatorMode);
        shader.setUniform1f("brightness", _brightness);
        shader.setUniform1f("contrast", _contrast);
        ofDrawRectangle(0,0, 640, 480);
        shader.end();
        mFbo.end();

        
        
        // these control the size of the camer grab/////////////////////////////////////////////////////////////////////////
        //tr1 = tr2 = tr3 = triangleCameraGrab;
        
        radius[0] = triangleCameraGrab - offSet[0];
        radius[1] = triangleCameraGrab - offSet[1];
        
        //		if (radius[0] > 350) radius[0] = 350;
        //		if (radius[1] > 350) radius[1] = 350;
        
        bDrawDebug = false;
    }
    
    void render(float _gDrawWidth, float _gDrawHeight, ofTexture& textureImg){
        gDrawWidth = _gDrawWidth;
        gDrawHeight = _gDrawHeight;
        
        // println("My texture is "+textureImg);
        float cc = 70;
        float ix,iy;
        
        /*int vw = gVideoWidth;
         int vh = gVideoHeight;*/
        
//        mFbo.draw(0,0);
        
        vw = textureImg.getWidth();
        vh = textureImg.getHeight();
        float vw10 = vw / 10.0f;
        float vh10 = vh / 10.0f;
        float vw2 = vw / 2.0f;
        float vh2 = vh / 2.0f;
        float mmm = 100000.0;
        
        //change direction of 'spinStep' when hitting maximum float value in 'count'
        //to keep kaleidoscope rotating
        if (count<-4194303. || count>4194303.) {
            spinStep=-spinStep;
        }
        
        //  vw2 = (int) ((float) (millis() % mmm) * (float) vw / mmm);
        int maxWidth = 640;//320;
        int maxHeight = 480;//240;
        u1 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+0)*radius[0]), maxWidth-centerPoint.x));
        v1 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+0)*radius[0]), maxHeight-centerPoint.y));
        u2 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[0]), maxWidth-centerPoint.x));
        v2 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[0]), maxHeight-centerPoint.y));
        u3 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxWidth-centerPoint.x));
        v3 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxHeight-centerPoint.y));
        //		cout<<u1<<" "<<v1<<" "<<u2<<" "<<v2<<" "<<u3<<" "<<v3<<endl;
        int numI = 2;
        
        
        
        

        for(iy=0;iy<(gDrawHeight/ksy)+1;) {
        
            for(int i = 0; i < numI; i++, iy++){
                
                
                mFbo.getTexture().bind(0);
                
                glBegin(GL_TRIANGLE_STRIP);
                //textureImg.blend(textureImg,0,0,640,480,0,0,640,480, ADD);
                glTexCoord2f(u2, v2);
                glVertex2f(ix * ksx- ksx/2, (iy+1)*ksy);
                for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
                    glTexCoord2f(u1, v1);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u3, v3);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                    ix++;
                    glTexCoord2f(u2,v2);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u1, v1);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                    
                    ix++;
                    glTexCoord2f(u3,v3);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u2, v2);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                }
                glEnd();
                
                mFbo.getTexture().unbind();
            }

        }
        
    }
    
    void render(float _gDrawWidth, float _gDrawHeight, ofImage textureImg) {
        render(_gDrawWidth, _gDrawHeight, textureImg.getTextureReference());
    }
    /*
     void render(float _gDrawWidth, float _gDrawHeight, ofImage textureImg, ofImage textureImg2) {
     
     gDrawWidth = _gDrawWidth;
     gDrawHeight = _gDrawHeight;
     
     // println("My texture is "+textureImg);
     float cc = 70;
     float ix,iy;
     
     vw = textureImg.width;
     vh = textureImg.height;
     
     int vw10 = vw / 10;
     int vh10 = vh / 10;
     float vw2 = vw / 2.0f;
     float vh2 = vh / 2.0f;
     float mmm = 100000.0;
     
     //  vw2 = (int) ((float) (millis() % mmm) * (float) vw / mmm);
     
     u1 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+0)*radius[0]), maxWidth-centerPoint.x));
     v1 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+0)*radius[0]), maxHeight-centerPoint.y));
     u2 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[0]), maxWidth-centerPoint.x));
     v2 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[0]), maxHeight-centerPoint.y));
     u3 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxWidth-centerPoint.x));
     v3 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxHeight-centerPoint.y));
     
     u4 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+0)*radius[1]), maxWidth-centerPoint2.x));
     v4 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+0)*radius[1]), maxHeight-centerPoint2.y));
     u5 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[1]), maxWidth-centerPoint2.x));
     v5 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[1]), maxHeight-centerPoint2.y));
     u6 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[1]), maxWidth-centerPoint2.x));
     v6 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[1]), maxHeight-centerPoint2.y));
     
     for(iy=0;iy<(gDrawHeight/ksy)+1;iy++) {
     
     ix=0;
     
     ofTexture& tex = textureImg.getTextureReference();
     ofTexture& tex2 = textureImg2.getTextureReference();
     
     
     //**********************************************************
     // ROW ONE
     //**********************************************************
     
     // 0 UP
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u3, v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     glTexCoord2f(u3,v3);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     }
     glEnd();
     tex.unbind();
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -2 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u4, v4);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u6,v6);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u5, v5);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     glTexCoord2f(u6, v6);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u5,v5);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u4, v4);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u4, v4);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u6,v6);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     glTexCoord2f(u5,v5);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     }
     glEnd();
     tex2.unbind();
     
     //**********************************************************
     // ROW TWO
     //**********************************************************
     
     iy++;
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u3,v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2, v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2,v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u3,v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     glTexCoord2f(u2,v2);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     }
     glEnd();
     tex.unbind();
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -1.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u4, v4);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u6, v6);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u5,v5);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u5,v5);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u4, v4);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     glTexCoord2f(u6,v6);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     
     glTexCoord2f(u5,v5);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u4, v4);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u6, v6);
     glVertex2f(ix * ksx, iy * ksy);
     }
     glEnd();
     tex2.unbind();
     
     }
     }
     */
    void renderDebug( ofImage textureImg){
        textureImg.draw(0,0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u1, v1);
        ofVertex(u2, v2);
        ofVertex(u3, v3);
        ofEndShape(true);
        ofPopStyle();
        
        bDrawDebug = true;
    }
    
    void renderDebug( ofImage textureImg, ofImage textureImg2 ){
        textureImg.draw(0,0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u1, v1);
        ofVertex(u2, v2);
        ofVertex(u3, v3);
        ofEndShape(true);
        ofPopStyle();
        
        textureImg2.draw(textureImg.getWidth(),0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u4 + vw, v4 );
        ofVertex(u5 + vw, v5 );
        ofVertex(u6 + vw, v6 );
        ofEndShape(true);
        ofPopStyle();
        
        bDrawDebug = true;
    }
    /*
     void renderAsChecked(float _gDrawWidth, float _gDrawHeight, ofImage textureImg, ofImage textureImg2) {
     
     gDrawWidth = _gDrawWidth;
     gDrawHeight = _gDrawHeight;
     
     // println("My texture is "+textureImg);
     float cc = 70;
     float ix,iy;
     
     vw = textureImg.width;
     vh = textureImg.height;
     int vw10 = vw / 10;
     int vh10 = vh / 10;
     int vw2 = vw / 2;
     int vh2 = vh / 2;
     float mmm = 100000.0;
     
     //  vw2 = (int) ((float) (millis() % mmm) * (float) vw / mmm);
     
     u1 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+0)*radius[0]), 320-centerPoint.x));
     v1 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+0)*radius[0]), 240-centerPoint.y));
     u2 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[0]), 320-centerPoint.x));
     v2 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[0]), 240-centerPoint.y));
     u3 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[0]), 320-centerPoint.x));
     v3 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[0]), 240-centerPoint.y));
     
     u4 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+0)*radius[1]), 320-centerPoint2.x));
     v4 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+0)*radius[1]), 240-centerPoint2.y));
     u5 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[1]), 320-centerPoint2.x));
     v5 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[1]), 240-centerPoint2.y));
     u6 = fmax(0, centerPoint2.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[1]), 320-centerPoint2.x));
     v6 = fmax(0, centerPoint2.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[1]), 240-centerPoint2.y));
     
     for(iy=0;iy<(gDrawHeight/ksy)+1;iy++) {
     
     ix=0;
     
     ofTexture& tex = textureImg.getTextureReference();
     ofTexture& tex2 = textureImg2.getTextureReference();
     
     //**********************************************************
     // ROW ONE
     //**********************************************************
     
     // 0 UP
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u3,v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     //ix--;
     glTexCoord2f(u2,v2);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     ix += 2;
     }
     glEnd();
     tex.unbind();
     
     //0 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u3, v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     
     ix ++;
     }
     glEnd();
     tex2.unbind();
     
     //1 up
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 1 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     //ix--;
     glTexCoord2f(u3,v3);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     
     ix += 2;
     }
     glEnd();
     tex.unbind();
     
     //1 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 1 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     
     ix ++;
     }
     glEnd();
     tex2.unbind();
     
     //2 UP
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 1 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     ix++;
     glTexCoord2f(u3,v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2, v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     ix ++;
     }
     glEnd();
     tex.unbind();
     
     //2 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 2 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2,v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     
     ix ++;
     }
     glEnd();
     tex2.unbind();
     
     
     //**********************************************************
     // ROW TWO
     //**********************************************************
     
     iy++;
     
     //1 up
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -.5; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     //ix--;
     glTexCoord2f(u3,v3);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     
     ix += 2;
     }
     glEnd();
     tex.unbind();
     
     //1 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     
     ix ++;
     }
     glEnd();
     tex2.unbind();
     
     //2 UP
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = -.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     ix++;
     glTexCoord2f(u3,v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2, v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     ix ++;
     }
     glEnd();
     tex.unbind();
     //2 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = .5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u3, v3);
     glVertex2f(ix * ksx, iy * ksy);
     glTexCoord2f(u2,v2);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     ix++;
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     
     ix ++;
     }
     glEnd();
     tex2.unbind();
     
     // 0 UP
     
     tex.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 1.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy);
     
     glTexCoord2f(u3,v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy);
     
     //ix--;
     glTexCoord2f(u2,v2);
     glVertex2f((ix)* ksx- ksx/2, (iy+1)*ksy);
     ix += 2;
     }
     glEnd();
     tex.unbind();
     
     //0 down
     
     tex2.bind();
     glBegin(GL_TRIANGLES);
     for(ix = 1.5 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {			
     glTexCoord2f(u1, v1);
     glVertex2f(ix * ksx, iy * ksy); 
     glTexCoord2f(u3, v3);
     glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy); 
     ix++;
     glTexCoord2f(u2,v2);
     glVertex2f(ix * ksx, iy * ksy); 
     
     ix ++;
     }
     tex2.unbind();
     
     
     glEnd(); 
     
     tex2.unbind();
     }
     
     
     // ucomment this section to see the video overlay with the triangle being grabbed
     
     //////////////////////////////////////////////////////
     
     //////////////////
     
     
     // this controls the speed of triangle rotation
     count  -= spinStep;
     // make decrement variable...
     
     
     // these control the size of the camer grab/////////////////////////////////////////////////////////////////////////
     radius[0] = triangleCameraGrab - offSet[0];
     radius[1] = triangleCameraGrab - offSet[1];
     
     //		if (radius[0] > 350) radius[0] = 350;
     //		if (radius[1] > 350) radius[1] = 350;
     
     }
     */
    void mousePressed( int x, int y){
        bDragging[0] = false;
        bDragging[1] = false;
        
        if ( bDrawDebug ){
            if (x < vw && y < vh){
                bDragging[0] = true;
                centerPoint.x = x - vw/2.0f;
                centerPoint.y = y - vh/2.0f;
            } else if ( x < vw*2 && y < vh ){				
                bDragging[1] = true;
                centerPoint2.x = x - vw - vw/2.0f;
                centerPoint2.y = y - vh/2.0f;
            }
        }
    };
    
    void mouseDragged( int x, int y){
        if ( bDrawDebug ){
            if (bDragging[0]){
                centerPoint.x = x - vw/2.0f;
                centerPoint.y = y - vh/2.0f;
            } else if ( bDragging[1] ){
                centerPoint2.x = x - vw - vw/2.0f;
                centerPoint2.y = y - vh/2.0f;
            }
        }
    };
    
    void mouseReleased(){
        bDragging[0] = false;
        bDragging[1] = false;
    }
    
};