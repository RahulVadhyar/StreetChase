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

int main(){
    //Initalize  window
    Window window;

    //make and initialize the background
    //moon and stars
    Shader bg_shader_moon_and_stars = Shader(shader_dir::vs::shader, shader_dir::fs::shader);  
    BackgroundObject bg_moon_and_stars(0.0f, 0.5f, 2.0f, 1.0f, 0.01f, &bg_shader_moon_and_stars, texture_dirs::background::moon_and_stars);
    window.addRenderObject(&bg_moon_and_stars);

    //background buildings
    Shader bg_shader_buildings = Shader(shader_dir::vs::shader, shader_dir::fs::shader);
    BackgroundObject bg_background_buildings(0.0f, 0.1f, 2.0f, 1.0f, 0.06f, &bg_shader_buildings, texture_dirs::background::background_buildings);
    window.addRenderObject(&bg_background_buildings);
    //ground 
    Shader bg_shader_ground = Shader(shader_dir::vs::shader, shader_dir::fs::shader);
    BackgroundObject bg_ground(0.0f, -0.9f, 2.0f, 1.0f, 0.1f, &bg_shader_ground, texture_dirs::background::ground);
    window.addRenderObject(&bg_ground);


    //create a shader
    Shader ground_shader(shader_dir::vs::shader, shader_dir::fs::shader);
    Shader wall_shader(shader_dir::vs::shader, shader_dir::fs::shader);
    Shader player_shader(shader_dir::vs::shader, shader_dir::fs::shader);
    Shader hud_shader(shader_dir::vs::shader, shader_dir::fs::shader);
    Shader gun_shader(shader_dir::vs::shader, shader_dir::fs::shader);
    Shader enemy_shader(shader_dir::vs::shader, shader_dir::fs::shader);

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &ground_shader, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.4, 0.4, 0.2, &wall_shader, STONE_TEX_DIR);
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


    player.shouldRender = true;
    player.addCollisionObject(&ground);
    player.addCollisionObject(&wall);
    player.addCollisionObject(enemy);
    enemy->addCollisionObject(&ground);
    enemy->addCollisionObject(&wall);
    enemy->addCollisionObject(&player);

    window.addRenderObject(&ground);
    window.addRenderObject(&wall);
    window.addRenderObject(&hud1);
    window.addEnemy(enemy);
    window.addMouseCallbackObject(&hud1);
    window.addPlayer(&player);
    
    irrklang::ISoundEngine* sound_engine = irrklang::createIrrKlangDevice();

    //render and play the game
//     window.mainMenu();
    sound_engine->play2D(MUSIC_DIR, false);
    sound_engine->setSoundVolume(0.5f);
    window.play();

    return 0;
}