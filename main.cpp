#include <C:\Users\vadhy\StreetChase\definitions.hpp>

//command:clang++ main.cpp glad/glad.c  -o main.exe "C:\Users\vadhy\StreetChase\GLFW\lib\glfw3dll.lib" "C:\Users\vadhy\StreetChase\freetype.lib" -I"C:\Users\vadhy\StreetChase\"  -Wall -Wextra &&.\main.exe
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
//for powershell git ls-files --exclude-standard -- '*.cpp' '*.hpp' | ForEach-Object { Get-Content $_ } | Measure-Object -Line
//lldb clang++ main.cpp glad/glad.c  -o main.exe "C:\Users\vadhy\StreetChase\GLFW\lib\glfw3dll.lib" -Wall -Wextra -g && lldb .\main.exe
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader ground_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader wall_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader player_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader hud_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader gun_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader enemy_shader(VS_SHADER_DIR, FS_SHADER_DIR);

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &ground_shader, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.4, 0.4, 0.2, &wall_shader, STONE_TEX_DIR);
    PlayerClass player(0, 0, 0.1f, 0.2f, &player_shader, PLAYER_TEX_DIR);
    SimpleGun gun(&gun_shader);
    EnemyClass* enemy = new EnemyClass(2, 0, 0.1f, 0.2f, &enemy_shader, PLAYER_TEX_DIR);
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
    window.addRenderObject(&player);
    window.addRenderObject(&hud1);
    window.addRenderObject(&gun);
    window.addPerson(enemy);
    window.addMouseCallbackObject(&hud1);
    window.addPlayer(&player);
    
    
    //render and play the game
    window.play();
    return 0;
}