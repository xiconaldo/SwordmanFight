#include <scene.h>

/**
 * Contrutor vazio.
 */
Player::Player(){}

Player::~Player(){
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &player_position);
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &samp);
	glDeleteTextures(1, &text_gl_name);
}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Player::loadData(const std::string& object_name, const std::string& text_name){

	load_grouped_data(base_data_location + object_name, triangle_count, data, true);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

	float position[2];
	position[0] = m_model[3].x;
	position[1] = m_model[3].z;

	glGenBuffers(1, &player_position);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, player_position);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float), (void*)position, GL_STATIC_DRAW);

	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(text_target, samp);
	
	glGenTextures(1, &text_gl_name);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

void Player::draw(){
	int factor = 1.0f;
	static int stamina = 5000;

	if(Input::instance().getStateKey(GLFW_KEY_LEFT_SHIFT)){
		stamina -= 3;
		if(stamina > 10){
			factor = 2.0f;
		}
	}
	
	if(Input::instance().getStateKey(GLFW_KEY_W))
		localTranslate(0.1f * factor, 0.0f, 0.0f);
	else if(Input::instance().getStateKey(GLFW_KEY_S))
	 	localTranslate(-0.1f * factor, 0.0f, 0.0f);

	if(Input::instance().getStateKey(GLFW_KEY_A))
		localTranslate(0.0f, 0.0f, -0.1f * factor);
	else if(Input::instance().getStateKey(GLFW_KEY_D))
	 	localTranslate(0.0f, 0.0f, 0.1f * factor);

	if(Input::instance().moveMouseX())
		localRotate(0, 1, 0, -0.01f * Input::instance().moveMouseX());


	if(glfwJoystickPresent(GLFW_JOYSTICK_1)){

		if(Input::instance().isPressedJoyButton(JOY_R1)){
			stamina -= 3;
			if(stamina > 10){
				factor = 2.0f;
			}
		}

		if (Input::instance().moveJoyAxis1X() > 0.0f)
			localTranslate(0.0f, 0.0f, 0.1f * factor);
		else if(Input::instance().moveJoyAxis1X() < 0.0f)
			localTranslate(0.0f, 0.0f, -0.1f * factor);

		if (Input::instance().moveJoyAxis1Y() > 0.0f)
			localTranslate(-0.1f * factor, 0.0f, 0.0f);
		else if (Input::instance().moveJoyAxis1Y() < 0.0f)
			localTranslate(0.1f * factor, 0.0f, 0.0f);
		else
			stamina++;

		if(Input::instance().moveJoyAxis2X())
			localRotate(0, 1, 0, -0.01f * Input::instance().moveJoyAxis2X()*3);
	}

	stamina++;
	if(stamina > 700) stamina = 700;
	else if(stamina < 0) stamina = 0;

	if(this->x() < -0.99f)
		m_model[3].x = -0.99f;
	else if(this->x() > 0.99f)
		m_model[3].x = 0.99f;

	if(this->z() < -0.99f)
		m_model[3].z = -0.99f;
	else if(this->z() > 0.99f)
		m_model[3].z = 0.99f;

	const float position[] = {m_model[3].x, m_model[3].z};

	void *content;
	content = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
	memcpy(content, (const void*)position, 2*sizeof(float));

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	//Object::draw();

}