//different transform functions for objects
namespace Transform{
    //This is the default transform, used for just translating an object to the given position
    glm::mat4 Default(float x, float y, float rotation){
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x, y, 0));
        trans = glm::rotate(trans, rotation, glm::vec3(0.0, 0.0, 1.0));
        return trans;
    }


};