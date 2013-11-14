//
//  LightRender.h
//  Box2dLight
//
//  Created by Todd Vanderlin on 11/13/13.
//
//

#pragma once
#include "ofMain.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"


class Light {
public:
    Light() {
        color.set(ofRandomuf(), ofRandomuf(), ofRandomuf());
    }
    ofFloatColor color;
    ofPoint pos, vel;
};

class LightRender {
    
private:
    Poco::BasicEvent <void> renderEvent;
    ofShader occludersShader, shadowMapShader, lightShader;
public:
    
    vector <Light> lights;
    ofFbo occludersFBO, shadowMapFBO, objectsFBO, lightFBO;
    int width, height;
    
    LightRender() {
    }
    
    template <class TObj>
    void setRenderFuction(TObj  * listener, void (TObj::*listenerMethod)()){
        renderEvent += Poco::Delegate<TObj, void, false>(listener, listenerMethod);
    }
    
  
                          
    //--------------------------------------------------------------
    void setup(int w, int h) {
        
        ofDisableArbTex();  // <-- Very Important
        
        width = w;height=h;
        occludersFBO.allocate(width, height);
        lightFBO.allocate(width, height);
        objectsFBO.allocate(width, height, GL_RGBA);
        
        ofFbo::Settings s;
        s.width  = width;
        s.height = 1;
        s.maxFilter =GL_LINEAR;
        s.minFilter = GL_LINEAR;
        s.wrapModeHorizontal = GL_REPEAT;
        s.wrapModeVertical = GL_REPEAT;
        s.internalformat = GL_RGBA;
        s.useDepth = false;
        shadowMapFBO.allocate(s);
        
        ofFbo::Settings lightS;
        lightS.width  = width;
        lightS.height = height;
        lightS.useDepth = false;
        lightS.internalformat = GL_RGBA;
        lightFBO.allocate(lightS);
        
        if(!occludersShader.load("pass.vert", "pass.frag")) {
            printf("Error loading occludersShader\n");
        }
        if(!shadowMapShader.load("pass.vert", "shadowMap.frag")) {
            printf("Error loading shadowMapShader\n");
        }
        if(!lightShader.load("pass.vert", "light.frag")) {
            printf("Error loading lightShader\n");
        }
    
    }
    
    //--------------------------------------------------------------
    void addLight(float x, float y) {
        Light p;
        p.pos.set(x, y);
        p.vel.set(0, ofRandom(0.2, 1.0));
        lights.push_back(p);
    }
    void callRenderRunction() {
        try {
            renderEvent.notify(this);
        }
        catch(Poco::Exception &e) {
            
        }
    }
    //--------------------------------------------------------------
    void debugDraw(float x, float y, float drawWidth=500) {
        int nFbos = 2;
        float sw = drawWidth / (float)(width*nFbos);
        float sh = sw;
        
        ofPushMatrix();
        ofTranslate(x,y);
        ofScale((float)sw, (float)sh);
        ofEnableAlphaBlending();
        ofSetColor(255);
        occludersFBO.draw(0, 0);
        ofSetColor(255);
        shadowMapFBO.draw(width, 0, width, height);
       
        for(int i=0; i<lights.size(); i++) {
            ofSetColor(255, 255, 0);
            ofCircle(lights[i].pos.x, lights[i].pos.y, 5,5);
        }
        
        ofPopMatrix();
    }
    
    //--------------------------------------------------------------
    void renderLights() {
        
        
        /*
        objectsFBO.begin();
        ofClear(0, 0, 0, 0);
        callRenderRunction();
        objectsFBO.end();
        */
        
        lightFBO.begin();
        ofClear(0, 0, 0);
        
       
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
    
     
        
        for(int i=0; i<lights.size(); i++) {
            Light light = lights[i];
            
            
            // -------------------------------
            // Occlusion Map
            // -------------------------------
            occludersFBO.begin();
            ofClear(0, 0, 0, 0);
            occludersShader.begin();
            ofPushMatrix();
            ofTranslate((width/2)-light.pos.x, (height/2)-light.pos.y);
            callRenderRunction();
            ofPopMatrix();
            occludersShader.end();
            occludersFBO.end();
            
            
            
            
            
            // -------------------------------
            // 1d Shadow Map
            // -------------------------------
            shadowMapFBO.begin();
            shadowMapShader.begin();
            ofClear(0, 0, 0, 0);
            shadowMapShader.setUniform2f("lightLocation", lights[i].pos.x, lights[i].pos.y);
            shadowMapShader.setUniform2f("resolution", width, height);
            shadowMapShader.setUniformTexture("u_texture", occludersFBO.getTextureReference(), 0);
            occludersFBO.draw(0, 0);
            shadowMapShader.end();
            shadowMapFBO.end();
            
            // -------------------------------
            // Shadows
            // -------------------------------
            lightShader.begin();
            lightShader.setUniform2f("resolution", width, height);
            lightShader.setUniformTexture("u_texture", shadowMapFBO.getTextureReference(), 0);
            //lightShader.setUniformTexture("u_objects_tex", objectsFBO.getTextureReference(), 1);
            
            
            lightShader.setUniform2f("lightLocation", lights[i].pos.x, lights[i].pos.y);
            lightShader.setUniform3f("lightColor", lights[i].color.r, lights[i].color.g, lights[i].color.b);
            lightShader.setUniform1f("u_radius", 10.0);
            ofSetColor(255, 0, 255, 100);
          
            glBegin(GL_QUADS); {
                glVertex2f(0, 0);
                glVertex2f(0, height);
                glVertex2f(width, height);
                glVertex2f(width, 0);
            } glEnd();
            
            
        }
        glDisable(GL_BLEND);
        lightShader.end();
        lightFBO.end();
    }
    
    //--------------------------------------------------------------
    void draw() {
        ofSetColor(255);
        lightFBO.draw(0, 0);
    }
    
    
};

