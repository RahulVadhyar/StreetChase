//different transform functions for objects
namespace Transform{
    //This is the default transform, used for just translating an object to the given position
    glm::mat4 Default(float x, float y){
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x, y, 0));
        return trans;
    }
    
};