#include <iostream>
// #include "PXE/pe_lighting.hpp"
#include "PXE/pe_mesh.hpp"
#include "PXE/pe_texture.hpp"
#include "PXE/pe_window.hpp"
#include "PXE/pe_vertexarray.hpp"
#include "PXE/pe_program.hpp"
#include "PXE/pe_init.hpp"

#include <glm/gtc/type_ptr.hpp>


int main() {
    px::Window window(1280, 710, "Phoenix Engine");
    // px::Window window("Phoenix Engine");
    window.mouse_input.read_events = true;
    window.keyboard_input.read_events = true;

   // float pos[] = {
   //     0, 0, 0,           0, 0, 1, .7,         0, 0,
   //     .5, 0, .5,         0, 1, 0, .7,         1, 0,
   //     0, .5, .5,         0, 1, 1, .7,         .5, .5,
   //     0, 0, 1,           1, 0, 0, .7,         1, 1,
   //     -.5, 0, .5,        1, 0, 1, .7,         0, 1,
   //     0, -.5, .5,        1, 1, 0, .7,         .5, .5
   // };

    float s1b3 = 1.0/sqrt(3.0);
   // float pynorm[] {
   //     s1b3, s1b3, -s1b3,      -s1b3, s1b3, -s1b3,
   //     s1b3, -s1b3, -s1b3,     -s1b3, -s1b3, -s1b3,
   //     s1b3, s1b3, s1b3,       -s1b3, s1b3, s1b3,
   //     s1b3, -s1b3, s1b3,      -s1b3, -s1b3, s1b3
   // };


    float pos[] = {
//      x, y, z,           r, g, b,  a,           i,    j,     k,         u, v
        0, 0, 0,           0, 0, 1, .7,        s1b3, s1b3, -s1b3,         0, 0,
        .5, 0, .5,         0, 1, 0, .7,        s1b3, s1b3, -s1b3,         1, 0,
        0, .5, .5,         0, 1, 1, .7,        s1b3, s1b3, -s1b3,         .5, .5,

        0, 0, 1,           1, 0, 0, .7,        -s1b3, -s1b3, s1b3,        1, 1,
        0, -.5, .5,        1, 1, 0, .7,        -s1b3, -s1b3, s1b3,        .5, .5,
        -.5, 0, .5,        1, 0, 1, .7,        -s1b3, -s1b3, s1b3,        0, 1,

        0, 0, 0,           0, 0, 1, .7,        -s1b3, -s1b3, -s1b3,       0, 0,
        -.5, 0, .5,        1, 0, 1, .7,        -s1b3, -s1b3, -s1b3,       0, 1,
        0, -.5, .5,        1, 1, 0, .7,        -s1b3, -s1b3, -s1b3,       .5, .5,

        .5, 0, .5,         0, 1, 0, .7,        s1b3, s1b3, s1b3,          1, 0,
        0, 0, 1,           1, 0, 0, .7,        s1b3, s1b3, s1b3,          1, 1,
        0, .5, .5,         0, 1, 1, .7,        s1b3, s1b3, s1b3,          .5, .5,

        0, 0, 0,           0, 0, 1, .7,        s1b3, -s1b3, -s1b3,        0, 0,
        0, -.5, .5,        1, 1, 0, .7,        s1b3, -s1b3, -s1b3,        .5, .5,
        .5, 0, .5,         0, 1, 0, .7,        s1b3, -s1b3, -s1b3,        1, 0,

        0, 0, 1,           1, 0, 0, .7,        -s1b3, s1b3, s1b3,         1, 1,
        -.5, 0, .5,        1, 0, 1, .7,        -s1b3, s1b3, s1b3,         0, 1,
        0, .5, .5,         0, 1, 1, .7,        -s1b3, s1b3, s1b3,         .5, .5,

        0, 0, 0,           0, 0, 1, .7,        -s1b3, s1b3, -s1b3,        0, 0,
        0, .5, .5,         0, 1, 1, .7,        -s1b3, s1b3, -s1b3,        .5, .5,
        -.5, 0, .5,        1, 0, 1, .7,        -s1b3, s1b3, -s1b3,        0, 1,

        0, 0, 1,           1, 0, 0, .7,        s1b3, -s1b3, s1b3,         1, 1,
        .5, 0, .5,         0, 1, 0, .7,        s1b3, -s1b3, s1b3,         1, 0,
        0, -.5, .5,        1, 1, 0, .7,        s1b3, -s1b3, s1b3,         .5, .5
    };

// 0.4392, 0.3294, 0.2431,
// 0.4392, 0.3294, 0.2431,
// 0.4392, 0.3294, 0.2431,
// 0.4392, 0.3294, 0.2431,


    // float flpos[] = {
    //     -1, -1, -1,       0.4392, 0.3294, 0.2431, 
    //      1, -1, -1,       0.4392, 0.3294, 0.2431,
    //      1, -1,  1,       0.4392, 0.3294, 0.2431,
    //     -1, -1,  1,       0.4392, 0.3294, 0.2431,
    //     
    //     -1, -1.2, -1,       .5, .5, .5, 
    //      1, -1.2, -1,       .5, .5, .5,
    //      1, -1.2,  1,       .5, .5, .5,
    //     -1, -1.2,  1,       .5, .5, .5
    // };

    // float flpos[] = {
    //     -1, -1, -1,       1, 1, 1, 1,             0, 0,
    //      1, -1, -1,       1, 1, 1, 1,             1, 0,
    //      1, -1,  1,       1, 1, 1, 1,             1, 1,
    //     -1, -1,  1,       1, 1, 1, 1,             0, 1,
    //  
    //     -1, -1.2, -1,       .5, .5, .5, 1,        0, 0,
    //      1, -1.2, -1,       .5, .5, .5, 1,        1, 0,
    //      1, -1.2,  1,       .5, .5, .5, 1,        1, 1,
    //     -1, -1.2,  1,       .5, .5, .5, 1,        0, 1
    // };
    
    float flpos[] = {
//       x,  y,  z,         r, g, b, a,          i, j, k,        u, v
        -1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        0, 0,
         1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        1, 0,
         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
        -1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        0, 1,
        -1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        0, 0,
         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
     
        -1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       0, 0,
         1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       1, 1,
         1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       1, 0,
        -1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       0, 1,
         1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       1, 1,
        -1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       0, 0,

        -1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       0, 0,
        -1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        0, 0,
        -1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        0, 1,
        -1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       0, 0,
        -1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        0, 1,
        -1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       0, 1,

         1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        1, 0,
         1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       1, 0,
         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
         1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       1, 0,
         1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       1, 1,

        -1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        0, 0,
        -1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       0, 0,
         1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       1, 0,
        -1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        0, 0,
         1, -1.2, -1,       .5, .5, .5, 1,       0, -1, 0,       1, 0,
         1, -1, -1,         1, 1, 1, 1,          0, 1, 0,        1, 0,

         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
         1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       1, 1,
        -1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       0, 1,
         1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        1, 1,
        -1, -1.2,  1,       .5, .5, .5, 1,       0, -1, 0,       0, 1,
        -1, -1,  1,         1, 1, 1, 1,          0, 1, 0,        0, 1
    };
    
   // float flnorm[] {
   //     0, 1, 0,    0, 1, 0,
   //     0, -1, 0,   0, -1, 0,
   //     0, 0, -1,   0, 0, -1,    
   //     0, 0, 1,    0, 0, 1,
   //     -1, 0, 0,   -1, 0, 0,
   //     1, 0, 0,    1, 0, 0
   // };

    px::VertexArray vao;

    px::VertexBuffer vbo(sizeof(pos), pos);

    px::VertexBufferLayout blay;
    blay.push<float>(3);
    blay.push<float>(4);
    blay.push<float>(3);
    blay.push<float>(2);

    vao.attachVertexBuffer(vbo, blay);

    // px::ElementBuffer ibo('\0', 1, 2, 0, 2, 4, 0, 5, 1, 0, 4, 5, 3, 2, 1, 3, 4, 2, 3, 1, 5, 5, 4, 3);

    vbo.unbind(), /*ibo.unbind(),*/ vao.unbind();

    px::VertexArray vao1;

    px::VertexBuffer vbo1(sizeof(flpos), flpos);

    px::VertexBufferLayout blay1;
    blay1.push<float>(3);
    blay1.push<float>(4);
    blay1.push<float>(3);
    blay1.push<float>(2);

    vao1.attachVertexBuffer(vbo1, blay1);

    // px::ElementBuffer ibo1(
    //     '\0', 1, 2,  0, 2, 3,
    //        4, 6, 5,  4, 7, 6,
    //        0, 4, 1,  1, 4, 5,
    //        3, 2, 7,  2, 6, 7,
    //        0, 3, 7,  7, 4, 0,
    //        1, 5, 6,  6, 2, 1
    // );

    vbo1.unbind(), /*ibo1.unbind(),*/ vao1.unbind();

    px::Texture fpy("textures/forpyramid1.png"), ffl("textures/forfloor.png");

    // px::PhongLighting ambient;

    px::ShaderCode sc("shaders/example1.glsl");
    px::ShaderProgram p;
    p.createProgram(sc.parseCode("v"), sc.parseCode("f"));
    p.use();

    glm::vec3 sunshine(1.f, 1.f, 0.956862745f);
    glm::vec3 neon_blue(0.117647059f, 0.564705882f, 1.f);

    p.setUniform<3>("ambient.color", sunshine.r, sunshine.g, sunshine.b);
    p.setUniform("ambient.intensity", .1f);
 
    // ambient.setIntensity(1.3);
    // ambient.useLight(p, "AL.color", "AL.intensity");
    
    std::cout << "Tex[out-fpy]: " << fpy.getTexID() << ' ' << fpy.getWidth() << ' ' << fpy.getHeight() << std::endl;
    std::cout << "Tex[out-ffl]: " << ffl.getTexID() << ' ' << ffl.getWidth() << ' ' << ffl.getHeight() << std::endl;
    std::cout << sc.parseCode("f") << std::endl;

    px::Viewspace camera(glm::vec3(0, 0, -3), {glm::radians(-10.f), glm::radians(90.f), .0});

    glm::mat4 vm = camera.getViewMatrix();

    float* v = glm::value_ptr(vm);

    float sv[16], x2[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    p.setUniformMat<4, 4>("view", v);

    glm::mat4 proj = glm::perspective(45.0, double(window.width)/double(window.height), 0.1, 100.0);

    float* pa = glm::value_ptr(proj);

    p.setUniformMat<4, 4>("projection", pa);

    p.getUniform("view", sv);


    p.cleanShaders(px::ShaderProgram::FRAGMENT_SHADER | px::ShaderProgram::VERTEX_SHADER);

    double cx=0, cy=0, cs=0;
    float fa = 0.f;
    // std::cout << window.mouse_input.events.x << '\t' << window.mouse_input.events.y << '\t' << cx << '\t' << cy << std::endl;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);
    glFrontFace(GL_CCW);
    // window.setVsync(true);
    // camera.turn_speed = .001;

    auto cameraMovementFunction = [&](double deltaTime) -> void {
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) camera.movement_velocity = glm::vec3(3);
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_W]) 
            camera.setPosition(camera.getPosition() + camera.getFrontVector()*camera.movement_velocity*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_A]) 
            camera.setPosition(camera.getPosition() - camera.getRightVector()*camera.movement_velocity*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_S]) 
            camera.setPosition(camera.getPosition() - camera.getFrontVector()*camera.movement_velocity*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_D]) 
            camera.setPosition(camera.getPosition() + camera.getRightVector()*camera.movement_velocity*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_Q]) 
            camera.setPosition(camera.getPosition() - camera.getUpVector()*camera.movement_velocity*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_E]) 
            camera.setPosition(camera.getPosition() + camera.getUpVector()*camera.movement_velocity*float(deltaTime));
        if (!window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) camera.movement_velocity = glm::vec3(1);
    };


    px::Mesh light_source;
    light_source.translate(0, -.997, 0);
    glm::vec3 ls_movement_velocity(1);

    auto lightsrcMovementFunction = [&](double deltaTime) -> void {
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) ls_movement_velocity = glm::vec3(3);
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_I]) 
            light_source.translate(0, 0, -ls_movement_velocity.z*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_J]) 
            light_source.translate(-ls_movement_velocity.x*float(deltaTime), 0, 0);
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_K]) 
            light_source.translate(0, 0, ls_movement_velocity.z*float(deltaTime));
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_L]) 
            light_source.translate(ls_movement_velocity.x*float(deltaTime), 0, 0);
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_U]) 
            light_source.translate(0, ls_movement_velocity.y*float(deltaTime), 0);
        if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_O]) 
            light_source.translate(0, -ls_movement_velocity.y*float(deltaTime), 0);
        if (!window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) ls_movement_velocity = glm::vec3(1);
    };
    

    std::cout << window.title << '\n';
    window.run(
        [&](double deltaTime, double fps) {
            float rotang = 2*glm::pi<float>()*.1*glfwGetTime();
            // std::cout << "\033[H\033[J" << fps << '\t' << deltaTime << std::endl;
            // glClearColor(0, 0, 1, 0);
            if (window.keyboard_input.events.action == px::Action::PRESS && window.keyboard_input.events.key == px::Keyboard::KEY_ESCAPE) 
                window.closeWindow();
            camera.mouseControl(window.mouse_input.getXChange(), window.mouse_input.getYChange());            
            cameraMovementFunction(deltaTime);
            lightsrcMovementFunction(deltaTime);

            p.use();
            // p.setUniform<3>("dirsource", 0.f, float(2*sin(rotang)), float(2*cos(rotang)));
            p.setUniform<3>("dirsource", light_source.getPosition().x, light_source.getPosition().y, light_source.getPosition().z);
            p.setUniformMat<4, 4>("view", glm::value_ptr(camera.getViewMatrix()));
            p.setUniform("useMix", 0);
            vao1.bind()/*, ibo1.bind()*/;
            // glUniform1i(glGetUniformLocation(p.getProgramID(), "theTex"), ffl.getTexID());
            ffl.useTex();
            // glDrawElements(GL_TRIANGLES, ibo1.elementCount(), ibo1.draw_callType(), nullptr);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            p.setUniform("useMix", 1);
            vao.bind()/*, ibo.bind()*/;
            // glUniform1i(glGetUniformLocation(p.getProgramID(), "theTex"), fpy.getTexID());
            fpy.useTex();
            glEnable(GL_BLEND);
            // glEnable(GL_CULL_FACE);
            // glDrawElements(GL_TRIANGLES, ibo.elementCount(), ibo.draw_callType(), nullptr);
            glDrawArrays(GL_TRIANGLES, 0, 24);
            // glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
            px::VertexArray::unbind(), px::ElementBuffer::unbind(), px::ShaderProgram::idle();
            px::Texture::idle();
            
            if (window.keyboard_input.events.action == px::Action::PRESS && window.keyboard_input.events.key == px::Keyboard::KEY_F) std::cout << "Angle: " << camera.getYaw() << std::endl;
            
            std::cout << "{" << light_source.getPosition().x << ", " << light_source.getPosition().y << ", " << light_source.getPosition().z << "}\n";
        }
    );

    return 0;
}


