#define _TEXT
#include "text.h"

static std::map<std::string, std::map<char, cChar>> programFonts;

static constexpr uint fChar = 32;
static constexpr uint lChar = 127;

uint charSizeSmall = 32;
uint charSizeNormal = 128;
uint charSizeLarge = 512;

static uint FBO = 0;
static uint VAO, VBO, EBO;

Shader textShader;

void fontStart() {
	glGenFramebuffers(1, &FBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	textShader.createShader("./shaders/TextVertexShader.vert", "./shaders/TextFragmentShader.frag");
}

bool loadSizeMap(std::map<char, cChar> &map, FT_Face& face, uint size) {
	FT_Set_Pixel_Sizes(face, 0, size);

	for (char c = fChar; c < lChar; c++) {

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			return false;

		uint charTexture = 0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &charTexture);
		glBindTexture(GL_TEXTURE_2D, charTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		map.insert({ c, cChar(charTexture, {(float)face->glyph->bitmap_left / size, (float)face->glyph->bitmap_top / size}, {(float)face->glyph->bitmap.width / size, (float)face->glyph->bitmap.rows / size}, (face->glyph->advance.x >> 6) / size) });
	}
	return true;
}

bool loadFont(std::string font, std::string name) {
	programFonts.emplace(font, std::map<char, cChar>());

	FT_Library fontLib;
	FT_Face fontFace;
	if (FT_Init_FreeType(&fontLib))
		return false;
	if (FT_New_Face(fontLib, font.c_str(), 0, &fontFace))
		return false;

	uint pixelSize = 64;

	std::map<char, cChar>* map = nullptr;
	map = &programFonts[name + "_SMALL"];
	loadSizeMap(*map, fontFace, charSizeSmall);
	map = &programFonts[name + "_NORMAL"];
	loadSizeMap(*map, fontFace, charSizeNormal);
	map = &programFonts[name + "_LARGE"];
	loadSizeMap(*map, fontFace, charSizeLarge);
	
	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLib);

	return true;
}

cChar getChar(std::string name, char character) {
	return programFonts[name][character];
}

void createTextTexture(uint& texture, std::string font, std::string text, float lineSize, float width, float ratio, uint mode) {
	uint screenX = width * _Width;
	uint screenY = width * _Width / ratio;
	Vec2 textScale = { lineSize, 1 / _screenRatio / ratio * lineSize };

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenX, screenY, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glViewport(0, 0, screenX, screenY);
	glBindVertexArray(VAO);
	textShader.use();
	textShader.setInt("texTarget", 0);
	cChar curChar;
	Vec2 pos = { -_screenRatio, 1 };


	curChar = getChar(font, text[0]);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, curChar.texTarget);


	transform = glm::mat4(1);
	transform = glm::translate(transform, glm::vec3(pos.toGLM(), 1));
	transform = glm::scale(transform, glm::vec3(curChar.size.x * textScale.x, curChar.size.y * textScale.y, 1));

	textShader.setMat4("transform", glm::value_ptr(transform));
	textShader.setMat4("projection", glm::value_ptr(projection));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _Width, _Height);
}


void fontEnd() {
	for (auto map : programFonts) {
		for (int c = fChar; c < lChar; c++) {
			glDeleteTextures(1, &(map.second)[c].texTarget);
			map.second.clear();
		}
	}
	programFonts.clear();
}