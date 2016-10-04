#include <scene.h>

/**
 * Contrutor vazio.
 */
Terrain::Terrain(){}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Terrain::loadData(const std::string& object_name, const std::string& text_name){
	load_grouped_data(base_data_location + object_name, triangle_count, data);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

	GLenum text;
	GLuint samp;

	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(text, samp);
	glActiveTexture(GL_TEXTURE0);

	KTX_error_code_t error = ktxLoadTextureN((base_text_location + "grass_mip.ktx").c_str(), &text_image, &text, NULL, NULL, NULL, NULL, NULL);

	glActiveTexture(GL_TEXTURE1);
	error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_map, &text, NULL, NULL, NULL, NULL, NULL);
	
	switch(error){
		case KTX_SUCCESS:
			//std::cout << "KTX_SUCCESS" << std::endl;
			break;
		case KTX_FILE_OPEN_FAILED:
			std::cout << "KTX_FILE_OPEN_FAILED" << std::endl;
			break;
		case KTX_FILE_WRITE_ERROR:
			std::cout << "KTX_FILE_WRITE_ERROR" << std::endl;
			break;
		case KTX_INVALID_OPERATION:
			std::cout << "KTX_INVALID_OPERATION" << std::endl;
			break;
		case  KTX_INVALID_VALUE:
			std::cout << "KTX_INVALID_VALUE" << std::endl;
			break;
		case  KTX_OUT_OF_MEMORY:
			std::cout << " KTX_OUT_OF_MEMORY," << std::endl;
			break;
		case KTX_UNEXPECTED_END_OF_FILE:
			std::cout << "KTX_UNEXPECTED_END_OF_FILE" << std::endl;
			break;
		case KTX_UNKNOWN_FILE_FORMAT:
			std::cout << "KTX_UNKNOWN_FILE_FORMAT" << std::endl;
			break;
		case KTX_UNSUPPORTED_TEXTURE_TYPE:
			std::cout << "KTX_UNSUPPORTED_TEXTURE_TYPE" << std::endl;
			break;
		case KTX_GL_ERROR:
			std::cout << "KTX_GL_ERROR" << std::endl;
			break;
		case  KTX_NOT_FOUND:
			std::cout << " KTX_NOT_FOUND" << std::endl;
			break;
	}
}

void Terrain::draw(){
	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
}

void Terrain::setBaseTextLocation(const std::string& location){
	base_text_location = location;
}

std::string Terrain::base_text_location = "";