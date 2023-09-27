//location for all the textures that we are going to use
#define IMAGE1_DIR "Textures/container.jpg"
#define STONE_TEX_DIR "Textures/stone.jpg"
#define PLAYER_TEX_DIR "test/characterwalk_00000.png"
#define CONTAINER_TEX_DIR "Textures/container.jpg"
#define GUN_TEX_DIR "Textures/gun.png"
#define BULLET_TEX_DIR "Textures/bullet.png"

namespace texture_dirs{
    namespace background{
        const char* moon_and_stars = "Assets/Export/Background/moon and stars.png";
        const char* background_buildings = "Assets/Export/Background/background buildings.png";
        const char* foreground_buildings = "Assets/Export/Background/foreground buildings.png";
        const char* middle_buildings = "Assets/Export/Background/middle buildings.png";
        const char* foreground = "Assets/Export/Background/foreground.png";
        const char* ground = "Assets/Export/Background/ground.png";
    };

    namespace objects{
        const char* first_aid_kit = "Assets/Export/Objects/first aid kit.png";
        const char* frag_granade = "Assets/Export/Objects/frag granade.png";
        const char* med_kit = "Assets/Export/Objects/med kit.png";
        const char* pistol = "Assets/Export/Objects/pistol.png";
        const char* shotgun = "Assets/Export/Objects/shotgun.png";
        const char* sniper = "Assets/Export/Objects/sniper.png";
        const char* SMG = "Assets/Export/Objects/SMG.png";
        const char* smoke_granade = "Assets/Export/Objects/smoke granade.png";
    };
};
//location where all the shaders are located
namespace shader_dir{
    namespace vs{
        const char* shader = "Shaders/shader.vs";
        const char* healthbar = "Shaders/healthbar.vs";
        const char* text = "Shaders/TextShader.vs";
        const char* animation = "Shaders/AnimationShader.vs";
    };
    namespace fs{
        const char* shader = "Shaders/shader.fs";
        const char* healthbar = "Shaders/healthbar.fs";
        const char* text = "Shaders/TextShader.fs";
        const char* animation = "Shaders/AnimationShader.fs";
    };
};