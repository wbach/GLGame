#include "ShaderProgram.h"
void CShaderProgram::initShaderProgram(char* vertexShaderFile, char* fragmentShaderFile) {
	vertexShaderID = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
}
void CShaderProgram::initShaderProgram(char * vertexShaderFile, char * fragmentShaderFile, char * tesselationShaderFile, char * tesselationEvaluationShaderFile)
{	
	vertexShaderID = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	tesselationControlShaderID = loadShader(tesselationShaderFile, GL_TESS_CONTROL_SHADER);
	tesselationEvaluationShaderID = loadShader(tesselationEvaluationShaderFile, GL_TESS_EVALUATION_SHADER);	
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glAttachShader(programID, tesselationControlShaderID);
	glAttachShader(programID, tesselationEvaluationShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
}
void CShaderProgram::loadFile(const char* fn, std::string& str)
{
    std::ifstream in(fn);
        if(!in.is_open())
        {
            std::cout << "The file " << fn << " cannot be opened\n";
            return;
        }
        char tmp[300];
        while(!in.eof())
        {
            in.getline(tmp,300);
            str+=tmp;
            str+='\n';
          //  cout << tmp <<endl ;
        }
        in.close();
	/*std::ifstream in;
	in.exceptions(ifstream::failbit | std::ifstream::badbit);
	try {
		in.open(fn);
		char tmp[300];
		while (!in.eof())
		{
			in.getline(tmp, 300);
			str += tmp;
			str += '\n';
			cout << tmp <<endl ;
		}
		in.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "The file " << fn << " exceptions in reading: \n" << e.what() << endl;
	}*/
}

unsigned int CShaderProgram::loadShader(char *filename, unsigned int mode)
{
	std::string source;
	loadFile(filename, source);

	unsigned int id;
	id = glCreateShader(mode);

	const char* csource = source.c_str();

	glShaderSource(id, 1, &csource, NULL);
	glCompileShader(id);
	char error[1000];
	int length = 0;
	glGetShaderInfoLog(id, 1000, &length, error);
	GLint compiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << "\n\nERRORS in Shader! \nFile name:\t" << filename << "\nCompile status: \n\n" << error << std::endl;
	}
	if (id == GL_FALSE)
	{
		system("pause");
		exit(0);
	}
	return id;
}
void CShaderProgram::start() {
	glUseProgram(programID);
}
void CShaderProgram::stop() {
	glUseProgram(0);
}
void CShaderProgram::cleanUp() {
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}
int CShaderProgram::getUniformLocation(const string& uniformName) {
	return glGetUniformLocation(programID, uniformName.c_str());
}
void CShaderProgram::bindAttribute(int attribute, const string& variableName) {
	glBindAttribLocation(programID, attribute, variableName.c_str());
}
void CShaderProgram::loadValue(unsigned int loacation, const glm::mat4& value) {
	glUniformMatrix4fv(loacation, 1, GL_FALSE, glm::value_ptr(value));
}
void CShaderProgram::loadValue(unsigned int loacation, const glm::mat3& value) {
	glUniformMatrix3fv(loacation, 1, GL_FALSE, glm::value_ptr(value));
}

void CShaderProgram::loadValue(unsigned int loacation, const float& value) {
	glUniform1f(loacation, value);
}

void CShaderProgram::loadValue(unsigned int loacation, const int& value) {
	glUniform1i(loacation, value);
}

void CShaderProgram::loadValue(unsigned int loacation, const glm::vec2& value) {
	glUniform2fv(loacation, 1, glm::value_ptr(value));
}

void CShaderProgram::loadValue(unsigned int loacation, const glm::vec3& value) {
	glUniform3fv(loacation, 1, glm::value_ptr(value));
}

void CShaderProgram::loadValue(unsigned int loacation, const glm::vec4& value) {
	glUniform4fv(loacation, 1, glm::value_ptr(value));
}
