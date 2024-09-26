#pragma once
constexpr float triangleVertices[] = {
	-0.5, -0.5, 0, 
	0.5, -0.5, 0,
	0, 0.5, 0
};
constexpr unsigned int triangleIndices[] = {
	0, 1, 2
};

constexpr float squareVertices[] = {
	-1, 1, 0, 0, 1,
	1, 1, 0, 1, 1,
	1, -1, 0, 1, 0,
	-1, -1, 0, 0, 0
};
constexpr unsigned int squareIndices[] = {
	0, 2, 1,
	0, 3, 2
};