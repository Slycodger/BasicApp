#define _TEXT
#include "text.h"

static std::map<std::string, std::map<char, cChar>> programFonts;

static constexpr uint fChar = 32;
static constexpr uint lChar = 127;

static constexpr uint charSizeExtraSmall = 16;
static constexpr uint charSizeSmall = 32;
static constexpr uint charSizeNormal = 64;
static constexpr uint charSizeBig = 128;
static constexpr uint charSizeLarge = 256;
static constexpr uint charSizeHuge = 512;

static uint FBO = 0;
static uint VAO, VBO, EBO;

Shader textShader;

//Create buffers that will be used for character rendering
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

	//And shader
	textShader.createShader("./shaders/TextVertexShader.vert", "./shaders/TextFragmentShader.frag");
}

//Load a font, hidden function used internally so fonts can be used if loaded once anywhere
bool loadSizeMap(std::map<char, cChar> &map, FT_Face& face, uint size) {
	//Set to whatever size
	FT_Set_Pixel_Sizes(face, 0, size);

	for (char c = fChar; c < lChar; c++) {

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			return false;

		uint charTexture = 0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &charTexture);
		glBindTexture(GL_TEXTURE_2D, charTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 0, 0, 0, 0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		map.insert({ c, cChar(charTexture, {(float)face->glyph->bitmap_left / size, (float)face->glyph->bitmap_top / size}, {(float)face->glyph->bitmap.width / size, (float)face->glyph->bitmap.rows / size}, (float)(face->glyph->advance.x >> 6) / size) });
	}
	return true;
}

//Shown loading font function
//Also generates the 5 different sizes of a font
bool loadFont(std::string font, std::string name) {
	programFonts.emplace(font, std::map<char, cChar>());

	FT_Library fontLib;
	FT_Face fontFace;
	if (FT_Init_FreeType(&fontLib))
		return false;
	if (FT_New_Face(fontLib, font.c_str(), 0, &fontFace))
		return false;

	std::map<char, cChar>* map = nullptr;
	map = &programFonts[name + "_EXSMALL"];
	loadSizeMap(*map, fontFace, charSizeExtraSmall);
	map = &programFonts[name + "_SMALL"];
	loadSizeMap(*map, fontFace, charSizeSmall);
	map = &programFonts[name + "_NORMAL"];
	loadSizeMap(*map, fontFace, charSizeNormal);
	map = &programFonts[name + "_BIG"];
	loadSizeMap(*map, fontFace, charSizeBig);
	map = &programFonts[name + "_LARGE"];
	loadSizeMap(*map, fontFace, charSizeLarge);
	map = &programFonts[name + "_HUGE"];
	loadSizeMap(*map, fontFace, charSizeHuge);
	
	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLib);

	return true;
}

//Gets a character from some font
cChar getChar(std::string name, char character) {
	return programFonts[name][character];
}

//Draws text at position with size to current framebuffer
void renderTextBuffer(std::string font, std::string text, float fontSize, float x, float yT) {
	cChar curChar;
	float w = 0;
	float h = 0;

	for (uint i = 0; i < text.length(); i++) {
		char c = text[i];
		curChar = getChar(font, c);
		if (c == ' ') {
			x += curChar.advance * fontSize;
			continue;
		}

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

//Create a text texture to be used
void createTextTexture(uint& texture, float fontSize, float lineSize, int sharpness, float width, float height, uint mode, std::string font, std::string text) {
	uint screenX = width * _Height * sharpness;
	uint screenY = height * _Height * sharpness;
	float ratio = width / height;
	glm::mat4 ortho = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
	cChar curChar;


	textShader.use();
	textShader.setInt("texTarget", 0);
	textShader.setMat4("projection", glm::value_ptr(ortho));

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenX, screenY, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glViewport(0, 0, screenX, screenY);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	//Draws text on the left hand side of the text
	if (mode == TEXT_LEFT_RENDER)
	{
		Vec2 realPos = { -ratio, 1 - fontSize };
		Vec2 fakePos = realPos;
		bool oneWord = true;
		std::string word;
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

			if (fakePos.x > ratio) {
				if (oneWord)
				{
					if (word.length() == 1) {
						renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
						word.clear();
						fakePos.x = -width;
						fakePos.y -= lineSize;
						realPos = fakePos;
						continue;
					}
					if (word.length() != 0) {
						word.pop_back();
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
	}

	//Draws text centered within the texture
	//Best with text that includes line breaks often since it will act the same as left drawing if the line fills
	if (mode == TEXT_CENTER_RENDER)
	{
		float wordWidth = 0;
		Vec2 pos = { 0, 1 - fontSize };
		std::string sentence;
		std::string word;
		bool oneWord = true;
		for (uint i = 0; i < text.length(); i++) {
			char c = text[i];

			if (c == '\n')
			{
				renderTextBuffer(font, sentence, fontSize, -pos.x / 2, pos.y);
				sentence.clear();
				word.clear();
				pos.x = 0;
				pos.y -= lineSize;
				wordWidth = 0;
				oneWord = true;
			}

			curChar = getChar(font, c);

			float charWidth = (curChar.bearing.x + curChar.advance) * fontSize;

			pos.x += charWidth;
			wordWidth += charWidth;

			sentence += c;
			if (c != ' ')
				word += c;
			else {
				word.clear();
				wordWidth = 0;
				oneWord = false;
			}
			
			if (pos.x > 2 * ratio) {
				if (!oneWord) {
					sentence.erase(sentence.end() - word.length(), sentence.end());
					i -= word.length();
					renderTextBuffer(font, sentence, fontSize, -ratio + wordWidth / 2, pos.y);
					sentence.clear();
					pos.x = 0;
					pos.y -= lineSize;
					oneWord = true;
					continue;
				}
				else
				{	
					if (sentence.length() == 1) {
						renderTextBuffer(font, sentence, fontSize, -ratio + charWidth / 2, pos.y);
						sentence.clear();
						word.clear();
						wordWidth = 0;
						pos.x = 0;
						pos.y -= lineSize;
						continue;
					}
					renderTextBuffer(font, sentence, fontSize, -ratio + charWidth / 2, pos.y);
					i--;
				}
				sentence.clear();
				word.clear();
				wordWidth = 0;
				pos.x = 0;
				pos.y -= lineSize;

				oneWord = true;
			}
			

			if (i == text.length() - 1)
				renderTextBuffer(font, sentence, fontSize, -pos.x / 2, pos.y);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _Width, _Height);
}

//Deletes the fonts
void fontEnd() {
	for (auto& map : programFonts) {
		for (int c = fChar; c < lChar; c++) {
			glDeleteTextures(1, &(map.second)[c].texTarget);
			map.second.clear();
		}
	}
	programFonts.clear();
}