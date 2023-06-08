#ifndef SHADERS_HPP
#define SHADERS_HPP

class Shader{
    public:
        //program ID
        unsigned int shader_id;

        //constructor
        Shader(const char* vertexPath, const char* fragmentPath){
            
            //retrieve the source codes from the filepaths
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try{
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } catch(std::ifstream::failure){
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
                exit(-1);
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            //compile shaders
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            //vertex shaders
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            //check for errors
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                exit(-1);
            } 

            //fragment shaders
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            //check for errors
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                exit(-1);
            }

            //create the program
            shader_id = glCreateProgram();
            //attach the shaders
            glAttachShader(shader_id, vertex);
            glAttachShader(shader_id, fragment);
            //link the program
            glLinkProgram(shader_id);
            //check for errors
            glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
            if(!success){
                glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                exit(-1);
            } 
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void addTextureUniform(int texture_num){
            use();
            std::string texture_name = "texture" + std::to_string(texture_num);
            setInt(texture_name, texture_num -1);
        }

        //use/activate shader
        void use(){
            glUseProgram(shader_id);
        }

        //utility uniform functions
        void setBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(shader_id, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const{
            glUniform1f(glGetUniformLocation(shader_id, name.c_str()), value);
        }

        void setVec2(const std::string &name, const glm::vec2 &value) const{
            glUniform2fv(glGetUniformLocation(shader_id, name.c_str()), 1, &value[0]);
        }

};

#endif
