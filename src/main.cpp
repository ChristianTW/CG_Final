#include <iostream>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include "Canis/Canis.hpp"
#include "Canis/Window.hpp"
#include "Canis/Shader.hpp"
#include "Canis/Debug.hpp"
#include "Canis/IOManager.hpp"
#include "Canis/InputManager.hpp"
#include "Canis/Camera.hpp"
#include "Canis/Model.hpp"
#include "Canis/World.hpp"
#include "Canis/FrameRateManager.hpp"

using namespace glm;

// git restore .
// git fetch
// git pull

// 3d array
std::vector<std::vector<std::vector<unsigned int>>> map = {};

// declaring functions
void SpawnLights(Canis::World &_world);
void LoadMap(std::string _path);

int main(int argc, char *argv[])
{
    Canis::Init();
    Canis::InputManager inputManager;
    Canis::FrameRateManager frameRateManager;
    frameRateManager.Init(60);

    /// SETUP WINDOW
    Canis::Window window;
    window.MouseLock(true);

    unsigned int flags = 0;

    if (Canis::GetProjectConfig().fullscreen)
        flags |= Canis::WindowFlags::FULLSCREEN;

    window.Create("Hello Graphics",
                  Canis::GetProjectConfig().width,
                  Canis::GetProjectConfig().heigth,
                  flags);
    /// END OF WINDOW SETUP

    Canis::World world(&window, &inputManager);
    SpawnLights(world);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA);

    /// SETUP SHADER
    Canis::Shader shader;
    shader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    shader.AddAttribute("aPosition");
    shader.Link();
    shader.Use();
    shader.SetInt("MATERIAL.diffuse", 0);
    shader.SetInt("MATERIAL.specular", 1);
    shader.SetFloat("MATERIAL.shininess", 64);
    shader.SetBool("WIND", false);
    shader.UnUse();

    // grass shader
    Canis::Shader grassShader;
    grassShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    grassShader.AddAttribute("aPosition");
    grassShader.Link();
    grassShader.Use();
    grassShader.SetInt("MATERIAL.diffuse", 0);
    grassShader.SetInt("MATERIAL.specular", 1);
    grassShader.SetFloat("MATERIAL.shininess", 64);
    grassShader.SetBool("WIND", true);
    grassShader.SetFloat("WINDEFFECT", 0.2);
    grassShader.UnUse();
    /// END OF SHADER

    /// Load Image
    Canis::GLTexture texture = Canis::LoadImageGL("assets/textures/glass.png", true);
    Canis::GLTexture grassTexture = Canis::LoadImageGL("assets/textures/grass.png", false);
    Canis::GLTexture poppyTexture = Canis::LoadImageGL("assets/textures/poppy.png", false);
    Canis::GLTexture woodTexture = Canis::LoadImageGL("assets/textures/oak_planks.png", true);
    Canis::GLTexture stoneTexture = Canis::LoadImageGL("assets/textures/cobblestone.png", true);
    Canis::GLTexture brickTexture = Canis::LoadImageGL("assets/textures/bricks.png", true);
    Canis::GLTexture grassBlockTexture = Canis::LoadImageGL("assets/textures/grass_block_top.png", true);
    Canis::GLTexture knux = Canis::LoadImageGL("assets/textures/KnuxHolding.jpg", true);
    Canis::GLTexture textureSpecular = Canis::LoadImageGL("assets/textures/container2_specular.png", true);
    Canis::GLTexture specShiny = Canis::LoadImageGL("assets/textures/white_texture.png", true);
    Canis::GLTexture woodSpec = Canis::LoadImageGL("assets/textures/woodSpecular.png", true);
    /// End of Image Loading
    /// Fire Texture
    Canis::GLTexture fireList[31] = {
        Canis::LoadImageGL("assets/textures/fire_textures/fire_1.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_2.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_3.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_4.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_5.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_6.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_7.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_8.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_9.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_10.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_11.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_12.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_13.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_14.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_15.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_16.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_17.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_18.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_19.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_20.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_21.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_22.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_23.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_24.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_25.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_26.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_27.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_28.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_29.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_30.png", true),
        Canis::LoadImageGL("assets/textures/fire_textures/fire_31.png", true)
    };
    /// End if fire texture


    /// Load Models
    Canis::Model cubeModel = Canis::LoadModel("assets/models/cube.obj");
    Canis::Model grassModel = Canis::LoadModel("assets/models/plants.obj");
    Canis::Model fireModel = Canis::LoadModel("assets/models/fire.obj");
    /// END OF LOADING MODEL

    // Load Map into 3d array
    LoadMap("assets/maps/level.map");

    int randomNum;
    srand(time(NULL));

    // Loop map and spawn objects
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            for (int z = 0; z < map[y][x].size(); z++)
            {
                Canis::Entity entity;
                entity.active = true;

                randomNum = rand() % 5;

                switch (map[y][x][z])
                {
                case 1: // places a glass block
                    entity.tag = "glass";
                    entity.albedo = &texture;
                    entity.specular = &specShiny;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 2: // places a grass, a poppy, or nothing, randomly
                    switch (randomNum)
                    {
                    case 0:
                        break;
                    case 1:  // places grass
                        entity.tag = "grass";
                        entity.albedo = &grassTexture;
                        entity.specular = &textureSpecular;
                        entity.model = &grassModel;
                        entity.shader = &grassShader;
                        entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                        world.Spawn(entity);
                        break;
                    case 2: // places a poppy
                        entity.tag = "poppy";
                        entity.albedo = &poppyTexture;
                        entity.specular = &textureSpecular;
                        entity.model = &grassModel;
                        entity.shader = &grassShader;
                        entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                        world.Spawn(entity);
                        break;
                    default:
                        break;
                    }
                    break;
                    
                case 3: // places a wood block
                    entity.tag = "wood";
                    entity.albedo = &woodTexture;
                    entity.specular = &woodSpec;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 4: // places a stone block
                    entity.tag = "stone";
                    entity.albedo = &stoneTexture;
                    entity.specular = &specShiny;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 5: // places a brick block
                    entity.tag = "brick";
                    entity.albedo = &brickTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 6: // places a grass block
                    entity.tag = "grassBlock";
                    entity.albedo = &grassBlockTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 7: // places a knux block
                    entity.tag = "knux";
                    entity.albedo = &knux;
                    entity.specular = &specShiny;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 8: // places fire
                    entity.tag = "fire";
                    entity.albedo = &fireList[0];
                    entity.specular = &textureSpecular;
                    entity.model = &fireModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                default:
                    break;
                }
            }
        }
    }

    double deltaTime = 0.0;
    double fps = 0.0;

    double timeS = 0.0;

    // Fire point light
    Canis::PointLight *fireLight = world.GetPointLight(vec3(5.0f, 1.0f, 8.5f));

    // Application loop
    while (inputManager.Update(Canis::GetProjectConfig().width, Canis::GetProjectConfig().heigth))
    {
        deltaTime = frameRateManager.StartFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        timeS += deltaTime;

        // Makes the fire light flicker (using diffuse because ambient is broken)
        if (int(fmod(timeS * 100, 10)) ==5)
        {
            fireLight->diffuse = vec3(0.4f, 0.4f, 0.01f);
        } else {
            fireLight->diffuse = vec3(0.7f, 0.7f, 0.1f);
        }

        //How to move a specific tagged thing (use for fire)
        std::vector<Canis::Entity*> glass = world.GetEntitiesWithTag("fire");
        for(Canis::Entity* g : glass)
        {
            g->albedo = &fireList[int(fmod(timeS * 20, 31))];
            //Canis::Log(std::to_string(g->transform.position.x));
            //Canis::Log(std::to_string(g->transform.position.y));
            //Canis::Log(std::to_string(g->transform.position.z));
        }

        world.Update(deltaTime);
        world.Draw(deltaTime);

        window.SwapBuffer();
        
        // EndFrame will pause the app when running faster than frame limit
        fps = frameRateManager.EndFrame();

        Canis::Log("FPS: " + std::to_string(fps) + " DeltaTime: " + std::to_string(deltaTime));
    }

    return 0;
}

void LoadMap(std::string _path)
{
    std::ifstream file;
    file.open(_path);

    if (!file.is_open())
    {
        printf("file not found at: %s \n", _path.c_str());
        exit(1);
    }

    int number = 0;
    int layer = 0;

    map.push_back(std::vector<std::vector<unsigned int>>());
    map[layer].push_back(std::vector<unsigned int>());

    while (file >> number)
    {
        if (number == -2) // add new layer
        {
            layer++;
            map.push_back(std::vector<std::vector<unsigned int>>());
            map[map.size() - 1].push_back(std::vector<unsigned int>());
            continue;
        }

        if (number == -1) // add new row
        {
            map[map.size() - 1].push_back(std::vector<unsigned int>());
            continue;
        }

        map[map.size() - 1][map[map.size() - 1].size() - 1].push_back((unsigned int)number);
    }
}

void SpawnLights(Canis::World &_world)
{
    Canis::DirectionalLight directionalLight;
    _world.SpawnDirectionalLight(directionalLight);

    Canis::PointLight pointLight;
    pointLight.position = vec3(0.0f);
    pointLight.ambient = vec3(0.2f);
    pointLight.diffuse = vec3(0.5f);
    pointLight.specular = vec3(1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;

    _world.SpawnPointLight(pointLight);

    pointLight.position = vec3(0.0f, 0.0f, 28.0f);
    pointLight.ambient = vec3(8.0f, 0.0f, 0.0f);

    _world.SpawnPointLight(pointLight);

    pointLight.position = vec3(-18.0f);
    pointLight.ambient = vec3(0.0f, 10.0f, 0.0f);

    _world.SpawnPointLight(pointLight);

    pointLight.position = vec3(18.0f);
    pointLight.ambient = vec3(0.0f, 0.0f, 6.0f);

    _world.SpawnPointLight(pointLight);

    pointLight.position = vec3(5.0f, 1.0f, 8.5f);
    pointLight.ambient = vec3(0.0f, 0.0f, 0.0f);

    _world.SpawnPointLight(pointLight);
}