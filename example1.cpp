#include <vector>
#include <string>
#include <iostream>

#include "PXE/pe_window.hpp"
#include "PXE/pe_vertexarray.hpp"
#include "PXE/pe_program.hpp"
#include "PXE/pe_init.hpp"

#include <glm/gtc/type_ptr.hpp>


int main() {

    px::Window window(1280, 768, "Phoenix Engine");
    // px::Window window("Test");
    window.mouse_input.read_events = true;
    window.keyboard_input.read_events = true;

    float pos[] = {
        0, 0, 0,           0, 0, 1, .7,
        .5, 0, .5,         0, 1, 0, .7,
        0, .5, .5,         0, 1, 1, .7,
        0, 0, 1,           1, 0, 0, .7,
        -.5, 0, .5,        1, 0, 1, .7,
        0, -.5, .5,        1, 1, 0, .7
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
        
    //     -1, -1.2, -1,       .5, .5, .5, 
    //      1, -1.2, -1,       .5, .5, .5,
    //      1, -1.2,  1,       .5, .5, .5,
    //     -1, -1.2,  1,       .5, .5, .5
    // };

    float flpos[] = {
        -1, -1, -1,       1, 1, 1, 
         1, -1, -1,       1, 1, 1,
         1, -1,  1,       1, 1, 1,
        -1, -1,  1,       1, 1, 1,
        
        -1, -1.2, -1,       .5, .5, .5, 
         1, -1.2, -1,       .5, .5, .5,
         1, -1.2,  1,       .5, .5, .5,
        -1, -1.2,  1,       .5, .5, .5
    };

    px::VertexArray vao;

    px::VertexBuffer vbo(sizeof(pos), pos);

    px::VertexBufferLayout blay;
    blay.push<float>(3);
    blay.push<float>(4);

    vao.attachVertexBuffer(vbo, blay);

    px::ElementBuffer ibo('\0', 1, 2, 0, 2, 4, 0, 5, 1, 0, 4, 5, 3, 2, 1, 3, 4, 2, 3, 1, 5, 5, 4, 3);

    vbo.unbind(), ibo.unbind(), vao.unbind();

    px::VertexArray vao1;

    px::VertexBuffer vbo1(sizeof(flpos), flpos);

    px::VertexBufferLayout blay1;
    blay1.push<float>(3);
    blay1.push<float>(3);

    vao1.attachVertexBuffer(vbo1, blay1);

    px::ElementBuffer ibo1(
        '\0', 1, 2,  0, 2, 3,
           4, 6, 5,  4, 7, 6,
           0, 4, 1,  1, 4, 5,
           3, 2, 7,  2, 6, 7,
           0, 3, 7,  7, 4, 0,
           1, 5, 6,  6, 2, 1
    );

    vbo1.unbind(), ibo1.unbind(), vao1.unbind();


    px::ShaderCode sc("example1.glsl");
    px::ShaderProgram p;
    p.createProgram(sc.parseCode("v"), sc.parseCode("f"));
    p.use();

    
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


    ibo.unbind(), ibo1.unbind(), vao.unbind();

    p.cleanShaders(px::ShaderProgram::FRAGMENT_SHADER | px::ShaderProgram::VERTEX_SHADER);

    double cx=0, cy=0, cs=0;
    float fa = 0.f;
    // std::cout << window.mouse_input.events.x << '\t' << window.mouse_input.events.y << '\t' << cx << '\t' << cy << std::endl;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);
    glFrontFace(GL_CCW);

    // camera.turn_speed = .001;
    window.run(
        [&](double deltaTime) {

            // #pragma omp parallel 
            std::cout << "\033[H\033[J" << deltaTime << std::endl;

            // camera.xyzShift(window.mouse_input.events.x/window.width, -window.mouse_input.events.y/window.height); 
            camera.mouseControl(window.mouse_input.getXChange(), window.mouse_input.getYChange());
    
            

            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) camera.movement_velocity = glm::vec3(3);
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_W]) camera.setPosition(camera.getPosition() + camera.getFrontVector()*camera.movement_velocity*float(deltaTime));
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_A]) camera.setPosition(camera.getPosition() - camera.getRightVector()*camera.movement_velocity*float(deltaTime));
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_S]) camera.setPosition(camera.getPosition() - camera.getFrontVector()*camera.movement_velocity*float(deltaTime));
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_D]) camera.setPosition(camera.getPosition() + camera.getRightVector()*camera.movement_velocity*float(deltaTime));
            if (window.keyboard_input.events.action == px::Action::PRESS && window.keyboard_input.events.key == px::Keyboard::KEY_ESCAPE) window.closeWindow();
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_Q]) camera.setPosition(camera.getPosition() - camera.getUpVector()*camera.movement_velocity*float(deltaTime));
            if (window.keyboard_input.events.key_action_record[px::Keyboard::KEY_E]) camera.setPosition(camera.getPosition() + camera.getUpVector()*camera.movement_velocity*float(deltaTime));
            if (!window.keyboard_input.events.key_action_record[px::Keyboard::KEY_LEFT_SHIFT]) camera.movement_velocity = glm::vec3(1);

            p.use();
            p.setUniformMat<4, 4>("view", glm::value_ptr(camera.getViewMatrix()));
            
            vao1.bind(), ibo1.bind();
            glDrawElements(GL_TRIANGLES, ibo1.elementCount(), ibo1.draw_callType(), nullptr);
            
            vao.bind(), ibo.bind();
            glEnable(GL_BLEND);
            // glEnable(GL_CULL_FACE);
            glDrawElements(GL_TRIANGLES, ibo.elementCount(), ibo.draw_callType(), nullptr);
            // glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
            px::VertexArray::unbind(), px::ElementBuffer::unbind(), px::ShaderProgram::idle();

            // std::cout << "\033[H\033[J[ pos: (" << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << "), front: (" << camera.getFrontVector().x << ", " << camera.getFrontVector().y << ", " << camera.getFrontVector().z << "), Up: (" << camera.getUpVector().x << ", " << camera.getUpVector().y << ", " << camera.getUpVector().z << "), Pitch: " << glm::degrees(camera.getPitch()) << "°, Yaw: " << glm::degrees(camera.getYaw()) << "°, Roll: " << glm::degrees(camera.getRoll()) << "°\n";
            // [[hot]] for (int i = 0; i < camera.getViewMatrix().length(); i++)
            // {
            //     std::cout << "\t[";
            //     [[hot]] for (int j = 0; j < camera.getViewMatrix()[i].length()-1; j++)
            //     {
            //         std::cout << ( (10 > std::to_string(camera.getViewMatrix()[i][j]).length()) ? std::string(10-std::to_string(camera.getViewMatrix()[i][j]).length(), ' ') : "") + std::to_string(camera.getViewMatrix()[i][j]) << ", ";  
            //     }
            //     std::cout << std::to_string(camera.getViewMatrix()[i][camera.getViewMatrix()[i].length()-1]) << " ]" << std::endl;
            // }
            // std::cout << "]\n";
            // std::cout << window.keyboard_input.events.action << '\t' << window.keyboard_input.events.key << '\t' << (int)window.keyboard_input.events.key_action_record[window.keyboard_input.events.key] << std::endl;
            // std::cout << window.mouse_input.events.x << '\t' << window.mouse_input.events.y << '\t' << cx << '\t' << cy << std::endl;
            // std::cout << window.mouse_input.events.x - cx << '\t' << window.mouse_input.events.y - cy << std::endl;
            
            if (window.keyboard_input.events.action == px::Action::PRESS && window.keyboard_input.events.key == px::Keyboard::KEY_F) std::cout << "Angle: " << camera.getYaw() << std::endl;
        }
    );

    return 0;
}
