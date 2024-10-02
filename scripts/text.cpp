#define _TEXT
#include "text.h"

static std::map<std::string, std::map<char, cChar>> programFonts;

static constexpr uint fChar = 32;
static constexpr uint lChar = 127;

static constexpr uint charSizeSmall = 32;
static constexpr uint charSizeNormal = 128;
static constexpr uint charSizeLarge = 512;

static uint FBO = 0;
static uint VAO, VBO, EBO;

Shader textShader;

void fontStart() {
	glGenFramebuffers(1, &FBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STREAM_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STREAM_DRAW);

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 0, 0, 0, 0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		map.insert({ c, cChar(charTexture, {(float)face->glyph->bitmap_left / size, (float)face->glyph->bitmap_top / size}, {(float)face->glyph->bitmap.width / size, (float)face->glyph->bitmap.rows / size}, (double)(face->glyph->advance.x >> 6) / size) });
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

void renderTextBuffer(std::string font, std::string text, float fontSize, float x, float yT) {
	cChar curChar;
	float w = 0;
	float h = 0;

	for (uint i = 0; i < text.length(); i++) {
		char c = text[i];
		if (c == ' ') {
			x += curChar.advance * fontSize;
			continue;
		}
		curChar = getChar(font, c);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, curChar.texTarget);

		x += curChar.bearing.x * fontSize;

		float y = yT + (curChar.bearing.y - curChar.size.y) * fontSize;
		w = curChar.size.x * fontSize;
		h = curChar.size.y * fontSize;

		float vertices[20] = {
			x,		y + h,	0,			0, 0,
			x + w,	y + h,	0,			1, 0,
			x + w,	y,		0,			1, 1,
			x,		y,		0,			0, 1
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		x += curChar.advance * fontSize;
	}
}

void createTextTexture(uint& texture, float fontSize, float lineSize, char sharpness, float width, float height, uint mode, std::string font, std::string text) {
	uint screenX = width * _Height * sharpness;
	uint screenY = height * _Height * sharpness;
	glm::mat4 ortho = glm::ortho(-width, width, -height, height, -1.0f, 1.0f);
	cChar curChar;
	Vec2 realPos = { -width, height - fontSize };
	Vec2 fakePos = realPos;
	float w = 0;
	float h = 0;
	bool oneWord = true;
	std::string word;


	textShader.use();
	textShader.setInt("texTarget", 0);
	textShader.setMat4("projection", glm::value_ptr(ortho));


	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenX, screenY, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glViewport(0, 0, screenX, screenY);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	for (uint i = 0; i < text.length(); i++) {
		char c = text[i];
		
		if (c == '\n') {
			if (word.length() != 0) {
				renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
				word.clear();
			}

			fakePos.x = -width;
			fakePos.y -= lineSize;
			realPos = fakePos;
		}

		curChar = getChar(font, c);

		if (c == ' ') {
			if (word.length() != 0)
			{
				renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
				word.clear();
			}
			realPos.x = fakePos.x + (curChar.bearing.x + curChar.advance) * fontSize;
			oneWord = false;
		}

		fakePos.x += (curChar.bearing.x + curChar.advance) * fontSize;
		if (c != ' ')
			word += c;
		
		if (fakePos.x > 1) {
			if (oneWord)
			{
				if (word.length() != 0) {
					renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
					word.clear();
				}
				fakePos.x = -width;
				fakePos.y -= lineSize;
				realPos = fakePos;
				i--;
				continue;
			}
			if (word.length() != 0) {
				i -= word.length();
				word.clear();
			}
			fakePos.x = -width;
			fakePos.y -= lineSize;
			realPos = fakePos;
			oneWord = true;
			continue;
		}

		if (i == text.length() - 1)
			renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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