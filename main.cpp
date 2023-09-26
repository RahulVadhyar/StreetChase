#include "definitions.hpp"

//command:g++ main.cpp glad/glad.c -o main -lglfw -lfreetype -I/usr/include/freetype2/ -I/home/starsystem/StreetChase/ SoundEngine/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so -pthread &&./main
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
//for powershell git ls-files --exclude-standard -- '*.cpp' '*.hpp' | ForEach-Object { Get-Content $_ } | Measure-Object -Line
//sudo apt install libasound2
//sudo apt install libfreetype6
//sudo apt install libglfw3-dev
//number of textures 35661

void sleep(int milliseconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void in(Window* window){
    while(!glfwWindowShouldClose(window->window)){
        window->processInput();
    }
}

void update(PlayerClass* player, Window* window){
    while(!glfwWindowShouldClose(window->window)){
        player->update();
        sleep(1);       
    }
}

void personupdate(Window* window){
    while(!glfwWindowShouldClose(window->window)){
        window->updatePersons();
        sleep(1);       
    }
}

int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader ground_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader wall_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader new_wall_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader player_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader hud_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader gun_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader enemy_shader(VS_SHADER_DIR, FS_SHADER_DIR);

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &ground_shader, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.4, 0.4, 0.2, &wall_shader, STONE_TEX_DIR);
    RenderObject new_wall(0.4, 0.4, 0.4, 0.2, &new_wall_shader, STONE_TEX_DIR);
    PlayerClass player(0, 0, 0.08f*0.8f, 0.5f*0.8f);
    SimpleGun gun(&gun_shader);
    EnemyClass* enemy = new EnemyClass(2, 0, 0.08f*0.8f, 0.5f*0.8f);

    SimpleGun enemy_gun(&gun_shader);
    player.addWeapon(&gun);
    enemy->addWeapon(&enemy_gun);

    //create HUD
    HUDObject hud1(0.0f, -0.9f, 0.2f, 0.2f, &hud_shader, CONTAINER_TEX_DIR);


    wall.attachTexture(PLAYER_TEX_DIR);
    wall.attachTexture(CONTAINER_TEX_DIR);
    wall.mixValue[0] = 0.5f;
    wall.mixValue[1] = 0.5f;
    wall.TexScale[1] = 0.2f;

    new_wall.attachTexture(PLAYER_TEX_DIR);
    new_wall.attachTexture(CONTAINER_TEX_DIR);
    new_wall.mixValue[0] = 0.5f;
    new_wall.mixValue[1] = 0.5f;
    new_wall.TexScale[1] = 0.2f;

    player.shouldRender = true;
    player.addCollisionObject(&ground);
    player.addCollisionObject(&wall);
    player.addCollisionObject(&new_wall);
    player.addCollisionObject(enemy);
    enemy->addCollisionObject(&ground);
    enemy->addCollisionObject(&wall);
    enemy->addCollisionObject(&new_wall);
    enemy->addCollisionObject(&player);

    window.addRenderObject(&ground);
    window.addRenderObject(&wall);
    window.addRenderObject(&new_wall);
    window.addRenderObject(&hud1);
    window.addEnemy(enemy);
    window.addMouseCallbackObject(&hud1);
    window.addPlayer(&player);
    
    irrklang::ISoundEngine* sound_engine = irrklang::createIrrKlangDevice();

    //render and play the game
    // window.mainMenu();
    std::thread input_thread(in, &window);
    std::thread updatet(update, &player, &window);
    std::thread personupdatet(personupdate, &window);

    sound_engine->play2D(MUSIC_DIR, false);
    sound_engine->setSoundVolume(0.5f);
    window.play();
    input_thread.join();
    updatet.join();
    personupdatet.join();
    return 0;
}